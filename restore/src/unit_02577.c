#include "restore_types.h"
#define DAT_005cc8c0 (*(undefined *)0x005cc8c0u)
#define DAT_005cfcf0 (*(undefined4 *)0x005cfcf0u)
#define DAT_005cfcf4 (*(undefined4 *)0x005cfcf4u)
#define DAT_0060323c (*(undefined4 *)0x0060323cu)
#define DAT_00603254 (*(undefined4 *)0x00603254u)
#define DAT_00603258 (*(undefined4 *)0x00603258u)
#define DAT_0070e44c (*(undefined *)0x0070e44cu)
#define DAT_0070e45c (*(undefined *)0x0070e45cu)
#define DAT_00af55fc (*(undefined4 *)0x00af55fcu)
#define DAT_00af5988 (*(undefined4 *)0x00af5988u)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00af5990 (*(undefined4 *)0x00af5990u)
#define DAT_00af5d1c (*(undefined4 *)0x00af5d1cu)
#define DAT_00bc9768 (*(undefined *)0x00bc9768u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bca2e0 (*(undefined *)0x00bca2e0u)
#define DAT_00bca2e8 (*(undefined *)0x00bca2e8u)
#define DAT_00bca2ec (*(undefined *)0x00bca2ecu)
#define DAT_00bca300 (*(undefined *)0x00bca300u)
#define DAT_00bca304 (*(undefined *)0x00bca304u)
#define DAT_00bca308 (*(undefined *)0x00bca308u)
#define DAT_00f56be4 (*(undefined *)0x00f56be4u)
extern bool FUN_0046f9a0();
extern void FUN_004f0ba0();
extern int FUN_0053f4f0();

void FUN_00545fc0(int param_1,int param_2)

