#include <stdint.h>

uint32_t inverse_sp_network (uint32_t A)
{
  /* S-box contents */
  const uint16_t SBOX00[ 16] = {    0,   12,    5,    3,    1,   15,    7,    8,
                                    9,    4,    2,   11,   10,   14,    6,   13 };
  const uint16_t SBOX01[ 32] = {   18,    9,   14,    8,   22,   30,   19,   12,
                                   21,   15,    2,    6,   20,   25,   11,   27,
                                    0,    7,    1,   10,   23,   28,    4,   16,
                                   31,    3,   26,   13,   17,    5,   24,   29 };
  const uint16_t SBOX02[ 16] = {    8,   15,    6,   14,    5,   10,    1,   12,
                                    3,    9,    7,    2,    4,    0,   11,   13 };
  const uint16_t SBOX03[  2] = {    0,    1 };
  const uint16_t SBOX04[  4] = {    2,    1,    3,    0 };

  /* define substitution-permutation network */
  uint32_t SBOX00_A = ((A & 0x00000400) >> 10)|
                      ((A & 0x00000010) >>  3)|
                      ((A & 0x00000004)      )|
                      ((A & 0x00000800) >>  8);
  uint32_t SBOX00_Y = SBOX00[SBOX00_A & 0xF];
  uint32_t Y = ((SBOX00_Y & 0x00000001) <<  3);
  Y = (Y & 0xFFFFFFBF) | ((SBOX00_Y & 0x00000002) <<  5);
  Y = (Y & 0xFFFFFFFD) | ((SBOX00_Y & 0x00000004) >>  1);
  Y = (Y & 0xFFFFFFDF) | ((SBOX00_Y & 0x00000008) <<  2);

  uint32_t SBOX01_A = ((A & 0x00008000) >> 15)|
                      ((A & 0x00000040) >>  5)|
                      ((A & 0x00002000) >> 11)|
                      ((A & 0x00001000) >>  9)|
                      ((A & 0x00000008) <<  1);
  uint32_t SBOX01_Y = SBOX01[SBOX01_A & 0x1F];
  Y = (Y & 0xFFFFFFEF) | ((SBOX01_Y & 0x00000001) <<  4);
  Y = (Y & 0xFFFFFFFB) | ((SBOX01_Y & 0x00000002) <<  1);
  Y = (Y & 0xFFFFFBFF) | ((SBOX01_Y & 0x00000004) <<  8);
  Y = (Y & 0xFFFF7FFF) | ((SBOX01_Y & 0x00000008) << 12);
  Y = (Y & 0xFFFFFF7F) | ((SBOX01_Y & 0x00000010) <<  3);

  uint32_t SBOX02_A = ((A & 0x00000001)      )|
                      ((A & 0x00000080) >>  6)|
                      ((A & 0x00004000) >> 12)|
                      ((A & 0x00000020) >>  2);
  uint32_t SBOX02_Y = SBOX02[SBOX02_A & 0xF];
  Y = (Y & 0xFFFFF7FF) | ((SBOX02_Y & 0x00000001) << 11);
  Y = (Y & 0xFFFFFDFF) | ((SBOX02_Y & 0x00000002) <<  8);
  Y = (Y & 0xFFFFBFFF) | ((SBOX02_Y & 0x00000004) << 12);
  Y = (Y & 0xFFFFFFFE) | ((SBOX02_Y & 0x00000008) >>  3);

  uint32_t SBOX03_A = ((A & 0x00000002) >>  1);
  uint32_t SBOX03_Y = SBOX03[SBOX03_A & 0x1];
  Y = (Y & 0xFFFFDFFF) | ((SBOX03_Y & 0x00000001) << 13);

  uint32_t SBOX04_A = ((A & 0x00000100) >>  8)|
                      ((A & 0x00000200) >>  8);
  uint32_t SBOX04_Y = SBOX04[SBOX04_A & 0x3];
  Y = (Y & 0xFFFFEFFF) | ((SBOX04_Y & 0x00000001) << 12);
  Y = (Y & 0xFFFFFEFF) | ((SBOX04_Y & 0x00000002) <<  7);

  return Y;
}
