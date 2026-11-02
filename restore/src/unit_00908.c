#include "restore_types.h"
#define DAT_0070cdf4 (*(undefined4 *)0x0070cdf4u)
#define DAT_00784bbc (*(undefined *)0x00784bbcu)
#define DAT_0078d790 (*(undefined *)0x0078d790u)
#define DAT_00799a2c (*(undefined4 *)0x00799a2cu)
#define DAT_00799b5f (*(undefined4 *)0x00799b5fu)
#define DAT_00ea1890 (*(undefined4 *)0x00ea1890u)
#define DAT_00ea41e0 (*(undefined4 *)0x00ea41e0u)
#define DAT_00ea41e4 (*(undefined4 *)0x00ea41e4u)
#define DAT_00ea5e4c (*(undefined4 *)0x00ea5e4cu)
#define DAT_00ea5e50 (*(undefined4 *)0x00ea5e50u)
#define DAT_00ea5e54 (*(undefined4 *)0x00ea5e54u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define DAT_00f53dd8 (*(undefined *)0x00f53dd8u)
#define DAT_00f53ddc (*(undefined4 *)0x00f53ddcu)
extern void FUN_0045d630();
extern undefined4 FUN_004c5340();
extern int FUN_004cde40();

void FUN_0045e420(void)

{
  short sVar1;
  int iVar2;
  int iVar3;
  undefined4 uVar4;
  int iVar5;
  int iVar6;
  uint uVar7;
  byte *pbVar8;
  int *piVar9;
  int local_14;
  int local_10;
  int local_c;
  
  FUN_0045d630(DAT_00eaac28);
  iVar2 = DAT_00eaac28;
  iVar3 = DAT_00eaac28 * 0x49d8;
  *(undefined4 *)(&DAT_00ea41e4 + iVar3) = 0;
  *(undefined4 *)(&DAT_00ea1890 + iVar3) = 0;
  uVar4 = FUN_004c5340(iVar2);
  *(undefined4 *)(&DAT_00ea41e4 + DAT_00eaac28 * 0x49d8) = 1;
  local_14 = 0;
  local_c = 1;
  if (0 < DAT_0078d790) {
    local_10 = 0x84;
    pbVar8 = &DAT_00799b5f;
    do {
      if ((pbVar8[-0x7f] == 1) && (pbVar8[-0x7a] != 0)) {
        iVar2 = *(int *)(&DAT_0070cdf4 + *(short *)(pbVar8 + -0x6d) * 0x58);
        iVar5 = FUN_004cde40((int)*(short *)(pbVar8 + -99),uVar4);
        iVar3 = DAT_00784bbc;
        if (1 < iVar2) {
          iVar5 = iVar5 / iVar2;
        }
        *pbVar8 = 0;
        iVar6 = DAT_00eaac28;
        if (iVar3 == 0) {
          if ((0 < iVar2) && (iVar3 = 0, 0 < DAT_00f53dd8)) {
            piVar9 = &DAT_00f53ddc;
            do {
              if (*piVar9 != 0) {
                switch(*piVar9) {
                case 1: ;
                  iVar6 = 0;
                  break;
                case 2: ;
                  iVar6 = 1;
                  break;
                case 3: ;
                  iVar6 = 2;
                  break;
                case 4: ;
                  iVar6 = 3;
                  break;
                case 5: ;
                  iVar6 = 4;
                  break;
                case 6: ;
                  iVar6 = 5;
                  break;
                case 7: ;
                  iVar6 = 6;
                  break;
                case 8: ;
                  iVar6 = 7;
                  break;
default: ;
                  iVar6 = -1;
                  break;
                case 0xd: ;
                  iVar6 = 9;
                  break;
                case 0xf: ;
                  iVar6 = 8;
                  break;
                case 0x11: ;
                  iVar6 = 10;
                  break;
                case 0x13: ;
                  iVar6 = 0xb;
                }
                iVar6 = local_10 + iVar6;
                sVar1 = (&DAT_00799a2c)[iVar6];
                if (sVar1 < iVar5) {
                  if (sVar1 != 0) {
                    (&DAT_00799a2c)[iVar6] = 0;
                    goto LAB_0045e5c0;
                  }
                }
                else {
                  (&DAT_00799a2c)[iVar6] = sVar1 - (short)iVar5;
LAB_0045e5c0: ;
                  local_14 = local_14 + iVar5;
                  *pbVar8 = *pbVar8 + 1;
                }
                uVar7 = (uint)*pbVar8;
                if (*(int *)(&DAT_00ea41e4 + DAT_00eaac28 * 0x49d8) < (int)uVar7) {
                  *(uint *)(&DAT_00ea41e4 + DAT_00eaac28 * 0x49d8) = uVar7;
                }
                if (iVar2 <= (int)uVar7) break;
              }
              iVar3 = iVar3 + 1;
              piVar9 = piVar9 + 1;
            } while (iVar3 < DAT_00f53dd8);
          }
        }
        else {
          *(short *)(pbVar8 + -0x2b) = (short)iVar5;
          *pbVar8 = 1;
          *(undefined4 *)(&DAT_00ea41e0 + iVar6 * 0x49d8) = 1;
        }
      }
      local_c = local_c + 1;
      local_10 = local_10 + 0x84;
      pbVar8 = pbVar8 + 0x108;
    } while (local_c <= DAT_0078d790);
  }
  iVar2 = DAT_00eaac28;
  (&DAT_00ea5e4c)[DAT_00eaac28 * 0x1276] = local_14;
  (&DAT_00ea5e50)[iVar2 * 0x1276] = (&DAT_00ea5e54)[iVar2 * 0x1276];
  (&DAT_00ea5e54)[iVar2 * 0x1276] = 0;
  return;
}
