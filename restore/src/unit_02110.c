#include "restore_types.h"
#define DAT_0056f7d8 (*(undefined4 *)0x0056f7d8u)
#define DAT_0056f7ec (*(undefined4 *)0x0056f7ecu)
#define DAT_00570070 (*(undefined4 *)0x00570070u)
#define DAT_005e4ef4 (*(undefined *)0x005e4ef4u)
#define DAT_005f3878 (*(undefined4 *)0x005f3878u)
#define DAT_007999d8 (*(undefined4 *)0x007999d8u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00799a2c (*(undefined4 *)0x00799a2cu)
#define DAT_00799a3c (*(undefined4 *)0x00799a3cu)
extern uint FUN_004eea10();
extern undefined4 FUN_004ef100();
extern void FUN_004f3000();

undefined4 FUN_004f01e0(undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  byte bVar1;
  char cVar2;
  int iVar3;
  undefined4 uVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  
  if (DAT_005e4ef4 != 5) {
    iVar5 = 1;
    do {
      if (((&DAT_007999d8)[iVar5 * 0x108] == '\x01') && ((&DAT_007999e8)[iVar5 * 0x84] == 0xb7)) {
        iVar6 = 0;
        bVar1 = *(byte *)(&DAT_00799a2c + iVar5 * 0x84);
        if (0 < *(int *)(&DAT_00570070 + (uint)bVar1 * 4)) {
          do {
            iVar7 = *(int *)(&DAT_005f3878 +
                            ((uint)*(byte *)(&DAT_00799a2c + iVar5 * 0x84) * 0x30 + iVar6) * 0x14) +
                    iVar5;
            iVar3 = FUN_004ef100(iVar7);
            if (iVar3 != 0) {
              if (((&DAT_00799a3c)[iVar7 * 0x84] == 0) ||
                 (cVar2 = FUN_004eea10(iVar7), cVar2 == '\0')) {
                (&DAT_00799a3c)[iVar7 * 0x84] = (short)param_3;
                uVar4 = (FUN_004f3000(param_3,iVar7), restore_unresolved_return(0x004f3000u));
                return uVar4;
              }
              break;
            }
            iVar6 = iVar6 + 1;
          } while (iVar6 < *(int *)(&DAT_00570070 + (uint)bVar1 * 4));
        }
        if (DAT_005e4ef4 == 5) {
          iVar6 = *(int *)(&DAT_0056f7ec + (uint)*(byte *)(&DAT_00799a2c + iVar5 * 0x84) * 4);
        }
        else {
          iVar6 = *(int *)(&DAT_0056f7d8 + (uint)*(byte *)(&DAT_00799a2c + iVar5 * 0x84) * 4);
        }
        iVar5 = iVar5 + -1 + iVar6;
      }
      iVar5 = iVar5 + 1;
    } while (iVar5 < 4000);
  }
  return 0;
}
