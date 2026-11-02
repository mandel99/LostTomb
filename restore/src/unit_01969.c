#include "restore_types.h"
#define DAT_005cc8c0 (*(undefined *)0x005cc8c0u)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00bc9768 (*(undefined *)0x00bc9768u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bca2e8 (*(undefined *)0x00bca2e8u)
#define DAT_00bca2ec (*(undefined *)0x00bca2ecu)
extern bool FUN_0046f9a0();

char FUN_004e5330(int param_1,int param_2,int param_3)

{
  char cVar1;
  int iVar2;
  int *in_ECX;
  int *piVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  int local_10;
  
  iVar6 = param_1 - DAT_00bca2d8;
  cVar1 = '\x01';
  local_10 = 0;
  if (0 < param_3) {
    piVar3 = (int *)(param_2 + 8);
    do {
      if (cVar1 == '\0') {
        return '\0';
      }
      iVar8 = piVar3[-1] + iVar6 / 0xe4;
      iVar4 = piVar3[-2] + iVar6 % 0xe4;
      iVar5 = 0;
      DAT_00bc9768 = DAT_00bca2d8 + iVar8 * 0xe4 + iVar4;
      iVar2 = *piVar3;
      if (0 < iVar2) {
        param_1 = 0;
        do {
          if (cVar1 == '\0') break;
          iVar7 = 0;
          if (0 < iVar2) {
            do {
              if (cVar1 == '\0') break;
              cVar1 = FUN_0046f9a0(DAT_00bca2e8 + iVar7 + iVar4,DAT_00bca2ec + iVar5 + iVar8,3);
              if (cVar1 == '\0') {
                cVar1 = (**(code **)(*in_ECX + 0x10))
                                  ((&DAT_00af598c)
                                   [(&DAT_005cc8c0)[(param_1 + iVar7) * 2] + DAT_00bc9768],local_10)
                ;
              }
              else {
                cVar1 = '\0';
              }
              iVar7 = iVar7 + 1;
            } while (iVar7 < *piVar3);
          }
          param_1 = param_1 + 6;
          iVar5 = iVar5 + 1;
          iVar2 = *piVar3;
        } while (iVar5 < iVar2);
      }
      local_10 = local_10 + 1;
      piVar3 = piVar3 + 0xd;
    } while (local_10 < param_3);
  }
  return cVar1;
}
