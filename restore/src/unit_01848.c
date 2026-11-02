#include "restore_types.h"
#define DAT_00e38e30 (*(int32_t *)0x00e38e30u)
#define DAT_00e38e34 (*(int32_t *)0x00e38e34u)
#define DAT_00e94d98 (*(int32_t *)0x00e94d98u)
#define _DAT_00e945f8 (*(undefined4 *)0x00e945f8u)
#define _DAT_00e945fc (*(undefined4 *)0x00e945fcu)
#define _DAT_00e94600 (*(undefined4 *)0x00e94600u)
#define _DAT_00e94604 (*(undefined4 *)0x00e94604u)
#define _DAT_00e94608 (*(undefined4 *)0x00e94608u)
#define _DAT_00e9460c (*(undefined4 *)0x00e9460cu)
extern void FUN_004d6050();
extern void FUN_00536c30();

void FUN_004d6090(int param_1,int param_2,int param_3,int param_4,int param_5,int param_6,
                 int param_7,int param_8,int param_9)

{
  int iVar1;
  int iVar2;
  
  iVar1 = param_6 + param_8;
  iVar2 = param_7 + param_9;
  if ((((param_1 == DAT_00e94d98) && (iVar1 < DAT_00e38e34)) && (DAT_00e38e34 < iVar1 + param_4)) &&
     ((iVar2 < DAT_00e38e30 && (DAT_00e38e30 < param_5 + iVar2)))) {
    FUN_004d6050();
    _DAT_00e94600 = param_6;
    _DAT_00e945f8 = param_2 - param_8;
    _DAT_00e945fc = param_3 - param_9;
    _DAT_00e94604 = param_7;
    _DAT_00e94608 = param_4;
    _DAT_00e9460c = param_5;
    FUN_00536c30(1,iVar1,iVar2,param_4,param_5,0xffff);
    FUN_00536c30(0,iVar1,iVar2,param_4,param_5,0xffff);
  }
  return;
}
