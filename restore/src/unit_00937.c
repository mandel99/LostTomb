#include "restore_types.h"
#define DAT_005cd678 (*(undefined4 *)0x005cd678u)
#define DAT_0078c40c (*(undefined *)0x0078c40cu)
#define DAT_0078c410 (*(undefined *)0x0078c410u)
#define DAT_0078c414 (*(undefined4 *)0x0078c414u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00799a0d (*(undefined4 *)0x00799a0du)
#define DAT_00ae8e7c (*(undefined4 *)0x00ae8e7cu)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00b285cc (*(undefined4 *)0x00b285ccu)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00b7482c (*(undefined4 *)0x00b7482cu)
#define DAT_00bc9768 (*(undefined *)0x00bc9768u)
#define DAT_00f56b84 (*(undefined *)0x00f56b84u)
extern void FUN_0040f8f0();
extern void FUN_00487ec0();

void FUN_00468590(int param_1)

{
  bool bVar1;
  int iVar2;
  undefined4 *puVar3;
  int iVar4;
  undefined4 *puVar5;
  int *piVar6;
  int iVar7;
  int iVar8;
  int local_c;
  int local_8;
  
  if (((&DAT_00af598c)[param_1] & 0x100) != 0) {
    iVar2 = DAT_00f56b84 + 0x30;
    FUN_0040f8f0(&DAT_0078c414,4000,0);
    DAT_0078c410 = 0;
    DAT_0078c40c = 0;
    local_8 = 1;
    do {
      if ((*(byte *)(&DAT_00af598c + param_1) & 4) == 0) {
        if ((((*(short *)(&DAT_00b4e6fc + param_1 * 2) == 0) ||
             ((&DAT_007999e8)[*(short *)(&DAT_00b4e6fc + param_1 * 2) * 0x84] == 0x8a)) &&
            (iVar2 <= (int)(&DAT_00b7482c)[param_1])) && (((&DAT_00b285cc)[param_1] & 0x80) == 0)) {
          (&DAT_00b7482c)[param_1] = (&DAT_00b7482c)[param_1] + -0x30;
        }
        (&DAT_00ae8e7c)[param_1] = 1;
        local_c = -1;
        piVar6 = &DAT_005cd678;
        do {
          iVar8 = param_1 + *piVar6;
          iVar4 = (int)*(short *)(&DAT_00b4e6fc + iVar8 * 2);
          iVar7 = local_c;
          if ((iVar4 == 0) || ((&DAT_007999e8)[iVar4 * 0x84] != 0x5a)) {
            if ((((&DAT_00af598c)[iVar8] & 0x100) != 0) && ((&DAT_00ae8e7c)[iVar8] == '\0')) {
              DAT_00bc9768 = iVar8 + -0x1ca;
              iVar7 = 5;
              puVar5 = &DAT_00af598c + DAT_00bc9768;
              do {
                iVar4 = 5;
                puVar3 = puVar5;
                do {
                  *(byte *)((int)puVar3 + 3) = *(byte *)((int)puVar3 + 3) | 1;
                  puVar3 = puVar3 + 1;
                  iVar4 = iVar4 + -1;
                } while (iVar4 != 0);
                DAT_00bc9768 = DAT_00bc9768 + 0xe4;
                puVar5 = puVar5 + 0xe4;
                iVar7 = iVar7 + -1;
              } while (iVar7 != 0);
              iVar7 = iVar8;
              if (local_c != -1) {
                (&DAT_0078c414)[DAT_0078c40c] = iVar8;
                DAT_0078c40c = DAT_0078c40c + 1;
                iVar7 = local_c;
                if (999 < DAT_0078c40c) {
                  DAT_0078c40c = 0;
                }
              }
            }
          }
          else if ((&DAT_00799a0d)[iVar4 * 0x108] == '\0') {
            (&DAT_00799a0d)[iVar4 * 0x108] = 2;
            FUN_00487ec0(iVar4);
          }
          local_c = iVar7;
          piVar6 = piVar6 + 1;
        } while ((int)piVar6 < 0x5cd688);
        if (local_c == -1) {
          if (DAT_0078c410 == DAT_0078c40c) {
            return;
          }
          local_c = (&DAT_0078c414)[DAT_0078c410];
          DAT_0078c410 = DAT_0078c410 + 1;
          if (999 < DAT_0078c410) {
            DAT_0078c410 = 0;
          }
        }
        param_1 = local_c;
        if (local_c < 0) {
          return;
        }
      }
      bVar1 = local_8 < 0xcb10;
      local_8 = local_8 + 1;
    } while (bVar1);
  }
  return;
}
