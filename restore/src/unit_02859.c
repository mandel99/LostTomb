#include "restore_types.h"
#define DAT_01266418 (*(undefined4 *)0x01266418u)
#define DAT_0126641c (*(undefined4 *)0x0126641cu)
extern int FUN_0056172c();
extern void FUN_005637ee();
extern void FUN_00563844();
extern void FUN_00563adc();
extern undefined4 FUN_00563b84();
extern void FUN_00563c95();
extern void FUN_0056400a();
extern bool FUN_0056876f();

void FUN_00563935(int *param_1,int param_2,undefined4 param_3,undefined4 param_4,int param_5,
                 char param_6,undefined4 param_7,undefined4 param_8)

{
  int iVar1;
  int iVar2;
  int *piVar3;
  int *piVar4;
  uint local_1c;
  uint local_18;
  int local_14;
  int local_10;
  int local_c;
  uint local_8;
  
  local_18 = local_18 & 0xffffff00;
  local_14 = *(int *)(param_2 + 8);
  if ((local_14 < -1) || (*(int *)(param_5 + 4) <= local_14)) {
    FUN_00563844();
  }
  piVar3 = DAT_01266418;
  if (*param_1 == -0x1f928c9d) {
    if (((param_1[4] == 3) && (param_1[5] == 0x19930520)) && (param_1[7] == 0)) {
      if (DAT_01266418 == (int *)0x0) {
        return;
      }
      param_3 = DAT_0126641c;
      local_18 = CONCAT31(RESTORE_SLICE3(local_18,1),1);
      iVar2 = FUN_0056876f(DAT_01266418,1);
      if (iVar2 == 0) {
        FUN_00563844();
      }
      param_1 = piVar3;
      if (*piVar3 != -0x1f928c9d) goto LAB_00563ab1;
      if (((piVar3[4] == 3) && (piVar3[5] == 0x19930520)) && (piVar3[7] == 0)) {
        FUN_00563844();
      }
    }
    iVar2 = local_14;
    if (((*param_1 == -0x1f928c9d) && (param_1[4] == 3)) && (param_1[5] == 0x19930520)) {
      piVar3 = (int *)FUN_0056172c(param_5,param_7,local_14,&local_8,&local_1c);
      do {
        if (local_1c <= local_8) {
          if (param_6 == '\0') {
            return;
          }
          FUN_0056400a(param_1,1);
          return;
        }
        if ((*piVar3 <= iVar2) && (iVar2 <= piVar3[1])) {
          iVar1 = piVar3[4];
          for (local_10 = piVar3[3]; iVar2 = local_14, 0 < local_10; local_10 = local_10 + -1) {
            piVar4 = *(int **)(param_1[7] + 0xc);
            for (local_c = *piVar4; 0 < local_c; local_c = local_c + -1) {
              piVar4 = piVar4 + 1;
              iVar2 = FUN_00563b84(iVar1,*piVar4,param_1[7]);
              if (iVar2 != 0) {
                FUN_00563c95(param_1,param_2,param_3,param_4,param_5,iVar1,*piVar4,piVar3,param_7,
                             param_8,local_18);
                iVar2 = local_14;
                goto LAB_00563a91;
              }
            }
            iVar1 = iVar1 + 0x10;
          }
        }
LAB_00563a91: ;
        local_8 = local_8 + 1;
        piVar3 = piVar3 + 5;
      } while( true );
    }
  }
LAB_00563ab1: ;
  if (param_6 == '\0') {
    FUN_00563adc(param_1,param_2,param_3,param_4,param_5,local_14,param_7,param_8);
    return;
  }
  FUN_005637ee();
  return;
}
