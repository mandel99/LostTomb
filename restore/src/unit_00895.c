#include "restore_types.h"
#define DAT_005d2c4c (*(undefined4 *)0x005d2c4cu)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00799a12 (*(undefined4 *)0x00799a12u)
#define DAT_00799a14 (*(undefined4 *)0x00799a14u)
#define DAT_00ea187c (*(undefined4 *)0x00ea187cu)
#define DAT_00ea44d4 (*(undefined4 *)0x00ea44d4u)
#define DAT_00ea5e3c (*(undefined4 *)0x00ea5e3cu)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern undefined4 FUN_004434e0();
extern int FUN_0045b570();
extern int FUN_0045d510();
extern int FUN_004cde40();

int FUN_0045d010(int param_1,int param_2,undefined4 param_3)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  
  iVar3 = param_2;
  iVar4 = 0;
  if (((&DAT_007999e8)[param_2 * 0x84] != 0x48) && ((&DAT_007999e8)[param_2 * 0x84] != 0xc1)) {
    return 0;
  }
  param_2 = 0;
  iVar5 = iVar3;
  do {
    iVar5 = (int)*(short *)(&DAT_00799a12 + iVar5 * 0x108);
    if (((0 < iVar5) && (0 < (short)(&DAT_00799a14)[iVar5 * 0x84])) &&
       (iVar2 = FUN_004434e0(param_1,param_3,(int)(short)(&DAT_007999ea)[iVar5 * 0x84]), iVar2 != 0)
       ) {
      iVar4 = FUN_0045d510(iVar3,(int)(short)(&DAT_007999ea)[iVar5 * 0x84]);
      if (iVar4 < 100) {
        return 0;
      }
      iVar4 = (int)(short)(&DAT_007999ea)[iVar5 * 0x84];
    }
    param_2 = param_2 + 1;
  } while (param_2 < 8);
  if (iVar4 == 0) {
    return 0;
  }
  FUN_0045b570(param_1,iVar3,iVar4,100);
  if ((int)(&DAT_00ea5e3c)[DAT_00eaac28 * 0x1276] < 1) {
    iVar3 = *(int *)(&DAT_005d2c4c + iVar4 * 8);
    *(int *)(&DAT_00ea187c + param_1 * 0x49d8) = *(int *)(&DAT_00ea187c + param_1 * 0x49d8) + iVar3;
    (&DAT_00ea44d4)[param_1 * 0x1276] = (&DAT_00ea44d4)[param_1 * 0x1276] + iVar3;
    return iVar4;
  }
  iVar3 = FUN_004cde40(*(undefined4 *)(&DAT_005d2c4c + iVar4 * 8),0x96);
  uVar1 = *(undefined4 *)(&DAT_005d2c4c + iVar4 * 8);
  *(int *)(&DAT_00ea187c + param_1 * 0x49d8) = *(int *)(&DAT_00ea187c + param_1 * 0x49d8) + iVar3;
  iVar3 = FUN_004cde40(uVar1,0x96);
  (&DAT_00ea44d4)[param_1 * 0x1276] = (&DAT_00ea44d4)[param_1 * 0x1276] + iVar3;
  return iVar4;
}
