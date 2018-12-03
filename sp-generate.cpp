/*
BSD 3-Clause License

Copyright (c) 2018, Jamil J. Weatherbee
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of the copyright holder nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <iostream>
#include <iomanip>
#include <chrono>
#include <vector>
#include <cstdlib>
#include <cstdint>
#include <sstream>
#include <fstream>
#include <typeinfo>

/* outside C linkages */
extern "C" uint32_t arc4random(void);
extern "C" uint32_t arc4random_uniform(uint32_t upper_bound);

/* BEGIN user controlled settings for this SP-network generation */
#define SP_NETWORK_BITS 18 /* number of bits in the SP-network */
#define S_BOX_MAX_BITS 8 /* how many bits wide can an S-Box be */
#define SP_NETWORK_FILENAME "sp-network.c"
#define INVERSE_SP_NETWORK_FILENAME "inverse-sp-network.c"
/* END user controlled settings for this SP-network generation */

const std::string sp_network_filename (SP_NETWORK_FILENAME);
const std::string inverse_sp_network_filename (INVERSE_SP_NETWORK_FILENAME);
constexpr size_t BITS_PER_BYTE = 8;
constexpr size_t SP_NETWORK_SIZE = (SP_NETWORK_BITS/BITS_PER_BYTE)+(SP_NETWORK_BITS%BITS_PER_BYTE?1:0);

/* print out a vector */
void print_box (std::vector<unsigned> const &v, unsigned columns, unsigned width)
{
  auto i=0;
  for (auto x: v)
   {
     std::cout << "[" << std::dec << std::right << std::setw(width) << x << "]";
     i++;
     if (0==i%columns) std::cout << std::endl;
   }
  if (0!=i%columns) std::cout << std::endl;
}

/* print out a block of 64-bit data using C declaration */
void print_block (uint64_t *v, size_t length, unsigned columns)
{
  for (auto n=0;n<length;n++)
   {
     std::cout << std::hex << std::uppercase << std::noshowbase << std::internal
               << std::setfill('0') << std::setw(16) << v[n] << " ";
     if (0==(n+1)%columns) std::cout << std::endl;
   }
  std::cout << std::endl;
}

/* print out a matrix */
template<typename T, typename A>
void print_vector (std::vector<T,A> const &v, unsigned columns)
{
  auto i=0;
  for (auto x: v)
   {
     std::cout << std::hex << std::uppercase << std::noshowbase << std::internal
               << std::setfill('0') << std::setw(sizeof(T)*2)<< x << " ";
     i++;
     if (0==i%columns) std::cout << std::endl;
   }
  std::cout << std::endl;
}

/* template function to randomly permutate a vector */
template<typename T, typename A>
void random_permute (std::vector<T,A> &v)
{
  auto K = (uint32_t)v.size();
  for (auto n=0;n<K;n++) std::swap(v[n],v[n+arc4random_uniform(K-n)]);
}

/* template function to fill a vector of scalars using increment operator */
template<typename T, typename A>
void vector_fill (std::vector<T,A> &v)
{
  auto K = v.size();
  T value = 0;
  for (auto n=0;n<K;n++)
   {
     v[n] = value++;
   }
}


