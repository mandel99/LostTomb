#include "restore_types.h"
#include "menu_layout.h"
#define DAT_00e38dda (*(uint8_t *)0x00e38ddau)
#define DAT_00e94d9c (*(int32_t *)0x00e94d9cu)
extern void FUN_004cce00();
extern void FUN_004d5d90();
extern void FUN_004d6090();

void FUN_004d6860(undefined4 param_1,undefined4 param_2,undefined4 param_3,int param_4,int param_5,
                 undefined4 param_6,undefined4 param_7,int param_8,int param_9)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  
  if (DAT_00e94d9c == 0) {
    FUN_004d5d90();
  }
  iVar4 = param_5 - param_9;
  iVar3 = param_4 - param_8;
  uVar1 = FUN_004d5d70(DAT_00e38dda);
  iVar2 = (int)(uintptr_t)FUN_004d5fe0(1,uVar1,iVar3,iVar4);
  if (iVar2 != 0) {
    iVar3 = *(int *)(iVar2 + 8);
    iVar4 = *(int *)(iVar2 + 0xc);
  }
  FUN_004d6090(1,param_4,param_5,0x18,0x18,iVar3,iVar4,param_8,param_9);
  FUN_004cce00(param_1,param_2,param_3,iVar3 + param_8,iVar4 + param_9,param_6,param_7);
  return;
}
