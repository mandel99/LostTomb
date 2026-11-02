#include "restore_types.h"
#define DAT_01266a64 (*(undefined *)0x01266a64u)
#define DAT_01266a74 (*(undefined *)0x01266a74u)
#define DAT_01266a78 (*(undefined *)0x01266a78u)
#define DAT_01266a7c (*(undefined *)0x01266a7cu)

undefined4 * FUN_00565ac4(void)

{
  undefined4 *puVar1;
  LPVOID pvVar2;
  
  if (DAT_01266a74 == DAT_01266a64) {
    pvVar2 = HeapReAlloc(DAT_01266a7c,0,DAT_01266a78,(DAT_01266a64 * 5 + 0x50) * 4);
    if (pvVar2 == (LPVOID)0x0) {
      return (undefined4 *)0x0;
    }
    DAT_01266a64 = DAT_01266a64 + 0x10;
    DAT_01266a78 = pvVar2;
  }
  puVar1 = (undefined4 *)((int)DAT_01266a78 + DAT_01266a74 * 0x14);
  pvVar2 = HeapAlloc(DAT_01266a7c,8,0x41c4);
  puVar1[4] = pvVar2;
  if (pvVar2 != (LPVOID)0x0) {
    pvVar2 = VirtualAlloc((LPVOID)0x0,0x100000,0x2000,4);
    puVar1[3] = pvVar2;
    if (pvVar2 != (LPVOID)0x0) {
      puVar1[2] = 0xffffffff;
      *puVar1 = 0;
      puVar1[1] = 0;
      DAT_01266a74 = DAT_01266a74 + 1;
      *(undefined4 *)puVar1[4] = 0xffffffff;
      return puVar1;
    }
    HeapFree(DAT_01266a7c,0,(LPVOID)puVar1[4]);
  }
  return (undefined4 *)0x0;
}
