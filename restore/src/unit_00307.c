#include "restore_types.h"
#define DAT_00704db0 (*(int32_t *)0x00704db0u)
#define DAT_00704db4 (*(int32_t *)0x00704db4u)
#define DAT_00704db8 (*(int32_t *)0x00704db8u)
#define DAT_00e38da4 (*(int32_t *)0x00e38da4u)
#define DAT_00e38da8 (*(int32_t *)0x00e38da8u)
#define DAT_00e38dca (*(byte *)0x00e38dcau)
#define DAT_00e38e00 (*(char *)0x00e38e00u)
#define DAT_00e38e30 (*(int32_t *)0x00e38e30u)
#define DAT_00e38e34 (*(int32_t *)0x00e38e34u)
extern int FUN_004cde40();
extern int FUN_004cde60();

undefined4 FUN_0041f4d0(void)

{
  int iVar1;
  int iVar2;
  undefined4 uVar3;
  int iVar4;
  int iVar5;
  
  if (DAT_00704db4 <= (int)((-(uint)(DAT_00704db0 != 1) & 2) + 0xd)) {
    return 0;
  }
  if (DAT_00e38e00 == '\0') {
    return 0;
  }
  iVar5 = DAT_00704db4 - ((-(uint)(DAT_00704db0 != 1) & 2) + 0xd);
  iVar2 = ((-(uint)(DAT_00704db0 != 1) & 2) + 0xd) * 0x10 + -0x4c;
  iVar4 = (-(uint)(DAT_00704db0 != 1) & 0xffffffe0) + 0xfc;
  if (DAT_00e38e34 < DAT_00e38da8 + 0x118) {
    return 0;
  }
  if (DAT_00e38da8 + 0x138 < DAT_00e38e34) {
    return 0;
  }
  iVar1 = DAT_00e38da4 + iVar4;
  if (DAT_00e38e30 < iVar1 + 0x19) {
    return 0;
  }
  if (iVar1 + 0x32 + iVar2 < DAT_00e38e30) {
    return 0;
  }
  iVar4 = ((DAT_00e38e30 - DAT_00e38da4) - iVar4) + -0x19;
  if (iVar2 < iVar4) {
    iVar4 = iVar2;
  }
  uVar3 = FUN_004cde60(iVar4,iVar2);
  DAT_00704db8 = FUN_004cde40(iVar5,uVar3);
  DAT_00e38dca = 1;
  return 1;
}
