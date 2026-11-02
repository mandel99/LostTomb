#include "restore_types.h"
#define DAT_007900c2 (*(undefined4 *)0x007900c2u)
#define DAT_007900ea (*(undefined4 *)0x007900eau)
#define DAT_00790134 (*(undefined4 *)0x00790134u)
#define DAT_007999e4 (*(undefined4 *)0x007999e4u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00799a12 (*(undefined4 *)0x00799a12u)
#define DAT_00799a14 (*(undefined4 *)0x00799a14u)
#define DAT_00799a18 (*(undefined4 *)0x00799a18u)
#define DAT_00799a22 (*(undefined4 *)0x00799a22u)
#define DAT_00799a98 (*(undefined4 *)0x00799a98u)
#define DAT_00b7482c (*(undefined4 *)0x00b7482cu)
#define DAT_00ea60d0 (*(undefined4 *)0x00ea60d0u)
#define DAT_00ea6160 (*(undefined4 *)0x00ea6160u)
#define DAT_00f566bc (*(undefined *)0x00f566bcu)
#define DAT_00f57204 (*(undefined *)0x00f57204u)
extern undefined4 FUN_0045b230();
extern undefined4 FUN_0045b490();
extern int FUN_0045d510();
extern int FUN_00475b50();

int FUN_0045b2a0(int param_1,int param_2,int param_3,int param_4)

{
  short sVar1;
  int iVar2;
  char cVar3;
  int iVar4;
  int iVar5;
  uint uVar6;
  short sVar7;
  int iVar8;
  int iVar9;
  
  iVar2 = param_3;
  iVar4 = FUN_00475b50(param_2);
  iVar8 = 0;
  uVar6 = (uint)*(byte *)((int)&DAT_00799a98 + iVar4 * 0x108 + 1);
  if (((0 < (short)(&DAT_00799a18)[iVar4 * 0x84]) &&
      (*(short *)(&DAT_00799a22 + iVar4 * 0x108) == 0)) &&
     (cVar3 = FUN_0045b230(iVar4,0,param_3), cVar3 == '\0')) {
    if (*(char *)((int)&DAT_007900c2 + uVar6 * 0xc4 + param_3) == '\0') {
      sVar7 = *(short *)((int)&DAT_007900ea + (param_3 + uVar6 * 0x62) * 2 + 2);
    }
    else {
      sVar7 = *(short *)(&DAT_00790134 + (param_3 + uVar6 * 0x62) * 2);
    }
    cVar3 = FUN_0045b230(iVar4,param_4,param_3);
    if (cVar3 != '\0') {
      iVar8 = FUN_0045d510(iVar4,param_3);
      iVar8 = iVar8 + (param_4 - sVar7);
    }
    param_3 = 0;
    param_4 = param_4 - iVar8;
    if (param_4 < 1) {
      return param_4;
    }
    while( true ) {
      sVar7 = -1;
      iVar4 = 0;
      iVar8 = FUN_00475b50(param_2);
      iVar9 = 0;
      do {
        iVar8 = (int)*(short *)(&DAT_00799a12 + iVar8 * 0x108);
        if (iVar8 < 1) {
          return 0;
        }
        if ((((&DAT_007999ea)[iVar8 * 0x84] == 0) || ((short)(&DAT_007999ea)[iVar8 * 0x84] == iVar2)
            ) && ((sVar1 = (&DAT_00799a14)[iVar8 * 0x84], sVar1 < 400 && (sVar7 < sVar1)))) {
          iVar4 = iVar8;
          sVar7 = sVar1;
        }
        iVar9 = iVar9 + 1;
      } while (iVar9 < 8);
      if (iVar4 < 1) break;
      iVar8 = param_1 * 0x1276 + iVar2;
      (&DAT_00ea60d0)[iVar8] = (&DAT_00ea60d0)[iVar8] + -1;
      (&DAT_00ea6160)[iVar8] = (&DAT_00ea6160)[iVar8] + 1;
      (&DAT_007999ea)[iVar4 * 0x84] = (short)iVar2;
      (&DAT_00799a14)[iVar4 * 0x84] = (&DAT_00799a14)[iVar4 * 0x84] + 1;
      iVar8 = *(int *)(&DAT_007999e4 + iVar4 * 0x108);
      if (iVar2 < 0x1f) {
        iVar9 = DAT_00f566bc + -4 + iVar2 * 4;
      }
      else {
        iVar9 = DAT_00f57204 + -0x7c + iVar2 * 4;
      }
      iVar5 = FUN_0045b490((int)(short)(&DAT_00799a14)[iVar4 * 0x84]);
      (&DAT_00b7482c)[iVar8] = iVar5 + iVar9;
      param_3 = param_3 + 1;
      param_2 = iVar4;
      if (param_4 <= param_3) {
        return param_4;
      }
    }
  }
  return 0;
}
