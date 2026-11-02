#include "restore_types.h"
#define DAT_006057e4 (*(undefined *)0x006057e4u)
#define DAT_00e38ef0 (*(undefined *)0x00e38ef0u)
extern void FUN_0044c1a0();
extern void FUN_004e2970();

undefined4 FUN_004e0f90(int param_1,short *param_2)

{
  undefined4 uVar1;
  
  uVar1 = 1;
  if (param_1 == 0x84) {
    return uVar1;
  }
  if (param_1 == 0x15d) {
    if ((DAT_00e38ef0 & 2) != 0) {
      FUN_004e2970(0xdb,0,0);
      return 0;
    }
  }
  else if (param_1 == 0x15c) {
    if ((DAT_00e38ef0 & 2) != 0) {
      FUN_004e2970(0xdc,0,0);
      return 0;
    }
  }
  else if (param_1 == 0x15b) {
    if ((DAT_00e38ef0 & 2) != 0) {
      FUN_004e2970(0xdd,0,0);
      return 0;
    }
  }
  else {
    if (param_1 != 0x156) {
      if (param_1 != 0x15a) {
        if (param_1 != 0x157) {
          if (param_1 != 0x159) {
            if (param_1 == 0x158) {
              if ((DAT_00e38ef0 & 2) == 0) {
                return 1;
              }
              FUN_004e2970(0xe0,0,0);
              return 0;
            }
            if (param_1 == 0x115) {
              if ((DAT_00e38ef0 & 0x80) == 0) {
                return 1;
              }
              FUN_004e2970(0xe1,0,0);
              return 0;
            }
            if (param_1 == 0x116) {
              if ((DAT_00e38ef0 & 0x80) == 0) {
                return 1;
              }
              FUN_004e2970(0xe2,0,0);
              return 0;
            }
            if (param_1 == 0x10c) {
              if ((DAT_00e38ef0 & 0x200) == 0) {
                return 1;
              }
              FUN_004e2970(0xe3,0,0);
              return 0;
            }
            if (param_1 == 0x10b) {
              if ((DAT_00e38ef0 & 0x200) == 0) {
                return 1;
              }
              FUN_004e2970(0xe4,0,0);
              return 0;
            }
            if (param_1 == 0x112) {
              if ((DAT_00e38ef0 & 0x100) == 0) {
                return 1;
              }
              FUN_004e2970(0xe5,0,0);
              return 0;
            }
            if (param_1 == 0x111) {
              if ((DAT_00e38ef0 & 0x100) == 0) {
                return 1;
              }
              FUN_004e2970(0xe6,0,0);
              return 0;
            }
            if (param_1 == 0x83) {
              if (*param_2 != 0xc0) {
                return 1;
              }
              if ((DAT_00e38ef0 & 0x10) == 0) {
                return 1;
              }
              FUN_004e2970(0xe7,0,0);
              return 0;
            }
            if (((99 < param_1) && (param_1 < 0x6e)) && ((DAT_00e38ef0 & 4) != 0)) {
              FUN_004e2970(0xe8,0,0);
              return 0;
            }
            if ((((0x140 < param_1) && (param_1 < 0x146)) || (param_1 == 0x14b)) &&
               ((DAT_00e38ef0 & 0x40) != 0)) {
              FUN_004e2970(0xe9,0,0);
              return 0;
            }
            if (((0x88 < param_1) && (param_1 < 0x8c)) && ((DAT_00e38ef0 & 0x20) != 0)) {
              FUN_004e2970(0xea,0,0);
              return 0;
            }
            if (param_1 != 0x135) {
              if (param_1 != 0x13a) {
                if (param_1 == 0x146) {
                  if ((DAT_00e38ef0 & 0x400) == 0) {
                    return 1;
                  }
                  FUN_004e2970(0xf6,0,0);
                  return 0;
                }
                if (param_1 == 0x148) {
                  if ((DAT_00e38ef0 & 0x800) == 0) {
                    return 1;
                  }
                  FUN_004e2970(0xf7,0,0);
                  return 0;
                }
                if (param_1 != 0x88) {
                  return 1;
                }
                if ((DAT_00e38ef0 & 0x1000) == 0) {
                  return 1;
                }
                FUN_004e2970(0x10f,0,0);
                return 0;
              }
              if ((DAT_00e38ef0 & 8) == 0) {
                return uVar1;
              }
            }
            FUN_004e2970(0xeb,0,0);
            FUN_0044c1a0(DAT_006057e4);
            return 0;
          }
          if ((DAT_00e38ef0 & 2) == 0) {
            return uVar1;
          }
        }
        FUN_004e2970(0xdf,0,0);
        return 0;
      }
      if ((DAT_00e38ef0 & 2) == 0) {
        return uVar1;
      }
    }
    FUN_004e2970(0xde,0,0);
    uVar1 = 0;
  }
  return uVar1;
}
