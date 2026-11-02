#include "restore_types.h"
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00799a12 (*(undefined4 *)0x00799a12u)
#define DAT_00799a14 (*(undefined4 *)0x00799a14u)
#define DAT_00bcd667 (*(undefined4 *)0x00bcd667u)
extern undefined4 FUN_004434e0();
extern int FUN_0045d510();

undefined4 FUN_0045cf50(int param_1,undefined4 param_2,int param_3,undefined4 param_4)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  if ((((&DAT_007999e8)[param_3 * 0x84] == 0x48) || ((&DAT_007999e8)[param_3 * 0x84] == 0xc1)) &&
     ((byte)(&DAT_00bcd667)[param_1 * 0x184] < 8)) {
    iVar2 = 0;
    iVar3 = param_3;
    do {
      iVar3 = (int)*(short *)(&DAT_00799a12 + iVar3 * 0x108);
      if ((0 < iVar3) && (0 < (short)(&DAT_00799a14)[iVar3 * 0x84])) {
        iVar1 = FUN_004434e0(param_2,param_4,(int)(short)(&DAT_007999ea)[iVar3 * 0x84]);
        if (iVar1 != 0) {
          iVar1 = FUN_0045d510(param_3,(int)(short)(&DAT_007999ea)[iVar3 * 0x84]);
          if (99 < iVar1) {
            return 1;
          }
        }
      }
      iVar2 = iVar2 + 1;
    } while (iVar2 < 8);
  }
  return 0;
}
