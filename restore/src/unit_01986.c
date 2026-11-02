#include "restore_types.h"
#define DAT_00586b08 (*(undefined4 *)0x00586b08u)
#define DAT_0058fe48 (*(undefined4 *)0x0058fe48u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)

void FUN_004e6070(int param_1,int *param_2,int param_3)

{
  undefined4 *puVar1;
  int iVar2;
  int iVar3;
  
  *param_2 = 0;
  iVar3 = 0;
  for (iVar2 = 0; iVar2 < (int)((-(uint)((&DAT_007999e8)[param_3 * 0x84] != 0xcf) & 8) + 10);
      iVar2 = iVar2 + 1) {
    puVar1 = (undefined4 *)&DAT_00586b08;
    if ((&DAT_007999e8)[param_3 * 0x84] != 0xcf) {
      puVar1 = &DAT_0058fe48;
    }
    if ((*(short *)(*(int *)(iVar3 + 8 + (int)puVar1) + 0x12) != 0) &&
       (*(int *)(iVar3 + 4 + (int)puVar1) == param_1)) {
      *param_2 = *param_2 + 1;
    }
    iVar3 = iVar3 + 0x10;
  }
  return;
}
