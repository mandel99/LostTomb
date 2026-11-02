#include "restore_types.h"
#define DAT_006059f4 (*(int32_t *)0x006059f4u)
#define DAT_006059f8 (*(int32_t *)0x006059f8u)
#define DAT_00e38da4 (*(int32_t *)0x00e38da4u)
#define DAT_00e38da8 (*(int32_t *)0x00e38da8u)
#define DAT_00e38dca (*(byte *)0x00e38dcau)
#define DAT_00e38e00 (*(char *)0x00e38e00u)
#define DAT_00e38e30 (*(int32_t *)0x00e38e30u)
#define DAT_00e38e34 (*(int32_t *)0x00e38e34u)
extern int FUN_004cde40();
extern int FUN_004cde60();

undefined4 FUN_004029d0(void)

{
  int iVar1;
  int iVar2;
  undefined4 uVar3;
  
  if (DAT_006059f8 < 0xd) {
    return 0;
  }
  if (DAT_00e38e00 == '\0') {
    return 0;
  }
  iVar1 = DAT_006059f8 + -0xc;
  if (DAT_00e38e34 < DAT_00e38da8 + 0x1d0) {
    return 0;
  }
  if (DAT_00e38da8 + 0x1f0 < DAT_00e38e34) {
    return 0;
  }
  if (DAT_00e38e30 < DAT_00e38da4 + 0x91) {
    return 0;
  }
  if (DAT_00e38da4 + 300 < DAT_00e38e30) {
    return 0;
  }
  iVar2 = (DAT_00e38e30 - DAT_00e38da4) + -0x91;
  if (0x82 < iVar2) {
    iVar2 = 0x82;
  }
  uVar3 = FUN_004cde60(iVar2,0x82);
  DAT_006059f4 = FUN_004cde40(iVar1,uVar3);
  DAT_00e38dca = 1;
  return 1;
}
