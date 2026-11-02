#include "restore_types.h"
#define DAT_005cc8c0 (*(undefined *)0x005cc8c0u)
#define DAT_00601ddc (*(undefined *)0x00601ddcu)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00bc9768 (*(undefined *)0x00bc9768u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bca2e8 (*(undefined *)0x00bca2e8u)
#define DAT_00bca2ec (*(undefined *)0x00bca2ecu)
extern bool FUN_0046f9a0();

uint FUN_00541ae0(int param_1)

{
  int iVar1;
  int iVar2;
  uint uVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int local_14;
  int *local_c;
  
  iVar4 = param_1 - DAT_00bca2d8;
  local_c = &DAT_00601ddc;
  do {
    iVar5 = *local_c + iVar4 / 0xe4;
    iVar6 = local_c[-1] + iVar4 % 0xe4;
    iVar1 = local_c[1];
    DAT_00bc9768 = DAT_00bca2d8 + iVar5 * 0xe4 + iVar6;
    iVar2 = 0;
    param_1 = 0;
    if (0 < iVar1) {
      local_14 = 0;
      do {
        iVar2 = 0;
        do {
          uVar3 = FUN_0046f9a0(DAT_00bca2e8 + iVar2 + iVar6,DAT_00bca2ec + param_1 + iVar5,3);
          if (((char)uVar3 != '\0') ||
             (uVar3 = (&DAT_005cc8c0)[(local_14 + iVar2) * 2] + DAT_00bc9768,
             ((&DAT_00af598c)[uVar3] & 0xfeffd76e) != 0)) {
            return uVar3 & 0xffffff00;
          }
          iVar2 = iVar2 + 1;
        } while (iVar2 < iVar1);
        iVar2 = param_1 + 1;
        local_14 = local_14 + 6;
        param_1 = iVar2;
      } while (iVar2 < iVar1);
    }
    local_c = local_c + 0xd;
    if (0x60322b < (int)local_c) {
      return CONCAT31((int3)((uint)iVar2 >> 8),1);
    }
  } while( true );
}
