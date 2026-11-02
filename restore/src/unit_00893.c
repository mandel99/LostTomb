#include "restore_types.h"
#define DAT_00705eb8 (*(undefined4 *)0x00705eb8u)
#define DAT_007900c2 (*(undefined4 *)0x007900c2u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00799a18 (*(undefined4 *)0x00799a18u)
#define DAT_00799a2e (*(undefined4 *)0x00799a2eu)
#define DAT_00799a98 (*(undefined4 *)0x00799a98u)
extern int FUN_004cde60();

int FUN_0045ce90(int param_1,int param_2)

{
  int iVar1;
  uint uVar2;
  
  uVar2 = (uint)*(byte *)((int)&DAT_00799a98 + param_1 * 0x108 + 1);
  iVar1 = FUN_004cde60((int)(short)(&DAT_00799a18)[param_1 * 0x84],
                       *(undefined4 *)
                        (&DAT_00705eb8 + (short)(&DAT_007999e8)[param_1 * 0x84] * 0x28));
  if (0x31 < iVar1) {
    if (param_2 != 0) {
      iVar1 = 0;
      while (iVar1 = iVar1 + 1, iVar1 < 9) {
        while (iVar1 < 1) {
          iVar1 = iVar1 + 1;
          if (8 < iVar1) goto LAB_0045cf05;
        }
        if (*(char *)((int)&DAT_007900c2 + uVar2 * 0xc4 + iVar1) == '\x02') {
          return 0;
        }
      }
    }
LAB_0045cf05: ;
    iVar1 = 0;
    while (iVar1 = iVar1 + 1, iVar1 < 9) {
      while (iVar1 < 1) {
        iVar1 = iVar1 + 1;
        if (8 < iVar1) {
          return -1;
        }
      }
      if ((*(char *)((int)&DAT_007900c2 + uVar2 * 0xc4 + iVar1) == '\x03') &&
         ((&DAT_00799a2e)[iVar1 + param_1 * 0x84] != 0)) {
        return iVar1;
      }
    }
  }
  return -1;
}