void old_emit_sp_network_code (std::ostream &fs, std::string function_name,
                                         std::vector<unsigned> S_box_width, std::vector<std::vector<unsigned>> S_box,
                                         std::vector<unsigned> in_P_box, std::vector<unsigned> out_P_box)
{
    fs << "#include <stdint.h>" << std::endl << std::endl;
    fs << "void " << function_name << std::dec << " (uint8_t A[" << SP_NETWORK_SIZE
       << "], Y[" << SP_NETWORK_SIZE << "])" << std::endl;
    fs << "{" << std::endl;

    /* emit S-Box contents */
    fs << "  /* S-box contents */" << std::endl;
    for (auto n=0;n<S_box_width.size();n++)
     {
       fs << "  const uint8_t SBOX" << std::dec << std::internal << std::setfill('0')
          << std::setw(2) << n << "[" << std::setfill(' ') << std::setw(3) << S_box[n].size() << "] = {";
       for (auto S_box_entry=0;S_box_entry<S_box[n].size();S_box_entry++)
        {
          fs << std::dec << std::right << std::setfill(' ') << std::setw(3) << S_box[n][S_box_entry];
          if (S_box[n].size()-1==S_box_entry) fs << " };" << std::endl;
          else
           {
             fs << ",";
             if (7==S_box_entry%8) fs << std::endl << "                               ";
           }
         }
     }

    fs << std::endl << "  /* Define Substitution-Permutation Network */" << std::endl;
    auto P_box_bit = 0;
    for (auto n=0;n<S_box_width.size();n++)
     {
       /* emit input (A) P_box bits to temporary sbox_N_in extraction */
       for (auto S_box_bit=0;S_box_bit<S_box_width[n];S_box_bit++)
        {
          auto word_index = (in_P_box[P_box_bit+S_box_bit])/BITS_PER_BYTE;
          auto word_bit = (in_P_box[P_box_bit+S_box_bit])%BITS_PER_BYTE;
          unsigned word_bit_mask = 0x1<<word_bit;

          if (0==S_box_bit) fs << "  uint8_t SBOX" << std::dec << std::internal
                               << std::setfill('0') << std::setw(2) << n << "_A = ";
          else fs << "                     ";

          fs << "((A[" << std::dec << word_index << "]&0x" << std::hex << std::uppercase << std::noshowbase
            << std::internal << std::setfill('0') << std::setw(2) << word_bit_mask << ")";

          if (S_box_bit>word_bit) fs << "<<" << std::dec << S_box_bit-word_bit << ")";
          else if (word_bit>S_box_bit) fs << ">>" << std::dec << word_bit-S_box_bit << ")";
          else fs << ")";

          if (S_box_width[n]-1==S_box_bit) fs << ";" << std::endl;
          else fs << "|" << std::endl;
        }

       /* calculating sbox_N_out from sbox_N and sbox_N_in */
       unsigned sbox_index_bit_mask = (0x1<<S_box_width[n])-1;
       fs << "  uint8_t SBOX" << std::dec << std::internal << std::setfill('0') << std::setw(2) << n << "_Y = SBOX"
          << std::dec << std::internal << std::setfill('0') << std::setw(2) << n
          << "[SBOX" << std::setw(2) << n << "_A & 0x"
          << std::hex << std::uppercase << std::noshowbase << sbox_index_bit_mask << "];" << std::endl;

       /* emit output (Y) P_box bits from temporary sbox_N_out result */
       for (auto S_box_bit=0;S_box_bit<S_box_width[n];S_box_bit++)
        {
          auto word_index = (out_P_box[P_box_bit+S_box_bit])/BITS_PER_BYTE;
          auto word_bit = (out_P_box[P_box_bit+S_box_bit])%BITS_PER_BYTE;
          unsigned word_bit_clear_mask = (uint8_t)~(0x1<<word_bit);
          unsigned sbox_bit_mask = 0x1<<S_box_bit;

          fs << "  Y[" << std::dec << word_index << "] = (Y[" << word_index << "]&0x"
            << std::hex << std::uppercase << std::noshowbase << std::internal << std::setfill('0')
            << std::setw(2) << word_bit_clear_mask
            << ")|((SBOX" << std::dec << std::internal << std::setfill('0') << std::setw(2) << n << "_Y&0x"
            << std::hex << std::uppercase << std::noshowbase << std::internal << std::setfill('0')
            << std::setw(2) << sbox_bit_mask << ")";

          if (word_bit>S_box_bit) fs << "<<" << std::dec << word_bit-S_box_bit << ")";
          else if (S_box_bit>word_bit) fs << ">>" << std::dec << S_box_bit-word_bit << ")";
          else fs << ")";
          fs << ";" << std::endl;
        }

       fs << std::endl;
       P_box_bit += S_box_width[n];
     }

    fs << "}" << std::endl;
}


