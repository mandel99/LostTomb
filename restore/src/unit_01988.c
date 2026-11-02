#include "restore_types.h"
#define DAT_00586b08 (*(undefined4 *)0x00586b08u)
#define DAT_0058fe48 (*(undefined4 *)0x0058fe48u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
extern int FUN_004e5a30();

undefined4 FUN_004e6150(int param_1,int param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  undefined4 *puVar4;
  
  iVar2 = 0;
  iVar3 = 0;
  while( true ) {
    if ((int)((-(uint)((&DAT_007999e8)[param_2 * 0x84] != 0xcf) & 8) + 10) <= iVar2) {
      return 0;
    }
    puVar4 = (undefined4 *)&DAT_00586b08;
    if ((&DAT_007999e8)[param_2 * 0x84] != 0xcf) {
      puVar4 = &DAT_0058fe48;
    }
    if ((((*(short *)(*(int *)((int)puVar4 + iVar3 + 8) + 0x12) == 0) &&
         (iVar1 = FUN_004e5a30(param_2), *(int *)((int)puVar4 + iVar3) == iVar1)) &&
        (*(int *)((int)puVar4 + iVar3 + 4) == param_1)) &&
       (iVar1 = *(int *)((int)puVar4 + iVar3 + 8), 0 < *(int *)(iVar1 + 0xc))) break;
    iVar2 = iVar2 + 1;
    iVar3 = iVar3 + 0x10;
  }
  return CONCAT31((int3)((uint)iVar1 >> 8),1);
}
