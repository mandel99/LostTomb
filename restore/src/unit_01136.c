#include "restore_types.h"
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00b7482c (*(undefined4 *)0x00b7482cu)
#define DAT_00bc96c0 (*(undefined *)0x00bc96c0u)
#define DAT_00bc96c4 (*(undefined *)0x00bc96c4u)
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bca318 (*(undefined *)0x00bca318u)
#define DAT_00bca31c (*(undefined *)0x00bca31cu)
#define DAT_00bca320 (*(undefined *)0x00bca320u)
#define DAT_00bca324 (*(undefined *)0x00bca324u)
#define DAT_00e94e04 (*(undefined *)0x00e94e04u)
#define DAT_00f56bfc (*(undefined *)0x00f56bfcu)
#define _DAT_00784c64 (*(undefined4 *)0x00784c64u)
#define _DAT_00784c66 (*(undefined4 *)0x00784c66u)
#define _DAT_00784c68 (*(undefined4 *)0x00784c68u)
#define _DAT_00784c6a (*(undefined4 *)0x00784c6au)
#define _DAT_00d3b4e0 (*(undefined4 *)0x00d3b4e0u)
#define _DAT_00d3b4e4 (*(undefined4 *)0x00d3b4e4u)
extern undefined4 FUN_00482810();

void FUN_0047d870(int param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  short sVar4;
  short sVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  
  iVar6 = DAT_00bca2d8;
  if (param_1 != 0) {
    iVar1 = DAT_00bca318 + DAT_00bca31c * 0xe4 + DAT_00bca2d8;
    (&DAT_00af598c)[iVar1] = (&DAT_00af598c)[iVar1] & 0xdfcffffd;
    (&DAT_00af598c)[DAT_00bca320 + DAT_00bca324 * 0xe4 + iVar6] =
         (&DAT_00af598c)[DAT_00bca320 + DAT_00bca324 * 0xe4 + iVar6] & 0xdfcffffd;
    return;
  }
  if (_DAT_00784c64 == 0) {
    iVar6 = 2;
  }
  else if ((int)_DAT_00784c64 == DAT_00bca2d0 + -1) {
    iVar6 = 6;
  }
  else if (_DAT_00784c66 == 0) {
    iVar6 = 0;
  }
  else {
    iVar6 = (-(uint)((int)_DAT_00784c66 != DAT_00bca2d4 + -1) & 0xfffffffb) + 4;
  }
  if (_DAT_00784c68 == 0) {
    iVar1 = 2;
  }
  else if ((int)_DAT_00784c68 == DAT_00bca2d0 + -1) {
    iVar1 = 6;
  }
  else if (_DAT_00784c6a == 0) {
    iVar1 = 0;
  }
  else {
    iVar1 = (-(uint)((int)_DAT_00784c6a != DAT_00bca2d4 + -1) & 0xfffffffb) + 4;
  }
  iVar9 = DAT_00bca2d8;
  sVar4 = _DAT_00784c68;
  if (-1 < iVar6) {
    iVar2 = (int)_DAT_00784c66;
    iVar7 = DAT_00bca2d8 + _DAT_00784c64;
    iVar8 = 1;
    do {
      iVar3 = FUN_00482810((int)_DAT_00784c64,(int)_DAT_00784c66,1,iVar8,iVar7 + iVar2 * 0xe4);
      iVar9 = DAT_00bca2d8;
      if (iVar3 != 0) break;
      iVar8 = iVar8 + 1;
    } while (iVar8 < 10);
    DAT_00bca31c = _DAT_00d3b4e0;
    DAT_00bca318 = _DAT_00d3b4e4;
    iVar2 = _DAT_00d3b4e4 + _DAT_00d3b4e0 * 0xe4 + DAT_00bca2d8;
    iVar6 = DAT_00e94e04 + iVar6;
    DAT_00bc96c4 = iVar2;
    (&DAT_00af598c)[iVar2] = (&DAT_00af598c)[iVar2] | 2;
    sVar4 = _DAT_00784c68;
    if (7 < iVar6) {
      iVar6 = iVar6 + -8;
    }
    (&DAT_00b7482c)[iVar2] = iVar6 / 2 + DAT_00f56bfc;
  }
  if (-1 < iVar1) {
    iVar6 = (int)_DAT_00784c6a;
    iVar2 = 1;
    sVar5 = sVar4;
    do {
      iVar7 = FUN_00482810((int)sVar5,(int)_DAT_00784c6a,1,iVar2,iVar9 + sVar4 + iVar6 * 0xe4);
      if (iVar7 != 0) break;
      iVar2 = iVar2 + 1;
      sVar5 = _DAT_00784c68;
    } while (iVar2 < 10);
    DAT_00bca324 = _DAT_00d3b4e0;
    DAT_00bca320 = _DAT_00d3b4e4;
    iVar6 = _DAT_00d3b4e4 + _DAT_00d3b4e0 * 0xe4 + DAT_00bca2d8;
    iVar1 = DAT_00e94e04 + iVar1;
    DAT_00bc96c0 = iVar6;
    (&DAT_00af598c)[iVar6] = (&DAT_00af598c)[iVar6] | 2;
    if (7 < iVar1) {
      iVar1 = iVar1 + -8;
    }
    (&DAT_00b7482c)[iVar6] = iVar1 / 2 + 4 + DAT_00f56bfc;
  }
  return;
}
