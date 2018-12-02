#include <stdint.h>

uint32_t inverse_sp_network (uint32_t A)
{
  /* S-box contents */
  const uint16_t SBOX00[  4] = {    0,    1,    3,    2 };
  const uint16_t SBOX01[  4] = {    0,    3,    1,    2 };
  const uint16_t SBOX02[ 16] = {    6,    0,    8,    4,    5,    9,    1,    2,
                                   11,   10,   13,   15,   14,   12,    3,    7 };
  const uint16_t SBOX03[  8] = {    4,    0,    6,    5,    2,    3,    7,    1 };
  const uint16_t SBOX04[ 32] = {   27,   12,    6,    0,    7,   24,   11,   29,
                                    3,    8,   10,    2,   26,   31,   28,    4,
                                   30,   14,   19,   23,    9,   17,    1,   25,
                                   18,   15,   21,   16,   20,    5,   13,   22 };

  /* define substitution-permutation network */
  uint32_t SBOX00_A = ((A & 0x00001000) >> 12)|
                      ((A & 0x00008000) >> 14);
  uint32_t SBOX00_Y = SBOX00[SBOX00_A & 0x3];
  uint32_t Y = ((SBOX00_Y & 0x00000001) <<  3);
  Y = (Y & 0xFFFFFFFB) | ((SBOX00_Y & 0x00000002) <<  1);

  uint32_t SBOX01_A = ((A & 0x00000800) >> 11)|
                      ((A & 0x00000008) >>  2);
  uint32_t SBOX01_Y = SBOX01[SBOX01_A & 0x3];
  Y = (Y & 0xFFFFFDFF) | ((SBOX01_Y & 0x00000001) <<  9);
  Y = (Y & 0xFFFFFFEF) | ((SBOX01_Y & 0x00000002) <<  3);

  uint32_t SBOX02_A = ((A & 0x00000080) >>  7)|
                      ((A & 0x00000400) >>  9)|
                      ((A & 0x00000004)      )|
                      ((A & 0x00000100) >>  5);
  uint32_t SBOX02_Y = SBOX02[SBOX02_A & 0xF];
  Y = (Y & 0xFFFFFFBF) | ((SBOX02_Y & 0x00000001) <<  6);
  Y = (Y & 0xFFFFBFFF) | ((SBOX02_Y & 0x00000002) << 13);
  Y = (Y & 0xFFFFFFDF) | ((SBOX02_Y & 0x00000004) <<  3);
  Y = (Y & 0xFFFFEFFF) | ((SBOX02_Y & 0x00000008) <<  9);

  uint32_t SBOX03_A = ((A & 0x00004000) >> 14)|
                      ((A & 0x00000020) >>  4)|
                      ((A & 0x00002000) >> 11);
  uint32_t SBOX03_Y = SBOX03[SBOX03_A & 0x7];
  Y = (Y & 0xFFFFF7FF) | ((SBOX03_Y & 0x00000001) << 11);
  Y = (Y & 0xFFFFDFFF) | ((SBOX03_Y & 0x00000002) << 12);
  Y = (Y & 0xFFFFFFFE) | ((SBOX03_Y & 0x00000004) >>  2);

  uint32_t SBOX04_A = ((A & 0x00000010) >>  4)|
                      ((A & 0x00000200) >>  8)|
                      ((A & 0x00000002) <<  1)|
                      ((A & 0x00000001) <<  3)|
                      ((A & 0x00000040) >>  2);
  uint32_t SBOX04_Y = SBOX04[SBOX04_A & 0x1F];
  Y = (Y & 0xFFFFFEFF) | ((SBOX04_Y & 0x00000001) <<  8);
  Y = (Y & 0xFFFF7FFF) | ((SBOX04_Y & 0x00000002) << 14);
  Y = (Y & 0xFFFFFF7F) | ((SBOX04_Y & 0x00000004) <<  5);
  Y = (Y & 0xFFFFFBFF) | ((SBOX04_Y & 0x00000008) <<  7);
  Y = (Y & 0xFFFFFFFD) | ((SBOX04_Y & 0x00000010) >>  3);

  return Y;
}
