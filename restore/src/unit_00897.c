#include "restore_types.h"
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00ea5b34 (*(undefined4 *)0x00ea5b34u)
#define DAT_00ea5b38 (*(undefined4 *)0x00ea5b38u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern bool FUN_004435f0();
extern int FUN_0045b2a0();
extern void FUN_0045d4d0();

int FUN_0045d370(int param_1,int param_2,undefined4 param_3)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  if (((&DAT_007999e8)[param_2 * 0x84] == 0x48) || ((&DAT_007999e8)[param_2 * 0x84] == 0xc1)) {
    iVar3 = 1;
    do {
      iVar1 = FUN_004435f0(param_1,param_3,(&DAT_00ea5b34)[param_1 * 0x1276]);
      if (iVar1 != 0) {
        if (0x23 < iVar3) {
          return 0;
        }
        iVar3 = (&DAT_00ea5b34)[param_1 * 0x1276];
        (&DAT_00ea5b34)[param_1 * 0x1276] = iVar3 + 1;
        if (0x23 < iVar3 + 1) {
          (&DAT_00ea5b34)[param_1 * 0x1276] = 1;
        }
        iVar1 = FUN_0045b2a0(DAT_00eaac28,param_2,iVar3,100);
        if (iVar1 != 100) {
          iVar3 = 1;
          do {
            iVar1 = (&DAT_00ea5b38)[param_1 * 0x1276];
            (&DAT_00ea5b38)[param_1 * 0x1276] = iVar1 + 1;
            if (0x23 < iVar1 + 1) {
              (&DAT_00ea5b38)[param_1 * 0x1276] = 1;
            }
            iVar1 = FUN_004435f0(param_1,param_3,(&DAT_00ea5b38)[param_1 * 0x1276]);
            if (iVar1 != 0) {
              iVar1 = (&DAT_00ea5b34)[param_1 * 0x1276];
              (&DAT_00ea5b34)[param_1 * 0x1276] = iVar1 + 1;
              if (0x23 < iVar1 + 1) {
                (&DAT_00ea5b34)[param_1 * 0x1276] = 1;
              }
              iVar2 = FUN_0045b2a0(DAT_00eaac28,param_2,iVar1,100);
              if (iVar2 == 100) {
                FUN_0045d4d0(param_1,param_2,iVar1);
                return iVar1;
              }
            }
            iVar3 = iVar3 + 1;
            if (0x23 < iVar3) {
              return 0;
            }
          } while( true );
        }
        FUN_0045d4d0(param_1,param_2,iVar3);
        return iVar3;
      }
      iVar1 = (&DAT_00ea5b34)[param_1 * 0x1276];
      (&DAT_00ea5b34)[param_1 * 0x1276] = iVar1 + 1;
      if (0x23 < iVar1 + 1) {
        (&DAT_00ea5b34)[param_1 * 0x1276] = 1;
      }
      iVar3 = iVar3 + 1;
    } while (iVar3 < 0x24);
  }
  return 0;
}
