#include "restore_types.h"
#define DAT_0078d788 (*(undefined *)0x0078d788u)
#define DAT_0078d78c (*(undefined *)0x0078d78cu)
#define DAT_0078d790 (*(undefined *)0x0078d790u)
#define DAT_00790184 (*(undefined4 *)0x00790184u)
#define DAT_00799ae0 (*(undefined4 *)0x00799ae0u)

void FUN_0045aa30(void)

{
  char *pcVar1;
  int iVar2;
  
  DAT_0078d790 = 0;
  iVar2 = 1;
  pcVar1 = &DAT_00799ae0;
  do {
    if (*pcVar1 != '\0') {
      DAT_0078d790 = iVar2;
    }
    pcVar1 = pcVar1 + 0x108;
    iVar2 = iVar2 + 1;
  } while ((int)pcVar1 < 0x89b6d8);
  if (DAT_0078d78c < DAT_0078d790) {
    DAT_0078d78c = DAT_0078d790;
  }
  DAT_0078d788 = 0;
  iVar2 = 1;
  pcVar1 = &DAT_00790184;
  do {
    if (*pcVar1 != '\0') {
      DAT_0078d788 = iVar2;
    }
    pcVar1 = pcVar1 + 0xc4;
    iVar2 = iVar2 + 1;
  } while ((int)pcVar1 < 0x7999e0);
  return;
}
