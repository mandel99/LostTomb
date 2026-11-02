#include "restore_types.h"
#define DAT_005763bc (*(undefined4 *)0x005763bcu)
#define DAT_005e05ac (*(undefined *)0x005e05acu)
#define DAT_00e38e62 (*(undefined1 *)0x00e38e62u)
extern int FUN_004ce9e0(void);
extern undefined4 FUN_00561994();
extern undefined4 * FUN_00561a0a();
extern uint FUN_00561ca1();
extern int FUN_00561ea4();
extern int FUN_0056b1b6();
extern undefined4 FUN_0056b22e();

undefined4 FUN_004ceb60(char *param_1,undefined4 param_2,int param_3)

{
  char cVar1;
  int iVar2;
  undefined4 uVar3;
  uint uVar4;
  uint uVar5;
  int iVar6;
  char *pcVar7;
  char *pcVar8;
  char *pcVar9;
  
  iVar6 = 1;
  iVar2 = FUN_00561a0a(param_1,&DAT_005763bc);
  if (iVar2 == 0) {
    uVar4 = 0xffffffff;
    RESTORE_SLICE1(DAT_005e05ac,0) = DAT_00e38e62;
    RESTORE_SLICE1(DAT_005e05ac,3) = 0;
    do {
      pcVar7 = param_1;
      if (uVar4 == 0) break;
      uVar4 = uVar4 - 1;
      pcVar7 = param_1 + 1;
      cVar1 = *param_1;
      param_1 = pcVar7;
    } while (cVar1 != '\0');
    uVar4 = ~uVar4;
    iVar2 = -1;
    pcVar9 = (char *)&DAT_005e05ac;
    do {
      pcVar8 = pcVar9;
      if (iVar2 == 0) break;
      iVar2 = iVar2 + -1;
      pcVar8 = pcVar9 + 1;
      cVar1 = *pcVar9;
      pcVar9 = pcVar8;
    } while (cVar1 != '\0');
    pcVar7 = pcVar7 + -uVar4;
    pcVar9 = pcVar8 + -1;
    for (uVar5 = uVar4 >> 2; uVar5 != 0; uVar5 = uVar5 - 1) {
      *(undefined4 *)pcVar9 = *(undefined4 *)pcVar7;
      pcVar7 = pcVar7 + 4;
      pcVar9 = pcVar9 + 4;
    }
    for (uVar4 = uVar4 & 3; uVar4 != 0; uVar4 = uVar4 - 1) {
      *pcVar9 = *pcVar7;
      pcVar7 = pcVar7 + 1;
      pcVar9 = pcVar9 + 1;
    }
    iVar2 = FUN_00561a0a(&DAT_005e05ac,&DAT_005763bc);
    while (iVar2 == 0) {
      if (iVar6 != 1) {
        return 0;
      }
      iVar6 = FUN_004ce9e0();
      iVar2 = FUN_00561a0a(&DAT_005e05ac,&DAT_005763bc);
    }
  }
  uVar3 = FUN_0056b22e(iVar2);
  iVar6 = FUN_0056b1b6(uVar3);
  FUN_00561ea4(iVar2,param_3,0);
  FUN_00561ca1(param_2,1,iVar6 - param_3,iVar2);
  FUN_00561994(iVar2);
  return 1;
}
