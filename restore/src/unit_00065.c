#include "restore_types.h"
#define DAT_005d2a18 (*(undefined *)0x005d2a18u)
#define DAT_005d2a1c (*(undefined *)0x005d2a1cu)
#define DAT_005d2a20 (*(undefined *)0x005d2a20u)
#define DAT_0077590c (*(undefined *)0x0077590cu)
#define DAT_00775910 (*(undefined *)0x00775910u)
#define DAT_0077f75c (*(undefined4 *)0x0077f75cu)
#define DAT_00784cb4 (*(undefined *)0x00784cb4u)
#define DAT_00d3b4fc (*(undefined *)0x00d3b4fcu)
#define DAT_00e38d98 (*(undefined *)0x00e38d98u)
#define DAT_00e38d9c (*(undefined *)0x00e38d9cu)
#define DAT_00e38dca (*(byte *)0x00e38dcau)
#define DAT_00e38df7 (*(char *)0x00e38df7u)
#define DAT_00e38e30 (*(undefined *)0x00e38e30u)
#define DAT_00e38e34 (*(undefined *)0x00e38e34u)
#define _DAT_00d3b4f8 (*(undefined4 *)0x00d3b4f8u)

int FUN_00407870(void)

{
  short sVar1;
  short sVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int local_10;
  short *local_c;
  int local_8;
  int local_4;
  
  if (DAT_00e38df7 == '\0') {
    return 0;
  }
  if (DAT_00e38e34 < DAT_005d2a18) {
    return 0;
  }
  if (DAT_00e38d9c + DAT_005d2a18 * -2 <= DAT_00e38e34) {
    return 0;
  }
  if (DAT_005d2a1c <= DAT_00e38e30) {
    if ((DAT_00e38d98 - DAT_005d2a20) - DAT_005d2a1c <= DAT_00e38e30) {
      return 0;
    }
    local_8 = 10000;
    iVar5 = (DAT_00775910 - DAT_005d2a18) + DAT_00e38e34;
    iVar3 = (DAT_0077590c - DAT_005d2a1c) + DAT_00e38e30;
    local_4 = 0;
    local_10 = 0;
    local_c = &DAT_0077f75c;
    do {
      if (*(char *)((int)local_c + -0x13) == '\0') {
        DAT_00e38dca = 1;
        return local_4;
      }
      if (DAT_00784cb4 == 0) {
        sVar1 = local_c[-8];
        sVar2 = local_c[-7];
      }
      else {
        sVar1 = *local_c;
        sVar2 = local_c[1];
      }
      iVar6 = (int)sVar1;
      iVar4 = (int)sVar2;
      if ((((iVar6 + -8 <= iVar5) && (iVar5 < local_c[-6] + 8 + iVar6)) && (iVar4 + -8 <= iVar3)) &&
         (iVar3 < iVar4 + 8 + (int)local_c[-5])) {
        iVar6 = (int)local_c[-6] / 2 + iVar6;
        iVar4 = (int)local_c[-5] / 2 + iVar4;
        if (iVar6 < iVar5) {
          DAT_00d3b4fc = iVar5 - iVar6;
        }
        else if (iVar5 < iVar6) {
          DAT_00d3b4fc = iVar6 - iVar5;
        }
        else {
          DAT_00d3b4fc = 0;
        }
        if (iVar4 < iVar3) {
          _DAT_00d3b4f8 = iVar3 - iVar4;
        }
        else if (iVar3 < iVar4) {
          _DAT_00d3b4f8 = iVar4 - iVar3;
        }
        else {
          _DAT_00d3b4f8 = 0;
        }
        iVar4 = _DAT_00d3b4f8;
        if (_DAT_00d3b4f8 <= DAT_00d3b4fc) {
          iVar4 = DAT_00d3b4fc;
        }
        if (iVar4 < local_8) {
          local_4 = local_10 + 1;
          local_8 = iVar4;
        }
      }
      local_c = local_c + 0x31;
      local_10 = local_10 + 1;
    } while ((int)local_c < 0x7843ec);
    DAT_00e38dca = 1;
    return local_4;
  }
  return 0;
}
