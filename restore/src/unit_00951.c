/* Recovered C normalization: switch case labels must be integer constants. */
#include "restore_types.h"
#define DAT_00784e78 (*(undefined4 *)0x00784e78u)
#define DAT_0078d790 (*(undefined *)0x0078d790u)
#define DAT_00799af6 (*(undefined4 *)0x00799af6u)
#define DAT_00e20768 (*(uint32_t *)0x00e20768u)
#define DAT_00e38ea8 (*(undefined1 *)0x00e38ea8u)
extern void FUN_004cc110();

uint FUN_00469d50(void)

{
  short *psVar1;
  short sVar2;
  short *psVar3;
  short *psVar4;
  int *piVar5;
  int iVar6;
  
  FUN_004cc110(0x133,(int)DAT_00e38ea8);
  if (((*(char *)(DAT_00e20768 + 4) == 'A') || (*(char *)(DAT_00e20768 + 4) == 'V')) &&
     (iVar6 = 1, 0 < DAT_0078d790)) {
    psVar3 = &DAT_00799af6;
    do {
      sVar2 = psVar3[-3];
      if ((((((sVar2 == 0xde) && (*psVar3 == -1)) || ((sVar2 == 0xe5 && (*psVar3 == -1)))) ||
           ((sVar2 == 0xea && (*psVar3 == -1)))) || ((sVar2 == 0xeb && (*psVar3 == -1)))) ||
         ((sVar2 == 0xec && (*psVar3 == -1)))) goto LAB_00469e0b;
      iVar6 = iVar6 + 1;
      psVar3 = psVar3 + 0x84;
    } while (iVar6 <= DAT_0078d790);
  }
  piVar5 = &DAT_00784e78;
  psVar3 = (short *)0xffffffff;
  do {
    switch((int)(psVar3)) {
    case 0x0: ;
    case 0x7: ;
    case 0x9: ;
    case 0xc: ;
    case 0xe: ;
    case 0x10: ;
    case 0x11: ;
    case 0x12: ;
    case 0x13: ;
    case 0x16: ;
    case 0x17: ;
    case 0x18: ;
    case 0x19: ;
    case 0x1b: ;
    case 0x1d: ;
      if (0 < *piVar5) {
LAB_00469e0b: ;
        return CONCAT31((int3)((uint)psVar3 >> 8),1);
      }
    }
    psVar4 = (short *)((int)psVar3 + 1);
    piVar5 = piVar5 + 1;
    psVar1 = psVar3 + 1;
    psVar3 = psVar4;
    if (0x23 < (int)psVar1) {
      return (uint)psVar4 & 0xffffff00;
    }
  } while( true );
}
