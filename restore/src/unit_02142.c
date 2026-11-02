#include "restore_types.h"
#define DAT_0056f7d8 (*(undefined4 *)0x0056f7d8u)
#define DAT_0056f7ec (*(undefined4 *)0x0056f7ecu)
#define DAT_005e4ef4 (*(undefined *)0x005e4ef4u)
#define DAT_0078d790 (*(undefined *)0x0078d790u)
#define DAT_0078d7a4 (*(undefined *)0x0078d7a4u)
#define DAT_007999d8 (*(undefined4 *)0x007999d8u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00799a2c (*(undefined4 *)0x00799a2cu)
#define DAT_00bcd786 (*(undefined4 *)0x00bcd786u)
#define DAT_00eaaf5c (*(undefined1 *)0x00eaaf5cu)
extern void FUN_004b5420();
extern void FUN_004f1310();
extern undefined4 FUN_004f53d0();

void FUN_004f3aa0(int param_1)

{
  char cVar1;
  int iVar2;
  uint uVar3;
  int iVar4;
  char *pcVar5;
  int iVar6;
  int local_78;
  int local_74;
  undefined1 local_70 [64];
  int local_30;
  int local_2c;
  
  local_74 = 999999;
  local_78 = 1;
  if (1 < DAT_0078d790) {
    do {
      if (((&DAT_007999d8)[local_78 * 0x108] == '\x01') &&
         ((&DAT_007999e8)[local_78 * 0x84] == 0xb7)) {
        iVar6 = 999999;
        iVar4 = 1;
        pcVar5 = &DAT_00bcd786;
        do {
          if (pcVar5[4] != '\0') {
            cVar1 = *pcVar5;
            if (cVar1 == 'U') {
LAB_004f3b1b: ;
              if (local_78 <= *(short *)(pcVar5 + 0x54)) {
LAB_004f3b23: ;
                if (DAT_005e4ef4 == 5) {
                  iVar2 = *(int *)(&DAT_0056f7ec +
                                  (uint)*(byte *)(&DAT_00799a2c + local_78 * 0x84) * 4);
                }
                else {
                  iVar2 = *(int *)(&DAT_0056f7d8 +
                                  (uint)*(byte *)(&DAT_00799a2c + local_78 * 0x84) * 4);
                }
                if ((int)*(short *)(pcVar5 + 0x54) < iVar2 + local_78) {
                  pcVar5[0x2e] = '\x04';
                  FUN_004b5420(iVar4);
                }
              }
            }
            else if ((((cVar1 == 'Q') || (cVar1 == 'O')) || (cVar1 == 'P')) || (cVar1 == '`')) {
              if (cVar1 == 'U') goto LAB_004f3b1b;
              goto LAB_004f3b23;
            }
          }
          pcVar5 = pcVar5 + 0x184;
          iVar4 = iVar4 + 1;
        } while ((int)pcVar5 < 0xc8ad42);
        if (*(byte *)((int)&DAT_00799a2c + local_78 * 0x108 + 1) < 2) {
          cVar1 = FUN_004f53d0(local_78);
          if (cVar1 == '\0') {
            if (DAT_005e4ef4 == 5) {
              iVar6 = *(int *)(&DAT_0056f7ec + (uint)*(byte *)(&DAT_00799a2c + local_78 * 0x84) * 4)
              ;
            }
            else {
              iVar6 = *(int *)(&DAT_0056f7d8 + (uint)*(byte *)(&DAT_00799a2c + local_78 * 0x84) * 4)
              ;
            }
            iVar6 = iVar6 * param_1;
          }
        }
        else {
          FUN_004f1310(local_78,local_70);
          iVar6 = (local_2c - local_30) + 1;
          uVar3 = (uint)*(byte *)(&DAT_00799a2c + local_78 * 0x84);
          if (DAT_005e4ef4 == 5) {
            iVar4 = *(int *)(&DAT_0056f7ec + uVar3 * 4);
          }
          else {
            iVar4 = *(int *)(&DAT_0056f7d8 + uVar3 * 4);
          }
          if (iVar4 * param_1 * 2 < iVar6) {
            if (DAT_005e4ef4 == 5) {
              iVar6 = *(int *)(&DAT_0056f7ec + uVar3 * 4) * param_1 * 2;
            }
            else {
              iVar6 = *(int *)(&DAT_0056f7d8 + uVar3 * 4) * param_1 * 2;
            }
          }
        }
        if (iVar6 < local_74) {
          local_74 = iVar6;
        }
        if (DAT_005e4ef4 == 5) {
          iVar6 = *(int *)(&DAT_0056f7ec + (uint)*(byte *)(&DAT_00799a2c + local_78 * 0x84) * 4);
        }
        else {
          iVar6 = *(int *)(&DAT_0056f7d8 + (uint)*(byte *)(&DAT_00799a2c + local_78 * 0x84) * 4);
        }
        local_78 = local_78 + -1 + iVar6;
      }
      local_78 = local_78 + 1;
    } while (local_78 < DAT_0078d790);
    if (local_74 != 999999) {
      DAT_00eaaf5c = 1;
      DAT_0078d7a4 = local_74;
    }
  }
  return;
}
