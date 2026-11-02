#include "restore_types.h"
#define DAT_00eaaf68 (*(undefined4 *)0x00eaaf68u)
#define DAT_00eab044 (*(undefined4 *)0x00eab044u)
#define DAT_00eab048 (*(undefined4 *)0x00eab048u)
extern void FUN_0040f8f0();

int FUN_0046efd0(undefined4 param_1)

{
  int *piVar1;
  int iVar2;
  
  iVar2 = 1;
  piVar1 = &DAT_00eab048;
  do {
    if (*piVar1 == 0) {
      FUN_0040f8f0(&DAT_00eaaf68 + iVar2 * 0x38,0xe0,0);
      (&DAT_00eaaf68)[iVar2 * 0x38] = 1;
      *(undefined4 *)(&DAT_00eab044 + iVar2 * 0xe0) = param_1;
      return iVar2;
    }
    piVar1 = piVar1 + 0x38;
    iVar2 = iVar2 + 1;
  } while ((int)piVar1 < 0xeadb28);
  return 0;
}
