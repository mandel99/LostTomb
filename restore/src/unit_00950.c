#include "restore_types.h"
#define DAT_0070ce0c (*(undefined4 *)0x0070ce0cu)
#define DAT_0078d790 (*(undefined *)0x0078d790u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00799abc (*(undefined4 *)0x00799abcu)
#define DAT_00799b0a (*(undefined4 *)0x00799b0au)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd606 (*(undefined4 *)0x00bcd606u)
#define DAT_00bcd652 (*(undefined4 *)0x00bcd652u)
extern uint FUN_00469d50();
extern void FUN_00469e40();

void FUN_00469c60(int param_1)

{
  short sVar1;
  int iVar2;
  undefined4 uVar3;
  short *psVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  
  iVar5 = DAT_0078d790;
  if (0 < param_1) {
    do {
      iVar7 = 1;
      iVar8 = 0;
      iVar6 = 0;
      if (0 < iVar5) {
        psVar4 = &DAT_00799b0a;
        do {
          if ((((char)psVar4[-0x15] == '\x01') && (*(char *)((int)psVar4 + -0x25) != '\0')) &&
             (psVar4[-7] != 0)) {
            sVar1 = *psVar4;
            if (0 < sVar1) {
              iVar2 = sVar1 * 0x184;
              if ((((&DAT_00bcd606)[iVar2] != '\0') &&
                  (((&DAT_00bcd602)[iVar2] == '\x17' || ((&DAT_00bcd602)[iVar2] == '\x18')))) &&
                 ((short)(&DAT_00bcd652)[sVar1 * 0xc2] == iVar7)) goto LAB_00469cf2;
              *psVar4 = 0;
            }
            if (iVar8 < *(int *)(psVar4 + 0x5d)) {
              iVar6 = iVar7;
              iVar8 = *(int *)(psVar4 + 0x5d);
            }
          }
LAB_00469cf2: ;
          iVar7 = iVar7 + 1;
          psVar4 = psVar4 + 0x84;
        } while (iVar7 <= iVar5);
        if (0 < iVar6) {
          uVar3 = FUN_00469d50();
          FUN_00469e40(iVar6,uVar3);
          iVar5 = DAT_0078d790;
          (&DAT_00799abc)[iVar7 * 0x42] =
               *(undefined4 *)(&DAT_0070ce0c + (short)(&DAT_007999ea)[iVar7 * 0x84] * 0x58);
        }
      }
      param_1 = param_1 + -1;
    } while (param_1 != 0);
  }
  return;
}
