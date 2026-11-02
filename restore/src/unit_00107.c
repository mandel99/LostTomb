#include "restore_types.h"
#define DAT_005cc8c0 (*(undefined *)0x005cc8c0u)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00799a30 (*(undefined4 *)0x00799a30u)
#define DAT_00acf85c (*(undefined4 *)0x00acf85cu)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00b285cc (*(undefined4 *)0x00b285ccu)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00b67d1c (*(undefined4 *)0x00b67d1cu)
#define DAT_00b7482c (*(undefined4 *)0x00b7482cu)
#define DAT_00bc9768 (*(undefined *)0x00bc9768u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00f56b54 (*(undefined *)0x00f56b54u)
#define DAT_00f573bc (*(undefined *)0x00f573bcu)
extern void FUN_0047bf50();

void FUN_0040dca0(int param_1)

{
  byte *pbVar1;
  int iVar2;
  int *in_ECX;
  int iVar3;
  int iVar4;
  byte unaff_BL;
  int *piVar5;
  int iVar6;
  int *piVar7;
  int iStack_1c;
  int local_18 [2];
  int iStack_10;
  int iStack_c;
  int iStack_4;
  
  iVar2 = (**(code **)(*in_ECX + 0x1c))(param_1,local_18);
  iVar3 = param_1 * 0x108;
  iStack_10 = (int)(short)(&DAT_007999ea)[param_1 * 0x84];
  local_18[0] = param_1 - iStack_10;
  iStack_c = in_ECX[iStack_10 * 0xd + 0x15];
  if ((unaff_BL & 1) == 0) {
    FUN_0047bf50(param_1,(int)*(short *)((int)&DAT_007999dc + iVar3 + 2),
                 (int)*(short *)(&DAT_007999e0 + iVar3),iStack_c,iStack_c,iVar2,8);
  }
  else {
    DAT_00bc9768 = DAT_00bca2d8 + *(short *)((int)&DAT_007999dc + iVar3 + 2) +
                   *(short *)(&DAT_007999e0 + iVar3) * 0xe4;
    if (0 < iStack_c) {
      piVar7 = &DAT_005cc8c0;
      iStack_1c = iStack_c;
      do {
        iVar6 = 0;
        iVar3 = (&DAT_00799a30)[local_18[0] * 0x42];
        piVar5 = piVar7;
        do {
          iVar4 = *piVar5 + DAT_00bc9768;
          if (iVar3 < 3) {
            pbVar1 = (byte *)((int)&DAT_00af598c + iVar4 * 4 + 3);
            *pbVar1 = *pbVar1 | 0x10;
          }
          else {
            (&DAT_00af598c)[iVar4] = 8;
          }
          (&DAT_00b285cc)[iVar4] = (&DAT_00b285cc)[iVar4] & 0xe0;
          (&DAT_00b67d1c)[iVar4] = (&DAT_00b67d1c)[iVar4] | 0x40;
          *(short *)(&DAT_00b4e6fc + iVar4 * 2) = (short)param_1;
          if (((unaff_BL & 2) == 0) || (iVar2 != DAT_00f573bc)) {
LAB_0040ddfa: ;
            if (iVar2 != 0) {
              if (iVar2 == DAT_00f56b54) {
                (&DAT_00b7482c)[iVar4] = ((byte)(&DAT_00acf85c)[iVar4] & 7) + iVar2;
              }
              else {
                (&DAT_00b7482c)[iVar4] = iVar2;
              }
            }
          }
          else if (iStack_10 == 0xd) {
            if (iVar6 < 1) {
LAB_0040ddc5: ;
              if (0x5cc91f < (int)piVar7) goto LAB_0040ddfa;
            }
          }
          else if (iStack_10 == 0xe) {
            if (1 < iVar6) goto LAB_0040ddc5;
          }
          else if (iStack_10 == 0xf) {
            if (iVar6 < 1) {
joined_r0x0040ddf8: ;
              if ((int)piVar7 < 0x5cc8c1) goto LAB_0040ddfa;
            }
          }
          else {
            if (iStack_10 != 0x10) goto LAB_0040ddfa;
            if (1 < iVar6) goto joined_r0x0040ddf8;
          }
          iVar6 = iVar6 + 1;
          piVar5 = piVar5 + 2;
          param_1 = iStack_4;
        } while (iVar6 < iStack_c);
        piVar7 = piVar7 + 0xc;
        iStack_1c = iStack_1c + -1;
        if (iStack_1c == 0) {
          return;
        }
      } while( true );
    }
  }
  return;
}
