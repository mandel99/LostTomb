#include "restore_types.h"
#define DAT_01266420 (*(undefined4 *)0x01266420u)
#define DAT_01266428 (*(undefined4 *)0x01266428u)
#define DAT_0126642c (*(undefined4 *)0x0126642cu)
extern void FUN_005644e0();
extern void FUN_00564507();
extern void FUN_00568d11();
extern undefined * FUN_00568d88();

void FUN_00564445(undefined8 *param_1,int param_2,int param_3,undefined4 param_4)

{
  char *pcVar1;
  char *pcVar2;
  
  DAT_01266420 = (int *)FUN_00568d88(*param_1);
  DAT_01266428 = ((undefined4 *)&DAT_01266420)[1] + -1;
  pcVar1 = (char *)((uint)(*(undefined4 *)(DAT_01266420) == 0x2d) + param_2);
  FUN_00568d11(pcVar1,param_3,DAT_01266420);
  DAT_0126642c = DAT_01266428 < ((undefined4 *)&DAT_01266420)[1] + -1;
  DAT_01266428 = ((undefined4 *)&DAT_01266420)[1] + -1;
  if ((DAT_01266428 < -4) || (param_3 <= DAT_01266428)) {
    FUN_005644e0(param_1,param_2,param_3,param_4);
  }
  else {
    if ((bool)DAT_0126642c) {
      do {
        pcVar2 = pcVar1;
        pcVar1 = pcVar2 + 1;
      } while (*pcVar2 != '\0');
      pcVar2[-1] = '\0';
    }
    FUN_00564507(param_1,param_2,param_3);
  }
  return;
}
