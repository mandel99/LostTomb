#include "restore_types.h"
#define DAT_00603238 (*(undefined4 *)0x00603238u)
#define DAT_00799a30 (*(undefined4 *)0x00799a30u)
#define DAT_00f56d74 (*(undefined *)0x00f56d74u)
extern int FUN_0053e590();

void FUN_00542e10(int param_1,int param_2,int param_3)

{
  int iVar1;
  uint uVar2;
  int *piVar3;
  int iVar4;
  
  switch((&DAT_00799a30)[param_1 * 0x42]) {
default: ;
    iVar4 = 0;
    break;
  case 1: ;
  case 10: ;
  case 0xb: ;
    iVar4 = 1;
    break;
  case 2: ;
  case 8: ;
  case 9: ;
    iVar4 = 2;
    break;
  case 3: ;
  case 6: ;
  case 7: ;
    iVar4 = 3;
    break;
  case 4: ;
  case 5: ;
    iVar4 = 4;
  }
  uVar2 = 0;
  if (iVar4 != 0) {
    piVar3 = &DAT_00603238;
    do {
      if ((uVar2 & 1) == 0) {
        iVar1 = DAT_00f56d74 + 0xd;
      }
      else {
        iVar1 = DAT_00f56d74 + 0xe;
      }
      FUN_0053e590(iVar1,param_2 + *piVar3,piVar3[1] + param_3);
      uVar2 = uVar2 + 1;
      piVar3 = piVar3 + 2;
    } while ((int)uVar2 < iVar4);
  }
  return;
}
