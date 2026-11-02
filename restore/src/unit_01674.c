#include "restore_types.h"
#define DAT_00e2079c (*(uint8_t *)0x00e2079cu)
#define DAT_00e207ec (*(uint8_t *)0x00e207ecu)
#define DAT_00e20850 (*(uint8_t *)0x00e20850u)
#define DAT_00e208a0 (*(uint8_t *)0x00e208a0u)
#define DAT_00e208f0 (*(uint8_t *)0x00e208f0u)
#define DAT_00e20940 (*(uint8_t *)0x00e20940u)
#define DAT_00e20990 (*(int32_t *)0x00e20990u)
#define DAT_00e20994 (*(int32_t *)0x00e20994u)
extern int FUN_004cbf30();

void FUN_004cc1b0(void)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  
  DAT_00e20994 = 0;
  if (0 < DAT_00e20990) {
    iVar3 = FUN_004cbf30(*(undefined4 *)(&DAT_00e2079c + DAT_00e20990 * 4),
                         *(undefined4 *)(&DAT_00e20940 + DAT_00e20990 * 4));
    iVar2 = DAT_00e20990;
    piVar1 = (int *)(&DAT_00e207ec + DAT_00e20990 * 4);
    if (*(int *)(&DAT_00e208a0 + DAT_00e20990 * 4) + 1 <= *(int *)(&DAT_00e207ec + DAT_00e20990 * 4)
       ) {
      *piVar1 = *(int *)(&DAT_00e208a0 + DAT_00e20990 * 4) + 1;
      DAT_00e20994 = 1;
    }
    if (*(int *)(&DAT_00e208f0 + iVar2 * 4) <= *piVar1) {
      *piVar1 = *(int *)(&DAT_00e208f0 + iVar2 * 4);
      DAT_00e20994 = 2;
    }
    if (*(int *)(&DAT_00e20850 + iVar2 * 4) <= iVar3) {
      DAT_00e20994 = 2;
    }
    if (*piVar1 < 0) {
      *piVar1 = 0;
    }
  }
  return;
}
