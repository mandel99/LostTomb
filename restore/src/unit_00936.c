#include "restore_types.h"
#define DAT_00799a7e (*(undefined4 *)0x00799a7eu)
#define DAT_00799a80 (*(undefined4 *)0x00799a80u)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd656 (*(undefined4 *)0x00bcd656u)

void FUN_00468530(int param_1)

{
  char cVar1;
  int iVar2;
  
  cVar1 = (&DAT_00bcd602)[param_1 * 0x184];
  iVar2 = (int)(short)(&DAT_00bcd656)[param_1 * 0xc2];
  if (cVar1 == '\x0f') {
    *(undefined1 *)((int)&DAT_00799a7e + iVar2 * 0x108 + 1) = 0x20;
  }
  else {
    if (cVar1 == '\x10') {
      *(undefined1 *)(&DAT_00799a80 + iVar2 * 0x84) = 0x20;
      return;
    }
    if (cVar1 == '\x11') {
      *(undefined1 *)((int)&DAT_00799a80 + iVar2 * 0x108 + 1) = 0x20;
      return;
    }
  }
  return;
}
