#include "restore_types.h"
#define DAT_00604018 (*(undefined4 *)0x00604018u)
#define DAT_00604058 (*(undefined4 *)0x00604058u)
#define DAT_00604098 (*(undefined4 *)0x00604098u)
#define DAT_006040a8 (*(undefined4 *)0x006040a8u)
#define DAT_006040c8 (*(undefined4 *)0x006040c8u)
#define DAT_006040d8 (*(undefined4 *)0x006040d8u)
#define DAT_006040e8 (*(undefined4 *)0x006040e8u)
extern uint FUN_0055cd50();
extern void FUN_0055d110();
extern void FUN_0055d150();
extern void FUN_0055d250();

byte FUN_0055cbd0(undefined4 param_1,undefined4 param_2,int param_3,undefined4 param_4)

{
  undefined4 *puVar1;
  uint uVar2;
  int iVar3;
  
  puVar1 = (undefined4 *)(param_3 + 0x1c);
  *(undefined4 *)(param_3 + 0x28) = param_1;
  *(undefined4 *)(param_3 + 0x2c) = param_2;
  *(undefined4 *)(param_3 + 0x24) = param_4;
  *puVar1 = 0x800;
  uVar2 = (**(code **)(param_3 + 0x28))
                    ((byte *)(param_3 + 0x2234),puVar1,*(undefined4 *)(param_3 + 0x24));
  *(uint *)(param_3 + 0x20) = uVar2;
  if (uVar2 < 5) {
    return 3;
  }
  *(uint *)(param_3 + 4) = (uint)*(byte *)(param_3 + 0x2234);
  *(uint *)(param_3 + 0xc) = (uint)*(byte *)(param_3 + 0x2235);
  *(uint *)(param_3 + 0x14) = (uint)*(byte *)(param_3 + 0x2236);
  *(undefined4 *)(param_3 + 0x18) = 0;
  *puVar1 = 3;
  uVar2 = *(uint *)(param_3 + 0xc);
  if ((3 < uVar2) && (uVar2 < 7)) {
    *(int *)(param_3 + 0x10) = 0xffff >> (0x10U - (char)uVar2 & 0x1f);
    if (*(int *)(param_3 + 4) != 0) {
      if (*(int *)(param_3 + 4) != 1) {
        return 2;
      }
      FUN_0055d250(param_3 + 0x2fb4,&DAT_006040e8,0x100);
      FUN_0055d150(param_3);
    }
    FUN_0055d250(param_3 + 0x30f4,&DAT_006040c8,0x10);
    FUN_0055d110(0x10,param_3 + 0x30f4,&DAT_006040d8,param_3 + 0x2b34);
    FUN_0055d250(param_3 + 0x3104,&DAT_00604098,0x10);
    FUN_0055d250(param_3 + 0x3114,&DAT_006040a8,0x20);
    FUN_0055d250(param_3 + 0x30b4,&DAT_00604018,0x40);
    FUN_0055d110(0x40,param_3 + 0x30b4,&DAT_00604058,param_3 + 0x2a34);
    iVar3 = FUN_0055cd50(param_3);
    return -(iVar3 == 0x306) & 4;
  }
  return 1;
}
