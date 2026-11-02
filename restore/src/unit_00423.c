#include "restore_types.h"
#define DAT_005c9348 (*(undefined *)0x005c9348u)
#define DAT_00705734 (*(undefined *)0x00705734u)
#define DAT_00705738 (*(undefined *)0x00705738u)
#define DAT_00ea5b3c (*(undefined4 *)0x00ea5b3cu)
#define DAT_00ea5ca4 (*(undefined4 *)0x00ea5ca4u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern void FUN_00426a00();
extern void FUN_00426a80();

void FUN_004269b0(void)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  
  FUN_00426a00();
  FUN_00426a80();
  iVar2 = DAT_00eaac28;
  uVar1 = DAT_00705738;
  iVar3 = DAT_005c9348 + DAT_00705734;
  if (iVar3 < 100) {
    if (iVar3 < 0) {
      iVar3 = 0;
      goto LAB_004269de;
    }
    if (iVar3 < 100) goto LAB_004269de;
  }
  iVar3 = 100;
LAB_004269de: ;
  (&DAT_00ea5b3c)[DAT_00eaac28 * 0x1276] = iVar3;
  (&DAT_00ea5ca4)[iVar2 * 0x1276] = uVar1;
  return;
}
