#include "restore_types.h"
#define DAT_00570070 (*(undefined4 *)0x00570070u)
#define DAT_005e4ef4 (*(undefined *)0x005e4ef4u)
#define DAT_005f387c (*(undefined4 *)0x005f387cu)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00799a2c (*(undefined4 *)0x00799a2cu)
#define DAT_00799a2e (*(undefined4 *)0x00799a2eu)
#define DAT_00799a30 (*(undefined4 *)0x00799a30u)
#define DAT_00bca2e0 (*(undefined *)0x00bca2e0u)
extern void FUN_004edf50();
extern undefined4 FUN_004ef100();

void FUN_004ef200(int param_1)

{
  int iVar1;
  bool bVar2;
  int iVar3;
  int *piVar4;
  uint uVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  int local_c;
  
  if (DAT_005e4ef4 != 5) {
    iVar6 = 0;
    iVar8 = param_1 * 0x108;
    bVar2 = false;
    iVar7 = 0;
    uVar5 = (uint)*(byte *)(&DAT_00799a2c + param_1 * 0x84);
    iVar3 = FUN_004ef100(param_1);
    if (iVar3 != 0) {
      iVar3 = *(int *)(&DAT_00570070 + uVar5 * 4);
      local_c = 0;
      if (0 < iVar3) {
        piVar4 = (int *)(&DAT_005f387c + uVar5 * 0x3c0);
        do {
          if ((int)(short)(&DAT_007999ea)[param_1 * 0x84] == piVar4[-1]) {
            iVar1 = *piVar4;
            if (((iVar1 < 1) ||
                ((int)((uint)*(byte *)(&DAT_00799a2e + param_1 * 0x84) +
                      (uint)*(byte *)((int)&DAT_00799a2e + iVar8 + 1) * 6) < iVar1)) ||
               (*(char *)((int)&DAT_00799a30 + iVar8 + 1) != '\0')) {
              iVar1 = piVar4[1];
              if (((0 < iVar1) &&
                  (iVar1 <= (int)((uint)*(byte *)(&DAT_00799a2e + param_1 * 0x84) +
                                 (uint)*(byte *)((int)&DAT_00799a2e + iVar8 + 1) * 6))) &&
                 (*(char *)((int)&DAT_00799a30 + iVar8 + 2) == '\0')) {
                iVar7 = piVar4[3];
                *(char *)((int)&DAT_00799a30 + iVar8 + 2) = (char)iVar1;
                bVar2 = false;
                iVar6 = iVar1;
              }
            }
            else {
              iVar7 = piVar4[2];
              *(char *)((int)&DAT_00799a30 + iVar8 + 1) = (char)iVar1;
              bVar2 = true;
              iVar6 = iVar1;
            }
            if (iVar6 != 0) {
              iVar3 = DAT_00bca2e0 / 2;
              FUN_004edf50(param_1,iVar3,uVar5);
              if (DAT_005e4ef4 != 3) {
                return;
              }
              if (iVar7 == 0) {
                return;
              }
              iVar7 = iVar7 + (param_1 - (short)(&DAT_007999ea)[param_1 * 0x84]);
              if (bVar2) {
                *(char *)((int)&DAT_00799a30 + iVar7 * 0x108 + 1) = (char)iVar6;
              }
              else {
                *(char *)((int)&DAT_00799a30 + iVar7 * 0x108 + 2) = (char)iVar6;
              }
              FUN_004edf50(iVar7,iVar3,uVar5);
              return;
            }
          }
          piVar4 = piVar4 + 5;
          local_c = local_c + 1;
          if (iVar3 <= local_c) {
            return;
          }
        } while( true );
      }
    }
  }
  return;
}
