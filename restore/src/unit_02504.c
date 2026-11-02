#include "restore_types.h"
#define DAT_005e4c20 (*(undefined4 *)0x005e4c20u)
#define DAT_006012ec (*(undefined4 *)0x006012ecu)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd603 (*(undefined4 *)0x00bcd603u)
#define DAT_00bcd652 (*(undefined4 *)0x00bcd652u)
#define DAT_00bcd675 (*(undefined4 *)0x00bcd675u)
#define DAT_00eadcdc (*(undefined *)0x00eadcdcu)
#define DAT_00eadce0 (*(undefined *)0x00eadce0u)
#define DAT_00eadce4 (*(undefined *)0x00eadce4u)
#define DAT_00eadd24 (*(undefined4 *)0x00eadd24u)
#define _DAT_00784cb0 (*(undefined4 *)0x00784cb0u)
extern undefined4 FUN_004c5120();
extern void FUN_0053b4a0();
extern void FUN_0053bfe0();

void FUN_0053d0b0(void)

{
  char cVar1;
  int iVar2;
  int iVar3;
  undefined4 uVar4;
  
  iVar2 = (&DAT_00eadd24)[DAT_00eadce4];
  if (iVar2 < 1) {
    return;
  }
  iVar3 = iVar2 * 0x184;
  cVar1 = (&DAT_00bcd602)[iVar3];
  if (cVar1 == 'S') {
    uVar4 = 0;
LAB_0053d139: ;
    FUN_0053b4a0(iVar2,uVar4,0);
  }
  else {
    if (cVar1 == 'g') {
      uVar4 = 1;
      goto LAB_0053d139;
    }
    if (cVar1 == 'h') {
      uVar4 = 2;
      goto LAB_0053d139;
    }
    if (cVar1 == 'f') {
      uVar4 = 3;
      goto LAB_0053d139;
    }
    if (cVar1 == 'E') {
      uVar4 = 4;
      goto LAB_0053d139;
    }
    if (cVar1 == 'F') {
      uVar4 = 5;
      goto LAB_0053d139;
    }
    if (cVar1 == 'T') {
      uVar4 = 6;
      goto LAB_0053d139;
    }
    if (cVar1 == 'R') {
      uVar4 = 7;
      goto LAB_0053d139;
    }
    if (cVar1 == 'D') {
      uVar4 = 8;
      goto LAB_0053d139;
    }
  }
  DAT_00eadce0 = *(int *)(&DAT_006012ec + (uint)(byte)(&DAT_00bcd602)[iVar3] * 4);
  if (DAT_00eadce0 == -2) {
    iVar2 = *(int *)(&DAT_005e4c20 + (_DAT_00784cb0 & 0xff) * 0x30);
    if (iVar2 == 0x44) {
      DAT_00eadce0 = 0xd2;
    }
    else if (iVar2 == 0x45) {
      DAT_00eadce0 = 0xd1;
    }
    else if (iVar2 == 0x46) {
      DAT_00eadce0 = 0xd0;
    }
    goto LAB_0053d22d;
  }
  if (DAT_00eadce0 == -3) {
    uVar4 = FUN_004c5120((int)(short)(&DAT_00bcd652)[iVar2 * 0xc2]);
    switch((int)(uVar4)) {
    case 0: ;
      goto switchD_0053d1ba_caseD_0;
    case 1: ;
switchD_0053d1ba_caseD_1: ;
      DAT_00eadce0 = 0xe7;
      goto LAB_0053d22d;
    case 2: ;
switchD_0053d1ba_caseD_2: ;
      DAT_00eadce0 = 0xe8;
      goto LAB_0053d22d;
    case 3: ;
switchD_0053d1ba_caseD_3: ;
      DAT_00eadce0 = 0xe9;
      goto LAB_0053d22d;
    case 4: ;
switchD_0053d1ba_caseD_4: ;
      DAT_00eadce0 = 0xea;
      goto LAB_0053d22d;
default: ;
      break;
    }
  }
  if ((DAT_00eadce0 == -4) &&
     (DAT_00eadce0 = *(int *)(&DAT_006012ec + (uint)(byte)(&DAT_00bcd603)[iVar3] * 4),
     DAT_00eadce0 == -3)) {
    uVar4 = FUN_004c5120((int)(short)(&DAT_00bcd652)[iVar2 * 0xc2]);
    switch((int)(uVar4)) {
    case 0: ;
      goto switchD_0053d1ba_caseD_0;
    case 1: ;
      goto switchD_0053d1ba_caseD_1;
    case 2: ;
      goto switchD_0053d1ba_caseD_2;
    case 3: ;
      goto switchD_0053d1ba_caseD_3;
    case 4: ;
      goto switchD_0053d1ba_caseD_4;
    }
  }
  if (DAT_00eadce0 == -1) {
    return;
  }
LAB_0053d22d: ;
  DAT_00eadce0 = DAT_00eadce0 + -0xc9;
  if (-1 < DAT_00eadce0) {
    DAT_00eadcdc = (int)(char)(&DAT_00bcd675)[iVar3];
    FUN_0053bfe0(DAT_00eadce0,DAT_00eadcdc);
  }
  return;
switchD_0053d1ba_caseD_0: ;
  DAT_00eadce0 = 0xe6;
  goto LAB_0053d22d;
}
