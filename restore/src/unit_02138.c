#include "restore_types.h"
#define DAT_0056f7d8 (*(undefined4 *)0x0056f7d8u)
#define DAT_0056f7ec (*(undefined4 *)0x0056f7ecu)
#define DAT_005e4ef4 (*(undefined *)0x005e4ef4u)
#define DAT_005f6b68 (*(undefined4 *)0x005f6b68u)
#define DAT_005f6b94 (*(undefined4 *)0x005f6b94u)
#define DAT_005f6bbc (*(undefined4 *)0x005f6bbcu)
#define DAT_005f6bcc (*(undefined4 *)0x005f6bccu)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_00799a2c (*(undefined4 *)0x00799a2cu)
#define DAT_00799a30 (*(undefined4 *)0x00799a30u)
#define DAT_00799a3c (*(undefined4 *)0x00799a3cu)
extern uint FUN_004eea10();
extern int FUN_004f3360();

undefined4 FUN_004f3370(int param_1,int *param_2,int *param_3,char param_4)

{
  short sVar1;
  short sVar2;
  short sVar3;
  short sVar4;
  short sVar5;
  short sVar6;
  short sVar7;
  bool bVar9;
  char cVar10;
  int iVar11;
  int iVar12;
  uint uVar13;
  int iVar14;
  int iVar15;
  byte *pbVar16;
  int iVar17;
  int iVar18;
  short *psVar19;
  bool bVar20;
  uint local_48;
  int local_44;
  int local_38;
  uint local_34;
  int local_2c;
  int local_28;
  int local_24;
  uint local_20;
  short sVar8;
  
  local_38 = 0;
  local_20 = 0;
  iVar15 = param_1 * 0x108;
  local_2c = 99999;
  sVar1 = *(short *)((int)&DAT_007999dc + iVar15 + 2);
  uVar13 = (uint)*(byte *)(&DAT_00799a2c + param_1 * 0x84);
  sVar2 = *(short *)(&DAT_007999e0 + iVar15);
  bVar9 = false;
  iVar11 = (*(int *)(&DAT_005f6b94 + uVar13 * 4) + param_1) * 0x108;
  sVar3 = *(short *)((int)&DAT_007999dc + iVar11 + 2);
  sVar4 = *(short *)(&DAT_007999e0 + iVar11);
  iVar11 = (*(int *)(&DAT_005f6b94 + uVar13 * 4) + param_1) * 0x108;
  sVar5 = *(short *)((int)&DAT_007999dc + iVar11 + 2);
  sVar6 = *(short *)(&DAT_007999e0 + iVar11);
  bVar20 = DAT_005e4ef4 != 5;
  pbVar16 = (byte *)((int)&DAT_00799a2c + iVar15 + 1);
  iVar11 = param_1;
  do {
    if (DAT_005e4ef4 == 5) {
      iVar12 = *(int *)(&DAT_0056f7ec + uVar13 * 4);
    }
    else {
      iVar12 = *(int *)(&DAT_0056f7d8 + uVar13 * 4);
    }
    if (iVar12 + param_1 <= iVar11) {
      if (local_38 != 0) {
        return 1;
      }
      if ((((!bVar9) && (DAT_005e4ef4 != 5)) && (*(char *)((int)&DAT_00799a2c + iVar15 + 1) == '\n')
          ) && (*(char *)((int)&DAT_00799a30 + iVar15 + 3) == '\x0f')) {
        if (param_4 != '\0') {
          iVar11 = 0;
          psVar19 = &DAT_00799a3c + param_1 * 0x84;
          do {
            if ((0 < *psVar19) && (cVar10 = FUN_004eea10(iVar11 + param_1), cVar10 != '\0')) {
              return 0;
            }
            iVar11 = iVar11 + 1;
            psVar19 = psVar19 + 0x84;
          } while (iVar11 < 4);
        }
        *param_2 = *(short *)((int)&DAT_007999dc + iVar15 + 2) + 1;
        *param_3 = *(short *)(&DAT_007999e0 + iVar15) + 1;
        return 1;
      }
      return 0;
    }
    if (((param_4 == '\0') || (*(short *)(pbVar16 + 0xf) < 1)) ||
       (cVar10 = FUN_004eea10(iVar11), cVar10 == '\0')) {
      local_48 = (uint)*pbVar16;
      if (local_48 == 3) {
        local_48 = 4;
      }
      local_34 = (uint)pbVar16[6];
      if (local_34 == 0xf) {
        local_34 = 0;
        local_48 = FUN_004f3360(local_48);
      }
      if ((int)local_48 <= (int)((-(uint)bVar20 & 5) + 5)) {
        iVar12 = 0;
        local_44 = 99999;
        local_28 = 0;
        local_24 = 0;
        do {
          if ((local_34 & 1 << ((byte)iVar12 & 0x1f)) == 0) {
            iVar17 = (int)*(short *)(pbVar16 + -0x4f) + (&DAT_005f6bbc)[iVar12];
            iVar18 = (int)*(short *)(pbVar16 + -0x4d) + (&DAT_005f6bcc)[iVar12];
            iVar14 = *(int *)(&DAT_005f6b68 + local_48 * 4);
            sVar7 = sVar2;
            sVar8 = sVar1;
            if ((iVar14 == 0) || (sVar7 = sVar6, sVar8 = sVar5, iVar14 == 1)) {
              iVar14 = (iVar18 - sVar7) * (iVar18 - sVar7) + (iVar17 - sVar8) * (iVar17 - sVar8);
            }
            else if (iVar14 == 2) {
              iVar14 = iVar17 - sVar3;
              iVar14 = (1000 - (iVar18 - sVar4) * (iVar18 - sVar4)) - iVar14 * iVar14;
            }
            else {
              iVar14 = 99999;
            }
            if (iVar14 < local_44) {
              local_44 = iVar14;
              local_28 = iVar17;
              local_24 = iVar18;
            }
          }
          iVar12 = iVar12 + 1;
        } while (iVar12 < 4);
        if (local_38 == 0) {
LAB_004f35ab: ;
          local_2c = local_44;
          *param_2 = local_28;
          *param_3 = local_24;
          local_38 = iVar11;
          local_20 = local_48;
        }
        else if (local_48 == local_20) {
          if (local_44 < local_2c) goto LAB_004f35ab;
        }
        else if ((int)local_48 < (int)local_20) goto LAB_004f35ab;
      }
    }
    else {
      bVar9 = true;
    }
    iVar11 = iVar11 + 1;
    pbVar16 = pbVar16 + 0x108;
  } while( true );
}
