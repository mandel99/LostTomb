#include "restore_types.h"
#define DAT_00775810 (*(undefined4 *)0x00775810u)
#define DAT_0077592c (*(undefined4 *)0x0077592cu)
#define DAT_0077646c (*(undefined4 *)0x0077646cu)
#define DAT_0077de04 (*(undefined4 *)0x0077de04u)
#define DAT_0077de06 (*(undefined4 *)0x0077de06u)
#define DAT_00ea4096 (*(undefined4 *)0x00ea4096u)
#define DAT_00ea40de (*(undefined4 *)0x00ea40deu)
#define DAT_00ea6160 (*(undefined4 *)0x00ea6160u)
extern int FUN_004c5240();

undefined4 FUN_004434e0(int param_1,int param_2,int param_3)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  iVar3 = (param_1 * 0x24ec + param_3) * 2;
  if ((*(short *)(&DAT_00ea4096 + iVar3) != 2) && (*(short *)(&DAT_00ea4096 + iVar3) != 4)) {
    return 0;
  }
  if ((param_3 == 0x1b) || ((param_3 < 0 || (0x23 < param_3)))) {
    return 0;
  }
  iVar2 = param_2 * 0x6a;
  iVar1 = FUN_004c5240(*(undefined4 *)
                        (&DAT_0077646c + (param_3 + (uint)(byte)(&DAT_0077de04)[iVar2] * 0x24) * 4))
  ;
  if ((param_2 != 0) &&
     (iVar1 <= *(int *)(&DAT_0077592c + (param_3 + (uint)(byte)(&DAT_0077de04)[iVar2] * 0x24) * 4)))
  {
    return 0;
  }
  if (*(short *)(&DAT_00ea4096 + iVar3) == 2) {
    if ((int)(&DAT_00ea6160)[param_1 * 0x1276 + param_3] <= *(short *)(&DAT_00ea40de + iVar3) * 100)
    {
      return 0;
    }
  }
  else if ((int)(&DAT_00ea6160)[param_1 * 0x1276 + param_3] <= *(int *)(&DAT_00775810 + param_3 * 4)
          ) {
    return 0;
  }
  if ((param_2 != 0) && ((&DAT_0077de06)[param_3 + iVar2] == '\0')) {
    return 0;
  }
  return 1;
}
