#include "restore_types.h"
#define DAT_00775810 (*(undefined4 *)0x00775810u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00799a12 (*(undefined4 *)0x00799a12u)
#define DAT_00799a14 (*(undefined4 *)0x00799a14u)
#define DAT_00ea4096 (*(undefined4 *)0x00ea4096u)
#define DAT_00ea40de (*(undefined4 *)0x00ea40deu)
#define DAT_00ea6160 (*(undefined4 *)0x00ea6160u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern undefined4 FUN_0045b230();
extern int FUN_0045d510();

int FUN_004454d0(int param_1,int param_2,int param_3)

{
  char cVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  
  if (((&DAT_007999e8)[param_1 * 0x84] != 0x48) && ((&DAT_007999e8)[param_1 * 0x84] != 0xc1)) {
    return 0;
  }
  iVar4 = 0;
  FUN_0045d510(param_1,param_2);
  cVar1 = FUN_0045b230(param_1,0,param_2);
  if (cVar1 != '\0') {
    return 0;
  }
  cVar1 = FUN_0045b230(param_1,param_3,param_2);
  if (cVar1 == '\0') {
    iVar3 = 8;
    do {
      param_1 = (int)*(short *)(&DAT_00799a12 + param_1 * 0x108);
      if ((short)(&DAT_00799a14)[param_1 * 0x84] < 1) {
        iVar2 = 0;
      }
      else {
        iVar2 = (int)(short)(&DAT_00799a14)[param_1 * 0x84];
      }
      iVar4 = iVar4 + iVar2;
      iVar3 = iVar3 + -1;
    } while (iVar3 != 0);
    iVar4 = 0xc80 - iVar4;
    if ((iVar4 <= param_3) && (param_3 = iVar4, iVar4 < 100)) {
      return 0;
    }
    param_3 = param_3 / 100;
    iVar4 = (DAT_00eaac28 * 0x24ec + param_2) * 2;
    if (*(short *)(&DAT_00ea4096 + iVar4) == 1) {
      iVar3 = (&DAT_00ea6160)[DAT_00eaac28 * 0x1276 + param_2];
      iVar4 = *(short *)(&DAT_00ea40de + iVar4) * 100;
      if (iVar4 <= iVar3 + param_3 * 100) {
        if (iVar3 < iVar4) {
          return (iVar4 - iVar3) / 100;
        }
        return 0;
      }
    }
    else {
      iVar4 = *(int *)(&DAT_00775810 + param_2 * 4);
      iVar3 = (&DAT_00ea6160)[DAT_00eaac28 * 0x1276 + param_2];
      if (iVar4 <= iVar3 + param_3 * 100) {
        if (iVar4 <= iVar3) {
          return 0;
        }
        param_3 = (iVar4 - iVar3) / 100;
      }
    }
    return param_3;
  }
  return 0;
}
