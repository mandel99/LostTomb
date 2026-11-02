#include "restore_types.h"
#define DAT_00bcb9cb (*(undefined4 *)0x00bcb9cbu)
#define DAT_00bcb9d0 (*(undefined4 *)0x00bcb9d0u)
#define DAT_00bcd5f8 (*(undefined4 *)0x00bcd5f8u)
#define DAT_00bcd608 (*(undefined4 *)0x00bcd608u)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
extern void FUN_004b3250();
extern void FUN_004b5420();

void FUN_004a4470(int param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  short *psVar4;
  
  iVar2 = 0;
  (&DAT_00bcb9cb)[param_1 * 0x90] = 0;
  psVar4 = (short *)(&DAT_00bcb9d0 + param_1 * 0x90);
  do {
    if (*psVar4 == 0) {
      return;
    }
    iVar3 = (int)*psVar4;
    iVar1 = iVar3 * 0x184;
    if ((&DAT_00bcd630)[iVar1] != '\x04') {
      (&DAT_00bcd5f8)[iVar1] = 0;
      (&DAT_00bcd608)[iVar1] = (&DAT_00bcd630)[iVar1];
      (&DAT_00bcd630)[iVar1] = 0x12;
      FUN_004b3250(iVar3);
      FUN_004b5420(iVar3);
    }
    iVar2 = iVar2 + 1;
    psVar4 = psVar4 + 1;
  } while (iVar2 < 0x10);
  return;
}
