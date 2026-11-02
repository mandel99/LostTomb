#include "restore_types.h"
#define DAT_005e49d0 (*(undefined4 *)0x005e49d0u)
#define DAT_00ea3e34 (*(undefined4 *)0x00ea3e34u)
#define DAT_00ea3f4c (*(undefined4 *)0x00ea3f4cu)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern void FUN_00458e20();
extern int FUN_004cde40();
extern int FUN_004e92c0();

void FUN_004e8720(void)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  undefined4 *puVar5;
  int iVar6;
  
  iVar4 = DAT_00eaac28;
  iVar6 = 9;
  *(undefined4 *)(&DAT_00ea3f4c + DAT_00eaac28 * 0x49d8) = 0;
  puVar5 = &DAT_005e49d0;
  do {
    uVar1 = FUN_004e92c0(iVar4,iVar6);
    uVar1 = FUN_004cde40(uVar1,*puVar5);
    iVar2 = (FUN_00458e20(DAT_00eaac28,uVar1), restore_unresolved_return(0x00458e20u));
    iVar4 = DAT_00eaac28;
    puVar5 = puVar5 + -1;
    iVar3 = DAT_00eaac28 * 0x49d8;
    *(short *)(&DAT_00ea3e34 + iVar3) = *(short *)(&DAT_00ea3e34 + iVar3) + (short)iVar2;
    iVar6 = iVar6 + -1;
    *(int *)(&DAT_00ea3f4c + iVar3) = *(int *)(&DAT_00ea3f4c + iVar3) + iVar2;
  } while (0x5e49ab < (int)puVar5);
  return;
}
