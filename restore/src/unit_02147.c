#include "restore_types.h"
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00799a2c (*(undefined4 *)0x00799a2cu)
#define DAT_00799a3c (*(undefined4 *)0x00799a3cu)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00bca2d8 (*(uint32_t *)0x00bca2d8u)
#define DAT_00bcd61c (*(undefined4 *)0x00bcd61cu)
#define DAT_00bcd61e (*(undefined4 *)0x00bcd61eu)
#define DAT_00bcd63c (*(undefined4 *)0x00bcd63cu)
#define DAT_00bcd642 (*(undefined4 *)0x00bcd642u)
#define DAT_00bcd644 (*(undefined4 *)0x00bcd644u)
#define DAT_00bcd656 (*(undefined4 *)0x00bcd656u)
extern void FUN_004b4220();
extern undefined4 FUN_004f3370();

int FUN_004f4320(int param_1)

{
  short sVar1;
  int iVar2;
  uint3 uVar3;
  int iVar4;
  int iVar5;
  int local_4;
  
  iVar2 = param_1;
  iVar5 = param_1 * 0x184;
  iVar4 = (int)(short)(&DAT_00bcd656)[param_1 * 0xc2];
  uVar3 = (uint3)((uint)(iVar4 * 0x108) >> 8);
  if ((&DAT_007999e8)[iVar4 * 0x84] != 0xb7) {
    return (uint)uVar3 << 8;
  }
  if (*(byte *)((int)&DAT_00799a2c + iVar4 * 0x108 + 1) < 2) {
    return (uint)uVar3 << 8;
  }
  iVar4 = FUN_004f3370(iVar4 - (short)(&DAT_007999ea)[iVar4 * 0x84],&local_4,&param_1,1);
  if ((char)iVar4 == '\0') {
    return iVar4;
  }
  *(undefined2 *)(&DAT_00bcd61c + iVar5) = (undefined2)local_4;
  *(short *)(&DAT_00bcd61e + iVar5) = (short)param_1;
  FUN_004b4220(iVar2,(int)(short)(&DAT_00bcd642)[iVar2 * 0xc2],
               (int)(short)(&DAT_00bcd644)[iVar2 * 0xc2],*(short *)(&DAT_00bcd61c + iVar5) * 0xf,
               (short)param_1 * 0xf,0);
  *(undefined2 *)(&DAT_00bcd63c + iVar5) = 0;
  sVar1 = *(short *)(&DAT_00b4e6fc + (DAT_00bca2d8 + param_1 * 0xe4 + local_4) * 2);
  (&DAT_00bcd656)[iVar2 * 0xc2] = sVar1;
  (&DAT_00799a3c)[sVar1 * 0x84] = (short)iVar2;
  return CONCAT31((int3)(char)((ushort)sVar1 >> 8),1);
}
