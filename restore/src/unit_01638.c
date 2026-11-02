#include "restore_types.h"
#define DAT_0077de02 (*(undefined4 *)0x0077de02u)
#define DAT_0077de05 (*(undefined4 *)0x0077de05u)
#define DAT_00784f14 (*(undefined *)0x00784f14u)
#define DAT_00784f18 (*(undefined *)0x00784f18u)
#define DAT_00e20778 (*(undefined *)0x00e20778u)
#define DAT_00e38dac (*(undefined *)0x00e38dacu)
#define DAT_00e38db0 (*(undefined *)0x00e38db0u)
extern undefined4 FUN_00443000();
extern undefined4 FUN_00443040();
extern bool FUN_00447790();
extern undefined4 FUN_004cf620();

int FUN_004c9340(undefined1 *param_1)

{
  char cVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  uint uVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  bool bVar9;
  int local_c;
  uint local_8;
  
  iVar7 = DAT_00e38dac;
  iVar2 = DAT_00784f14 + -1;
  cVar1 = (&DAT_0077de02)[DAT_00784f18 * 0x6a];
  if (((cVar1 != '\x01') && (cVar1 != '\x03')) && (cVar1 != '\x05')) {
    return 0;
  }
  iVar6 = (DAT_00e38db0 + -500) / 2;
  if ((&DAT_0077de05)[DAT_00784f18 * 0x6a] != '\0') {
    iVar8 = 0;
    iVar3 = DAT_00e38dac + -0x5d;
    local_8 = 0;
    local_c = 1;
    iVar7 = iVar3;
    do {
      iVar4 = FUN_00443000(iVar2,local_c);
      if (iVar4 != 0) {
        cVar1 = FUN_00447790(DAT_00784f18,local_c);
        if (cVar1 != '\0') {
          iVar4 = FUN_004cf620(iVar8 * 0x87 + iVar6,iVar7,0x1a,0x1a);
          if (iVar4 != 0) {
            *param_1 = 0;
            return local_c;
          }
          iVar8 = iVar8 + 1;
          local_8 = local_8 + 1;
          uVar5 = local_8 & 0x80000001;
          bVar9 = uVar5 == 0;
          if ((int)uVar5 < 0) {
            bVar9 = (uVar5 - 1 | 0xfffffffe) == 0xffffffff;
          }
          if (bVar9) {
            iVar8 = 0;
            iVar7 = iVar7 + 0x14;
          }
        }
      }
      local_c = local_c + 1;
    } while (local_c < 0x24);
    iVar8 = 0;
    iVar7 = 1;
    local_8 = 0;
    do {
      iVar4 = FUN_00443040(iVar2,iVar7);
      if (iVar4 != 0) {
        cVar1 = FUN_00447790(DAT_00784f18,iVar7);
        if (cVar1 != '\0') {
          iVar4 = FUN_004cf620(iVar8 * 0x87 + 0x118 + iVar6,iVar3,0x1a,0x1a);
          if (iVar4 != 0) {
            *param_1 = 1;
            return iVar7;
          }
          iVar8 = iVar8 + 1;
          local_8 = local_8 + 1;
          uVar5 = local_8 & 0x80000001;
          if ((int)uVar5 < 0) {
            uVar5 = (uVar5 - 1 | 0xfffffffe) + 1;
          }
          if (uVar5 == 0) {
            iVar8 = 0;
            iVar3 = iVar3 + 0x14;
          }
        }
      }
      iVar7 = iVar7 + 1;
    } while (iVar7 < 0x24);
    return 0;
  }
  iVar8 = 1;
  DAT_00e20778 = 0;
  do {
    iVar3 = FUN_00443000(iVar2,iVar8);
    if (iVar3 != 0) {
      cVar1 = FUN_00447790(DAT_00784f18,iVar8);
      if (cVar1 != '\0') {
        iVar3 = FUN_004cf620(DAT_00e20778 + 100 + iVar6,iVar7 + -0x50,0x13,0x11);
        if (iVar3 != 0) {
          *param_1 = 0;
          return iVar8;
        }
        DAT_00e20778 = DAT_00e20778 + 0x20;
      }
    }
    iVar8 = iVar8 + 1;
  } while (iVar8 < 0x24);
  DAT_00e20778 = 0;
  iVar8 = 1;
  do {
    iVar3 = FUN_00443040(iVar2,iVar8);
    if (iVar3 != 0) {
      cVar1 = FUN_00447790(DAT_00784f18,iVar8);
      if (cVar1 != '\0') {
        iVar3 = FUN_004cf620(DAT_00e20778 + 100 + iVar6,iVar7 + -0x3a,0x13,0x11);
        if (iVar3 != 0) {
          *param_1 = 1;
          return iVar8;
        }
        DAT_00e20778 = DAT_00e20778 + 0x20;
      }
    }
    iVar8 = iVar8 + 1;
  } while (iVar8 < 0x24);
  return 0;
}
