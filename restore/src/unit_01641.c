#include "restore_types.h"
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00799a2c (*(undefined4 *)0x00799a2cu)
#define DAT_00799a2e (*(undefined4 *)0x00799a2eu)
#define DAT_00d3a4c0 (*(undefined4 *)0x00d3a4c0u)
#define DAT_00e34238 (*(undefined *)0x00e34238u)
extern int FUN_0045d510();
extern int FUN_00475b50();
extern void FUN_004cc110();
extern int FUN_004cc5c0();

void FUN_004ca3d0(int param_1,int param_2,int param_3,int *param_4)

{
  char cVar1;
  undefined4 uVar2;
  int iVar3;
  uint uVar4;
  uint uVar5;
  int iVar6;
  int iVar7;
  char *pcVar8;
  char *pcVar9;
  
  switch((int)(param_2)) {
  case 1: ;
    iVar7 = 0;
    break;
  case 2: ;
    iVar7 = 6;
    break;
  case 3: ;
    iVar7 = 4;
    break;
default: ;
    iVar7 = 0xe;
    break;
  case 6: ;
    iVar7 = 2;
    break;
  case 0xd: ;
    iVar7 = 8;
    break;
  case 0x11: ;
    iVar7 = 0xc;
    break;
  case 0x13: ;
    iVar7 = 10;
  }
  switch((&DAT_007999e8)[param_1 * 0x84]) {
  case 0x46: ;
  case 0xbe: ;
    iVar6 = (int)(short)(&DAT_00799a2e)[param_3 + param_1 * 0x84];
    break;
  case 0x47: ;
  case 0xbf: ;
    iVar6 = (int)(short)(&DAT_00799a2e)[param_2 + param_1 * 0x84];
    break;
  case 0x48: ;
  case 0xc1: ;
    iVar6 = FUN_0045d510(param_1,param_2);
    break;
  case 0x49: ;
    uVar2 = FUN_00475b50(param_1,param_2);
    iVar6 = FUN_0045d510(uVar2);
    break;
default: ;
    if (*(char *)((int)&DAT_007999dc + param_1 * 0x108 + 1) == '\0') {
      FUN_004cc110(0x42,0x7e);
      return;
    }
    iVar6 = (int)(short)(&DAT_00799a2c)[param_3 + param_1 * 0x84];
  }
  iVar3 = FUN_004cc5c0(iVar6,0,0);
  *param_4 = *param_4 + iVar3;
  uVar4 = 0xffffffff;
  pcVar8 = &DAT_00e34238;
  do {
    pcVar9 = pcVar8;
    if (uVar4 == 0) break;
    uVar4 = uVar4 - 1;
    pcVar9 = pcVar8 + 1;
    cVar1 = *pcVar8;
    pcVar8 = pcVar9;
  } while (cVar1 != '\0');
  uVar4 = ~uVar4;
  pcVar8 = pcVar9 + -uVar4;
  pcVar9 = (char *)&DAT_00d3a4c0;
  for (uVar5 = uVar4 >> 2; uVar5 != 0; uVar5 = uVar5 - 1) {
    *(undefined4 *)pcVar9 = *(undefined4 *)pcVar8;
    pcVar8 = pcVar8 + 4;
    pcVar9 = pcVar9 + 4;
  }
  for (uVar4 = uVar4 & 3; uVar4 != 0; uVar4 = uVar4 - 1) {
    *pcVar9 = *pcVar8;
    pcVar8 = pcVar8 + 1;
    pcVar9 = pcVar9 + 1;
  }
  if (iVar6 == 1) {
    FUN_004cc110(0x42,iVar7 + 0x6f);
    return;
  }
  FUN_004cc110(0x42,iVar7 + 0x6e);
  return;
}
