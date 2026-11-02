#include "restore_types.h"
#define DAT_01266400 (*(undefined *)0x01266400u)
#define DAT_01266458 (*(undefined *)0x01266458u)
#define DAT_01266a80 (*(undefined *)0x01266a80u)
#define DAT_01266a84 (*(undefined *)0x01266a84u)
extern void * FUN_00561b68();
extern void FUN_00561bdc();
extern void FUN_005630e0();
extern size_t FUN_00563380();
extern uint * FUN_00566750();
extern void FUN_00569bf5();

void FUN_00567994(void)

{
  char cVar1;
  size_t sVar2;
  undefined4 *puVar3;
  void *pvVar4;
  int iVar5;
  char *pcVar6;
  
  if (DAT_01266a84 == 0) {
    FUN_00569bf5();
  }
  iVar5 = 0;
  for (pcVar6 = DAT_01266400; *pcVar6 != '\0'; pcVar6 = pcVar6 + sVar2 + 1) {
    if (*pcVar6 != '=') {
      iVar5 = iVar5 + 1;
    }
    sVar2 = FUN_00563380(pcVar6);
  }
  puVar3 = FUN_00561b68(iVar5 * 4 + 4);
  DAT_01266458 = puVar3;
  if (puVar3 == (undefined4 *)0x0) {
    FUN_005630e0(9);
  }
  cVar1 = *(undefined4 *)(DAT_01266400);
  pcVar6 = DAT_01266400;
  while (cVar1 != '\0') {
    sVar2 = FUN_00563380(pcVar6);
    if (*pcVar6 != '=') {
      pvVar4 = FUN_00561b68(sVar2 + 1);
      *puVar3 = pvVar4;
      if (pvVar4 == (void *)0x0) {
        FUN_005630e0(9);
      }
      FUN_00566750(*puVar3,pcVar6);
      puVar3 = puVar3 + 1;
    }
    pcVar6 = pcVar6 + sVar2 + 1;
    cVar1 = *pcVar6;
  }
  FUN_00561bdc(DAT_01266400);
  DAT_01266400 = (char *)0x0;
  *puVar3 = 0;
  DAT_01266a80 = 1;
  return;
}
