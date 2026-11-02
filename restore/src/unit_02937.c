#include "restore_types.h"
#define DAT_00604e64 (*(undefined *)0x00604e64u)
#define DAT_0126648c (*(undefined *)0x0126648cu)
#define PTR_DAT_00604c58 (*(unsigned char * *)0x00604c58u)
extern uint FUN_00565f66();
extern int FUN_0056947e();

uint FUN_005676ec(uint param_1)

{
  uint uVar1;
  uint uVar2;
  int iVar3;
  undefined4 uVar4;
  uint local_8;
  
  uVar1 = param_1;
  if (DAT_0126648c == 0) {
    if ((0x60 < (int)param_1) && ((int)param_1 < 0x7b)) {
      uVar1 = param_1 - 0x20;
    }
  }
  else {
    if ((int)param_1 < 0x100) {
      if (DAT_00604e64 < 2) {
        uVar2 = (byte)((undefined4 *)&PTR_DAT_00604c58)[param_1 * 2] & 2;
      }
      else {
        uVar2 = FUN_00565f66(param_1,2);
      }
      if (uVar2 == 0) {
        return uVar1;
      }
    }
    if ((((undefined4 *)&PTR_DAT_00604c58)[((int)uVar1 >> 8 & 0xffU) * 2 + 1] & 0x80) == 0) {
      param_1 = CONCAT31((int3)(param_1 >> 8),(char)uVar1) & 0xffff00ff;
      uVar4 = 1;
    }
    else {
      uVar2 = param_1 >> 0x10;
      RESTORE_SLICE2(param_1,0) = CONCAT11((char)uVar1,(char)(uVar1 >> 8));
      param_1 = CONCAT22((short)uVar2,(undefined2)param_1) & 0xff00ffff;
      uVar4 = 2;
    }
    iVar3 = FUN_0056947e(DAT_0126648c,0x200,&param_1,uVar4,&local_8,3,0,1);
    if (iVar3 != 0) {
      if (iVar3 == 1) {
        uVar1 = local_8 & 0xff;
      }
      else {
        uVar1 = local_8 & 0xffff;
      }
    }
  }
  return uVar1;
}
