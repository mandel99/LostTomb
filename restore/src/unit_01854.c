#include "restore_types.h"
#include "menu_layout.h"
#define DAT_00e38dda (*(uint8_t *)0x00e38ddau)
#define DAT_00e94d9c (*(int32_t *)0x00e94d9cu)
extern void FUN_004d5d90();
extern void FUN_004d6090();
extern void FUN_00536c30();

void FUN_004d63d0(undefined4 param_1,int param_2,int param_3,undefined4 param_4,undefined4 param_5,
                 undefined4 param_6,int param_7,int param_8)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  
  if (DAT_00e94d9c == 0) {
    FUN_004d5d90();
  }
  iVar4 = param_3 - param_8;
  iVar3 = param_2 - param_7;
  uVar1 = FUN_004d5d70(DAT_00e38dda);
  iVar2 = (int)(uintptr_t)FUN_004d5fe0(3,uVar1,iVar3,iVar4);
  if (iVar2 != 0) {
    param_4 = *(undefined4 *)(iVar2 + 0x10);
    param_5 = *(undefined4 *)(iVar2 + 0x14);
    iVar3 = *(int *)(iVar2 + 8);
    iVar4 = *(int *)(iVar2 + 0xc);
  }
  FUN_004d6090(3,param_2,param_3,param_4,param_5,iVar3,iVar4,param_7,param_8);
  FUN_00536c30(param_1,iVar3 + param_7,iVar4 + param_8,param_4,param_5,param_6);
  return;
}
