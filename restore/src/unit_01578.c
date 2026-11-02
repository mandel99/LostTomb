#include "restore_types.h"
#define DAT_005df818 (*(undefined4 *)0x005df818u)
#define DAT_00f56be4 (*(undefined *)0x00f56be4u)
extern undefined4 FUN_0047c880();
extern void FUN_004f0ba0();
extern int FUN_0053f4f0();

void FUN_004c0040(int param_1,int param_2,int param_3)

{
  int iVar1;
  int iVar2;
  int *piVar3;
  int iVar4;
  int iVar5;
  undefined4 uVar6;
  undefined4 local_14;
  undefined4 local_10;
  int local_c;
  int local_8;
  int local_4;
  
  piVar3 = (int *)&DAT_005df818;
  do {
    if (param_3 == 0) {
      iVar5 = piVar3[1];
      iVar1 = *piVar3;
      iVar4 = param_2;
LAB_004c0080: ;
      iVar1 = iVar4 + iVar1;
      iVar5 = param_1 + iVar5;
    }
    else {
      iVar1 = param_2;
      iVar5 = param_1;
      if (param_3 == 1) {
        iVar5 = *piVar3;
        iVar4 = piVar3[1];
        goto LAB_004c0080;
      }
    }
    iVar4 = piVar3[-1];
    local_4 = iVar4;
    FUN_004f0ba0(iVar5,iVar1,&local_10,&local_14);
    if (0 < iVar4) {
      do {
        local_8 = iVar4;
        local_c = local_4;
        iVar4 = iVar1;
        do {
          FUN_004f0ba0(iVar5,iVar4,&local_10,&local_14);
          if ((((char)piVar3[-2] == '\0') ||
              (iVar2 = FUN_0047c880(iVar5,iVar4,1,1,0x40,0,0), iVar2 != 0)) &&
             (iVar2 = FUN_0047c880(iVar5,iVar4,1,1,0xffffffff,0,0), iVar2 == 0)) {
            uVar6 = 0xfffff863;
          }
          else {
            uVar6 = 0x1fe3;
          }
          FUN_0053f4f0(DAT_00f56be4,local_10,local_14,uVar6);
          iVar4 = iVar4 + 1;
          local_c = local_c + -1;
        } while (local_c != 0);
        iVar5 = iVar5 + 1;
        local_8 = local_8 + -1;
        local_c = 0;
        iVar4 = local_8;
      } while (local_8 != 0);
    }
    piVar3 = piVar3 + 4;
    if (0x5df857 < (int)piVar3) {
      return;
    }
  } while( true );
}
