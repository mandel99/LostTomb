#include "restore_types.h"
#define DAT_00e2078c (*(int32_t *)0x00e2078cu)
#define DAT_00e20790 (*(int32_t *)0x00e20790u)
#define DAT_00e20794 (*(int32_t *)0x00e20794u)
#define DAT_00e2079b (*(char *)0x00e2079bu)
#define DAT_00e92e0c (*(uint32_t *)0x00e92e0cu)
#define DAT_00f53fe4 (*(uint32_t *)0x00f53fe4u)
#define DAT_00f53fe8 (*(int32_t *)0x00f53fe8u)
extern void FUN_004cd510();
extern void FUN_004cd9d0();

void FUN_00537140(int param_1,int param_2,undefined4 param_3)

{
  int iVar1;
  int32_t elapsed=(int32_t)(DAT_00e92e0c-DAT_00f53fe4);
  
  if (DAT_00f53fe8 == 0) {
    if (elapsed < 200) {
      return;
    }
    DAT_00f53fe4 = DAT_00e92e0c;
    DAT_00f53fe8 = 1;
  }
  else if ((DAT_00f53fe8 == 1) && (399 < elapsed)) {
    DAT_00f53fe4 = DAT_00e92e0c;
    DAT_00f53fe8 = 0;
    return;
  }
  if (DAT_00e2079b == '\0') {
    FUN_004cd9d0(DAT_00e20790 + param_1,DAT_00e20794 + 0xe + param_2,DAT_00e2078c,2,param_3);
    return;
  }
  iVar1 = DAT_00e20794 + -3 + param_2;
  FUN_004cd510(DAT_00e20790 + -3 + param_1,iVar1,DAT_00e20790 + 1 + param_1,iVar1,param_3);
  iVar1 = DAT_00e20790 + -1 + param_1;
  FUN_004cd510(iVar1,DAT_00e20794 + -3 + param_2,iVar1,DAT_00e20794 + 0xd + param_2,param_3);
  param_2 = DAT_00e20794 + 0xe + param_2;
  FUN_004cd510(DAT_00e20790 + -3 + param_1,param_2,DAT_00e20790 + 1 + param_1,param_2,param_3);
  return;
}
