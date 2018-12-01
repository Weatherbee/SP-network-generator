#include <stdint.h>

uint32_t sp_network (uint32_t A)
{
  /* S-box contents */
  const uint16_t SBOX00[ 16] = {    0,    4,   10,    3,    9,    2,   14,    6,
                                    7,    8,   12,   11,    1,   15,   13,    5 };
  const uint16_t SBOX01[ 32] = {   16,   18,   10,   25,   22,   29,   11,   17,
                                    3,    1,   19,   14,    7,   27,    2,    9,
                                   23,   28,    0,    6,   12,    8,    4,   20,
                                   30,   13,   26,   15,   21,   31,    5,   24 };
  const uint16_t SBOX02[ 16] = {   13,    6,   11,    8,   12,    4,    2,   10,
                                    0,    9,    5,   14,    7,   15,    3,    1 };
  const uint16_t SBOX03[  2] = {    0,    1 };
  const uint16_t SBOX04[  4] = {    3,    1,    0,    2 };

  /* define substitution-permutation network */
  uint32_t SBOX00_A = ((A & 0x00000008) >>  3)|
                      ((A & 0x00000040) >>  5)|
                      ((A & 0x00000002) <<  1)|
                      ((A & 0x00000020) >>  2);
  uint32_t SBOX00_Y = SBOX00[SBOX00_A & 0xF];
  uint32_t Y = ((SBOX00_Y & 0x00000001) << 10);
  Y = (Y & 0xFFFFFFEF) | ((SBOX00_Y & 0x00000002) <<  3);
  Y = (Y & 0xFFFFFFFB) | ((SBOX00_Y & 0x00000004)      );
  Y = (Y & 0xFFFFF7FF) | ((SBOX00_Y & 0x00000008) <<  8);

  uint32_t SBOX01_A = ((A & 0x00000010) >>  4)|
                      ((A & 0x00000004) >>  1)|
                      ((A & 0x00000400) >>  8)|
                      ((A & 0x00008000) >> 12)|
                      ((A & 0x00000080) >>  3);
  uint32_t SBOX01_Y = SBOX01[SBOX01_A & 0x1F];
  Y = (Y & 0xFFFF7FFF) | ((SBOX01_Y & 0x00000001) << 15);
  Y = (Y & 0xFFFFFFBF) | ((SBOX01_Y & 0x00000002) <<  5);
  Y = (Y & 0xFFFFDFFF) | ((SBOX01_Y & 0x00000004) << 11);
  Y = (Y & 0xFFFFEFFF) | ((SBOX01_Y & 0x00000008) <<  9);
  Y = (Y & 0xFFFFFFF7) | ((SBOX01_Y & 0x00000010) >>  1);

  uint32_t SBOX02_A = ((A & 0x00000800) >> 11)|
                      ((A & 0x00000200) >>  8)|
                      ((A & 0x00004000) >> 12)|
                      ((A & 0x00000001) <<  3);
  uint32_t SBOX02_Y = SBOX02[SBOX02_A & 0xF];
  Y = (Y & 0xFFFFFFFE) | ((SBOX02_Y & 0x00000001)      );
  Y = (Y & 0xFFFFFF7F) | ((SBOX02_Y & 0x00000002) <<  6);
  Y = (Y & 0xFFFFBFFF) | ((SBOX02_Y & 0x00000004) << 12);
  Y = (Y & 0xFFFFFFDF) | ((SBOX02_Y & 0x00000008) <<  2);

  uint32_t SBOX03_A = ((A & 0x00002000) >> 13);
  uint32_t SBOX03_Y = SBOX03[SBOX03_A & 0x1];
  Y = (Y & 0xFFFFFFFD) | ((SBOX03_Y & 0x00000001) <<  1);

  uint32_t SBOX04_A = ((A & 0x00001000) >> 12)|
                      ((A & 0x00000100) >>  7);
  uint32_t SBOX04_Y = SBOX04[SBOX04_A & 0x3];
  Y = (Y & 0xFFFFFEFF) | ((SBOX04_Y & 0x00000001) <<  8);
  Y = (Y & 0xFFFFFDFF) | ((SBOX04_Y & 0x00000002) <<  8);

  return Y;
}
