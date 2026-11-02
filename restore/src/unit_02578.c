#include "restore_types.h"
#define DAT_005cc8c0 (*(undefined *)0x005cc8c0u)
#define DAT_00af55fc (*(undefined4 *)0x00af55fcu)
#define DAT_00af5988 (*(undefined4 *)0x00af5988u)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00af5990 (*(undefined4 *)0x00af5990u)
#define DAT_00af5d1c (*(undefined4 *)0x00af5d1cu)
#define DAT_00bc9768 (*(undefined *)0x00bc9768u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bca2e8 (*(undefined *)0x00bca2e8u)
#define DAT_00bca2ec (*(undefined *)0x00bca2ecu)
extern bool FUN_0046f9a0();

char FUN_005463e0(int param_1,int param_2,int param_3)

{
  int iVar1;
  char cVar2;
  int iVar3;
  int *in_ECX;
  int iVar4;
  int iVar5;
  int unaff_EBX;
  int iVar6;
  int unaff_ESI;
  int *piVar7;
  char local_11;
  int local_c;
  int local_4;
  
  iVar5 = (param_1 - DAT_00bca2d8) % 0xe4;
  cVar2 = '\x01';
  local_11 = '\x01';
  local_c = 0;
  iVar1 = (param_1 - DAT_00bca2d8) / 0xe4;
  if (0 < param_3) {
    piVar7 = (int *)(param_2 + 4);
    local_4 = iVar5;
    while (iVar3 = local_c, cVar2 = '\0', local_11 != '\0') {
      if (local_c == in_ECX[0x14]) {
        iVar4 = local_c;
        cVar2 = (**(code **)(*in_ECX + 0x10))
                          (*(undefined4 *)
                            (&DAT_00af55fc +
                            (piVar7[-1] + (*piVar7 + iVar1) * 0xe4 + DAT_00bca2d8 + iVar5) * 4));
        if (cVar2 != '\0') {
          local_4 = CONCAT31(RESTORE_SLICE3(local_4,1),1);
        }
        cVar2 = (**(code **)(*in_ECX + 0x10))
                          (*(undefined4 *)
                            (&DAT_00af5d1c +
                            (piVar7[-1] + (*piVar7 + unaff_EBX) * 0xe4 + DAT_00bca2d8 + iVar5) * 4),
                           local_c);
        if (cVar2 != '\0') {
          local_c = CONCAT31(RESTORE_SLICE3(local_c,1),1);
        }
        (**(code **)(*in_ECX + 0x10))
                  (*(undefined4 *)
                    (&DAT_00af5988 +
                    (piVar7[-1] + (*piVar7 + unaff_ESI) * 0xe4 + DAT_00bca2d8 + iVar5) * 4),iVar3);
        cVar2 = (**(code **)(*in_ECX + 0x10))
                          (*(undefined4 *)
                            (&DAT_00af5990 +
                            (piVar7[-1] + (*piVar7 + iVar4) * 0xe4 + DAT_00bca2d8 + iVar5) * 4),
                           iVar3);
        if (cVar2 == '\0') {
          local_11 = '\0';
        }
      }
      iVar6 = *piVar7 + iVar1;
      iVar4 = piVar7[-1] + iVar5;
      DAT_00bc9768 = DAT_00bca2d8 + iVar6 * 0xe4 + iVar4;
      iVar3 = piVar7[1];
      param_1 = 0;
      if (0 < iVar3) {
        param_2 = 0;
        do {
          if (local_11 == '\0') break;
          iVar5 = 0;
          if (0 < iVar3) {
            do {
              if (local_11 == '\0') break;
              cVar2 = FUN_0046f9a0(DAT_00bca2e8 + iVar5 + iVar4,DAT_00bca2ec + param_1 + iVar6,3);
              if (cVar2 == '\0') {
                local_11 = (**(code **)(*in_ECX + 0x10))
                                     ((&DAT_00af598c)
                                      [(&DAT_005cc8c0)[(param_2 + iVar5) * 2] + DAT_00bc9768],
                                      local_c);
              }
              else {
                local_11 = '\0';
              }
              iVar5 = iVar5 + 1;
            } while (iVar5 < piVar7[1]);
          }
          param_2 = param_2 + 6;
          iVar3 = piVar7[1];
          param_1 = param_1 + 1;
          iVar5 = local_4;
        } while (param_1 < iVar3);
      }
      local_c = local_c + 1;
      piVar7 = piVar7 + 0xd;
      if (param_3 <= local_c) {
        return local_11;
      }
    }
  }
  return cVar2;
}
