#include "restore_types.h"
#define DAT_01266598 (*(undefined4 *)0x01266598u)
#define DAT_01266a84 (*(undefined *)0x01266a84u)
#define DAT_01267aa4 (*(undefined *)0x01267aa4u)
#define _DAT_0126644c (*(undefined4 *)0x0126644cu)
#define _DAT_01266450 (*(undefined4 *)0x01266450u)
#define _DAT_01266468 (*(undefined4 *)0x01266468u)
extern void * FUN_00561b68();
extern void FUN_005630e0();
extern void FUN_00567ae6();
extern void FUN_00569bf5();

void FUN_00567a4d(void)

{
  void *pvVar1;
  char *pcVar2;
  int local_c;
  int local_8;
  
  if (DAT_01266a84 == 0) {
    FUN_00569bf5();
  }
  GetModuleFileNameA((HMODULE)0x0,&DAT_01266598,0x104);
  _DAT_01266468 = &DAT_01266598;
  pcVar2 = &DAT_01266598;
  if (*(undefined4 *)(DAT_01267aa4) != '\0') {
    pcVar2 = DAT_01267aa4;
  }
  FUN_00567ae6(pcVar2,0,0,&local_8,&local_c);
  pvVar1 = FUN_00561b68(local_c + local_8 * 4);
  if (pvVar1 == (void *)0x0) {
    FUN_005630e0(8);
  }
  FUN_00567ae6(pcVar2,pvVar1,(void *)((int)pvVar1 + local_8 * 4),&local_8,&local_c);
  _DAT_01266450 = pvVar1;
  _DAT_0126644c = local_8 + -1;
  return;
}
