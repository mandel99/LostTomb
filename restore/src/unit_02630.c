#include "restore_types.h"
#define DAT_00ea41fc (*(undefined4 *)0x00ea41fcu)
#define DAT_00ea4205 (*(undefined4 *)0x00ea4205u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define DAT_00f53dd8 (*(undefined *)0x00f53dd8u)
#define DAT_00f53ddc (*(undefined4 *)0x00f53ddcu)
#define DAT_00f53e6c (*(undefined *)0x00f53e6cu)
#define DAT_00f53e70 (*(undefined4 *)0x00f53e70u)
extern undefined4 FUN_004439c0();
extern undefined4 FUN_00443b90();
extern undefined4 FUN_00559480();

void FUN_005596d0(void)

{
  char cVar1;
  int iVar2;
  int iVar3;
  int *piVar4;
  undefined4 *puVar5;
  
  DAT_00f53e6c = 0;
  puVar5 = &DAT_00f53e70;
  for (iVar3 = 0x24; iVar3 != 0; iVar3 = iVar3 + -1) {
    *puVar5 = 0;
    puVar5 = puVar5 + 1;
  }
  puVar5 = &DAT_00f53ddc;
  for (iVar3 = 0x24; iVar3 != 0; iVar3 = iVar3 + -1) {
    *puVar5 = 0;
    puVar5 = puVar5 + 1;
  }
  DAT_00f53dd8 = 0;
  iVar3 = 1;
  do {
    iVar2 = FUN_00443b90(iVar3);
    if ((((iVar2 != 0) || (iVar2 = FUN_004439c0(iVar3), iVar2 != 0)) ||
        (cVar1 = FUN_00559480(iVar3), cVar1 != '\0')) ||
       ((&DAT_00ea4205)[iVar3 + DAT_00eaac28 * 0x49d8] != '\0')) {
      (&DAT_00f53e70)[DAT_00f53e6c] = iVar3;
      DAT_00f53e6c = DAT_00f53e6c + 1;
    }
    iVar3 = iVar3 + 1;
  } while (iVar3 < 0x24);
  iVar3 = 0;
  while (iVar2 = DAT_00eaac28, iVar3 = iVar3 + 1, iVar3 < 9) {
    while (iVar3 < 1) {
      iVar3 = iVar3 + 1;
      if (8 < iVar3) goto LAB_005597b6;
    }
    iVar2 = FUN_00443b90(iVar3);
    if (((iVar2 != 0) || (iVar2 = FUN_004439c0(iVar3), iVar2 != 0)) ||
       ((cVar1 = FUN_00559480(iVar3), cVar1 != '\0' ||
        ((&DAT_00ea41fc)[iVar3 + DAT_00eaac28 * 0x49d8] != '\0')))) {
      (&DAT_00f53ddc)[DAT_00f53dd8] = iVar3;
      DAT_00f53dd8 = DAT_00f53dd8 + 1;
    }
  }
LAB_005597b6: ;
  if (0 < DAT_00f53e6c) {
    piVar4 = &DAT_00f53e70;
    iVar3 = DAT_00f53e6c;
    do {
      if (*piVar4 != 0) {
        (&DAT_00ea4205)[*piVar4 + iVar2 * 0x49d8] = 1;
      }
      piVar4 = piVar4 + 1;
      iVar3 = iVar3 + -1;
    } while (iVar3 != 0);
  }
  if (0 < DAT_00f53dd8) {
    piVar4 = &DAT_00f53ddc;
    iVar3 = DAT_00f53dd8;
    do {
      if (*piVar4 != 0) {
        (&DAT_00ea41fc)[*piVar4 + iVar2 * 0x49d8] = 1;
      }
      piVar4 = piVar4 + 1;
      iVar3 = iVar3 + -1;
    } while (iVar3 != 0);
  }
  return;
}
