#include "restore_types.h"
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00799a2c (*(undefined4 *)0x00799a2cu)
#define DAT_00f55bb4 (*(undefined4 *)0x00f55bb4u)
#define DAT_00f564dc (*(undefined *)0x00f564dcu)
#define DAT_00f56764 (*(undefined *)0x00f56764u)
extern void FUN_0047bf50();
extern int FUN_00488030();
extern void FUN_00488690();
extern void FUN_00488840();
extern uint FUN_004894a0();
extern uint FUN_004894d0();
extern longlong FUN_0056133c();

void FUN_00488c20(int param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  short sVar1;
  char cVar2;
  int iVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  int local_28 [10];
  
  local_28[6] = 0x180;
  local_28[7] = 0x180;
  local_28[8] = 0x180;
  local_28[0] = 0x171;
  local_28[1] = 0x1cf;
  local_28[2] = 0x145;
  local_28[3] = 0x11a;
  local_28[4] = 0x142;
  local_28[5] = 0x144;
  local_28[9] = 0x2cb;
  if ((&DAT_007999e8)[param_1 * 0x84] == 0xc3) {
    uVar5 = 2;
    uVar4 = 2;
    uVar6 = DAT_00f564dc;
  }
  else {
    if ((&DAT_007999e8)[param_1 * 0x84] != 0xc2) {
      iVar3 = -1;
      cVar2 = FUN_00488030(param_1);
      if (cVar2 != '\0') {
        switch((&DAT_007999e8)[param_1 * 0x84]) {
        case 100: ;
          iVar3 = 0;
          break;
        case 0x65: ;
          iVar3 = 1;
          break;
        case 0x66: ;
          iVar3 = 2;
          break;
        case 0x67: ;
          iVar3 = 3;
          break;
        case 0x68: ;
          iVar3 = 4;
          break;
        case 0x69: ;
          iVar3 = 5;
          break;
        case 0xc3: ;
          iVar3 = 8;
          break;
        case 0xc4: ;
          iVar3 = 7;
          break;
        case 0xe0: ;
          iVar3 = 9;
        }
      }
      uVar6 = (&DAT_00f55bb4)[local_28[iVar3] * 2];
      sVar1 = (&DAT_00799a2c)[param_1 * 0x84];
      iVar3 = FUN_0056133c();
      cVar2 = FUN_004894a0(param_1);
      if (cVar2 == '\0') {
        FUN_00488690(param_1,param_2,param_3,uVar6,(int)sVar1 / iVar3,param_4);
      }
      else {
        cVar2 = FUN_004894d0(param_1);
        if (cVar2 == '\0') {
          FUN_00488840(param_1,param_2,param_3,uVar6,(int)sVar1 / iVar3,param_4);
          return;
        }
      }
      return;
    }
    uVar5 = 3;
    uVar4 = 3;
    uVar6 = DAT_00f56764;
  }
  FUN_0047bf50(param_1,param_2,param_3,uVar4,uVar5,uVar6,8);
  return;
}
