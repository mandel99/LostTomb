#include "restore_types.h"
#define DAT_00e209a0 (*(undefined4 *)0x00e209a0u)
#define DAT_00e34c38 (*(undefined4 *)0x00e34c38u)
#define DAT_00e36b78 (*(undefined4 *)0x00e36b78u)
#define DAT_00e36c70 (*(uint32_t *)0x00e36c70u)
#define DAT_00e36c74 (*(uint32_t *)0x00e36c74u)
extern void * FUN_00561a1d();
extern void FUN_00561a2b();
extern undefined4 FUN_005647eb();
extern int FUN_005660ed();
extern uint FUN_00568eab();
extern int FUN_0056b1b6();

undefined4 FUN_004cec40(undefined4 param_1,undefined4 param_2)

{
  int iVar1;
  int iVar2;
  
  iVar1 = FUN_00568eab(param_1,0x8000);
  if (iVar1 == -1) {
    return 0;
  }
  iVar2 = FUN_0056b1b6(iVar1);
  if (DAT_00e36c70 != (void *)0x0) {
    FUN_00561a2b(DAT_00e36c70);
    DAT_00e36c70 = (void *)0x0;
  }
  DAT_00e36c70 = FUN_00561a1d(iVar2 - 0x1f5cU);
  FUN_005660ed(iVar1,&DAT_00e36b78,0x1c);
  FUN_005660ed(iVar1,&DAT_00e34c38,8000);
  FUN_005660ed(iVar1,DAT_00e36c70,iVar2 - 0x1f5cU);
  FUN_005647eb(iVar1);
  iVar1 = FUN_00568eab(param_2,0x8000);
  if (iVar1 == -1) {
    return 0;
  }
  iVar2 = FUN_0056b1b6(iVar1);
  if (DAT_00e36c74 != (void *)0x0) {
    FUN_00561a2b(DAT_00e36c74);
    DAT_00e36c74 = (void *)0x0;
  }
  DAT_00e36c74 = FUN_00561a1d(iVar2 - 0x13898U);
  FUN_005660ed(iVar1,&DAT_00e209a0,0x13898);
  FUN_005660ed(iVar1,DAT_00e36c74,iVar2 - 0x13898U);
  FUN_005647eb(iVar1);
  return 1;
}
