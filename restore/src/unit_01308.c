#include "restore_types.h"
#define DAT_00bcd603 (*(undefined4 *)0x00bcd603u)
#define DAT_00bcd60c (*(undefined4 *)0x00bcd60cu)
#define DAT_00bcd60e (*(undefined4 *)0x00bcd60eu)
#define DAT_00bcd656 (*(undefined4 *)0x00bcd656u)
extern undefined4 FUN_0045acf0();
extern int FUN_0045d9e0();
extern int FUN_0045eab0();
extern int FUN_0045f7b0();

void FUN_0048ae50(int param_1,undefined4 param_2)

{
  int iVar1;
  int iVar2;
  
  iVar2 = param_1 * 0x184;
  iVar1 = FUN_0045f7b0((int)(short)(&DAT_00bcd60c)[param_1 * 0xc2],
                       (int)(short)(&DAT_00bcd60e)[param_1 * 0xc2],(&DAT_00bcd603)[iVar2],param_2);
  if (iVar1 == 0) {
    iVar1 = FUN_0045eab0(param_1,(&DAT_00bcd603)[iVar2],param_2);
    if (iVar1 == 0) {
      iVar1 = FUN_0045acf0(0,param_1,(&DAT_00bcd603)[iVar2],param_2);
      if (iVar1 == 0) {
        iVar1 = FUN_0045d9e0(0,param_1,(&DAT_00bcd603)[iVar2],param_2);
        if (iVar1 == 0) {
          iVar1 = FUN_0045d9e0(1,param_1,(&DAT_00bcd603)[iVar2],param_2);
        }
      }
    }
  }
  (&DAT_00bcd656)[param_1 * 0xc2] = (short)iVar1;
  return;
}
