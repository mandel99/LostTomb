#include "restore_types.h"
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
extern int FUN_004eb800();
extern longlong FUN_0056133c();

undefined4 FUN_004e75a0(int param_1,int *param_2,int *param_3)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int local_10;
  int local_c;
  int local_8;
  int local_4;
  
  iVar5 = DAT_00bca2d0;
  if (DAT_00bca2d0 < DAT_00bca2d4) {
    iVar5 = DAT_00bca2d4;
  }
  iVar4 = param_1 * 0x108;
  iVar1 = FUN_004eb800((int)*(short *)((int)&DAT_007999dc + iVar4 + 2),
                       (int)*(short *)(&DAT_007999e0 + iVar4),0,0,0x40000,&local_10,&local_c,iVar5,0
                      );
  param_1 = iVar5;
  if (iVar1 != 0) {
    param_1 = FUN_0056133c();
  }
  iVar2 = FUN_004eb800((int)*(short *)((int)&DAT_007999dc + iVar4 + 2),
                       (int)*(short *)(&DAT_007999e0 + iVar4),0,0,4,&local_4,&local_8,iVar5,0);
  iVar1 = local_4;
  iVar3 = iVar5;
  if (iVar2 != 0) {
    local_4 = *(short *)((int)&DAT_007999dc + iVar4 + 2) - local_4;
    local_4 = (*(short *)(&DAT_007999e0 + iVar4) - local_8) *
              (*(short *)(&DAT_007999e0 + iVar4) - local_8) + local_4 * local_4;
    iVar3 = FUN_0056133c();
  }
  if ((iVar3 < param_1) && (iVar3 < iVar5)) {
    *param_2 = iVar1;
    *param_3 = local_8;
    return 1;
  }
  if ((param_1 < iVar3) && (param_1 < iVar5)) {
    *param_2 = local_10;
    *param_3 = local_c;
    return 1;
  }
  return 0;
}
