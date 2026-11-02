#include "restore_types.h"
#include "menu_layout.h"
#define DAT_00e38dda (*(uint8_t *)0x00e38ddau)
#define DAT_00e38e30 (*(int32_t *)0x00e38e30u)
#define DAT_00e38e34 (*(int32_t *)0x00e38e34u)
#define DAT_00e94d98 (*(int32_t *)0x00e94d98u)
#define DAT_00e94d9c (*(int32_t *)0x00e94d9cu)
#define _DAT_00e945f8 (*(undefined4 *)0x00e945f8u)
#define _DAT_00e945fc (*(undefined4 *)0x00e945fcu)
#define _DAT_00e94600 (*(undefined4 *)0x00e94600u)
#define _DAT_00e94604 (*(undefined4 *)0x00e94604u)
#define _DAT_00e94608 (*(undefined4 *)0x00e94608u)
#define _DAT_00e9460c (*(undefined4 *)0x00e9460cu)
extern void FUN_004d5d90();
extern void FUN_004d6050();
extern void FUN_00536a30();
extern void FUN_00536c30();

void FUN_004d65b0(int param_1,int param_2,int param_3,int param_4,int param_5,int param_6)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  
  if (DAT_00e94d9c == 0) {
    FUN_004d5d90();
  }
  param_1 = param_1 - param_5;
  param_2 = param_2 - param_6;
  uVar1 = FUN_004d5d70(DAT_00e38dda);
  iVar2 = (int)(uintptr_t)FUN_004d5fe0(4,uVar1,param_1,param_2);
  iVar3 = param_2;
  iVar5 = param_1;
  if (iVar2 != 0) {
    iVar5 = *(int *)(iVar2 + 8);
    iVar3 = *(int *)(iVar2 + 0xc);
    param_3 = *(int *)(iVar2 + 0x10);
    param_4 = *(int *)(iVar2 + 0x14);
  }
  param_5 = iVar5 + param_5;
  param_6 = iVar3 + param_6;
  if ((((DAT_00e94d98 == 4) && (param_5 < DAT_00e38e34)) &&
      (iVar2 = param_3 * 0x10, DAT_00e38e34 < param_5 + iVar2)) &&
     ((param_6 < DAT_00e38e30 && (iVar4 = param_4 * 0x10, DAT_00e38e30 < iVar4 + param_6)))) {
    FUN_004d6050();
    _DAT_00e94608 = param_3;
    _DAT_00e9460c = param_4;
    _DAT_00e945f8 = param_1;
    _DAT_00e945fc = param_2;
    _DAT_00e94600 = iVar5;
    _DAT_00e94604 = iVar3;
    FUN_00536c30(1,param_5,param_6,iVar2,iVar4,0xffff);
    FUN_00536c30(0,param_5,param_6,iVar2,iVar4,0xffff);
  }
  FUN_00536a30(param_5,param_6,param_3,param_4);
  return;
}
