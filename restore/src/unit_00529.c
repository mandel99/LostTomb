#include "restore_types.h"
#define DAT_0070cdf4 (*(undefined4 *)0x0070cdf4u)
#define DAT_0070e488 (*(undefined *)0x0070e488u)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_007999f4 (*(undefined4 *)0x007999f4u)
#define DAT_00799a2c (*(undefined4 *)0x00799a2cu)
#define DAT_00b7482c (*(undefined4 *)0x00b7482cu)
#define DAT_00d3b5c0 (*(undefined *)0x00d3b5c0u)
extern void FUN_004361a0();
extern undefined4 FUN_004439c0();
extern undefined4 FUN_00443b90();
extern int FUN_004cde60();

void FUN_00435880(int param_1,undefined4 param_2,undefined4 param_3)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  
  DAT_00d3b5c0 = (&DAT_00b7482c)[DAT_0070e488];
  if (*(char *)((int)&DAT_007999dc + param_1 * 0x108 + 1) != '\0') {
    iVar3 = 0;
    if (*(int *)(&DAT_0070cdf4 + (short)(&DAT_007999ea)[param_1 * 0x84] * 0x58) != 0) {
      iVar3 = (int)(short)(&DAT_007999f4)[param_1 * 0x84];
    }
    iVar2 = 0;
    iVar4 = 1;
    do {
      iVar1 = FUN_00443b90(iVar4);
      if ((iVar1 != 0) || (iVar1 = FUN_004439c0(iVar4), iVar1 != 0)) {
        switch((int)(iVar4)) {
        case 1: ;
          iVar1 = 0;
          break;
        case 2: ;
          iVar1 = 1;
          break;
        case 3: ;
          iVar1 = 2;
          break;
        case 4: ;
          iVar1 = 3;
          break;
        case 5: ;
          iVar1 = 4;
          break;
        case 6: ;
          iVar1 = 5;
          break;
        case 7: ;
          iVar1 = 6;
          break;
        case 8: ;
          iVar1 = 7;
          break;
default: ;
          iVar1 = -1;
          break;
        case 0xd: ;
          iVar1 = 9;
          break;
        case 0xf: ;
          iVar1 = 8;
          break;
        case 0x11: ;
          iVar1 = 10;
          break;
        case 0x13: ;
          iVar1 = 0xb;
        }
        iVar2 = iVar2 + (short)(&DAT_00799a2c)[iVar1 + param_1 * 0x84];
      }
      iVar4 = iVar4 + 1;
    } while (iVar4 < 9);
    if (iVar3 == 0) {
      iVar3 = 0;
    }
    else {
      iVar3 = FUN_004cde60(iVar2,iVar3);
    }
    FUN_004361a0(iVar3 / 100,param_2,param_3,0);
  }
  return;
}
