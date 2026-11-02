#include "restore_types.h"
#define DAT_00775900 (*(undefined *)0x00775900u)
#define DAT_00785d70 (*(undefined *)0x00785d70u)
#define DAT_00785d76 (*(undefined4 *)0x00785d76u)
#define DAT_00785d78 (*(undefined4 *)0x00785d78u)
#define DAT_00e950dc (*(undefined *)0x00e950dcu)
extern void FUN_0044e6f0();
extern uint FUN_0044ef60();
extern void FUN_004e0cf0();

void FUN_004463c0(int param_1,undefined4 param_2)

{
  char cVar1;
  undefined2 local_20;
  undefined2 local_1e;
  short local_10;
  
  if ((((-1 < param_1) && (param_1 < DAT_00785d70)) && (param_1 * 0x7c != -0x785d74)) &&
     ((&DAT_00785d76)[param_1 * 0x7c] == '\x02')) {
    switch((&DAT_00785d78)[param_1 * 0x3e]) {
    case 1: ;
      local_1e = 0xb6;
      local_10 = 0xbe;
      break;
    case 2: ;
      local_1e = 0xac;
      cVar1 = FUN_0044ef60();
      local_10 = (cVar1 != '\0') + 0xb4;
      break;
    case 3: ;
      local_1e = 0xa2;
      cVar1 = FUN_0044ef60();
      local_10 = (cVar1 != '\0') + 0xaa;
      break;
    case 4: ;
      local_1e = 0xff;
      local_10 = 0x107;
      break;
default: ;
      goto switchD_00446480_caseD_3;
    }
    FUN_0044e6f0(&local_20);
    switch((&DAT_00785d78)[param_1 * 0x3e]) {
    case 1: ;
      local_20 = 0x128;
      break;
    case 2: ;
      local_20 = 0x127;
      break;
default: ;
      goto switchD_00446480_caseD_3;
    case 4: ;
      local_20 = 0x129;
    }
    DAT_00e950dc = 1;
    FUN_004e0cf0(0x85,(int)DAT_00775900,param_2,&local_20,0);
  }
switchD_00446480_caseD_3: ;
  return;
}
