#include "restore_types.h"
#define DAT_0078d790 (*(undefined *)0x0078d790u)
#define DAT_00799ae5 (*(undefined4 *)0x00799ae5u)
#define DAT_00ea44ac (*(undefined4 *)0x00ea44acu)
#define DAT_00ea44b0 (*(undefined4 *)0x00ea44b0u)
#define DAT_00ea44b4 (*(undefined4 *)0x00ea44b4u)
#define DAT_00ea44b8 (*(undefined4 *)0x00ea44b8u)
#define DAT_00ea44c0 (*(undefined4 *)0x00ea44c0u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)

void FUN_00459e50(void)

{
  int iVar1;
  char *pcVar2;
  int iVar3;
  
  iVar1 = DAT_00eaac28;
  (&DAT_00ea44c0)[DAT_00eaac28 * 0x1276] =
       (&DAT_00ea44b0)[DAT_00eaac28 * 0x1276] + (&DAT_00ea44ac)[DAT_00eaac28 * 0x1276];
  iVar3 = DAT_0078d790;
  (&DAT_00ea44ac)[iVar1 * 0x1276] = 0;
  (&DAT_00ea44b0)[iVar1 * 0x1276] = 0;
  (&DAT_00ea44b4)[iVar1 * 0x1276] = 0;
  (&DAT_00ea44b8)[iVar1 * 0x1276] = 0;
  if (0 < iVar3) {
    pcVar2 = &DAT_00799ae5;
    do {
      if ((pcVar2[-5] == '\x01') && (*pcVar2 != '\0')) {
        pcVar2[0x87] = '\0';
        pcVar2[0x88] = '\0';
        pcVar2[0x89] = '\0';
        pcVar2[0x8a] = '\0';
      }
      pcVar2 = pcVar2 + 0x108;
      iVar3 = iVar3 + -1;
    } while (iVar3 != 0);
  }
  return;
}
