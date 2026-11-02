#include "restore_types.h"
#define DAT_00bcd60c (*(undefined4 *)0x00bcd60cu)
#define DAT_00bcd60e (*(undefined4 *)0x00bcd60eu)
#define DAT_00bcd61c (*(undefined4 *)0x00bcd61cu)
#define DAT_00bcd61e (*(undefined4 *)0x00bcd61eu)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd678 (*(undefined4 *)0x00bcd678u)
#define DAT_00bcd67a (*(undefined4 *)0x00bcd67au)
#define DAT_00bcd67c (*(undefined4 *)0x00bcd67cu)
#define DAT_00bcd67e (*(undefined4 *)0x00bcd67eu)
#define DAT_00bcd680 (*(undefined4 *)0x00bcd680u)
#define DAT_00d0563c (*(undefined *)0x00d0563cu)
#define DAT_00ea5db4 (*(undefined4 *)0x00ea5db4u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern undefined4 FUN_0048e820();
extern void FUN_004b3250();

undefined4 FUN_0048e6f0(int param_1)

{
  char cVar1;
  int iVar2;
  int iVar3;
  
  if (((int)(&DAT_00ea5db4)[DAT_00eaac28 * 0x1276] < 1) && (DAT_00d0563c < 1)) {
    return 0;
  }
  iVar3 = param_1 * 0x184;
  cVar1 = (&DAT_00bcd630)[iVar3];
  if (cVar1 == '\x05') {
    return 0;
  }
  if (cVar1 == '\x04') {
    return 0;
  }
  if (cVar1 == '\b') {
    return 0;
  }
  if (cVar1 == '\t') {
    return 0;
  }
  if (cVar1 == '\f') {
    return 0;
  }
  if (cVar1 == '\r') {
    return 0;
  }
  if (cVar1 == '\x0e') {
    return 0;
  }
  if (cVar1 == '\x0f') {
    return 0;
  }
  cVar1 = (&DAT_00bcd678)[iVar3];
  (&DAT_00bcd678)[iVar3] = cVar1 + 1U;
  if ((byte)(cVar1 + 1U) < 10) {
    return 0;
  }
  (&DAT_00bcd678)[iVar3] = 0;
  iVar2 = FUN_0048e820((int)(short)(&DAT_00bcd60c)[param_1 * 0xc2],
                       (int)(short)(&DAT_00bcd60e)[param_1 * 0xc2],0x1e);
  if (iVar2 < 1) {
    return 0;
  }
  (&DAT_00bcd678)[iVar3] = 0;
  (&DAT_00bcd630)[iVar3] = 0xe;
  *(undefined2 *)(&DAT_00bcd61c + iVar3) = (&DAT_00bcd60c)[iVar2 * 0xc2];
  *(undefined2 *)(&DAT_00bcd61e + iVar3) = (&DAT_00bcd60e)[iVar2 * 0xc2];
  *(short *)(&DAT_00bcd67a + iVar3) = (short)iVar2;
  (&DAT_00bcd67c)[iVar2 * 0xc2] = (short)param_1;
  *(undefined2 *)(&DAT_00bcd680 + iVar3) = (&DAT_00bcd67e)[iVar2 * 0xc2];
  FUN_004b3250(param_1);
  return 1;
}
