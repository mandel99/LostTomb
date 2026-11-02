#include "restore_types.h"
#define DAT_00e20768 (*(uint32_t *)0x00e20768u)
#define DAT_00e37078 (*(undefined4 *)0x00e37078u)
#define DAT_00e370bc (*(undefined4 *)0x00e370bcu)
#define DAT_00e37100 (*(undefined4 *)0x00e37100u)
#define DAT_00e38e74 (*(char *)0x00e38e74u)
extern void FUN_004cc110();

void FUN_004149d0(void)

{
  char cVar1;
  uint uVar2;
  uint uVar3;
  char *pcVar4;
  char *pcVar5;
  char *pcVar6;
  
  FUN_004cc110(9,5);
  uVar2 = 0xffffffff;
  pcVar5 = DAT_00e20768;
  do {
    pcVar4 = pcVar5;
    if (uVar2 == 0) break;
    uVar2 = uVar2 - 1;
    pcVar4 = pcVar5 + 1;
    cVar1 = *pcVar5;
    pcVar5 = pcVar4;
  } while (cVar1 != '\0');
  uVar2 = ~uVar2;
  pcVar5 = pcVar4 + -uVar2;
  pcVar4 = &DAT_00e38e74;
  for (uVar3 = uVar2 >> 2; uVar3 != 0; uVar3 = uVar3 - 1) {
    *(undefined4 *)pcVar4 = *(undefined4 *)pcVar5;
    pcVar5 = pcVar5 + 4;
    pcVar4 = pcVar4 + 4;
  }
  for (uVar2 = uVar2 & 3; uVar2 != 0; uVar2 = uVar2 - 1) {
    *pcVar4 = *pcVar5;
    pcVar5 = pcVar5 + 1;
    pcVar4 = pcVar4 + 1;
  }
  FUN_004cc110(9,6);
  pcVar5 = DAT_00e20768;
  uVar2 = 0xffffffff;
  pcVar4 = DAT_00e20768;
  do {
    pcVar6 = pcVar4;
    if (uVar2 == 0) break;
    uVar2 = uVar2 - 1;
    pcVar6 = pcVar4 + 1;
    cVar1 = *pcVar4;
    pcVar4 = pcVar6;
  } while (cVar1 != '\0');
  uVar2 = ~uVar2;
  pcVar4 = pcVar6 + -uVar2;
  pcVar6 = (char *)&DAT_00e37100;
  for (uVar3 = uVar2 >> 2; uVar3 != 0; uVar3 = uVar3 - 1) {
    *(undefined4 *)pcVar6 = *(undefined4 *)pcVar4;
    pcVar4 = pcVar4 + 4;
    pcVar6 = pcVar6 + 4;
  }
  for (uVar2 = uVar2 & 3; uVar2 != 0; uVar2 = uVar2 - 1) {
    *pcVar6 = *pcVar4;
    pcVar4 = pcVar4 + 1;
    pcVar6 = pcVar6 + 1;
  }
  uVar2 = 0xffffffff;
  do {
    pcVar4 = pcVar5;
    if (uVar2 == 0) break;
    uVar2 = uVar2 - 1;
    pcVar4 = pcVar5 + 1;
    cVar1 = *pcVar5;
    pcVar5 = pcVar4;
  } while (cVar1 != '\0');
  uVar2 = ~uVar2;
  pcVar5 = pcVar4 + -uVar2;
  pcVar4 = &DAT_00e370bc;
  for (uVar3 = uVar2 >> 2; uVar3 != 0; uVar3 = uVar3 - 1) {
    *(undefined4 *)pcVar4 = *(undefined4 *)pcVar5;
    pcVar5 = pcVar5 + 4;
    pcVar4 = pcVar4 + 4;
  }
  for (uVar2 = uVar2 & 3; uVar2 != 0; uVar2 = uVar2 - 1) {
    *pcVar4 = *pcVar5;
    pcVar5 = pcVar5 + 1;
    pcVar4 = pcVar4 + 1;
  }
  FUN_004cc110(9,7);
  uVar2 = 0xffffffff;
  pcVar5 = DAT_00e20768;
  do {
    pcVar4 = pcVar5;
    if (uVar2 == 0) break;
    uVar2 = uVar2 - 1;
    pcVar4 = pcVar5 + 1;
    cVar1 = *pcVar5;
    pcVar5 = pcVar4;
  } while (cVar1 != '\0');
  uVar2 = ~uVar2;
  pcVar5 = pcVar4 + -uVar2;
  pcVar4 = (char *)&DAT_00e37078;
  for (uVar3 = uVar2 >> 2; uVar3 != 0; uVar3 = uVar3 - 1) {
    *(undefined4 *)pcVar4 = *(undefined4 *)pcVar5;
    pcVar5 = pcVar5 + 4;
    pcVar4 = pcVar4 + 4;
  }
  for (uVar2 = uVar2 & 3; uVar2 != 0; uVar2 = uVar2 - 1) {
    *pcVar4 = *pcVar5;
    pcVar5 = pcVar5 + 1;
    pcVar4 = pcVar4 + 1;
  }
  return;
}
