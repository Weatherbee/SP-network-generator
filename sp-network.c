#include <stdint.h>

void sp_network (uint8_t A[4], uint8_t Y[4])
{
  /* S-box contents */
  const uint8_t SBOX00[  8] = {  7,  6,  5,  3,  2,  0,  4,  1 };
  const uint8_t SBOX01[  2] = {  0,  1 };
  const uint8_t SBOX02[  8] = {  2,  6,  3,  7,  1,  5,  4,  0 };
  const uint8_t SBOX03[  4] = {  1,  3,  2,  0 };
  const uint8_t SBOX04[ 16] = { 10, 11,  0,  5,  3,  2,  4,  9,
                                14, 12,  1, 15, 13,  7,  8,  6 };
  const uint8_t SBOX05[ 64] = { 52, 37, 57,  4, 31, 34,  1,  9,
                                42, 40, 46, 62, 63, 32, 41, 47,
                                53, 14, 23, 59,  0, 11, 28, 10,
                                38,  8, 25, 36, 54, 18, 13, 61,
                                30, 29, 22, 24, 51, 16, 60, 26,
                                12, 50,  6, 19, 39, 27,  2, 20,
                                43, 49, 44, 45, 33, 35, 21, 15,
                                55, 56,  5, 58, 48,  3,  7, 17 };
  const uint8_t SBOX06[  4] = {  0,  1,  3,  2 };
  const uint8_t SBOX07[  4] = {  1,  0,  2,  3 };
  const uint8_t SBOX08[  8] = {  1,  0,  2,  5,  6,  4,  7,  3 };
  const uint8_t SBOX09[  8] = {  6,  7,  2,  0,  4,  1,  3,  5 };
  const uint8_t SBOX10[  2] = {  1,  0 };
  const uint8_t SBOX11[  2] = {  1,  0 };
  const uint8_t SBOX12[  2] = {  1,  0 };

  /* Define Substitution-Permutation Network */
  uint8_t SBOX00_A = ((A[0]&0x20)>>5)|
                     ((A[2]&0x01)<<1)|
                     ((A[2]&0x04));
  uint8_t SBOX00_Y = SBOX00[SBOX00_A & 0x7];
  Y[3] = (Y[3]&0xF7)|((SBOX00_Y&0x01)<<3);
  Y[2] = (Y[2]&0xBF)|((SBOX00_Y&0x02)<<5);
  Y[3] = (Y[3]&0xEF)|((SBOX00_Y&0x04)<<2);

  uint8_t SBOX01_A = ((A[0]&0x02)>>1);
  uint8_t SBOX01_Y = SBOX01[SBOX01_A & 0x1];
  Y[3] = (Y[3]&0xDF)|((SBOX01_Y&0x01)<<5);

  uint8_t SBOX02_A = ((A[0]&0x01))|
                     ((A[3]&0x40)>>5)|
                     ((A[1]&0x02)<<1);
  uint8_t SBOX02_Y = SBOX02[SBOX02_A & 0x7];
  Y[3] = (Y[3]&0x7F)|((SBOX02_Y&0x01)<<7);
  Y[1] = (Y[1]&0x7F)|((SBOX02_Y&0x02)<<6);
  Y[1] = (Y[1]&0xFE)|((SBOX02_Y&0x04)>>2);

  uint8_t SBOX03_A = ((A[1]&0x80)>>7)|
                     ((A[1]&0x20)>>4);
  uint8_t SBOX03_Y = SBOX03[SBOX03_A & 0x3];
  Y[3] = (Y[3]&0xFB)|((SBOX03_Y&0x01)<<2);
  Y[0] = (Y[0]&0xDF)|((SBOX03_Y&0x02)<<4);

  uint8_t SBOX04_A = ((A[3]&0x04)>>2)|
                     ((A[3]&0x08)>>2)|
                     ((A[1]&0x01)<<2)|
                     ((A[1]&0x04)<<1);
  uint8_t SBOX04_Y = SBOX04[SBOX04_A & 0xF];
  Y[0] = (Y[0]&0x7F)|((SBOX04_Y&0x01)<<7);
  Y[1] = (Y[1]&0xEF)|((SBOX04_Y&0x02)<<3);
  Y[0] = (Y[0]&0xFB)|((SBOX04_Y&0x04));
  Y[0] = (Y[0]&0xBF)|((SBOX04_Y&0x08)<<3);

  uint8_t SBOX05_A = ((A[3]&0x02)>>1)|
                     ((A[0]&0x10)>>3)|
                     ((A[0]&0x08)>>1)|
                     ((A[2]&0x08))|
                     ((A[2]&0x10))|
                     ((A[3]&0x01)<<5);
  uint8_t SBOX05_Y = SBOX05[SBOX05_A & 0x3F];
  Y[1] = (Y[1]&0xDF)|((SBOX05_Y&0x01)<<5);
  Y[2] = (Y[2]&0xF7)|((SBOX05_Y&0x02)<<2);
  Y[0] = (Y[0]&0xEF)|((SBOX05_Y&0x04)<<2);
  Y[2] = (Y[2]&0x7F)|((SBOX05_Y&0x08)<<4);
  Y[1] = (Y[1]&0xFB)|((SBOX05_Y&0x10)>>2);
  Y[3] = (Y[3]&0xFD)|((SBOX05_Y&0x20)>>4);

  uint8_t SBOX06_A = ((A[3]&0x20)>>5)|
                     ((A[3]&0x10)>>3);
  uint8_t SBOX06_Y = SBOX06[SBOX06_A & 0x3];
  Y[1] = (Y[1]&0xFD)|((SBOX06_Y&0x01)<<1);
  Y[3] = (Y[3]&0xBF)|((SBOX06_Y&0x02)<<5);

  uint8_t SBOX07_A = ((A[1]&0x10)>>4)|
                     ((A[2]&0x40)>>5);
  uint8_t SBOX07_Y = SBOX07[SBOX07_A & 0x3];
  Y[1] = (Y[1]&0xBF)|((SBOX07_Y&0x01)<<6);
  Y[2] = (Y[2]&0xDF)|((SBOX07_Y&0x02)<<4);

  uint8_t SBOX08_A = ((A[2]&0x80)>>7)|
                     ((A[0]&0x80)>>6)|
                     ((A[1]&0x08)>>1);
  uint8_t SBOX08_Y = SBOX08[SBOX08_A & 0x7];
  Y[0] = (Y[0]&0xFD)|((SBOX08_Y&0x01)<<1);
  Y[2] = (Y[2]&0xFE)|((SBOX08_Y&0x02)>>1);
  Y[2] = (Y[2]&0xFD)|((SBOX08_Y&0x04)>>1);

  uint8_t SBOX09_A = ((A[1]&0x40)>>6)|
                     ((A[2]&0x02))|
                     ((A[0]&0x04));
  uint8_t SBOX09_Y = SBOX09[SBOX09_A & 0x7];
  Y[1] = (Y[1]&0xF7)|((SBOX09_Y&0x01)<<3);
  Y[3] = (Y[3]&0xFE)|((SBOX09_Y&0x02)>>1);
  Y[0] = (Y[0]&0xF7)|((SBOX09_Y&0x04)<<1);

  uint8_t SBOX10_A = ((A[0]&0x40)>>6);
  uint8_t SBOX10_Y = SBOX10[SBOX10_A & 0x1];
  Y[2] = (Y[2]&0xEF)|((SBOX10_Y&0x01)<<4);

  uint8_t SBOX11_A = ((A[3]&0x80)>>7);
  uint8_t SBOX11_Y = SBOX11[SBOX11_A & 0x1];
  Y[2] = (Y[2]&0xFB)|((SBOX11_Y&0x01)<<2);

  uint8_t SBOX12_A = ((A[2]&0x20)>>5);
  uint8_t SBOX12_Y = SBOX12[SBOX12_A & 0x1];
  Y[0] = (Y[0]&0xFE)|((SBOX12_Y&0x01));

}
