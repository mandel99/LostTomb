#include "restore_types.h"
#define DAT_00799a34 (*(undefined4 *)0x00799a34u)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd606 (*(undefined4 *)0x00bcd606u)
#define DAT_00bcd608 (*(undefined4 *)0x00bcd608u)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)

undefined1 FUN_004e6b20(int param_1)

{
  char cVar1;
  undefined1 uVar2;
  int iVar3;
  char cVar4;
  bool bVar5;
  
  uVar2 = 0;
  if (*(int *)(&DAT_00799a34 + param_1 * 0x108) == 0) {
    return uVar2;
  }
  iVar3 = *(int *)(&DAT_00799a34 + param_1 * 0x108) * 0x184;
  if ((&DAT_00bcd606)[iVar3] == '\0') {
LAB_004e6bb9: ;
    *(undefined4 *)(&DAT_00799a34 + param_1 * 0x108) = 0;
  }
  else {
    cVar4 = (&DAT_00bcd630)[iVar3];
    if ((cVar4 == '\x06') || (cVar4 == '\a')) {
      cVar4 = (&DAT_00bcd608)[iVar3];
    }
    cVar1 = (&DAT_00bcd602)[iVar3];
    if (cVar1 == 'O') {
      bVar5 = cVar4 == '\t';
    }
    else {
      if (cVar1 != 'Q') {
        if (cVar1 == 'U') {
          if (((cVar4 != '\x0f') && (cVar4 != '\x10')) && (cVar4 != '#')) {
            return uVar2;
          }
          return 1;
        }
        goto LAB_004e6bb9;
      }
      if (cVar4 == '\t') {
        return 1;
      }
      bVar5 = cVar4 == '\v';
    }
    if ((bVar5) || (cVar4 == '\n')) {
      return 1;
    }
  }
  return uVar2;
}
