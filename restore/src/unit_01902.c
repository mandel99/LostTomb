#include "restore_types.h"
#define DAT_005cc8c0 (*(undefined *)0x005cc8c0u)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00799a48 (*(undefined4 *)0x00799a48u)
#define DAT_00acf85c (*(undefined4 *)0x00acf85cu)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00b285cc (*(undefined4 *)0x00b285ccu)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00b67d1c (*(undefined4 *)0x00b67d1cu)
#define DAT_00b7482c (*(undefined4 *)0x00b7482cu)
#define DAT_00bc9768 (*(undefined *)0x00bc9768u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bca2e0 (*(undefined *)0x00bca2e0u)
#define DAT_00f56b54 (*(undefined *)0x00f56b54u)
#define DAT_00f5736c (*(undefined *)0x00f5736cu)
#define DAT_00f57464 (*(undefined *)0x00f57464u)
extern void FUN_00475bf0();
extern void FUN_004767d0();
extern void FUN_00477500();
extern void FUN_0047bf50();
extern void FUN_004ded50();
extern int FUN_004df360();

void FUN_004dfbb0(int param_1)

{
  uint *puVar1;
  byte bVar2;
  uint uVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int *piVar8;
  int iVar9;
  int local_4c;
  int *local_48;
  int local_44;
  byte local_40 [4];
  int local_3c;
  int local_38;
  undefined1 local_34 [8];
  int local_2c;
  
  iVar9 = param_1 * 0x108;
  FUN_004ded50(local_34,(int)(short)(&DAT_007999ea)[param_1 * 0x84],DAT_00bca2e0,
               *(undefined4 *)(&DAT_00799a48 + iVar9));
  iVar4 = FUN_004df360(param_1,local_40);
  if ((local_40[0] & 1) == 0) {
    FUN_0047bf50(param_1,(int)*(short *)((int)&DAT_007999dc + iVar9 + 2),
                 (int)*(short *)(&DAT_007999e0 + iVar9),local_2c,local_2c,iVar4,8);
  }
  else {
    iVar6 = (int)*(short *)(&DAT_007999e0 + iVar9);
    iVar7 = (int)*(short *)((int)&DAT_007999dc + iVar9 + 2);
    iVar9 = DAT_00bca2d8 + iVar6 * 0xe4 + iVar7;
    DAT_00bc9768 = iVar9;
    if (0 < local_2c) {
      local_44 = local_2c;
      local_48 = &DAT_005cc8c0;
      do {
        local_4c = local_2c;
        piVar8 = local_48;
        do {
          iVar5 = *piVar8 + iVar9;
          uVar3 = (&DAT_00af598c)[iVar5];
          *(undefined2 *)(&DAT_00b4e6fc + iVar5 * 2) = (undefined2)param_1;
          puVar1 = &DAT_00af598c + iVar5;
          (&DAT_00b285cc)[iVar5] = (&DAT_00b285cc)[iVar5] & 0xe0;
          bVar2 = (&DAT_00b67d1c)[iVar5];
          *puVar1 = uVar3 | 0x10000000;
          (&DAT_00b67d1c)[iVar5] = bVar2 | 0x40;
          if (iVar4 == 0) {
            (&DAT_00b7482c)[iVar5] = 0;
          }
          else {
            if (iVar4 == DAT_00f56b54) {
              (&DAT_00b7482c)[iVar5] = ((byte)(&DAT_00acf85c)[iVar5] & 7) + iVar4;
            }
            else {
              (&DAT_00b7482c)[iVar5] = iVar4;
            }
            if ((iVar4 == DAT_00f57464) || (iVar4 == DAT_00f5736c)) {
              *puVar1 = uVar3 & 0xfefed7fe | 0x10000000;
            }
            else {
              *puVar1 = uVar3 & 0xfefed77e | 0x10000000;
            }
          }
          piVar8 = piVar8 + 2;
          local_4c = local_4c + -1;
        } while (local_4c != 0);
        local_48 = local_48 + 0xc;
        local_44 = local_44 + -1;
      } while (local_44 != 0);
    }
    if (iVar4 == 0) {
      iVar4 = iVar6 + -1 + local_2c;
      iVar9 = local_2c + -1 + iVar7;
      local_3c = iVar6;
      local_38 = iVar7;
      FUN_00475bf0(iVar7,iVar6,iVar9,iVar4);
      FUN_004767d0(iVar7,iVar6,iVar9,iVar4,0,1);
      FUN_00477500(iVar7,iVar6,iVar9,iVar4);
      return;
    }
  }
  return;
}
