#include "restore_types.h"
#define DAT_00705eb8 (*(undefined4 *)0x00705eb8u)
#define DAT_00784bbc (*(undefined *)0x00784bbcu)
#define DAT_0078d790 (*(undefined *)0x0078d790u)
#define DAT_007900c2 (*(undefined4 *)0x007900c2u)
#define DAT_007900ea (*(undefined4 *)0x007900eau)
#define DAT_00790134 (*(undefined4 *)0x00790134u)
#define DAT_007999fe (*(undefined4 *)0x007999feu)
#define DAT_00799a00 (*(undefined4 *)0x00799a00u)
#define DAT_00799adc (*(undefined4 *)0x00799adcu)
#define DAT_00799ade (*(undefined4 *)0x00799adeu)
#define DAT_00799b24 (*(undefined4 *)0x00799b24u)
#define DAT_00799b36 (*(undefined4 *)0x00799b36u)
#define DAT_0096e6b0 (*(undefined4 *)0x0096e6b0u)
#define DAT_0096e6b4 (*(undefined4 *)0x0096e6b4u)
#define DAT_00972530 (*(undefined4 *)0x00972530u)
#define DAT_00972534 (*(undefined4 *)0x00972534u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bcd66c (*(undefined4 *)0x00bcd66cu)
#define DAT_00d05628 (*(undefined4 *)0x00d05628u)
#define DAT_00ea42f8 (*(undefined4 *)0x00ea42f8u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define _DAT_00d3b4e0 (*(undefined4 *)0x00d3b4e0u)
#define _DAT_00d3b4e4 (*(undefined4 *)0x00d3b4e4u)
extern undefined4 FUN_004806a0();
extern int FUN_004b28f0();
extern int FUN_004cde60();

int FUN_0045d9e0(int param_1,int param_2,int param_3,uint param_4)

{
  char cVar1;
  short sVar2;
  int iVar3;
  uint uVar4;
  short sVar5;
  short *psVar6;
  int iVar7;
  char *pcVar8;
  
  if (DAT_00784bbc != 0) {
    return 0;
  }
  if ((param_3 < 1) || (8 < param_3)) {
    return 0;
  }
  if ((*(int *)(&DAT_00ea42f8 + (DAT_00eaac28 * 0x1276 + param_3) * 4) != 0) && (param_1 == 0)) {
    return 0;
  }
  iVar7 = 1;
  DAT_0096e6b0 = 0;
  if (0 < DAT_0078d790) {
    pcVar8 = &DAT_00799b24;
    psVar6 = &DAT_00799b36 + param_3;
    do {
      if ((pcVar8[-0x44] == '\x01') &&
         ((((sVar5 = *(short *)(pcVar8 + -0x34), sVar5 == 0x47 || (sVar5 == 0xbf)) &&
           (*pcVar8 != '\0')) &&
          ((0 < *(short *)(pcVar8 + -0x24) && ((byte)pcVar8[-0x30] == param_4)))))) {
        iVar3 = FUN_004cde60((int)*(short *)(pcVar8 + -4),
                             *(undefined4 *)(&DAT_00705eb8 + sVar5 * 0x28));
        if (iVar3 < 100) {
          DAT_00d05628 = DAT_00d05628 + 1;
        }
        else {
          uVar4 = (uint)(byte)pcVar8[0x7d];
          cVar1 = *(char *)((int)&DAT_007900c2 + param_3 + uVar4 * 0xc4);
          if (cVar1 == '\0') {
            sVar5 = *psVar6;
            iVar3 = (uVar4 * 0x62 + param_3) * 2;
            if (sVar5 < *(short *)((int)&DAT_007900ea + iVar3 + 2)) {
LAB_0045db12: ;
              if (*(short *)(pcVar8 + 0xc0) < 1) {
LAB_0045db5e: ;
                if (99 < *(short *)(pcVar8 + 0x12)) {
                  sVar5 = *(short *)(pcVar8 + -0x3c);
                  sVar2 = *(short *)(pcVar8 + -0x3e);
                  (&DAT_00972534)[DAT_0096e6b0] = iVar7;
                  FUN_004806a0((int)sVar2,(int)sVar5);
                  (&DAT_0096e6b4)[DAT_0096e6b0] =
                       _DAT_00d3b4e4 + _DAT_00d3b4e0 * 0xe4 + DAT_00bca2d8;
                  DAT_0096e6b0 = DAT_0096e6b0 + 1;
                }
              }
              else if (cVar1 == '\0') {
                iVar3 = (int)*(short *)((int)&DAT_007900ea + iVar3 + 2) -
                        (int)*(short *)(pcVar8 + 0xc2);
                if (iVar3 != sVar5 && -1 < iVar3 - sVar5) goto LAB_0045db5e;
              }
              else if ((cVar1 == '\x02') &&
                      ((int)*(short *)(&DAT_00790134 + iVar3) - (int)*(short *)(pcVar8 + 0xc2) !=
                       (int)sVar5 &&
                       -1 < ((int)*(short *)(&DAT_00790134 + iVar3) - (int)*(short *)(pcVar8 + 0xc2)
                            ) - (int)sVar5)) goto LAB_0045db5e;
            }
          }
          else if (cVar1 == '\x02') {
            sVar5 = *psVar6;
            iVar3 = (uVar4 * 0x62 + param_3) * 2;
            if (sVar5 < *(short *)(&DAT_00790134 + iVar3)) goto LAB_0045db12;
          }
        }
      }
      iVar7 = iVar7 + 1;
      psVar6 = psVar6 + 0x84;
      pcVar8 = pcVar8 + 0x108;
    } while (iVar7 <= DAT_0078d790);
    if ((DAT_0096e6b0 != 0) &&
       (iVar7 = FUN_004b28f0(param_2,&DAT_0096e6b4,DAT_0096e6b0), iVar7 != 0)) {
      iVar7 = (&DAT_00972530)[iVar7];
      _DAT_00d3b4e4 = (int)(short)(&DAT_007999fe)[iVar7 * 0x84];
      _DAT_00d3b4e0 = (int)(short)(&DAT_00799a00)[iVar7 * 0x84];
      if (iVar7 != 0) {
        *(short *)(&DAT_00799adc + iVar7 * 0x108) = *(short *)(&DAT_00799adc + iVar7 * 0x108) + 1;
        *(short *)(&DAT_00799ade + iVar7 * 0x108) =
             *(short *)(&DAT_00799ade + iVar7 * 0x108) + *(short *)(&DAT_00bcd66c + param_2 * 0x184)
        ;
      }
      return iVar7;
    }
  }
  return 0;
}
