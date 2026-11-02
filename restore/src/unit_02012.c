#include "restore_types.h"
#define DAT_00d3b1c4 (*(undefined *)0x00d3b1c4u)
#define DAT_00d3b1c8 (*(undefined4 *)0x00d3b1c8u)
#define DAT_00d3b374 (*(undefined *)0x00d3b374u)
#define DAT_00d3b380 (*(undefined *)0x00d3b380u)
#define DAT_00d3b384 (*(undefined *)0x00d3b384u)
#define _DAT_00d3b364 (*(undefined4 *)0x00d3b364u)
#define _DAT_00d3b368 (*(undefined4 *)0x00d3b368u)
#define _DAT_00d3b36c (*(undefined4 *)0x00d3b36cu)
#define _DAT_00d3b370 (*(undefined4 *)0x00d3b370u)
#define _DAT_00d3b378 (*(undefined4 *)0x00d3b378u)
extern void FUN_004e7410();
extern undefined4 FUN_004e75a0();

uint FUN_004e7300(undefined4 param_1)

{
  undefined4 uVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  int iVar6;
  undefined4 local_4;
  
  uVar1 = param_1;
  (&DAT_00d3b1c8)[DAT_00d3b1c4] = DAT_00d3b374;
  DAT_00d3b1c4 = DAT_00d3b1c4 + 1;
  if (99 < DAT_00d3b1c4) {
    DAT_00d3b1c4 = 0;
  }
  iVar6 = 0x1f;
  do {
    uVar2 = DAT_00d3b384 >> 4;
    uVar4 = DAT_00d3b384 & 1;
    uVar5 = DAT_00d3b380 >> 4;
    uVar3 = DAT_00d3b380 & 1;
    DAT_00d3b384 = DAT_00d3b384 >> 1;
    DAT_00d3b380 = DAT_00d3b380 >> 1;
    if ((uVar2 & 1) != uVar4) {
      DAT_00d3b384 = DAT_00d3b384 | 0x40000000;
    }
    if ((uVar5 & 1) != uVar3) {
      DAT_00d3b380 = DAT_00d3b380 | 0x40000000;
    }
    iVar6 = iVar6 + -1;
  } while (iVar6 != 0);
  _DAT_00d3b378 = DAT_00d3b384 & 7;
  _DAT_00d3b364 = DAT_00d3b380 & 0x7fff;
  _DAT_00d3b370 = DAT_00d3b384 & 0x7fff;
  _DAT_00d3b368 = DAT_00d3b380 & 0x7f;
  _DAT_00d3b36c = DAT_00d3b380 & 7;
  DAT_00d3b374 = DAT_00d3b384 & 0x7f;
  uVar2 = _DAT_00d3b370 / 100;
  if (_DAT_00d3b370 % 100 < 0x32) {
    iVar6 = FUN_004e75a0(param_1,&local_4,&param_1);
    uVar2 = 0;
    if (iVar6 != 0) {
      uVar2 = (FUN_004e7410(uVar1,local_4,param_1), restore_unresolved_return(0x004e7410u));
    }
  }
  return uVar2;
}