{
  int iVar1;
  bool bVar2;
  int iVar3;
  char cVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  int *in_ECX;
  int iVar9;
  int iVar10;
  int unaff_EBP;
  int iVar11;
  int *piVar12;
  int unaff_EDI;
  int iStack_44;
  int iStack_40;
  int local_3c;
  int iStack_34;
  int iStack_30;
  int *piStack_2c;
  int *piStack_28;
  int *local_24;
  undefined4 uStack_1c;
  int local_14;
  int local_10;
  int local_c;
  int iStack_8;
  int iStack_4;
  
  iVar3 = DAT_0070e44c;
  iVar5 = DAT_00bca2e0 / 2;
  iVar6 = DAT_0070e45c + 1;
  FUN_004f0ba0(DAT_00bca304,DAT_00bca308,&local_10,&local_c);
  local_14 = (DAT_00bca300 - DAT_00bca2d8) % 0xe4;
  local_3c = 0;
  iVar1 = (DAT_00bca300 - DAT_00bca2d8) / 0xe4;
  if (0 < param_2) {
    local_24 = (int *)(param_1 + 0x24 + iVar5 * 4);
    piVar12 = (int *)(param_1 + 8);
    do {
      iVar5 = local_14;
      bVar2 = false;
      if (local_3c == in_ECX[0x14]) {
        iVar7 = *(int *)(&DAT_00af55fc +
                        (piVar12[-2] + (piVar12[-1] + iVar1) * 0xe4 + DAT_00bca2d8 + local_14) * 4);
        cVar4 = (**(code **)(*in_ECX + 0x10))(iVar7,local_3c);
        if (cVar4 != '\0') {
          iStack_4 = CONCAT31(RESTORE_SLICE3(iStack_4,1),1);
        }
        cVar4 = (**(code **)(*in_ECX + 0x10))
                          (*(undefined4 *)
                            (&DAT_00af5d1c +
                            (piVar12[-2] + (piVar12[-1] + unaff_EBP) * 0xe4 + DAT_00bca2d8 +
                            local_14) * 4),local_3c);
        if (cVar4 != '\0') {
          local_c = CONCAT31(RESTORE_SLICE3(local_c,1),1);
        }
        cVar4 = (**(code **)(*in_ECX + 0x10))
                          (*(undefined4 *)
                            (&DAT_00af5988 +
                            (piVar12[-2] + (piVar12[-1] + unaff_EDI) * 0xe4 + DAT_00bca2d8 +
                            local_14) * 4),local_3c);
        if (cVar4 != '\0') {
          local_14 = CONCAT31(RESTORE_SLICE3(local_14,1),1);
        }
        cVar4 = (**(code **)(*in_ECX + 0x10))
                          (*(undefined4 *)
                            (&DAT_00af5990 +
                            (piVar12[-2] + (piVar12[-1] + iVar7) * 0xe4 + DAT_00bca2d8 + iVar5) * 4)
                           ,local_3c);
        if (cVar4 != '\0') {
          bVar2 = true;
        }
      }
      iVar7 = piVar12[-1] + iVar1;
      iVar5 = piVar12[-2] + iVar5;
      DAT_00bc9768 = DAT_00bca2d8 + iVar7 * 0xe4 + iVar5;
      iStack_44 = 0;
      iStack_4 = iVar7;
      if (0 < *piVar12) {
        iStack_40 = 0;
        piStack_28 = (int *)&DAT_00603258;
        do {
          iStack_34 = 0;
          if (0 < *piVar12) {
            iStack_8 = iStack_44 + iVar7;
            iStack_30 = 0;
            piStack_2c = piStack_28;
            do {
              iVar11 = local_10 + local_24[-4];
              iVar7 = *local_24 + DAT_0070e44c + local_c;
              cVar4 = FUN_0046f9a0(DAT_00bca2e8 + iStack_34 + iVar5,iStack_8 + DAT_00bca2ec,3);
              if (cVar4 != '\0') goto LAB_0054633a;
              cVar4 = (**(code **)(*in_ECX + 0x10))
                                ((&DAT_00af598c)
                                 [(&DAT_005cc8c0)[(iStack_40 + iStack_34) * 2] + DAT_00bc9768],
                                 local_3c);
              if ((cVar4 == '\0') || ((local_3c == in_ECX[0x14] && (!bVar2)))) {
                uStack_1c = 0xf863;
              }
              else {
                uStack_1c = 0x1fe3;
              }
              iVar9 = 0;
              switch((int)(DAT_00bca2e0)) {
              case 0: ;
                iVar9 = *piStack_2c;
                break;
              case 2: ;
                iVar8 = *piVar12 + -1;
                iVar9 = *(int *)(&DAT_00603254 + ((iStack_40 - iStack_34) + *piVar12) * 4);
                iVar10 = iVar8 * iVar6;
                goto LAB_005462fe;
              case 4: ;
                iVar9 = (&DAT_0060323c)[(*piVar12 * 7 - iStack_30) - iStack_44];
                iVar7 = iVar7 + (*piVar12 + -1) * iVar3 * -2;
                break;
              case 6: ;
                iVar8 = *piVar12 + -1;
                iVar9 = *(int *)(&DAT_00603258 + (iStack_34 + ((*piVar12 - iStack_44) + -1) * 6) * 4
                                );
                iVar10 = -(iVar8 * iVar6);
LAB_005462fe: ;
                iVar11 = iVar11 + iVar10;
                iVar7 = iVar7 - iVar8 * iVar3;
              }
              FUN_0053f4f0(DAT_00f56be4,(&DAT_005cfcf0)[iVar9 * 2] + iVar11,
                           ((&DAT_005cfcf4)[iVar9 * 2] - DAT_0070e44c) + iVar7,uStack_1c);
LAB_0054633a: ;
              iStack_34 = iStack_34 + 1;
              iStack_30 = iStack_30 + 6;
              piStack_2c = piStack_2c + 6;
              iVar7 = iStack_4;
            } while (iStack_34 < *piVar12);
          }
          iStack_44 = iStack_44 + 1;
          piStack_28 = piStack_28 + 1;
          iStack_40 = iStack_40 + 6;
        } while (iStack_44 < *piVar12);
      }
      local_3c = local_3c + 1;
      local_24 = local_24 + 0xd;
      piVar12 = piVar12 + 0xd;
    } while (local_3c < param_2);
  }
  return;
}
