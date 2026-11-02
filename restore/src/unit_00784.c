#include "restore_types.h"
#define DAT_007854a8 (*(undefined4 *)0x007854a8u)
#define DAT_00785d70 (*(undefined *)0x00785d70u)
#define PTR_LAB_0044feb0 (*(unsigned char * *)0x0044feb0u)
extern undefined4 FUN_0044f120();
extern undefined4 FUN_0044f470();
extern int FUN_0044f660();
extern undefined4 FUN_0044fec0();

undefined4 FUN_0044fd30(int param_1)

{
  uint uVar1;
  int iVar2;
  int iVar3;
  undefined4 uVar4;
  int in_ECX;
  int iVar5;
  
  switch((int)(param_1)) {
  case 0: ;
    iVar5 = (int)*(short *)(in_ECX + 0x24);
    uVar1 = (uint)*(byte *)(in_ECX + 0x6a);
    break;
  case 1: ;
    iVar5 = (int)*(short *)(in_ECX + 0x26);
    uVar1 = (uint)*(byte *)(in_ECX + 0x6b);
    break;
  case 2: ;
    iVar5 = (int)*(short *)(in_ECX + 0x52);
    uVar1 = (uint)*(byte *)(in_ECX + 0x6c);
    break;
  case 3: ;
    iVar5 = (int)*(short *)(in_ECX + 0x54);
    uVar1 = (uint)*(byte *)(in_ECX + 0x6d);
    break;
default: ;
    goto switchD_0044fd43_default;
  }
  if ((6 < uVar1) || (uVar1 == 6)) {
    if (iVar5 < 0) {
      return 0xffffffff;
    }
    if (DAT_00785d70 <= iVar5) {
      return 0xffffffff;
    }
    if (iVar5 * 0x7c == -0x785d74) {
      return 0xffffffff;
    }
    iVar2 = FUN_0044f120(param_1);
    iVar3 = FUN_0044f470();
    if ((((iVar2 < 0) || (0x35 < iVar2)) || (iVar3 < 0)) || (0x23 < iVar3)) goto LAB_0044fe31;
    uVar1 = (uint)(byte)(&DAT_007854a8)[iVar2 * 0x24 + iVar3];
  }
  if (((2 < uVar1) && (uVar1 < 6)) &&
     ((-1 < iVar5 && ((iVar5 < DAT_00785d70 && (iVar5 * 0x7c != -0x785d74)))))) {
    uVar4 = FUN_0044f660(uVar1);
    uVar4 = FUN_0044fec0(uVar4,uVar1);
    return uVar4;
  }
  if (5 < uVar1) {
switchD_0044fd43_default: ;
    return 0xffffffff;
  }
LAB_0044fe31: ;
                    /* WARNING: Could not recover jumptable at 0x0044fe31. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  uVar4 = (*(code *)(&PTR_LAB_0044feb0)[param_1])();
  return uVar4;
}
