#include "restore_types.h"
#define DAT_005df818 (*(undefined4 *)0x005df818u)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_00799a82 (*(undefined4 *)0x00799a82u)
#define DAT_00bc9738 (*(undefined *)0x00bc9738u)
#define DAT_00bc973c (*(undefined *)0x00bc973cu)
extern void FUN_0047c480();

void FUN_004bfee0(int param_1)

{
  short sVar1;
  int iVar2;
  int iVar3;
  int *piVar4;
  int iVar5;
  
  piVar4 = (int *)&DAT_005df818;
  iVar5 = (int)*(short *)((int)&DAT_007999dc + param_1 * 0x108 + 2);
  sVar1 = *(short *)(&DAT_007999e0 + param_1 * 0x108);
  do {
    if (*(char *)(&DAT_00799a82 + param_1 * 0x84) == '\0') {
      iVar2 = piVar4[1];
      iVar3 = *piVar4;
LAB_004bff2b: ;
      iVar2 = iVar2 + iVar5;
      iVar3 = iVar3 + sVar1;
    }
    else {
      iVar2 = iVar5;
      iVar3 = (int)sVar1;
      if (*(char *)(&DAT_00799a82 + param_1 * 0x84) == '\x01') {
        iVar2 = *piVar4;
        iVar3 = piVar4[1];
        goto LAB_004bff2b;
      }
    }
    DAT_00bc9738 = piVar4[-1];
    DAT_00bc973c = DAT_00bc9738;
    FUN_0047c480(param_1,iVar2,iVar3);
    piVar4 = piVar4 + 4;
    if (0x5df857 < (int)piVar4) {
      return;
    }
  } while( true );
}
