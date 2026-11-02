#include "restore_types.h"
#define DAT_005df78c (*(undefined4 *)0x005df78cu)
#define DAT_005df814 (*(undefined4 *)0x005df814u)
#define DAT_005df818 (*(undefined4 *)0x005df818u)
#define DAT_005df81c (*(undefined4 *)0x005df81cu)
#define DAT_00bca2e0 (*(undefined *)0x00bca2e0u)
extern void FUN_0047d360();

void FUN_004bfa50(int param_1,int param_2,int param_3)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int *piVar5;
  int local_4;
  
  local_4 = 4;
  piVar5 = (int *)(&DAT_005df78c + param_3 * 0x10);
  do {
    iVar1 = *piVar5;
    if (param_3 == 0) {
      iVar4 = *(int *)(&DAT_005df81c + iVar1 * 0x10);
      iVar2 = *(int *)(&DAT_005df818 + iVar1 * 0x10);
LAB_004bfaae: ;
      iVar3 = iVar2 + param_2;
      iVar2 = iVar4 + param_1;
    }
    else {
      iVar3 = param_2;
      iVar2 = param_1;
      if (param_3 == 1) {
        iVar4 = *(int *)(&DAT_005df818 + iVar1 * 0x10);
        iVar2 = *(int *)(&DAT_005df81c + iVar1 * 0x10);
        goto LAB_004bfaae;
      }
    }
    if (DAT_00bca2e0 != 0) {
      if (DAT_00bca2e0 == 2) {
        iVar2 = iVar2 + -1 + (&DAT_005df814)[iVar1 * 4];
      }
      else {
        if (DAT_00bca2e0 == 4) {
          iVar4 = (&DAT_005df814)[iVar1 * 4];
          iVar2 = iVar2 + -1 + iVar4;
        }
        else {
          iVar4 = (&DAT_005df814)[iVar1 * 4];
        }
        iVar3 = iVar3 + -1 + iVar4;
      }
    }
    FUN_0047d360((&DAT_005df814)[iVar1 * 4],iVar2,iVar3,0xffffffbf,0,0);
    piVar5 = piVar5 + 1;
    local_4 = local_4 + -1;
    if (local_4 == 0) {
      return;
    }
  } while( true );
}
