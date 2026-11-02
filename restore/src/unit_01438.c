#include "restore_types.h"
#define DAT_00bcd609 (*(undefined4 *)0x00bcd609u)
#define DAT_00bcd646 (*(undefined4 *)0x00bcd646u)
#define DAT_00bcd648 (*(undefined4 *)0x00bcd648u)
#define DAT_00bcd64a (*(undefined4 *)0x00bcd64au)
#define DAT_00bcd64c (*(undefined4 *)0x00bcd64cu)
#define DAT_00bcd64e (*(undefined4 *)0x00bcd64eu)
#define DAT_00bcd650 (*(undefined4 *)0x00bcd650u)
extern undefined4 FUN_0051cba0();
extern undefined1 FUN_0051cc00();

void FUN_004b4220(int param_1,int param_2,int param_3,int param_4,int param_5,int param_6)

{
  undefined1 uVar1;
  char cVar2;
  short sVar3;
  int iVar4;
  short sVar5;
  
  iVar4 = param_1 * 0x184;
  sVar3 = (short)param_4;
  *(short *)(&DAT_00bcd646 + iVar4) = sVar3;
  sVar5 = (short)param_5;
  *(short *)(&DAT_00bcd648 + iVar4) = sVar5;
  if (param_4 < param_2) {
    (&DAT_00bcd64a)[param_1 * 0xc2] = (short)param_2 - sVar3;
  }
  else {
    (&DAT_00bcd64a)[param_1 * 0xc2] = sVar3 - (short)param_2;
  }
  if (param_5 < param_3) {
    (&DAT_00bcd64c)[param_1 * 0xc2] = (short)param_3 - sVar5;
  }
  else {
    (&DAT_00bcd64c)[param_1 * 0xc2] = sVar5 - (short)param_3;
  }
  sVar3 = (&DAT_00bcd64c)[param_1 * 0xc2];
  sVar5 = (&DAT_00bcd64a)[param_1 * 0xc2];
  if (sVar5 < sVar3) {
    (&DAT_00bcd64e)[param_1 * 0xc2] = sVar5 * 2 - sVar3;
  }
  else if (sVar3 < sVar5) {
    (&DAT_00bcd64e)[param_1 * 0xc2] = sVar3 * 2 - sVar5;
  }
  else {
    (&DAT_00bcd64e)[param_1 * 0xc2] = 0;
  }
  if (param_6 != 0) {
    uVar1 = FUN_0051cc00();
    (&DAT_00bcd609)[iVar4] = uVar1;
    goto LAB_004b436e;
  }
  cVar2 = FUN_0051cba0(param_2,param_3,param_4,param_5);
  (&DAT_00bcd609)[iVar4] = cVar2;
  if ((short)(&DAT_00bcd64a)[param_1 * 0xc2] * 2 < (int)(short)(&DAT_00bcd64c)[param_1 * 0xc2]) {
    if (cVar2 == '\x01') {
LAB_004b4324: ;
      (&DAT_00bcd609)[iVar4] = 0;
    }
    else if (cVar2 == '\x03') {
      (&DAT_00bcd609)[iVar4] = 4;
    }
    else if (cVar2 == '\x05') {
      (&DAT_00bcd609)[iVar4] = 4;
    }
    else if (cVar2 == '\a') goto LAB_004b4324;
  }
  if ((short)(&DAT_00bcd64c)[param_1 * 0xc2] * 2 < (int)(short)(&DAT_00bcd64a)[param_1 * 0xc2]) {
    cVar2 = (&DAT_00bcd609)[iVar4];
    if (cVar2 == '\x01') {
      (&DAT_00bcd609)[iVar4] = 2;
    }
    else if (cVar2 == '\x03') {
      (&DAT_00bcd609)[iVar4] = 2;
    }
    else if ((cVar2 == '\x05') || (cVar2 == '\a')) {
      (&DAT_00bcd609)[iVar4] = 6;
    }
  }
LAB_004b436e: ;
  if ((short)(&DAT_00bcd64a)[param_1 * 0xc2] < (short)(&DAT_00bcd64c)[param_1 * 0xc2]) {
    (&DAT_00bcd650)[iVar4] = 2;
    return;
  }
  (&DAT_00bcd650)[iVar4] = 1;
  return;
}
