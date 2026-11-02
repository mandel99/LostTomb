#include "restore_types.h"
#define DAT_005e05ac (*(undefined *)0x005e05acu)
#define DAT_00e38e62 (*(undefined1 *)0x00e38e62u)
extern int FUN_004ce9e0(void);
extern undefined4 FUN_005647eb();
extern uint FUN_00568eab();
extern int FUN_0056b1b6();

int FUN_004cea30(char *param_1,int param_2)

{
  char cVar1;
  int iVar2;
  uint uVar3;
  uint uVar4;
  int iVar5;
  char *pcVar6;
  char *pcVar7;
  char *pcVar8;
  
  iVar5 = 1;
  iVar2 = FUN_00568eab(param_1,0x8000);
  if (-1 < iVar2) {
    iVar5 = FUN_0056b1b6(iVar2);
    FUN_005647eb(iVar2);
    return iVar5 - param_2;
  }
  uVar3 = 0xffffffff;
  RESTORE_SLICE1(DAT_005e05ac,0) = DAT_00e38e62;
  RESTORE_SLICE1(DAT_005e05ac,3) = 0;
  do {
    pcVar6 = param_1;
    if (uVar3 == 0) break;
    uVar3 = uVar3 - 1;
    pcVar6 = param_1 + 1;
    cVar1 = *param_1;
    param_1 = pcVar6;
  } while (cVar1 != '\0');
  uVar3 = ~uVar3;
  iVar2 = -1;
  pcVar8 = (char *)&DAT_005e05ac;
  do {
    pcVar7 = pcVar8;
    if (iVar2 == 0) break;
    iVar2 = iVar2 + -1;
    pcVar7 = pcVar8 + 1;
    cVar1 = *pcVar8;
    pcVar8 = pcVar7;
  } while (cVar1 != '\0');
  pcVar6 = pcVar6 + -uVar3;
  pcVar8 = pcVar7 + -1;
  for (uVar4 = uVar3 >> 2; uVar4 != 0; uVar4 = uVar4 - 1) {
    *(undefined4 *)pcVar8 = *(undefined4 *)pcVar6;
    pcVar6 = pcVar6 + 4;
    pcVar8 = pcVar8 + 4;
  }
  for (uVar3 = uVar3 & 3; uVar3 != 0; uVar3 = uVar3 - 1) {
    *pcVar8 = *pcVar6;
    pcVar6 = pcVar6 + 1;
    pcVar8 = pcVar8 + 1;
  }
  iVar2 = FUN_00568eab(&DAT_005e05ac,0x8000);
  do {
    if (-1 < iVar2) {
LAB_004ceae8: ;
      iVar5 = FUN_0056b1b6(iVar2);
      FUN_005647eb(iVar2);
      return iVar5 - param_2;
    }
    if (iVar5 != 1) {
      if (iVar2 < 0) {
        return 0;
      }
      goto LAB_004ceae8;
    }
      iVar5 = FUN_004ce9e0();
    iVar2 = FUN_00568eab(&DAT_005e05ac,0x8000);
  } while( true );
}
