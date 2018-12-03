#include <stdint.h>

void inverse_sp_network (uint8_t A[4], uint8_t Y[4])
{
  /* S-box contents */
  const uint8_t SBOX00[  8] = {  5,  7,  4,  3,  6,  2,  1,  0 };
  const uint8_t SBOX01[  2] = {  0,  1 };
  const uint8_t SBOX02[  8] = {  7,  4,  0,  2,  6,  5,  1,  3 };
  const uint8_t SBOX03[  4] = {  3,  0,  2,  1 };
  const uint8_t SBOX04[ 16] = {  2, 10,  5,  4,  6,  3, 15, 13,
                                14,  7,  0,  1,  9, 12,  8, 11 };
  const uint8_t SBOX05[ 64] = { 20,  6, 46, 61,  3, 58, 42, 62,
                                25,  7, 23, 21, 40, 30, 17, 55,
                                37, 63, 29, 43, 47, 54, 34, 18,
                                35, 26, 39, 45, 22, 33, 32,  4,
                                13, 52,  5, 53, 27,  1, 24, 44,
                                 9, 14,  8, 48, 50, 51, 10, 15,
                                60, 49, 41, 36,  0, 16, 28, 56,
                                57,  2, 59, 19, 38, 31, 11, 12 };
  const uint8_t SBOX06[  4] = {  0,  1,  3,  2 };
  const uint8_t SBOX07[  4] = {  1,  0,  2,  3 };
  const uint8_t SBOX08[  8] = {  1,  0,  2,  7,  5,  3,  4,  6 };
  const uint8_t SBOX09[  8] = {  3,  5,  2,  6,  4,  7,  0,  1 };
  const uint8_t SBOX10[  2] = {  1,  0 };
  const uint8_t SBOX11[  2] = {  1,  0 };
  const uint8_t SBOX12[  2] = {  1,  0 };

  /* Define Substitution-Permutation Network */
  uint8_t SBOX00_A = ((A[3]&0x08)>>3)|
                     ((A[2]&0x40)>>5)|
                     ((A[3]&0x10)>>2);
  uint8_t SBOX00_Y = SBOX00[SBOX00_A & 0x7];
  Y[0] = (Y[0]&0xDF)|((SBOX00_Y&0x01)<<5);
  Y[2] = (Y[2]&0xFE)|((SBOX00_Y&0x02)>>1);
  Y[2] = (Y[2]&0xFB)|((SBOX00_Y&0x04));

  uint8_t SBOX01_A = ((A[3]&0x20)>>5);
  uint8_t SBOX01_Y = SBOX01[SBOX01_A & 0x1];
  Y[0] = (Y[0]&0xFD)|((SBOX01_Y&0x01)<<1);

  uint8_t SBOX02_A = ((A[3]&0x80)>>7)|
                     ((A[1]&0x80)>>6)|
                     ((A[1]&0x01)<<2);
  uint8_t SBOX02_Y = SBOX02[SBOX02_A & 0x7];
  Y[0] = (Y[0]&0xFE)|((SBOX02_Y&0x01));
  Y[3] = (Y[3]&0xBF)|((SBOX02_Y&0x02)<<5);
  Y[1] = (Y[1]&0xFD)|((SBOX02_Y&0x04)>>1);

  uint8_t SBOX03_A = ((A[3]&0x04)>>2)|
                     ((A[0]&0x20)>>4);
  uint8_t SBOX03_Y = SBOX03[SBOX03_A & 0x3];
  Y[1] = (Y[1]&0x7F)|((SBOX03_Y&0x01)<<7);
  Y[1] = (Y[1]&0xDF)|((SBOX03_Y&0x02)<<4);

  uint8_t SBOX04_A = ((A[0]&0x80)>>7)|
                     ((A[1]&0x10)>>3)|
                     ((A[0]&0x04))|
                     ((A[0]&0x40)>>3);
  uint8_t SBOX04_Y = SBOX04[SBOX04_A & 0xF];
  Y[3] = (Y[3]&0xFB)|((SBOX04_Y&0x01)<<2);
  Y[3] = (Y[3]&0xF7)|((SBOX04_Y&0x02)<<2);
  Y[1] = (Y[1]&0xFE)|((SBOX04_Y&0x04)>>2);
  Y[1] = (Y[1]&0xFB)|((SBOX04_Y&0x08)>>1);

  uint8_t SBOX05_A = ((A[1]&0x20)>>5)|
                     ((A[2]&0x08)>>2)|
                     ((A[0]&0x10)>>2)|
                     ((A[2]&0x80)>>4)|
                     ((A[1]&0x04)<<2)|
                     ((A[3]&0x02)<<4);
  uint8_t SBOX05_Y = SBOX05[SBOX05_A & 0x3F];
  Y[3] = (Y[3]&0xFD)|((SBOX05_Y&0x01)<<1);
  Y[0] = (Y[0]&0xEF)|((SBOX05_Y&0x02)<<3);
  Y[0] = (Y[0]&0xF7)|((SBOX05_Y&0x04)<<1);
  Y[2] = (Y[2]&0xF7)|((SBOX05_Y&0x08));
  Y[2] = (Y[2]&0xEF)|((SBOX05_Y&0x10));
  Y[3] = (Y[3]&0xFE)|((SBOX05_Y&0x20)>>5);

  uint8_t SBOX06_A = ((A[1]&0x02)>>1)|
                     ((A[3]&0x40)>>5);
  uint8_t SBOX06_Y = SBOX06[SBOX06_A & 0x3];
  Y[3] = (Y[3]&0xDF)|((SBOX06_Y&0x01)<<5);
  Y[3] = (Y[3]&0xEF)|((SBOX06_Y&0x02)<<3);

  uint8_t SBOX07_A = ((A[1]&0x40)>>6)|
                     ((A[2]&0x20)>>4);
  uint8_t SBOX07_Y = SBOX07[SBOX07_A & 0x3];
  Y[1] = (Y[1]&0xEF)|((SBOX07_Y&0x01)<<4);
  Y[2] = (Y[2]&0xBF)|((SBOX07_Y&0x02)<<5);

  uint8_t SBOX08_A = ((A[0]&0x02)>>1)|
                     ((A[2]&0x01)<<1)|
                     ((A[2]&0x02)<<1);
  uint8_t SBOX08_Y = SBOX08[SBOX08_A & 0x7];
  Y[2] = (Y[2]&0x7F)|((SBOX08_Y&0x01)<<7);
  Y[0] = (Y[0]&0x7F)|((SBOX08_Y&0x02)<<6);
  Y[1] = (Y[1]&0xF7)|((SBOX08_Y&0x04)<<1);

  uint8_t SBOX09_A = ((A[1]&0x08)>>3)|
                     ((A[3]&0x01)<<1)|
                     ((A[0]&0x08)>>1);
  uint8_t SBOX09_Y = SBOX09[SBOX09_A & 0x7];
  Y[1] = (Y[1]&0xBF)|((SBOX09_Y&0x01)<<6);
  Y[2] = (Y[2]&0xFD)|((SBOX09_Y&0x02));
  Y[0] = (Y[0]&0xFB)|((SBOX09_Y&0x04));

  uint8_t SBOX10_A = ((A[2]&0x10)>>4);
  uint8_t SBOX10_Y = SBOX10[SBOX10_A & 0x1];
  Y[0] = (Y[0]&0xBF)|((SBOX10_Y&0x01)<<6);

  uint8_t SBOX11_A = ((A[2]&0x04)>>2);
  uint8_t SBOX11_Y = SBOX11[SBOX11_A & 0x1];
  Y[3] = (Y[3]&0x7F)|((SBOX11_Y&0x01)<<7);

  uint8_t SBOX12_A = ((A[0]&0x01));
  uint8_t SBOX12_Y = SBOX12[SBOX12_A & 0x1];
  Y[2] = (Y[2]&0xDF)|((SBOX12_Y&0x01)<<5);

}