// void emit_sp_network_code (std::ostream &fs, std::string function_name,
//                            std::vector<unsigned> S_box_width, std::vector<std::vector<unsigned>> S_box,
//                            std::vector<unsigned> in_P_box, std::vector<unsigned> out_P_box)
// {
//     fs << "#include <stdint.h>" << std::endl << std::endl;
//     fs << "void " << function_name << std::dec << " (uint32_t A[" << SP_NETWORK_SIZE
//        << "], Y[" << SP_NETWORK_SIZE << "])" << std::endl;
//     fs << "{" << std::endl;
//
//     /* emit S-Box contents */
//     fs << "  /* S-box contents */" << std::endl;
//     for (auto n=0;n<S_box_width.size();n++)
//      {
//        fs << "  const uint8_t SBOX" << std::dec << std::internal << std::setfill('0')
//           << std::setw(2) << n << "[" << std::setfill(' ') << std::setw(3) << S_box[n].size() << "] = {";
//        for (auto S_box_entry=0;S_box_entry<S_box[n].size();S_box_entry++)
//         {
//           fs << std::dec << std::right << std::setfill(' ') << std::setw(4) << S_box[n][S_box_entry];
//           if (S_box[n].size()-1==S_box_entry) fs << " };" << std::endl;
//           else
//            {
//              fs << ",";
//              if (7==S_box_entry%8) fs << std::endl << "                               ";
//            }
//          }
//      }
//
//     fs << std::endl << "  /* define substitution-permutation network */" << std::endl;
//     auto P_box_bit = 0;
//     for (auto n=0;n<S_box_width.size();n++)
//      {
//        /* emit input (A) P_box bits to temporary sbox_N_in extraction */
//        for (auto S_box_bit=0;S_box_bit<S_box_width[n];S_box_bit++)
//         {
//           auto word_bit = in_P_box[P_box_bit+S_box_bit];
//           uint8_t word_bit_mask = ((uint8_t)0x1)<<word_bit;
//
//           if (0==S_box_bit) fs << "  uint32_t SBOX" << std::dec << std::internal
//                                << std::setfill('0') << std::setw(2) << n << "_A = ";
//           else fs << "                      ";
//
//           fs << "((A & 0x" << std::hex << std::uppercase << std::noshowbase << std::internal
//                            << std::setfill('0') << std::setw(8) << word_bit_mask << ")";
//
//           if (S_box_bit>word_bit) fs << " << " << std::dec << std::right << std::setfill(' ')
//                                      << std::setw(2) << S_box_bit-word_bit << ")";
//           else if (word_bit>S_box_bit) fs << " >> " << std::dec << std::right << std::setfill(' ')
//                                           << std::setw(2) << word_bit-S_box_bit << ")";
//           else fs << "      )";
//
//           if (S_box_width[n]-1==S_box_bit) fs << ";" << std::endl;
//           else fs << "|" << std::endl;
//         }
//
//        /* calculating sbox_N_out from sbox_N and sbox_N_in */
//        auto sbox_index_bit_mask = ((uint32_t)0x1<<S_box_width[n])-1;
//        fs << "  uint32_t SBOX" << std::dec << std::internal << std::setfill('0') << std::setw(2) << n << "_Y = SBOX"
//          << std::dec << std::internal << std::setfill('0') << std::setw(2) << n << "[SBOX" << std::setw(2) << n << "_A & 0x"
//          << std::hex << std::uppercase << std::noshowbase << sbox_index_bit_mask << "];" << std::endl;
//
//        /* emit output (Y) P_box bits from temporary sbox_N_out result */
//        for (auto S_box_bit=0;S_box_bit<S_box_width[n];S_box_bit++)
//         {
//           auto word_bit = out_P_box[P_box_bit+S_box_bit];
//           uint32_t word_bit_clear_mask = ~((uint32_t)0x1<<word_bit);
//           uint32_t sbox_bit_mask = (uint32_t)0x1<<S_box_bit;
//
//           if ((0==n)&&(0==S_box_bit)) fs << "  uint32_t Y = ";
//           else fs << "  Y = (Y & 0x" << std::hex << std::uppercase << std::noshowbase << std::internal
//                   << std::setfill('0') << std::setw(8) << word_bit_clear_mask << ") | ";
//
//           fs << "((SBOX" << std::dec << std::internal << std::setfill('0') << std::setw(2) << n << "_Y & 0x"
//              << std::hex << std::uppercase << std::noshowbase << std::internal
//              << std::setfill('0') << std::setw(8) << sbox_bit_mask << ")";
//
//           if (word_bit>S_box_bit) fs << " << " << std::dec << std::right << std::setfill(' ')
//                                      << std::setw(2) << word_bit-S_box_bit << ")";
//           else if (S_box_bit>word_bit) fs << " >> " << std::dec << std::right << std::setfill(' ')
//                                           << std::setw(2) << S_box_bit-word_bit << ")";
//           else fs << "      )";
//           fs << ";" << std::endl;
//         }
//
//        fs << std::endl;
//        P_box_bit += S_box_width[n];
//      }
//
//     fs << "  return Y;" << std::endl << "}" << std::endl;
// }


