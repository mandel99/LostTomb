#include "restore_types.h"
#define DAT_00784de0 (*(undefined *)0x00784de0u)
#define DAT_00799a10 (*(undefined4 *)0x00799a10u)
#define DAT_00eaa978 (*(undefined4 *)0x00eaa978u)
extern void FUN_004f4a60();
extern int FUN_004f4b70();

int FUN_004f4f80(int param_1)

{
  int iVar1;
  short *psVar2;
  int *piVar3;
  int iVar4;
  
  if ((*(short *)(&DAT_00799a10 + param_1 * 0x108) == 0) &&
     (iVar1 = FUN_004f4b70(param_1), -1 < iVar1)) {
    iVar4 = 0;
    piVar3 = &DAT_00eaa978;
    psVar2 = &DAT_00784de0;
    do {
      if ((*psVar2 == iVar1) && (*piVar3 == 0)) {
        (&DAT_00eaa978)[iVar4] = iVar1;
        if (2 < iVar4) {
          return -1;
        }
        FUN_004f4a60(param_1,iVar4);
        return iVar4;
      }
      psVar2 = psVar2 + 1;
      iVar4 = iVar4 + 1;
      piVar3 = piVar3 + 1;
    } while ((int)psVar2 < 0x784de6);
  }
  return -1;
}
