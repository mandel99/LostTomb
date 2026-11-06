#include "restore_types.h"
#define DAT_00bc96c8 (*(undefined4 *)0x00bc96c8u)
#define DAT_00bca2e0 (*(uint32_t *)0x00bca2e0u)
#define DAT_00d3b198 (*(uint32_t *)0x00d3b198u)
#define DAT_00e38dca (*(byte *)0x00e38dcau)
#define DAT_00e94e04 (*(uint32_t *)0x00e94e04u)
extern void FUN_00430b20();
extern void FUN_00430be0();
extern void FUN_0047d6e0();
extern void FUN_004de080();
extern void FUN_004e2970();

void FUN_00409eb0(void)

{
  if (DAT_00d3b198 == 2) {
    FUN_00430b20();
  }
  else {
    if (DAT_00d3b198 == 0) {
      if (DAT_00bca2e0 != 0) {
        if (DAT_00bca2e0 == 2) {
          DAT_00bc96c8 = 1;
          FUN_00430be0();
          DAT_00bca2e0 = DAT_00e94e04;
        }
        else {
          DAT_00bc96c8 = 0;
          if (DAT_00bca2e0 == 6) {
            FUN_00430b20();
            DAT_00bca2e0 = DAT_00e94e04;
          }
          else {
            FUN_00430b20();
            FUN_00430b20();
            DAT_00bca2e0 = DAT_00e94e04;
            FUN_0047d6e0();
          }
        }
        FUN_0047d6e0();
        FUN_004de080();
        FUN_004e2970(1,0,0);
        DAT_00e38dca = 1;
      }
      return;
    }
    if (DAT_00d3b198 == 1) {
      FUN_00430be0();
    }
  }
  DAT_00bc96c8 = DAT_00d3b198;
  DAT_00bca2e0 = DAT_00e94e04;
  FUN_0047d6e0();
  FUN_004de080();
  FUN_004e2970(1,0,0);
  DAT_00e38dca = 1;
  return;
}
