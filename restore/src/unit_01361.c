#include "restore_types.h"
#define DAT_009784dc (*(undefined4 *)0x009784dcu)
#define DAT_00bcd5fc (*(undefined4 *)0x00bcd5fcu)
#define DAT_00bcd606 (*(undefined4 *)0x00bcd606u)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd714 (*(undefined4 *)0x00bcd714u)
#define DAT_00bcd778 (*(undefined4 *)0x00bcd778u)
#define DAT_00bcd786 (*(undefined4 *)0x00bcd786u)
extern int FUN_00492360();
extern int FUN_0049b3c0();
extern int FUN_004a3540();
extern void FUN_004b3250();
extern void FUN_004b5420();
extern undefined4 FUN_004f6620();
extern int FUN_004f6630();

void FUN_004a36e0(int param_1,undefined4 param_2,int param_3)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  undefined1 *puVar5;
  
  iVar1 = *(int *)(&DAT_00bcd714 + param_1 * 0x184);
  iVar2 = FUN_004f6630();
  if (iVar2 <= param_3) {
    param_3 = FUN_004f6630();
  }
  param_2 = 0;
  if (0 < param_3) {
    do {
      iVar2 = FUN_004f6620();
      iVar3 = iVar2 * 0x184;
      if (((&DAT_00bcd606)[iVar3] == '\x01') && ((&DAT_00bcd630)[iVar3] == '\a')) {
        (&DAT_00bcd630)[iVar3] = 6;
        *(undefined4 *)(&DAT_00bcd5fc + iVar3) = 0;
        (&DAT_009784dc)[param_2 + iVar1 * 0xb] = iVar2;
        if (*(int *)(&DAT_00bcd778 + iVar3) != 0) {
          *(undefined4 *)(&DAT_00bcd778 + iVar3) = 0;
        }
        FUN_004b3250(iVar2);
        FUN_004b5420(iVar2);
        iVar3 = 1;
        puVar5 = &DAT_00bcd786;
        do {
          switch(*puVar5) {
          case 0x15: ;
            iVar4 = FUN_00492360(iVar3);
            break;
default: ;
            goto switchD_004a37d7_caseD_16;
          case 0x42: ;
            iVar4 = FUN_0049b3c0(iVar3);
            break;
          case 0x56: ;
          case 0x60: ;
            iVar4 = FUN_004a3540(iVar3);
          }
          if (iVar4 == iVar2) {
            FUN_004b3250(iVar3);
            FUN_004b5420(iVar3);
          }
switchD_004a37d7_caseD_16: ;
          puVar5 = puVar5 + 0x184;
          iVar3 = iVar3 + 1;
        } while ((int)puVar5 < 0xc8ad42);
      }
      param_2 = param_2 + 1;
    } while (param_2 < param_3);
  }
  return;
}
