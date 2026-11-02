#include "restore_types.h"
#define DAT_007027b8 (*(char *)0x007027b8u)
#define DAT_007027b9 (*(char *)0x007027b9u)
#define DAT_007027ba (*(char *)0x007027bau)
#define DAT_007027bb (*(char *)0x007027bbu)
#define DAT_007027bc (*(char *)0x007027bcu)
#define DAT_007027be (*(char *)0x007027beu)
#define DAT_007027bf (*(char *)0x007027bfu)
#define DAT_007027c1 (*(char *)0x007027c1u)
#define DAT_007027c2 (*(char *)0x007027c2u)
#define DAT_007027c3 (*(char *)0x007027c3u)
#define DAT_007027c4 (*(char *)0x007027c4u)
#define DAT_007027c5 (*(char *)0x007027c5u)
#define DAT_007057f4 (*(undefined *)0x007057f4u)
#define DAT_00705880 (*(undefined *)0x00705880u)
#define DAT_007059d8 (*(undefined *)0x007059d8u)
#define DAT_00705a74 (*(undefined *)0x00705a74u)
#define DAT_00784c2c (*(undefined *)0x00784c2cu)
#define DAT_00e38ea8 (*(undefined1 *)0x00e38ea8u)
#define DAT_00e38ebc (*(undefined *)0x00e38ebcu)
#define DAT_00ea5b44 (*(undefined4 *)0x00ea5b44u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern void FUN_004cc110();

undefined4 FUN_004e2530(void)

{
  if (DAT_00e38ebc == 0) {
    if (DAT_00e38ea8 == '\0') {
      if ((DAT_007027b8 != '\0') && (DAT_007059d8 < 1)) {
        FUN_004cc110(0x3e,0x11);
        return 1;
      }
      if ((DAT_007027bb != '\0') && (DAT_00705880 < 1)) {
        FUN_004cc110(0x3e,0x12);
        return 1;
      }
      if (DAT_007027ba != '\0') {
        FUN_004cc110(0x3e,0x14);
        return 1;
      }
      if (DAT_007027b9 != '\0') {
        FUN_004cc110(0x3e,0x13);
        return 1;
      }
      FUN_004cc110(0x3e,0x15);
      return 1;
    }
    if (DAT_00e38ea8 == '\x01') {
      if (RESTORE_SLICE1(DAT_007027bc,1) != '\0') {
        FUN_004cc110(0x3e,0x16);
        return 1;
      }
      if ((char)DAT_007027bc != '\0') {
        FUN_004cc110(0x3e,0x17);
        return 1;
      }
      FUN_004cc110(0x3e,0x18);
      return 1;
    }
    if (DAT_00e38ea8 == '\x02') {
      if (((DAT_007027be != '\0') && (DAT_00705a74 < 1)) && (DAT_007057f4 < 1)) {
        FUN_004cc110(0x3e,0x19);
        return 1;
      }
      if (DAT_007027c1 != '\0') {
        FUN_004cc110(0x3e,0x1a);
        return 1;
      }
      if (DAT_007027bf != '\0') {
        FUN_004cc110(0x3e,0x1b);
        return 1;
      }
      FUN_004cc110(0x3e,0x1c);
      return 1;
    }
    if (DAT_00e38ea8 == '\x03') {
      if (DAT_007027c2 == '\0') {
        FUN_004cc110(0x3e,0x21);
        return 1;
      }
    }
    else if ((DAT_00e38ea8 == '\x04') && ((&DAT_00ea5b44)[DAT_00eaac28 * 0x1276] != DAT_00784c2c)) {
      if (DAT_007027c5 != '\0') {
        FUN_004cc110(0x3e,0x20);
        return 1;
      }
      if (DAT_007027c4 != '\0') {
        FUN_004cc110(0x3e,0x1d);
        return 1;
      }
      if (DAT_007027c3 != '\0') {
        FUN_004cc110(0x3e,0x1e);
        return 1;
      }
      FUN_004cc110(0x3e,0x1f);
      return 1;
    }
  }
  return 0;
}
