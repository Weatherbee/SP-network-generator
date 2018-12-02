#include <stdint.h>

uint32_t sp_network (uint32_t A)
{
  /* S-box contents */
  const uint16_t SBOX00[  4] = {    0,    1,    3,    2 };
  const uint16_t SBOX01[  4] = {    0,    2,    3,    1 };
  const uint16_t SBOX02[ 16] = {    1,    6,    7,   14,    3,    4,    0,   15,
                                    2,    5,    9,    8,   13,   10,   12,   11 };
  const uint16_t SBOX03[  8] = {    1,    7,    4,    5,    0,    3,    2,    6 };
  const uint16_t SBOX04[ 32] = {    3,   22,   11,    8,   15,   29,    2,    4,
                                    9,   20,   10,    6,    1,   30,   17,   25,
                                   27,   21,   24,   18,   28,   26,   31,   19,
                                    5,   23,   12,    0,   14,    7,   16,   13 };

  /* define substitution-permutation network */
  uint32_t SBOX00_A = ((A & 0x00000008) >>  3)|
                      ((A & 0x00000004) >>  1);
  uint32_t SBOX00_Y = SBOX00[SBOX00_A & 0x3];
  uint32_t Y = ((SBOX00_Y & 0x00000001) << 12);
  Y = (Y & 0xFFFF7FFF) | ((SBOX00_Y & 0x00000002) << 14);

  uint32_t SBOX01_A = ((A & 0x00000200) >>  9)|
                      ((A & 0x00000010) >>  3);
  uint32_t SBOX01_Y = SBOX01[SBOX01_A & 0x3];
  Y = (Y & 0xFFFFF7FF) | ((SBOX01_Y & 0x00000001) << 11);
  Y = (Y & 0xFFFFFFF7) | ((SBOX01_Y & 0x00000002) <<  2);

  uint32_t SBOX02_A = ((A & 0x00000040) >>  6)|
                      ((A & 0x00004000) >> 13)|
                      ((A & 0x00000020) >>  3)|
                      ((A & 0x00001000) >>  9);
  uint32_t SBOX02_Y = SBOX02[SBOX02_A & 0xF];
  Y = (Y & 0xFFFFFF7F) | ((SBOX02_Y & 0x00000001) <<  7);
  Y = (Y & 0xFFFFFBFF) | ((SBOX02_Y & 0x00000002) <<  9);
  Y = (Y & 0xFFFFFFFB) | ((SBOX02_Y & 0x00000004)      );
  Y = (Y & 0xFFFFFEFF) | ((SBOX02_Y & 0x00000008) <<  5);

  uint32_t SBOX03_A = ((A & 0x00000800) >> 11)|
                      ((A & 0x00002000) >> 12)|
                      ((A & 0x00000001) <<  2);
  uint32_t SBOX03_Y = SBOX03[SBOX03_A & 0x7];
  Y = (Y & 0xFFFFBFFF) | ((SBOX03_Y & 0x00000001) << 14);
  Y = (Y & 0xFFFFFFDF) | ((SBOX03_Y & 0x00000002) <<  4);
  Y = (Y & 0xFFFFDFFF) | ((SBOX03_Y & 0x00000004) << 11);

  uint32_t SBOX04_A = ((A & 0x00000100) >>  8)|
                      ((A & 0x00008000) >> 14)|
                      ((A & 0x00000080) >>  5)|
                      ((A & 0x00000400) >>  7)|
                      ((A & 0x00000002) <<  3);
  uint32_t SBOX04_Y = SBOX04[SBOX04_A & 0x1F];
  Y = (Y & 0xFFFFFFEF) | ((SBOX04_Y & 0x00000001) <<  4);
  Y = (Y & 0xFFFFFDFF) | ((SBOX04_Y & 0x00000002) <<  8);
  Y = (Y & 0xFFFFFFFD) | ((SBOX04_Y & 0x00000004) >>  1);
  Y = (Y & 0xFFFFFFFE) | ((SBOX04_Y & 0x00000008) >>  3);
  Y = (Y & 0xFFFFFFBF) | ((SBOX04_Y & 0x00000010) <<  2);

  return Y;
}
