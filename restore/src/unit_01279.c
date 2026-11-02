#include "restore_types.h"
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00799a80 (*(undefined4 *)0x00799a80u)
extern int FUN_00488030();
extern int FUN_00489110();
extern longlong FUN_0056133c();

int FUN_00488140(int param_1)

{
  char cVar1;
  int iVar2;
  
  FUN_00489110(param_1);
  if (*(char *)((int)&DAT_00799a80 + param_1 * 0x108 + 1) == '\0') goto LAB_00488220;
  cVar1 = FUN_00488030(param_1);
  if (cVar1 != '\0') {
    switch((&DAT_007999e8)[param_1 * 0x84]) {
default: ;
      break;
    case 0x66: ;
      goto switchD_004881e3_caseD_66;
    }
  }
  cVar1 = FUN_00488030(param_1);
  if (cVar1 != '\0') {
    switch((&DAT_007999e8)[param_1 * 0x84]) {
    case 100: ;
      goto switchD_004881e3_caseD_66;
    }
  }
LAB_00488220: ;
  iVar2 = FUN_0056133c();
  if (iVar2 < 1) {
    iVar2 = 1;
  }
  return iVar2;
switchD_004881e3_caseD_66: ;
  goto LAB_00488220;
}
