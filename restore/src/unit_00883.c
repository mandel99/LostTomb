#include "restore_types.h"
#define DAT_007900c2 (*(undefined4 *)0x007900c2u)
#define DAT_007900ea (*(undefined4 *)0x007900eau)
#define DAT_00790134 (*(undefined4 *)0x00790134u)
#define DAT_00799a98 (*(undefined4 *)0x00799a98u)
extern int FUN_0045d510();

undefined4 FUN_0045b230(int param_1,int param_2,int param_3)

{
  short sVar1;
  int iVar2;
  uint uVar3;
  
  uVar3 = (uint)*(byte *)((int)&DAT_00799a98 + param_1 * 0x108 + 1);
  if (*(char *)((int)&DAT_007900c2 + uVar3 * 0xc4 + param_3) == '\0') {
    sVar1 = *(short *)((int)&DAT_007900ea + (param_3 + uVar3 * 0x62) * 2 + 2);
  }
  else {
    sVar1 = *(short *)(&DAT_00790134 + (param_3 + uVar3 * 0x62) * 2);
  }
  iVar2 = FUN_0045d510(param_1,param_3);
  return CONCAT31((int3)((uint)(iVar2 + param_2) >> 8),(int)sVar1 < iVar2 + param_2);
}
