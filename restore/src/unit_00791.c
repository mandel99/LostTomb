#include "restore_types.h"
#define DAT_0078d3ec (*(undefined4 *)0x0078d3ecu)
#define DAT_0078d4b8 (*(undefined4 *)0x0078d4b8u)
#define DAT_0078d5c0 (*(undefined *)0x0078d5c0u)
#define DAT_0078d5c4 (*(undefined *)0x0078d5c4u)
#define DAT_0078d78c (*(undefined *)0x0078d78cu)
#define DAT_0078d7a0 (*(undefined *)0x0078d7a0u)
#define DAT_0078d7a4 (*(undefined *)0x0078d7a4u)
#define DAT_0078d7a8 (*(undefined *)0x0078d7a8u)
#define DAT_0078d7b0 (*(undefined *)0x0078d7b0u)
#define DAT_0078d7bc (*(undefined *)0x0078d7bcu)
#define DAT_0078d7c0 (*(undefined1 *)0x0078d7c0u)
#define DAT_0078d7c4 (*(undefined *)0x0078d7c4u)
#define DAT_0078d7c8 (*(undefined *)0x0078d7c8u)
#define DAT_0078d7cc (*(undefined *)0x0078d7ccu)
#define DAT_0078d7e0 (*(undefined *)0x0078d7e0u)
extern void FUN_0046a080();

void FUN_00451be0(void)

{
  int iVar1;
  undefined4 *puVar2;
  
  DAT_0078d7e0 = 0;
  DAT_0078d7cc = 0;
  DAT_0078d7c8 = 0;
  DAT_0078d7c4 = 0;
  DAT_0078d7c0 = 0;
  DAT_0078d7bc = 0x832;
  DAT_0078d7b0 = 0;
  DAT_0078d7a8 = 0;
  DAT_0078d7a4 = 0;
  DAT_0078d7a0 = 0;
  DAT_0078d5c4 = 0;
  DAT_0078d5c0 = 0;
  DAT_0078d78c = 0;
  FUN_0046a080();
  puVar2 = &DAT_0078d3ec;
  for (iVar1 = 0x33; iVar1 != 0; iVar1 = iVar1 + -1) {
    *puVar2 = 0;
    puVar2 = puVar2 + 1;
  }
  puVar2 = &DAT_0078d4b8;
  for (iVar1 = 0x33; iVar1 != 0; iVar1 = iVar1 + -1) {
    *puVar2 = 0;
    puVar2 = puVar2 + 1;
  }
  return;
}
