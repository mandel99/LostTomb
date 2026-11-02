#include "restore_types.h"
#define DAT_00e95080 (*(undefined4 *)0x00e95080u)
#define DAT_00e950d4 (*(undefined *)0x00e950d4u)
#define DAT_00e950e0 (*(undefined *)0x00e950e0u)
#define DAT_00e950ec (*(undefined *)0x00e950ecu)
#define DAT_00e950f0 (*(undefined *)0x00e950f0u)
#define DAT_00e9511c (*(undefined *)0x00e9511cu)
#define DAT_00e95120 (*(undefined *)0x00e95120u)
#define DAT_00e95124 (*(undefined4 *)0x00e95124u)
#define DAT_00e95174 (*(undefined4 *)0x00e95174u)
#define DAT_00e951e4 (*(undefined *)0x00e951e4u)
#define DAT_00e951e8 (*(undefined *)0x00e951e8u)
#define DAT_00e951ec (*(undefined *)0x00e951ecu)
#define DAT_00e951f0 (*(undefined *)0x00e951f0u)
#define DAT_00e95238 (*(undefined4 *)0x00e95238u)
#define DAT_00ea0da8 (*(undefined *)0x00ea0da8u)
#define _DAT_00e950d0 (*(undefined4 *)0x00e950d0u)
#define _DAT_00e950e4 (*(undefined4 *)0x00e950e4u)
#define _DAT_00e950e8 (*(undefined4 *)0x00e950e8u)
#define _DAT_00e951c4 (*(undefined4 *)0x00e951c4u)
#define _DAT_00e951c8 (*(undefined4 *)0x00e951c8u)
#define _DAT_00e951cc (*(undefined4 *)0x00e951ccu)
extern void FUN_004e1830();

void FUN_004e1770(void)

{
  undefined2 *puVar1;
  int iVar2;
  undefined4 *puVar3;
  
  puVar1 = &DAT_00e95238;
  do {
    puVar1[-3] = 0;
    *puVar1 = 0xffff;
    puVar1 = puVar1 + 0x18;
  } while ((int)puVar1 < 0xea0db8);
  _DAT_00e951c4 = 0;
  _DAT_00e951c8 = 0;
  DAT_00ea0da8 = 0;
  puVar3 = &DAT_00e95174;
  for (iVar2 = 0x14; iVar2 != 0; iVar2 = iVar2 + -1) {
    *puVar3 = 0;
    puVar3 = puVar3 + 1;
  }
  puVar3 = &DAT_00e95124;
  for (iVar2 = 0x14; iVar2 != 0; iVar2 = iVar2 + -1) {
    *puVar3 = 0;
    puVar3 = puVar3 + 1;
  }
  puVar3 = &DAT_00e95080;
  for (iVar2 = 0x14; iVar2 != 0; iVar2 = iVar2 + -1) {
    *puVar3 = 0;
    puVar3 = puVar3 + 1;
  }
  _DAT_00e951cc = 0;
  DAT_00e951f0 = 0;
  DAT_00e951ec = 0;
  DAT_00e951e8 = 0xffffffff;
  DAT_00e951e4 = 0;
  DAT_00e9511c = 0;
  DAT_00e95120 = 0;
  DAT_00e950e0 = 1;
  DAT_00e950f0 = 0;
  DAT_00e950ec = 0;
  _DAT_00e950e8 = 0;
  _DAT_00e950e4 = 0;
  FUN_004e1830();
  DAT_00e950d4 = 0;
  _DAT_00e950d0 = 0;
  return;
}
