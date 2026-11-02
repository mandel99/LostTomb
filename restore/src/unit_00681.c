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

uint FUN_004487f0(void)

{
  longlong lVar1;
  ulonglong uVar2;
  uint in_EAX;
  uint uVar3;
  ushort *in_ECX;
  int iVar4;
  uint uVar5;
  ushort uVar6;
  uint uVar7;
  uint uVar8;
  
  uVar6 = in_ECX[1];
  if ((short)uVar6 < 0) {
    if (((-1 < (short)in_ECX[2]) && (-1 < (short)in_ECX[3])) && (in_EAX = 1, in_ECX[3] == in_ECX[2])
       ) goto LAB_00448955;
  }
  else if ((short)in_ECX[2] < 0) goto LAB_00448955;
  (&DAT_00d3b1c8)[DAT_00d3b1c4] = DAT_00d3b374;
  DAT_00d3b1c4 = DAT_00d3b1c4 + 1;
  if (99 < DAT_00d3b1c4) {
    DAT_00d3b1c4 = 0;
  }
  iVar4 = 0x1f;
  do {
    uVar3 = DAT_00d3b384 >> 4;
    uVar7 = DAT_00d3b384 & 1;
    uVar8 = DAT_00d3b380 >> 4;
    uVar5 = DAT_00d3b380 & 1;
    DAT_00d3b384 = DAT_00d3b384 >> 1;
    DAT_00d3b380 = DAT_00d3b380 >> 1;
    if ((uVar3 & 1) != uVar7) {
      DAT_00d3b384 = DAT_00d3b384 | 0x40000000;
    }
    if ((uVar8 & 1) != uVar5) {
      DAT_00d3b380 = DAT_00d3b380 | 0x40000000;
    }
    iVar4 = iVar4 + -1;
  } while (iVar4 != 0);
  _DAT_00d3b378 = DAT_00d3b384 & 7;
  _DAT_00d3b364 = DAT_00d3b380 & 0x7fff;
  _DAT_00d3b370 = DAT_00d3b384 & 0x7fff;
  DAT_00d3b374 = DAT_00d3b384 & 0x7f;
  _DAT_00d3b368 = DAT_00d3b380 & 0x7f;
  _DAT_00d3b36c = DAT_00d3b380 & 7;
  uVar6 = in_ECX[1];
  if ((short)uVar6 < 0) {
    lVar1 = (longlong)((int)(short)in_ECX[3] - (int)(short)in_ECX[2]);
    uVar2 = (ulonglong)_DAT_00d3b370;
    *in_ECX = (short)((longlong)uVar2 % lVar1) + in_ECX[2];
    return (uint)((longlong)uVar2 / lVar1);
  }
  if ((short)in_ECX[2] < 0) {
    iVar4 = 1;
  }
  else {
    iVar4 = (-1 < (short)in_ECX[3]) + 2;
  }
  in_EAX = (uint)((ulonglong)_DAT_00d3b370 / (ulonglong)(longlong)iVar4);
  iVar4 = (int)((ulonglong)_DAT_00d3b370 % (ulonglong)(longlong)iVar4);
  if (iVar4 != 0) {
    if (iVar4 == 1) {
      *in_ECX = in_ECX[2];
      return in_EAX;
    }
    *in_ECX = in_ECX[3];
    return (uint)in_ECX[3];
  }
LAB_00448955: ;
  *in_ECX = uVar6;
  return in_EAX;
}
