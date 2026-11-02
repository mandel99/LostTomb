#include "restore_types.h"
#define DAT_00799afc (*(short *)0x00799afcu)
extern int FUN_004cde60();

void FUN_004e7e60(undefined4 *param_1,undefined4 *param_2,undefined4 *param_3,undefined4 *param_4)

{
  short *psVar1;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  
  iVar7 = 0;
  iVar4 = 0;
  iVar6 = 0;
  iVar8 = 0;
  iVar5 = 0;
  psVar1 = &DAT_00799afc;
  do {
    if ((((char)psVar1[-0xe] == '\x01') && (*(char *)((int)psVar1 + -0x17) != '\0')) &&
       (*psVar1 != 0)) {
      iVar3 = (int)*psVar1;
      iVar7 = iVar7 + iVar3;
      if ((char)psVar1[0x2d] != '\0') {
        iVar4 = iVar4 + iVar3;
      }
      if ((char)psVar1[0x2e] != '\0') {
        iVar6 = iVar6 + iVar3;
      }
      if (*(char *)((int)psVar1 + 0x59) != '\0') {
        iVar5 = iVar5 + iVar3;
      }
      if ((char)psVar1[0x29] != '\0') {
        iVar8 = iVar8 + iVar3;
      }
    }
    psVar1 = psVar1 + 0x84;
  } while ((int)psVar1 < 0x89b6f4);
  uVar2 = FUN_004cde60(iVar4,iVar7);
  *param_1 = uVar2;
  uVar2 = FUN_004cde60(iVar6,iVar7);
  *param_2 = uVar2;
  uVar2 = FUN_004cde60(iVar5,iVar7);
  *param_4 = uVar2;
  uVar2 = FUN_004cde60(iVar8,iVar7);
  *param_3 = uVar2;
  return;
}
