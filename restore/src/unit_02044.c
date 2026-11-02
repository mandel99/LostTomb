#include "restore_types.h"
#define DAT_00d3b1c4 (*(undefined *)0x00d3b1c4u)
#define DAT_00d3b1c8 (*(undefined4 *)0x00d3b1c8u)
#define DAT_00ea3e34 (*(undefined4 *)0x00ea3e34u)

void FUN_004e9400(int param_1,int param_2)

{
  bool bVar1;
  uint uVar2;
  int iVar3;
  
  bVar1 = false;
  if (param_2 != 0) {
    iVar3 = DAT_00d3b1c4;
    do {
      uVar2 = (&DAT_00d3b1c8)[iVar3] & 0x3f;
      if (uVar2 < 0x33) {
        if ((uVar2 < 10) && (bVar1)) {
          uVar2 = uVar2 + 0x14;
        }
      }
      else {
        uVar2 = uVar2 - 0x1e;
      }
      *(short *)(&DAT_00ea3e34 + (uVar2 + param_1 * 0x24ec) * 2) =
           *(short *)(&DAT_00ea3e34 + (uVar2 + param_1 * 0x24ec) * 2) + 1;
      iVar3 = iVar3 + 1;
      if (99 < iVar3) {
        iVar3 = 0;
      }
      bVar1 = !bVar1;
      param_2 = param_2 + -1;
    } while (param_2 != 0);
  }
  return;
}
