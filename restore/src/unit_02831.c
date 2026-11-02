#include "restore_types.h"
#define DAT_012663cc (*(undefined4 *)0x012663ccu)
#define DAT_012663d0 (*(undefined4 *)0x012663d0u)
extern char * FUN_0056261c();

undefined1 * FUN_0056256e(undefined4 *param_1)

{
  int iVar1;
  int iVar2;
  char *pcVar3;
  char *pcVar4;
  undefined1 *puVar5;
  undefined4 uVar6;
  int iVar7;
  int iVar8;
  
  iVar1 = param_1[6];
  iVar2 = param_1[4];
  pcVar3 = &DAT_012663cc;
  iVar7 = 0;
  do {
    pcVar4 = pcVar3;
    iVar8 = iVar7 + 1;
    *pcVar4 = "SunMonTueWedThuFriSat"[iVar1 * 3 + iVar7];
    (&DAT_012663d0)[iVar7] = "SunMonTueWedThuFriSat"[iVar8 + iVar2 * 3 + 0x17];
    pcVar3 = pcVar4 + 1;
    iVar7 = iVar8;
  } while (iVar8 < 3);
  pcVar4[1] = ' ';
  pcVar4[5] = ' ';
  puVar5 = (undefined1 *)FUN_0056261c(pcVar4 + 6,param_1[3]);
  *puVar5 = 0x20;
  puVar5 = (undefined1 *)FUN_0056261c(puVar5 + 1,param_1[2]);
  *puVar5 = 0x3a;
  puVar5 = (undefined1 *)FUN_0056261c(puVar5 + 1,param_1[1]);
  *puVar5 = 0x3a;
  puVar5 = (undefined1 *)FUN_0056261c(puVar5 + 1,*param_1);
  *puVar5 = 0x20;
  uVar6 = FUN_0056261c(puVar5 + 1,(int)param_1[5] / 100 + 0x13);
  puVar5 = (undefined1 *)FUN_0056261c(uVar6,(int)param_1[5] % 100);
  *puVar5 = 10;
  puVar5[1] = 0;
  return &DAT_012663cc;
}
