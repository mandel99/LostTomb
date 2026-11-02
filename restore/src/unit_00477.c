#include "restore_types.h"
#define DAT_0070e440 (*(undefined *)0x0070e440u)
#define DAT_0070e444 (*(undefined *)0x0070e444u)
#define DAT_0070e448 (*(undefined *)0x0070e448u)
#define DAT_0070e44c (*(undefined *)0x0070e44cu)
#define DAT_0070e450 (*(undefined *)0x0070e450u)
#define DAT_0070e454 (*(undefined *)0x0070e454u)
#define DAT_0070e458 (*(undefined *)0x0070e458u)
#define DAT_0070e45c (*(undefined *)0x0070e45cu)
#define DAT_0070e488 (*(undefined *)0x0070e488u)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00b285cc (*(undefined4 *)0x00b285ccu)
#define DAT_00b7482c (*(undefined4 *)0x00b7482cu)
#define DAT_00d3b5c0 (*(undefined *)0x00d3b5c0u)
#define DAT_00f56d14 (*(undefined *)0x00f56d14u)
extern undefined4 FUN_0053f250();

void FUN_00432820(int param_1,int param_2)

{
  int iVar1;
  
  if (((&DAT_00af598c)[DAT_0070e488] & 0x20000002) == 0x20000002) {
    DAT_00d3b5c0 = DAT_00f56d14;
    FUN_0053f250(DAT_00f56d14,param_1,param_2);
    return;
  }
  DAT_00d3b5c0 = (&DAT_00b7482c)[DAT_0070e488];
  switch((&DAT_00b285cc)[DAT_0070e488] & 0xf) {
  case 1: ;
    iVar1 = DAT_0070e45c + 1;
    goto LAB_004328e9;
  case 2: ;
    param_1 = DAT_0070e458 + 2 + param_1;
    iVar1 = DAT_0070e448;
    break;
  case 3: ;
    param_1 = DAT_0070e454 + 3 + param_1;
    iVar1 = DAT_0070e444;
    break;
  case 4: ;
    param_1 = DAT_0070e450 + 4 + param_1;
    iVar1 = DAT_0070e440;
    break;
  case 5: ;
    param_2 = param_2 - DAT_0070e440;
    iVar1 = DAT_0070e450 + DAT_0070e45c + 5;
LAB_004328e9: ;
    param_1 = iVar1 + param_1;
    iVar1 = DAT_0070e44c;
    break;
default: ;
    goto switchD_00432886_default;
  }
  param_2 = param_2 - iVar1;
switchD_00432886_default: ;
  FUN_0053f250(DAT_00d3b5c0,param_1,param_2);
  return;
}
