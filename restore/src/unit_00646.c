#include "restore_types.h"
#define DAT_0077aefc (*(undefined *)0x0077aefcu)
#define DAT_0077af00 (*(undefined4 *)0x0077af00u)
#define DAT_00bcd677 (*(undefined4 *)0x00bcd677u)
extern void FUN_0040f8f0();

void FUN_00444b30(int param_1)

{
  int iVar1;
  
  iVar1 = DAT_0077aefc;
  (&DAT_00bcd677)[param_1 * 0x184] = (char)DAT_0077aefc;
  FUN_0040f8f0(&DAT_0077af00 + iVar1 * 0x58,0x58,0);
  DAT_0077aefc = DAT_0077aefc + 1;
  if (99 < DAT_0077aefc) {
    DAT_0077aefc = 0;
  }
  return;
}
