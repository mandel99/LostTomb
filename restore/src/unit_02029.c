#include "restore_types.h"
#define DAT_005e49d4 (*(undefined4 *)0x005e49d4u)
#define DAT_00ea1888 (*(undefined4 *)0x00ea1888u)
#define DAT_00ea3e34 (*(undefined4 *)0x00ea3e34u)
#define DAT_00ea3efa (*(undefined4 *)0x00ea3efau)
#define DAT_00ea3f50 (*(undefined4 *)0x00ea3f50u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern void FUN_00458f00();
extern int FUN_004cde40();
extern int FUN_004e92c0();
extern void FUN_004e92f0();

void FUN_004e8650(void)

{
  int iVar1;
  undefined4 uVar2;
  int iVar3;
  undefined2 *puVar4;
  int iVar5;
  int iVar6;
  
  iVar3 = DAT_00eaac28;
  iVar1 = DAT_00eaac28 * 0x49d8;
  iVar5 = 99;
  iVar6 = (int)*(short *)(&DAT_00ea3efa + iVar1);
  puVar4 = (undefined2 *)(&DAT_00ea3efa + iVar1);
  do {
    *puVar4 = puVar4[-1];
    puVar4 = puVar4 + -1;
    iVar5 = iVar5 + -1;
  } while (iVar5 != 0);
  iVar5 = 9;
  *(undefined2 *)(&DAT_00ea3e34 + iVar1) = 0;
  *(undefined4 *)(&DAT_00ea3f50 + iVar1) = 0;
  do {
    uVar2 = FUN_004e92c0(iVar3,iVar5);
    iVar3 = FUN_004cde40(uVar2,*(undefined4 *)
                                (&DAT_005e49d4 +
                                (iVar5 + (*(int *)(&DAT_00ea1888 + DAT_00eaac28 * 0x49d8) / 10) * 10
                                ) * 4));
    iVar1 = (FUN_00458f00(DAT_00eaac28,iVar3 + iVar6), restore_unresolved_return(0x00458f00u));
    FUN_004e92f0(DAT_00eaac28,iVar5,iVar1);
    iVar3 = DAT_00eaac28;
    iVar6 = 0;
    iVar5 = iVar5 + -1;
    *(int *)(&DAT_00ea3f50 + DAT_00eaac28 * 0x49d8) =
         *(int *)(&DAT_00ea3f50 + DAT_00eaac28 * 0x49d8) + iVar1;
  } while (-1 < iVar5);
  return;
}
