#include "restore_types.h"
#define DAT_0078d788 (*(undefined *)0x0078d788u)
#define DAT_007900b8 (*(undefined4 *)0x007900b8u)
#define DAT_007900bc (*(undefined4 *)0x007900bcu)
#define DAT_007900c0 (*(undefined4 *)0x007900c0u)
#define DAT_007900c2 (*(undefined4 *)0x007900c2u)
#define DAT_00790134 (*(undefined4 *)0x00790134u)
#define DAT_00790184 (*(undefined4 *)0x00790184u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00f53dd8 (*(undefined *)0x00f53dd8u)
#define DAT_00f53ddc (*(undefined4 *)0x00f53ddcu)
#define DAT_00f53e6c (*(undefined *)0x00f53e6cu)
#define DAT_00f53e70 (*(undefined4 *)0x00f53e70u)
extern void FUN_0040f8f0();
extern undefined4 FUN_005013a0();
extern void FUN_005596d0();

int FUN_0046b100(int param_1)

{
  int iVar1;
  char *pcVar2;
  int iVar3;
  undefined2 *puVar4;
  int *piVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  
  FUN_005596d0();
  pcVar2 = &DAT_00790184;
  iVar8 = 1;
  while (*pcVar2 != '\0') {
    pcVar2 = pcVar2 + 0xc4;
    iVar8 = iVar8 + 1;
    if (0x7999df < (int)pcVar2) {
      return 0;
    }
  }
  iVar7 = iVar8 * 0xc4;
  FUN_0040f8f0(&DAT_007900b8 + iVar8 * 0x31,0xc4,0);
  iVar6 = DAT_0078d788;
  (&DAT_007900c0)[iVar7] = 1;
  *(short *)(&DAT_007900bc + iVar8 * 0x31) = (short)param_1;
  if (iVar6 <= iVar8) {
    DAT_0078d788 = iVar8;
  }
  switch((&DAT_007999e8)[param_1 * 0x84]) {
  case 0x46: ;
  case 0xbe: ;
    iVar6 = 0;
    if (0 < DAT_00f53e6c) {
      piVar5 = &DAT_00f53e70;
      do {
        iVar1 = *piVar5;
        iVar3 = FUN_005013a0(iVar1);
        if (iVar3 != 0) {
          *(undefined1 *)((int)&DAT_007900c2 + iVar1 + iVar7) = 0;
        }
        iVar6 = iVar6 + 1;
        piVar5 = piVar5 + 1;
      } while (iVar6 < DAT_00f53e6c);
      return iVar8;
    }
    break;
  case 0x47: ;
    iVar6 = 0;
    puVar4 = (undefined2 *)(&DAT_00790134 + iVar7);
    do {
      *(undefined1 *)((int)&DAT_007900c2 + iVar6 + iVar7) = 1;
      puVar4[-0x24] = 0xc80;
      *puVar4 = 0xc80;
      iVar6 = iVar6 + 1;
      puVar4 = puVar4 + 1;
    } while (iVar6 < 0x24);
    if (0 < DAT_00f53dd8) {
      piVar5 = &DAT_00f53ddc;
      iVar6 = DAT_00f53dd8;
      do {
        iVar1 = *piVar5;
        piVar5 = piVar5 + 1;
        iVar6 = iVar6 + -1;
        *(undefined1 *)((int)&DAT_007900c2 + iVar7 + iVar1) = 0;
      } while (iVar6 != 0);
    }
    break;
  case 0x48: ;
    iVar6 = 0;
    puVar4 = (undefined2 *)(&DAT_00790134 + iVar7);
    do {
      *(undefined1 *)((int)&DAT_007900c2 + iVar6 + iVar7) = 1;
      puVar4[-0x24] = 0xc80;
      *puVar4 = 0xc80;
      iVar6 = iVar6 + 1;
      puVar4 = puVar4 + 1;
    } while (iVar6 < 0x24);
    if (0 < DAT_00f53e6c) {
      piVar5 = &DAT_00f53e70;
      iVar6 = DAT_00f53e6c;
      do {
        iVar1 = *piVar5;
        if ((iVar1 < 1) || (8 < iVar1)) {
          *(undefined1 *)((int)&DAT_007900c2 + iVar1 + iVar7) = 0;
        }
        piVar5 = piVar5 + 1;
        iVar6 = iVar6 + -1;
      } while (iVar6 != 0);
      return iVar8;
    }
  }
  return iVar8;
}
