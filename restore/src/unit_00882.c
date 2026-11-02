#include "restore_types.h"
#define DAT_0078d790 (*(undefined *)0x0078d790u)
#define DAT_007900c2 (*(undefined4 *)0x007900c2u)
#define DAT_007900ea (*(undefined4 *)0x007900eau)
#define DAT_00790134 (*(undefined4 *)0x00790134u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00799a12 (*(undefined4 *)0x00799a12u)
#define DAT_00799a14 (*(undefined4 *)0x00799a14u)
#define DAT_00799b24 (*(undefined4 *)0x00799b24u)
#define DAT_00799b36 (*(undefined4 *)0x00799b36u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern int FUN_0045b2a0();
extern int FUN_0045e220();

int FUN_0045afe0(short *param_1,int param_2,char param_3)

{
  char cVar1;
  short sVar2;
  short sVar3;
  int iVar4;
  int iVar5;
  uint uVar6;
  int iVar7;
  char *pcVar8;
  int iVar9;
  int local_8;
  int local_4;
  
  iVar5 = (int)param_1;
  local_8 = 0;
  local_4 = 1;
  iVar9 = 0;
  if (0 < DAT_0078d790) {
    pcVar8 = &DAT_00799b24;
    param_1 = &DAT_00799b36 + (int)param_1;
    iVar7 = param_2;
    do {
      if ((((pcVar8[-0x44] == '\x01') && (*pcVar8 != '\0')) && (0 < *(short *)(pcVar8 + -0x24))) &&
         (0 < *(short *)(pcVar8 + -4))) {
        sVar2 = *(short *)(pcVar8 + -0x34);
        if (sVar2 == 0x48) {
          uVar6 = (uint)(byte)pcVar8[0x7d];
          cVar1 = *(char *)((int)&DAT_007900c2 + uVar6 * 0xc4 + iVar5);
          if (cVar1 == '\0') {
            sVar2 = *(short *)((int)&DAT_007900ea + (iVar5 + uVar6 * 0x62) * 2 + 2);
          }
          else {
            if (cVar1 != '\x02') goto LAB_0045b1eb;
            sVar2 = *(short *)(&DAT_00790134 + (iVar5 + uVar6 * 0x62) * 2);
          }
          iVar9 = 0;
          sVar3 = *(short *)(pcVar8 + -10);
          while (iVar4 = (int)sVar3, iVar4 != 0) {
            if ((&DAT_007999ea)[iVar4 * 0x84] == 0) {
              iVar9 = iVar9 + 400;
            }
            else if ((short)(&DAT_007999ea)[iVar4 * 0x84] == iVar5) {
              iVar9 = iVar9 + (400 - (short)(&DAT_00799a14)[iVar4 * 0x84]);
              iVar7 = param_2;
            }
            sVar3 = *(short *)(&DAT_00799a12 + iVar4 * 0x108);
          }
          if (sVar2 <= iVar9) {
            iVar9 = (int)sVar2;
          }
          if (iVar7 <= iVar9) {
            iVar9 = iVar7;
          }
          if (param_3 == '\0') {
            FUN_0045b2a0(DAT_00eaac28,local_4,iVar5,iVar9);
          }
          iVar7 = iVar7 - iVar9;
joined_r0x0045b1e9: ;
          local_8 = local_8 + iVar9;
          param_2 = iVar7;
          if (iVar7 < 1) {
            return local_8;
          }
        }
        else if (((sVar2 == 0x47) || (sVar2 == 0xbf)) && ((0 < iVar5 && (iVar5 < 9)))) {
          uVar6 = (uint)(byte)pcVar8[0x7d];
          cVar1 = *(char *)((int)&DAT_007900c2 + uVar6 * 0xc4 + iVar5);
          if (cVar1 == '\0') {
            sVar2 = *(short *)((int)&DAT_007900ea + (iVar5 + uVar6 * 0x62) * 2 + 2);
          }
          else {
            if (cVar1 != '\x02') goto LAB_0045b1eb;
            sVar2 = *(short *)(&DAT_00790134 + (iVar5 + uVar6 * 0x62) * 2);
          }
          iVar9 = (int)sVar2 - (int)*param_1;
          if ((int)*(short *)(pcVar8 + 0x12) <= (int)sVar2 - (int)*param_1) {
            iVar9 = (int)*(short *)(pcVar8 + 0x12);
          }
          if (0 < iVar9) {
            if (iVar7 <= iVar9) {
              iVar9 = iVar7;
            }
            if (param_3 == '\0') {
              FUN_0045e220(local_4,iVar5,0,iVar9);
            }
            iVar7 = iVar7 - iVar9;
            goto joined_r0x0045b1e9;
          }
        }
      }
LAB_0045b1eb: ;
      local_4 = local_4 + 1;
      param_1 = param_1 + 0x84;
      pcVar8 = pcVar8 + 0x108;
      iVar9 = local_8;
    } while (local_4 <= DAT_0078d790);
  }
  return iVar9;
}
