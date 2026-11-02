#include "restore_types.h"
#define DAT_00775810 (*(undefined4 *)0x00775810u)
#define DAT_0077592c (*(undefined4 *)0x0077592cu)
#define DAT_0077646c (*(undefined4 *)0x0077646cu)
#define DAT_0077de04 (*(undefined4 *)0x0077de04u)
#define DAT_0077de2a (*(undefined4 *)0x0077de2au)
#define DAT_00ea187c (*(undefined4 *)0x00ea187cu)
#define DAT_00ea4096 (*(undefined4 *)0x00ea4096u)
#define DAT_00ea40de (*(undefined4 *)0x00ea40deu)
#define DAT_00ea6160 (*(undefined4 *)0x00ea6160u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern int FUN_004c5240();

bool FUN_004435f0(int param_1,int param_2,int param_3)

{
  int iVar1;
  int iVar2;
  
  if (*(int *)(&DAT_00ea187c + DAT_00eaac28 * 0x49d8) < -4999) {
    return false;
  }
  iVar2 = (param_1 * 0x24ec + param_3) * 2;
  if ((*(short *)(&DAT_00ea4096 + iVar2) != 1) && (*(short *)(&DAT_00ea4096 + iVar2) != 3)) {
    return false;
  }
  if ((param_3 == 0x1b) || ((param_3 < 0 || (0x23 < param_3)))) {
    return false;
  }
  param_2 = param_2 * 0x6a;
  if ((&DAT_0077de2a)[param_3 + param_2] == '\0') {
    return false;
  }
  iVar1 = FUN_004c5240(*(undefined4 *)
                        (&DAT_0077646c + (param_3 + (uint)(byte)(&DAT_0077de04)[param_2] * 0x24) * 4
                        ));
  if (iVar1 <= *(int *)(&DAT_0077592c + (param_3 + (uint)(byte)(&DAT_0077de04)[param_2] * 0x24) * 4)
     ) {
    return false;
  }
  if (param_3 < 1) {
    return false;
  }
  if (*(short *)(&DAT_00ea4096 + iVar2) == 1) {
    return (int)(&DAT_00ea6160)[param_1 * 0x1276 + param_3] <
           *(short *)(&DAT_00ea40de + iVar2) * 100;
  }
  return (int)(&DAT_00ea6160)[param_1 * 0x1276 + param_3] < *(int *)(&DAT_00775810 + param_3 * 4);
}
