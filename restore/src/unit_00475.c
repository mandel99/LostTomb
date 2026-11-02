#include "restore_types.h"
#define DAT_005d0050 (*(undefined4 *)0x005d0050u)
#define DAT_005d0070 (*(undefined4 *)0x005d0070u)
#define DAT_005d00b8 (*(undefined4 *)0x005d00b8u)
#define DAT_005d0138 (*(undefined4 *)0x005d0138u)
#define DAT_005d0200 (*(undefined4 *)0x005d0200u)
#define DAT_0070e440 (*(undefined *)0x0070e440u)
#define DAT_0070e444 (*(undefined *)0x0070e444u)
#define DAT_0070e448 (*(undefined *)0x0070e448u)
#define DAT_0070e44c (*(undefined *)0x0070e44cu)
#define DAT_0070e450 (*(undefined *)0x0070e450u)
#define DAT_0070e454 (*(undefined *)0x0070e454u)
#define DAT_0070e458 (*(undefined *)0x0070e458u)
#define DAT_0070e45c (*(undefined *)0x0070e45cu)
#define DAT_0070e488 (*(undefined *)0x0070e488u)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_00799a4f (*(undefined4 *)0x00799a4fu)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00b285cc (*(undefined4 *)0x00b285ccu)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00b7482c (*(undefined4 *)0x00b7482cu)
#define DAT_00d3b5c0 (*(undefined *)0x00d3b5c0u)
#define DAT_00f56d14 (*(undefined *)0x00f56d14u)
#define DAT_00f56d1c (*(undefined *)0x00f56d1cu)
extern void FUN_00432d30();
extern undefined4 FUN_0053f250();

void FUN_00432320(int param_1,int param_2)

{
  char cVar1;
  uint uVar2;
  int iVar3;
  byte bVar4;
  int *piVar5;
  int iVar6;
  int local_c;
  int local_8;
  undefined4 local_4;
  
  uVar2 = (&DAT_00af598c)[DAT_0070e488];
  if ((uVar2 & 0x26341777) == 0) {
    if ((uVar2 & 8) == 0) {
      if ((char)((uVar2 & 0x2080) >> 8) == '\0') {
        DAT_00d3b5c0 = DAT_00f56d14;
        if ((uVar2 & 0x2080) == 0x80) {
          DAT_00d3b5c0 = DAT_00f56d14 + 1;
        }
      }
      else {
        DAT_00d3b5c0 = DAT_00f56d14 + 2;
      }
      FUN_0053f250(DAT_00d3b5c0,param_1,param_2);
      return;
    }
    iVar3 = *(short *)(&DAT_00b4e6fc + DAT_0070e488 * 2) * 0x108;
    cVar1 = *(char *)((int)&DAT_007999dc + iVar3 + 1);
    if ((cVar1 != '\0') && ((&DAT_00799a4f)[iVar3] != '\0')) {
      FUN_00432d30((int)*(short *)(&DAT_00b4e6fc + DAT_0070e488 * 2),param_1,param_2,0);
      return;
    }
    local_8 = 0;
    local_4 = 0;
    switch((&DAT_00b285cc)[DAT_0070e488] & 0xf) {
default: ;
      piVar5 = &local_8;
      iVar6 = 1;
      break;
    case 1: ;
      piVar5 = &DAT_005d0050;
      iVar6 = 4;
      break;
    case 2: ;
      piVar5 = &DAT_005d0070;
      iVar6 = 9;
      break;
    case 3: ;
      piVar5 = &DAT_005d00b8;
      iVar6 = 0x10;
      break;
    case 4: ;
      piVar5 = &DAT_005d0138;
      iVar6 = 0x19;
      break;
    case 5: ;
      piVar5 = &DAT_005d0200;
      iVar6 = 0x24;
    }
    local_c = DAT_00f56d14;
    if (cVar1 != '\0') {
      local_c = DAT_00f56d1c;
    }
    if (((char)((uVar2 & 0x2080) >> 8) == '\0') &&
       ((cVar1 == '\0' || ((&DAT_00799a4f)[iVar3] == '\0')))) {
      if ((uVar2 & 0x2080) == 0x80) {
        local_c = local_c + 1;
      }
    }
    else {
      local_c = local_c + 2;
    }
    for (; iVar6 != 0; iVar6 = iVar6 + -1) {
      FUN_0053f250(local_c,*piVar5 + param_1,piVar5[1] + param_2);
      piVar5 = piVar5 + 2;
    }
  }
  else if ((uVar2 & 8) == 0) {
    DAT_00d3b5c0 = (&DAT_00b7482c)[DAT_0070e488];
    bVar4 = (&DAT_00b285cc)[DAT_0070e488] & 0xf;
    if (((&DAT_00b285cc)[DAT_0070e488] & 0xf) == 0) {
      FUN_0053f250(DAT_00d3b5c0,param_1,param_2);
      return;
    }
    if (bVar4 == 1) {
      FUN_0053f250(DAT_00d3b5c0,DAT_0070e45c + 1 + param_1,param_2 - DAT_0070e44c);
      return;
    }
    if (bVar4 == 2) {
      FUN_0053f250(DAT_00d3b5c0,DAT_0070e458 + 2 + param_1,param_2 - DAT_0070e448);
      return;
    }
    if (bVar4 == 3) {
      FUN_0053f250(DAT_00d3b5c0,DAT_0070e454 + 3 + param_1,param_2 - DAT_0070e444);
      return;
    }
    if (bVar4 == 4) {
      FUN_0053f250(DAT_00d3b5c0,DAT_0070e450 + 4 + param_1,param_2 - DAT_0070e440);
      return;
    }
    if (bVar4 == 5) {
      FUN_0053f250(DAT_00d3b5c0,DAT_0070e450 + DAT_0070e45c + 5 + param_1,
                   (param_2 - DAT_0070e440) - DAT_0070e44c);
      return;
    }
  }
  return;
}
