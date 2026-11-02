#include "restore_types.h"
#define DAT_0077646c (*(undefined4 *)0x0077646cu)
#define DAT_0077de04 (*(undefined4 *)0x0077de04u)
extern int FUN_004c5240();

bool FUN_00447790(int param_1,int param_2)

{
  int iVar1;
  
  iVar1 = FUN_004c5240(*(undefined4 *)
                        (&DAT_0077646c +
                        (param_2 + (uint)(byte)(&DAT_0077de04)[param_1 * 0x6a] * 0x24) * 4));
  return 0 < iVar1;
}
