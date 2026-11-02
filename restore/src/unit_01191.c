#include "restore_types.h"
#define DAT_005c9afe (*(undefined4 *)0x005c9afeu)
#define DAT_0070c8ec (*(undefined4 *)0x0070c8ecu)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00bc9768 (*(undefined *)0x00bc9768u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define s_restore_005c9b4a (*(char (*)[2])0x005c9b4au)
extern bool FUN_00481aa0();

undefined4 FUN_00481490(int param_1,int param_2,short *param_3)

{
  short sVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  
  if (0 < (int)param_3) {
    iVar3 = (int)param_3;
    if (10 < (int)param_3) {
      iVar3 = 0xb;
    }
    iVar4 = 0;
    sVar1 = *(short *)(&DAT_005c9afe + iVar3 * 2);
    param_3 = &DAT_0070c8ec + ((undefined4 *)&s_restore_005c9b4a)[iVar3 + 4];
    DAT_00bc9768 = DAT_00bca2d8 + param_2 * 0xe4 + param_1;
    if (0 < sVar1) {
      do {
        iVar2 = FUN_00481aa0(param_1,param_2,1,iVar3,iVar4);
        if ((iVar2 == 0) && ((*(byte *)(&DAT_00af598c + DAT_00bc9768 + *param_3) & 3) == 0)) {
          return 0;
        }
        iVar4 = iVar4 + 1;
        param_3 = param_3 + 1;
      } while (iVar4 < sVar1);
    }
  }
  return 1;
}
