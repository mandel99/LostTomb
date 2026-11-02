#include "restore_types.h"
#define DAT_00ea5e3c (*(undefined4 *)0x00ea5e3cu)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)

void FUN_004c51d0(void)

{
  int iVar1;
  int iVar2;
  
  iVar1 = DAT_00eaac28;
  iVar2 = DAT_00eaac28 * 0x49d8;
  if (-1 < *(short *)(0x00ea61f4u + (uint)iVar2)) {
    *(short *)(0x00ea61f4u + (uint)iVar2) =
        *(short *)(0x00ea61f4u + (uint)iVar2) + -1;
  }
  if (-1 < *(short *)(0x00ea61f6u + (uint)iVar2)) {
    *(short *)(0x00ea61f6u + (uint)iVar2) =
        *(short *)(0x00ea61f6u + (uint)iVar2) + -1;
  }
  if (-1 < *(short *)(0x00ea61f8u + (uint)iVar2)) {
    *(short *)(0x00ea61f8u + (uint)iVar2) =
        *(short *)(0x00ea61f8u + (uint)iVar2) + -1;
  }
  if (-1 < (int)(&DAT_00ea5e3c)[iVar1 * 0x1276]) {
    (&DAT_00ea5e3c)[iVar1 * 0x1276] = (&DAT_00ea5e3c)[iVar1 * 0x1276] + -1;
  }
  if (-1 < *(int *)(0x00ea6024u + (uint)iVar2)) {
    *(int *)(0x00ea6024u + (uint)iVar2) =
        *(int *)(0x00ea6024u + (uint)iVar2) + -1;
  }
  return;
}
