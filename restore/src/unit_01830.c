#include "restore_types.h"
#include "mission_loaders.h"
#define DAT_00605a3c (*(int32_t *)0x00605a3cu)
#define DAT_007848dc (*(int16_t *)0x007848dcu)
#define DAT_00784c2c (*(int32_t *)0x00784c2cu)
#define DAT_00784f10 (*(int32_t *)0x00784f10u)
extern void FUN_004bc720();
extern void FUN_004ce2d0();
extern void FUN_004f4520();
extern float10 FUN_004f78a0();
extern void FUN_0053d970();
extern undefined4 FUN_005647eb();
extern uint FUN_00568eab();

uint32_t FUN_004d5820(const char *param_1,int param_2)

{
  int iVar1;
  
  FUN_004ce2d0();
  FUN_004bd330(RESTORE_LOAD_SETTINGS);
  iVar1 = FUN_00568eab(param_1,0x8000);
  if (iVar1 == -1) {
    return 1;
  }
  FUN_004d4ae0(iVar1);
  FUN_005647eb(iVar1);
  DAT_00784f10 = (int)DAT_007848dc;
  if (param_2 == 0) {
    FUN_004f4520();
    FUN_0053d970();
    FUN_004bc720();
  }
  if (DAT_00605a3c == 1) {
    DAT_00784c2c = (int32_t)FUN_004f78a0(1);
  }
  return 0;
}
