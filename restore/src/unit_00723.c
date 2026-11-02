#include "restore_types.h"

int FUN_0044bd00(void)

{
  int iVar1;
  int *in_ECX;
  char *pcVar2;
  int iVar3;
  
  iVar3 = *in_ECX;
  iVar1 = 0;
  if (0 < iVar3) {
    pcVar2 = (char *)((int)in_ECX + 6);
    do {
      if (*pcVar2 == '\x03') {
        iVar1 = iVar1 + 1;
      }
      pcVar2 = pcVar2 + 0x7c;
      iVar3 = iVar3 + -1;
    } while (iVar3 != 0);
  }
  return iVar1;
}
