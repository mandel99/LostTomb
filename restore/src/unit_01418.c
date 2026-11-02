#include "restore_types.h"
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_00799a0c (*(undefined4 *)0x00799a0cu)
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00bcd61c (*(undefined4 *)0x00bcd61cu)
#define DAT_00bcd61e (*(undefined4 *)0x00bcd61eu)
#define DAT_00bcd631 (*(undefined4 *)0x00bcd631u)
#define DAT_00bcd63e (*(undefined4 *)0x00bcd63eu)
#define DAT_00bcd640 (*(undefined4 *)0x00bcd640u)
#define DAT_00bcd641 (*(undefined4 *)0x00bcd641u)
#define DAT_00bcd652 (*(undefined4 *)0x00bcd652u)
#define DAT_00d3b4e0 (*(undefined *)0x00d3b4e0u)
#define DAT_00d3b4e4 (*(undefined *)0x00d3b4e4u)
extern undefined4 FUN_0047fe60();

void FUN_004b1df0(int param_1)

{
  char cVar1;
  short sVar2;
  undefined2 uVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  
  iVar6 = param_1 * 0x184;
  sVar2 = (&DAT_00bcd652)[param_1 * 0xc2];
  (&DAT_00bcd631)[iVar6] = 0xf;
  (&DAT_00bcd63e)[iVar6] = 0;
  iVar4 = sVar2 * 0x108;
  (&DAT_00bcd641)[iVar6] = 0xff;
  (&DAT_00bcd640)[iVar6] = 2;
  cVar1 = (&DAT_00799a0c)[iVar4];
  (&DAT_00799a0c)[iVar4] = cVar1 + 2U;
  if (6 < (byte)(cVar1 + 2U)) {
    (&DAT_00799a0c)[iVar4] = 0;
  }
  iVar5 = (int)*(short *)((int)&DAT_007999dc + iVar4 + 2);
  iVar4 = (int)*(short *)(&DAT_007999e0 + iVar4);
  if (cVar1 == '\0') {
    iVar4 = iVar4 + -8;
  }
  else if (cVar1 == '\x02') {
    iVar5 = iVar5 + 8;
  }
  else if (cVar1 == '\x04') {
    iVar4 = iVar4 + 8;
  }
  else if (cVar1 == '\x06') {
    iVar5 = iVar5 + -8;
  }
  if (iVar5 < 0) {
    iVar5 = 0;
  }
  if (iVar4 < 0) {
    iVar4 = 0;
  }
  if (DAT_00bca2d0 <= iVar5) {
    iVar5 = DAT_00bca2d0 + -1;
  }
  if (DAT_00bca2d4 <= iVar4) {
    iVar4 = DAT_00bca2d4 + -1;
  }
  iVar4 = FUN_0047fe60(iVar5,iVar4,1,6);
  uVar3 = DAT_00d3b4e0;
  if (iVar4 != 0) {
    *(undefined2 *)(&DAT_00bcd61c + iVar6) = DAT_00d3b4e4;
    *(undefined2 *)(&DAT_00bcd61e + iVar6) = uVar3;
    return;
  }
  (&DAT_00bcd63e)[iVar6] = 1;
  return;
}
