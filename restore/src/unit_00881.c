#include "restore_types.h"
#define DAT_00705eb8 (*(undefined4 *)0x00705eb8u)
#define DAT_0078d790 (*(undefined *)0x0078d790u)
#define DAT_007900c2 (*(undefined4 *)0x007900c2u)
#define DAT_007900ea (*(undefined4 *)0x007900eau)
#define DAT_00790134 (*(undefined4 *)0x00790134u)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_007999fe (*(undefined4 *)0x007999feu)
#define DAT_00799a00 (*(undefined4 *)0x00799a00u)
#define DAT_00799a18 (*(undefined4 *)0x00799a18u)
#define DAT_00799a1c (*(undefined4 *)0x00799a1cu)
#define DAT_00799a98 (*(undefined4 *)0x00799a98u)
#define DAT_00799af0 (*(undefined4 *)0x00799af0u)
#define DAT_0096e6b0 (*(undefined4 *)0x0096e6b0u)
#define DAT_0096e6b4 (*(undefined4 *)0x0096e6b4u)
#define DAT_00972530 (*(undefined4 *)0x00972530u)
#define DAT_00972534 (*(undefined4 *)0x00972534u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00d05628 (*(undefined4 *)0x00d05628u)
#define _DAT_00d3b4e0 (*(undefined4 *)0x00d3b4e0u)
#define _DAT_00d3b4e4 (*(undefined4 *)0x00d3b4e4u)
extern int FUN_0045d510();
extern int FUN_00475b50();
extern undefined4 FUN_00480180();
extern int FUN_004b28f0();
extern int FUN_004cde60();

undefined4 FUN_0045acf0(int param_1,undefined4 param_2,int param_3,uint param_4)

{
  char cVar1;
  short sVar2;
  undefined4 uVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  uint uVar7;
  int iVar8;
  short *local_4;
  
  iVar4 = 0;
  DAT_0096e6b0 = 0;
  if ((param_3 != 0x15) && (iVar8 = 1, 0 < DAT_0078d790)) {
    local_4 = &DAT_00799af0;
    do {
      if (((((char)local_4[-8] == '\x01') && (*local_4 == 0x49)) && ((char)local_4[0x1a] != '\0'))
         && ((0 < local_4[8] && (*(byte *)(local_4 + 2) == param_4)))) {
        iVar5 = FUN_00475b50(iVar8);
        uVar7 = (uint)*(byte *)((int)&DAT_00799a98 + iVar5 * 0x108 + 1);
        iVar4 = DAT_0096e6b0;
        if ((param_1 != iVar5) || (*(char *)((int)&DAT_007900c2 + uVar7 * 0xc4 + param_3) == '\x02')
           ) {
          cVar1 = *(char *)((int)&DAT_007900c2 + uVar7 * 0xc4 + param_3);
          if (cVar1 == '\0') {
            iVar6 = FUN_0045d510(iVar5,param_3);
            iVar4 = DAT_0096e6b0;
            if (iVar6 < *(short *)((int)&DAT_007900ea + (param_3 + uVar7 * 0x62) * 2 + 2))
            goto LAB_0045ae1a;
          }
          else if ((cVar1 == '\x02') &&
                  (iVar6 = FUN_0045d510(iVar5,param_3), iVar4 = DAT_0096e6b0,
                  iVar6 < *(short *)(&DAT_00790134 + (param_3 + uVar7 * 0x62) * 2))) {
LAB_0045ae1a: ;
            iVar5 = FUN_004cde60((int)(short)(&DAT_00799a18)[iVar5 * 0x84],
                                 *(undefined4 *)
                                  (&DAT_00705eb8 + (short)(&DAT_007999e8)[iVar5 * 0x84] * 0x28));
            iVar4 = DAT_0096e6b0;
            if (iVar5 < 100) {
              DAT_00d05628 = DAT_00d05628 + 1;
            }
            else if ((local_4[1] == 0) || ((local_4[1] == param_3 && (local_4[0x16] < 400)))) {
              iVar4 = FUN_00475b50(iVar8);
              iVar5 = iVar4 * 0x108;
              if ((&DAT_00799a1c)[iVar5] == '\x01') {
                sVar2 = (&DAT_00799a00)[iVar4 * 0x84];
                iVar5 = DAT_00bca2d8 + (short)(&DAT_007999fe)[iVar4 * 0x84];
                (&DAT_00972534)[DAT_0096e6b0] = iVar8;
                iVar4 = DAT_0096e6b0 + 1;
                DAT_0096e6b0 = iVar4;
                (&DAT_0096e6b0)[iVar4] = iVar5 + sVar2 * 0xe4;
              }
              else {
                iVar6 = FUN_00480180((int)*(short *)((int)&DAT_007999dc + iVar5 + 2),
                                     (int)*(short *)(&DAT_007999e0 + iVar5),3);
                iVar5 = _DAT_00d3b4e0;
                iVar4 = DAT_0096e6b0;
                if (iVar6 != 0) {
                  (&DAT_00972534)[DAT_0096e6b0] = iVar8;
                  (&DAT_0096e6b4)[DAT_0096e6b0] = _DAT_00d3b4e4 + iVar5 * 0xe4 + DAT_00bca2d8;
                  DAT_0096e6b0 = DAT_0096e6b0 + 1;
                  iVar4 = DAT_0096e6b0;
                }
              }
            }
          }
        }
      }
      iVar8 = iVar8 + 1;
      local_4 = local_4 + 0x84;
    } while (iVar8 <= DAT_0078d790);
    if ((iVar4 != 0) && (iVar4 = FUN_004b28f0(param_2,&DAT_0096e6b4,iVar4), iVar4 != 0)) {
      uVar3 = (&DAT_00972530)[iVar4];
      iVar4 = FUN_00475b50(uVar3);
      iVar8 = iVar4 * 0x108;
      _DAT_00d3b4e4 = (int)(short)(&DAT_007999fe)[iVar4 * 0x84];
      _DAT_00d3b4e0 = (int)(short)(&DAT_00799a00)[iVar4 * 0x84];
      if (((&DAT_00799a1c)[iVar8] == '\x01') ||
         (iVar4 = FUN_00480180((int)*(short *)((int)&DAT_007999dc + iVar8 + 2),
                               (int)*(short *)(&DAT_007999e0 + iVar8),3), iVar4 != 0)) {
        return uVar3;
      }
    }
  }
  return 0;
}
