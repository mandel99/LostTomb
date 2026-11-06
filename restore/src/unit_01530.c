#include "restore_types.h"
#define DAT_00eaac28 (*(uint32_t *)0x00eaac28u)

void FUN_004bca90(void)

{
  int iVar1;
  char *pcVar2;
  
  iVar1 = DAT_00eaac28 * 0x49d8;
  *(undefined4 *)(0x00ea6210u + (uint)iVar1) = 0;
  pcVar2 = (char *)0x00bcd786u;
  do {
    if (((*pcVar2 == ']') || (*pcVar2 == 'd')) && (pcVar2[4] != '\0')) {
      *(int *)(0x00ea6210u + (uint)iVar1) =
          *(int *)(0x00ea6210u + (uint)iVar1) + 1;
    }
    pcVar2 = pcVar2 + 0x184;
  } while ((int)pcVar2 < 0xc8ad42);
  return;
}
