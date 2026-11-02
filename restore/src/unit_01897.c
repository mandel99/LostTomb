#include "restore_types.h"
#define DAT_005cc8c0 (*(undefined *)0x005cc8c0u)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00bc9768 (*(undefined *)0x00bc9768u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bca2e0 (*(undefined *)0x00bca2e0u)
#define DAT_00bca2e8 (*(undefined *)0x00bca2e8u)
#define DAT_00bca2ec (*(undefined *)0x00bca2ecu)
extern bool FUN_0046f9a0();
extern void FUN_004ded50();

undefined4 FUN_004def00(int param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  char cVar4;
  int iVar5;
  int iVar6;
  int local_4c;
  int local_48;
  int local_34;
  int local_30;
  int local_2c;
  
  iVar5 = param_1 - DAT_00bca2d8;
  local_48 = 0;
  do {
    FUN_004ded50(&local_34,local_48,DAT_00bca2e0,DAT_00bca2e0);
    iVar3 = local_2c;
    iVar1 = local_34 + iVar5 % 0xe4;
    param_1 = 0;
    iVar2 = local_30 + iVar5 / 0xe4;
    DAT_00bc9768 = DAT_00bca2d8 + iVar2 * 0xe4 + iVar1;
    if (0 < local_2c) {
      local_4c = 0;
      do {
        iVar6 = 0;
        do {
          cVar4 = FUN_0046f9a0(DAT_00bca2e8 + iVar6 + iVar1,DAT_00bca2ec + param_1 + iVar2,3);
          if ((cVar4 != '\0') ||
             (((&DAT_00af598c)[(&DAT_005cc8c0)[(local_4c + iVar6) * 2] + DAT_00bc9768] & 0xfeffd76e)
              != 0)) {
            return 0;
          }
          iVar6 = iVar6 + 1;
        } while (iVar6 < iVar3);
        param_1 = param_1 + 1;
        local_4c = local_4c + 6;
      } while (param_1 < iVar3);
    }
    local_48 = local_48 + 1;
    if (0x73 < local_48) {
      return 1;
    }
  } while( true );
}
