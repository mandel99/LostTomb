#include "restore_types.h"
#define DAT_00eaa628 (*(undefined4 *)0x00eaa628u)
#define _DAT_00eaa814 (*(undefined4 *)0x00eaa814u)
#define _DAT_00eaa820 (*(undefined4 *)0x00eaa820u)
#define _DAT_00eaa9fc (*(undefined4 *)0x00eaa9fcu)

void FUN_004e9ee0(void)

{
  int iVar1;
  undefined4 *puVar2;
  
  iVar1 = 0;
  puVar2 = &DAT_00eaa628;
  do {
    *(undefined1 *)(0x00eaa5d8u + (uint)iVar1) = 0x32;
    *(undefined1 *)(0x00eaa5e2u + (uint)iVar1) = 0x32;
    *(undefined1 *)(0x00eaa5ecu + (uint)iVar1) = 0;
    *(undefined1 *)(0x00eaaa5eu + (uint)iVar1) = 0;
    *(undefined1 *)(0x00eaa60au + (uint)iVar1) = 0;
    *(undefined1 *)(0x00eaa614u + (uint)iVar1) = 0;
    *(undefined1 *)(0x00eaa61eu + (uint)iVar1) = 0;
    *puVar2 = 0;
    puVar2 = puVar2 + 1;
    iVar1 = iVar1 + 1;
    *(undefined2 *)(uintptr_t)0x00eaabccu = 0xffff;
    *(undefined2 *)(uintptr_t)0x00eaabceu = 0xffff;
    *(undefined2 *)(uintptr_t)0x00eaabd0u = 0xffff;
    _DAT_00eaa814 = 0xffffffff;
    _DAT_00eaa9fc = 0xffffffff;
  } while ((int)puVar2 < 0xeaa63c);
  _DAT_00eaa820 = 0;
  return;
}
