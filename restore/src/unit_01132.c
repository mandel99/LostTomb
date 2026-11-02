#include "restore_types.h"
#define DAT_005cc8c0 (*(undefined *)0x005cc8c0u)
#define DAT_00605994 (*(undefined *)0x00605994u)
#define DAT_00b285cc (*(undefined4 *)0x00b285ccu)
#define DAT_00bc9738 (*(undefined *)0x00bc9738u)
#define DAT_00bc973c (*(undefined *)0x00bc973cu)
#define DAT_00bc9768 (*(undefined *)0x00bc9768u)

void FUN_0047d4d0(void)

{
  int iVar1;
  int iVar2;
  int *piVar3;
  int iVar4;
  int *piVar5;
  int local_4;
  
  iVar2 = DAT_00bc9768;
  iVar1 = DAT_00bc973c;
  DAT_00605994 = 1;
  if (0 < DAT_00bc9738) {
    piVar5 = &DAT_005cc8c0;
    local_4 = DAT_00bc9738;
    do {
      piVar3 = piVar5;
      iVar4 = iVar1;
      if (0 < iVar1) {
        do {
          (&DAT_00b285cc)[iVar2 + *piVar3] = (&DAT_00b285cc)[iVar2 + *piVar3] | 0x10;
          iVar4 = iVar4 + -1;
          piVar3 = piVar3 + 2;
        } while (iVar4 != 0);
      }
      piVar5 = piVar5 + 0xc;
      local_4 = local_4 + -1;
    } while (local_4 != 0);
  }
  return;
}
