#include "restore_types.h"
#define DAT_007900ea (*(undefined4 *)0x007900eau)
#define DAT_00799a98 (*(undefined4 *)0x00799a98u)
#define DAT_00799af0 (*(undefined4 *)0x00799af0u)
#define DAT_00eaac28 (*(uint32_t *)0x00eaac28u)
extern int FUN_0045b2a0();
extern int FUN_0045d510();

int FUN_004c3950(void)

{
  int iVar1;
  uint uVar2;
  int iVar3;
  int *piVar4;
  int iVar5;
  int iVar6;
  int local_24;
  short *local_20;
  int local_1c;
  int local_18 [6];
  
  local_18[0] = 0x12;
  local_18[1] = 0xb;
  local_18[2] = 0xd;
  local_18[3] = 0x10;
  local_18[4] = 0x11;
  local_18[5] = 0x13;
  local_1c = 10000;
  local_24 = 0;
  iVar3 = 1;
  local_20 = &DAT_00799af0;
  do {
    if ((char)local_20[-8] == '\x01') {
      iVar6 = 0;
      if ((*local_20 == 0x48) || (*local_20 == 0xc1)) {
        piVar4 = local_18;
        iVar5 = 6;
        do {
          iVar1 = FUN_0045d510(iVar3,*piVar4);
          iVar6 = iVar6 + iVar1;
          piVar4 = piVar4 + 1;
          iVar5 = iVar5 + -1;
        } while (iVar5 != 0);
        if ((iVar6 != 0) && (iVar6 < local_1c)) {
          local_24 = iVar3;
          local_1c = iVar6;
        }
      }
    }
    local_20 = local_20 + 0x84;
    iVar3 = iVar3 + 1;
  } while ((int)local_20 < 0x89b6e8);
  iVar6 = 0;
  iVar3 = 10000;
  if ((local_24 != 0) && (local_1c != 0)) {
    piVar4 = local_18;
    local_1c = 6;
    do {
      iVar5 = *piVar4;
      iVar1 = FUN_0045d510(local_24,iVar5);
      if ((iVar1 != 0) && (iVar1 < iVar3)) {
        iVar3 = iVar1;
        iVar6 = iVar5;
      }
      piVar4 = piVar4 + 1;
      local_1c = local_1c + -1;
    } while (local_1c != 0);
    if (iVar6 != 0) {
      uVar2 = *(short *)((int)&DAT_007900ea +
                        (iVar6 + (uint)*(byte *)((int)&DAT_00799a98 + local_24 * 0x108 + 1) * 0x62)
                        * 2 + 2) - iVar3;
      FUN_0045b2a0(DAT_00eaac28,local_24,iVar6,uVar2 & ((int)uVar2 < 0) - 1);
      return local_24;
    }
  }
  return local_24;
}
