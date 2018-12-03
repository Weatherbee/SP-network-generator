#include <stdint.h>

void inverse_sp_network (uint8_t A[3], Y[3])
{
  /* S-box contents */
  const uint8_t SBOX00[ 32] = { 13, 22,  2,  6, 15, 14, 11, 12,
                                17,  5, 30, 25,  1, 21,  4, 26,
                                 0, 24,  8, 23, 10, 16, 31, 28,
                                27, 19, 18, 29,  9,  3, 20,  7 };
  const uint8_t SBOX01[  2] = {  1,  0 };
  const uint8_t SBOX02[  4] = {  2,  0,  3,  1 };
  const uint8_t SBOX03[ 32] = { 27, 29, 24,  0,  5,  8, 15, 23,
                                14, 18,  3, 31, 26,  7, 28, 13,
                                20, 22, 10, 21, 30, 12, 11, 17,
                                19,  6, 25,  2,  9,  1,  4, 16 };
  const uint8_t SBOX04[  4] = {  2,  1,  0,  3 };
  const uint8_t SBOX05[  4] = {  3,  1,  2,  0 };
  const uint8_t SBOX06[  2] = {  0,  1 };

  /* Define Substitution-Permutation Network */
  uint8_t SBOX00_A = ((A[0]&0x04)>>2)|
                     ((A[2]&0x01)<<1)|
                     ((A[0]&0x40)>>4)|
                     ((A[1]&0x04)<<1)|
                     ((A[1]&0x08)<<1);
  uint8_t SBOX00_Y = SBOX00[SBOX00_A & 0x1F];
  Y[0] = (Y[0]&0xEF)|((SBOX00_Y&0x01)<<4);
  Y[0] = (Y[0]&0xBF)|((SBOX00_Y&0x02)<<5);
  Y[1] = (Y[1]&0xBF)|((SBOX00_Y&0x04)<<4);
  Y[1] = (Y[1]&0xFB)|((SBOX00_Y&0x08)>>1);
  Y[0] = (Y[0]&0xF7)|((SBOX00_Y&0x10)>>1);

  uint8_t SBOX01_A = ((A[1]&0x80)>>7);
  uint8_t SBOX01_Y = SBOX01[SBOX01_A & 0x1];
  Y[0] = (Y[0]&0xDF)|((SBOX01_Y&0x01)<<5);

  uint8_t SBOX02_A = ((A[1]&0x40)>>6)|
                     ((A[0]&0x80)>>6);
  uint8_t SBOX02_Y = SBOX02[SBOX02_A & 0x3];
  Y[0] = (Y[0]&0xFD)|((SBOX02_Y&0x01)<<1);
  Y[1] = (Y[1]&0xF7)|((SBOX02_Y&0x02)<<2);

  uint8_t SBOX03_A = ((A[0]&0x02)>>1)|
                     ((A[2]&0x02))|
                     ((A[0]&0x10)>>2)|
                     ((A[1]&0x02)<<2)|
                     ((A[0]&0x01)<<4);
  uint8_t SBOX03_Y = SBOX03[SBOX03_A & 0x1F];
  Y[2] = (Y[2]&0xFE)|((SBOX03_Y&0x01));
  Y[0] = (Y[0]&0xFB)|((SBOX03_Y&0x02)<<1);
  Y[1] = (Y[1]&0xFE)|((SBOX03_Y&0x04)>>2);
  Y[0] = (Y[0]&0xFE)|((SBOX03_Y&0x08)>>3);
  Y[1] = (Y[1]&0xDF)|((SBOX03_Y&0x10)<<1);

  uint8_t SBOX04_A = ((A[1]&0x10)>>4)|
                     ((A[1]&0x01)<<1);
  uint8_t SBOX04_Y = SBOX04[SBOX04_A & 0x3];
  Y[1] = (Y[1]&0x7F)|((SBOX04_Y&0x01)<<7);
  Y[0] = (Y[0]&0x7F)|((SBOX04_Y&0x02)<<6);

  uint8_t SBOX05_A = ((A[0]&0x08)>>3)|
                     ((A[1]&0x20)>>4);
  uint8_t SBOX05_Y = SBOX05[SBOX05_A & 0x3];
  Y[2] = (Y[2]&0xFD)|((SBOX05_Y&0x01)<<1);
  Y[1] = (Y[1]&0xFD)|((SBOX05_Y&0x02));

  uint8_t SBOX06_A = ((A[0]&0x20)>>5);
  uint8_t SBOX06_Y = SBOX06[SBOX06_A & 0x1];
  Y[1] = (Y[1]&0xEF)|((SBOX06_Y&0x01)<<4);

}
