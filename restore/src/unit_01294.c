#include "restore_types.h"
#define DAT_0056f5b8 (*(undefined4 *)0x0056f5b8u)
#define DAT_0078d7c0 (*(undefined1 *)0x0078d7c0u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00d386d0 (*(int *)0x00d386d0u)
extern int FUN_00488030();
extern uint FUN_004894a0();

int FUN_00489240(int param_1)

{
  char cVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int *piVar6;
  int iVar7;
  
  iVar4 = -1;
  iVar5 = -1;
  cVar1 = FUN_00488030(param_1);
  if (cVar1 != '\0') {
    switch((&DAT_007999e8)[param_1 * 0x84]) {
    case 100: ;
      iVar5 = 0;
      break;
    case 0x65: ;
      iVar5 = 1;
      break;
    case 0x66: ;
      iVar5 = 2;
      break;
    case 0x67: ;
      iVar5 = 3;
      break;
    case 0x68: ;
      iVar5 = 4;
      break;
    case 0x69: ;
      iVar5 = 5;
      break;
    case 0xc3: ;
      iVar5 = 8;
      break;
    case 0xc4: ;
      iVar5 = 7;
      break;
    case 0xe0: ;
      iVar5 = 9;
    }
  }
  cVar1 = FUN_004894a0(param_1);
  if (cVar1 == '\0') {
    iVar7 = 0;
    iVar3 = 0xc;
    piVar6 = &DAT_0056f5b8 + iVar5 * 4;
    do {
      iVar5 = *piVar6;
      if (iVar5 == -1) {
        return iVar4;
      }
      iVar2 = iVar5 - DAT_0078d7c0;
      if (iVar2 < 1) {
        iVar2 = iVar2 + 0xc;
      }
      if (iVar2 < iVar3) {
        iVar3 = iVar2;
        iVar4 = iVar5;
      }
      iVar7 = iVar7 + 1;
      piVar6 = piVar6 + 1;
    } while (iVar7 < 4);
    return iVar4;
  }
  return DAT_00d386d0;
}