void sp_network_generate (void)
{
    std::vector<unsigned> in_P_box (SP_NETWORK_BITS); /* defines the order the bits are picked out of the input block */
    std::vector<unsigned> out_P_box (SP_NETWORK_BITS); /* defines the order the bits are placed into the output block */
    std::vector<unsigned> S_box_width; /* defines the widths of the S-boxes */
    std::vector<std::vector<unsigned>> forward_S_box; /* vector of S-Box vectors */
    std::vector<std::vector<unsigned>> reverse_S_box; /* vector of S-Box vectors */
    std::ofstream code_fs; /* file stream for emitting generated C code */
    std::chrono::system_clock::time_point start_time, end_time;

    static_assert(BITS_PER_BYTE>=S_BOX_MAX_BITS, "S-box too wide");

    start_time = std::chrono::system_clock::now();

    std::cout << "Generating a " << std::dec << SP_NETWORK_BITS << " bit SP-network, maximum S-box width is "
              << S_BOX_MAX_BITS << " bits" << std::endl << std::endl;

    std::cout << "Input P-box:" << std::endl;
    vector_fill (in_P_box);
    random_permute (in_P_box);
    print_box (in_P_box, 16, 3);

    std::cout << std::endl << "Output P-box:" << std::endl;
    vector_fill (out_P_box);
    random_permute (out_P_box);
    print_box (out_P_box, 16, 3);

    auto bits_remaining = SP_NETWORK_BITS;
    while (bits_remaining)
     {
       /* randomly select an S-Box width that isn't wider than the bits remaining or the maximum S-Box width */
       auto w = arc4random_uniform(std::min(bits_remaining,S_BOX_MAX_BITS))+1;
       S_box_width.push_back(w);
       bits_remaining -= w;
     }
    std::cout << std::endl << "Generated " << std::dec << S_box_width.size() << " arbitrary S-Boxes" << std::endl;
    print_box (S_box_width, 32, 1);

    for (auto w: S_box_width) /* iterate over all S-Box bit widths */
     {
       std::vector<unsigned> v(1<<w); /* allocate a vector to hold the substitution table for a particular S-Box of bit width w */
       vector_fill (v); /* fill the S-Box to with sequential values */
       random_permute (v); /* randomly permute those values to create a functional 1 to 1 substitution box */

       /* allocate a vector to hold the inverse substitution table for a particular S-Box of bit width w */
       std::vector<unsigned> inverse_v(1<<w);
       for (auto n=0;n<v.size();n++) inverse_v[v[n]]=n;

       forward_S_box.push_back(v); /* save the forward S-Box using move constructor */
       reverse_S_box.push_back(inverse_v); /* save the reverse S-Box using move constructor */
     }

    for (auto n=0;n<S_box_width.size();n++)
     {
       std::cout << std::endl << "S-box " << std::dec << n << " width " << S_box_width[n] << " bit(s):" << std::endl;
       print_box (forward_S_box[n], 8, 4);
       std::cout << "inverse S-box " << std::dec << n << " width " << S_box_width[n] << " bit(s):" << std::endl;
       print_box (reverse_S_box[n], 8, 4);
     }

    std::cout << std::endl;
    code_fs.open(sp_network_filename, std::ios_base::out | std::ios_base::trunc);
    if (!code_fs)
     {
       std::cout << "Cannot create " << sp_network_filename << " source file for write." << std::endl;
       std::exit(-1);
     }
    else std::cout << "Opened " << sp_network_filename << " source file." << std::endl;

    std::cout << "Generating forward substitution-permutation C code." << std::endl;
    old_emit_sp_network_code (code_fs, "sp_network", S_box_width, forward_S_box, in_P_box, out_P_box);
    code_fs.close(); /* close C source file */

    std::cout << std::endl;
    code_fs.open(inverse_sp_network_filename, std::ios_base::out | std::ios_base::trunc);
    if (!code_fs)
    {
        std::cout << "Cannot create " << inverse_sp_network_filename << " source file for write." << std::endl;
        std::exit(-1);
    }
    else std::cout << "Opened " << inverse_sp_network_filename << " source file." << std::endl;

    std::cout << "Generating reverse substitution-permutation C code." << std::endl;
    old_emit_sp_network_code (code_fs, "inverse_sp_network", S_box_width, reverse_S_box, out_P_box, in_P_box);
    code_fs.close(); /* close C source file */

    end_time = std::chrono::system_clock::now();
    std::chrono::duration<double,std::micro> time_interval = end_time - start_time;
    std::cout << std::dec << time_interval.count() << " microseconds elapsed." << std::endl;
}

int main(int argc, const char * argv[])
{
  sp_network_generate();
  return 0;
}
