#include <stdint.h>

void sp_network (uint8_t A[3], Y[3])
{
  /* S-box contents */
  const uint8_t SBOX00[ 32] = { 16, 12,  2, 29, 14,  9,  3, 31,
                                18, 28, 20,  6,  7,  0,  5,  4,
                                21,  8, 26, 25, 30, 13,  1, 19,
                                17, 11, 15, 24, 23, 27, 10, 22 };
  const uint8_t SBOX01[  2] = {  1,  0 };
  const uint8_t SBOX02[  4] = {  1,  3,  0,  2 };
  const uint8_t SBOX03[ 32] = {  3, 29, 27, 10, 30,  4, 25, 13,
                                 5, 28, 18, 22, 21, 15,  8,  6,
                                31, 23,  9, 24, 16, 19, 17,  7,
                                 2, 26, 12,  0, 14,  1, 20, 11 };
  const uint8_t SBOX04[  4] = {  2,  1,  0,  3 };
  const uint8_t SBOX05[  4] = {  3,  1,  2,  0 };
  const uint8_t SBOX06[  2] = {  0,  1 };

  /* Define Substitution-Permutation Network */
  uint8_t SBOX00_A = ((A[0]&0x10)>>4)|
                     ((A[0]&0x40)>>5)|
                     ((A[1]&0x40)>>4)|
                     ((A[1]&0x04)<<1)|
                     ((A[0]&0x08)<<1);
  uint8_t SBOX00_Y = SBOX00[SBOX00_A & 0x1F];
  Y[0] = (Y[0]&0xFB)|((SBOX00_Y&0x01)<<2);
  Y[2] = (Y[2]&0xFE)|((SBOX00_Y&0x02)>>1);
  Y[0] = (Y[0]&0xBF)|((SBOX00_Y&0x04)<<4);
  Y[1] = (Y[1]&0xFB)|((SBOX00_Y&0x08)>>1);
  Y[1] = (Y[1]&0xF7)|((SBOX00_Y&0x10)>>1);

  uint8_t SBOX01_A = ((A[0]&0x20)>>5);
  uint8_t SBOX01_Y = SBOX01[SBOX01_A & 0x1];
  Y[1] = (Y[1]&0x7F)|((SBOX01_Y&0x01)<<7);

  uint8_t SBOX02_A = ((A[0]&0x02)>>1)|
                     ((A[1]&0x08)>>2);
  uint8_t SBOX02_Y = SBOX02[SBOX02_A & 0x3];
  Y[1] = (Y[1]&0xBF)|((SBOX02_Y&0x01)<<6);
  Y[0] = (Y[0]&0x7F)|((SBOX02_Y&0x02)<<6);

  uint8_t SBOX03_A = ((A[2]&0x01))|
                     ((A[0]&0x04)>>1)|
                     ((A[1]&0x01)<<2)|
                     ((A[0]&0x01)<<3)|
                     ((A[1]&0x20)>>1);
  uint8_t SBOX03_Y = SBOX03[SBOX03_A & 0x1F];
  Y[0] = (Y[0]&0xFD)|((SBOX03_Y&0x01)<<1);
  Y[2] = (Y[2]&0xFD)|((SBOX03_Y&0x02));
  Y[0] = (Y[0]&0xEF)|((SBOX03_Y&0x04)<<2);
  Y[1] = (Y[1]&0xFD)|((SBOX03_Y&0x08)>>2);
  Y[0] = (Y[0]&0xFE)|((SBOX03_Y&0x10)>>4);

  uint8_t SBOX04_A = ((A[1]&0x80)>>7)|
                     ((A[0]&0x80)>>6);
  uint8_t SBOX04_Y = SBOX04[SBOX04_A & 0x3];
  Y[1] = (Y[1]&0xEF)|((SBOX04_Y&0x01)<<4);
  Y[1] = (Y[1]&0xFE)|((SBOX04_Y&0x02)>>1);

  uint8_t SBOX05_A = ((A[2]&0x02)>>1)|
                     ((A[1]&0x02));
  uint8_t SBOX05_Y = SBOX05[SBOX05_A & 0x3];
  Y[0] = (Y[0]&0xF7)|((SBOX05_Y&0x01)<<3);
  Y[1] = (Y[1]&0xDF)|((SBOX05_Y&0x02)<<4);

  uint8_t SBOX06_A = ((A[1]&0x10)>>4);
  uint8_t SBOX06_Y = SBOX06[SBOX06_A & 0x1];
  Y[0] = (Y[0]&0xDF)|((SBOX06_Y&0x01)<<5);

}
