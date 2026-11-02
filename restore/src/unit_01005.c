#include "restore_types.h"
#define DAT_009784d8 (*(undefined4 *)0x009784d8u)
#define DAT_00978504 (*(undefined4 *)0x00978504u)
extern void FUN_0040f8f0();

int FUN_0046f140(void)

{
  char *pcVar1;
  int iVar2;
  
  iVar2 = 1;
  pcVar1 = &DAT_00978504;
  do {
    if (*pcVar1 == '\0') {
      FUN_0040f8f0(&DAT_009784d8 + iVar2 * 0x2c,0x2c,0);
      (&DAT_009784d8)[iVar2 * 0x2c] = 1;
      return iVar2;
    }
    pcVar1 = pcVar1 + 0x2c;
    iVar2 = iVar2 + 1;
  } while ((int)pcVar1 < 0x978d70);
  return 0;
}
