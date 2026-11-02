#include "restore_types.h"
#define DAT_00799a12 (*(undefined4 *)0x00799a12u)
#define DAT_00799aac (*(undefined4 *)0x00799aacu)
extern void FUN_004405c0();
extern void FUN_0047bf50();

void FUN_00440540(int param_1,int param_2,int param_3)

{
  short sVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  undefined4 local_4;
  
  iVar2 = param_1;
  iVar3 = (&DAT_00799aac)[param_1 * 0x42];
  sVar1 = *(short *)(&DAT_00799a12 + param_1 * 0x108);
  FUN_004405c0(iVar3,&param_1,&local_4);
  if (iVar3 == 0) {
    iVar3 = param_2;
    iVar4 = param_3 + 1;
  }
  else {
    iVar3 = param_2 + 1;
    iVar4 = param_3;
  }
  FUN_0047bf50(iVar2,param_2,param_3,1,1,param_1,8);
  FUN_0047bf50((int)sVar1,iVar3,iVar4,1,1,local_4,8);
  return;
}
