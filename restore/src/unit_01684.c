#include "restore_types.h"
#define DAT_00e2079b (*(char *)0x00e2079bu)
#define DAT_00e2079c (*(uint8_t *)0x00e2079cu)
#define DAT_00e207ec (*(uint8_t *)0x00e207ecu)
#define DAT_00e208a0 (*(uint8_t *)0x00e208a0u)
#define DAT_00e208f0 (*(uint8_t *)0x00e208f0u)
#define DAT_00e20990 (*(int32_t *)0x00e20990u)
#define DAT_00e20994 (*(int32_t *)0x00e20994u)
extern void FUN_004cbf10();
extern void FUN_004cc1b0();

void FUN_004cc400(undefined1 param_1)

{
  int iVar1;
  int iVar2;
  
  if (0 < DAT_00e20990) {
    FUN_004cc1b0();
    if (DAT_00e20994 != 2) {
      if (DAT_00e2079b == '\0') {
        *(undefined1 *)
         (*(int *)(&DAT_00e207ec + DAT_00e20990 * 4) + *(int *)(&DAT_00e2079c + DAT_00e20990 * 4)) =
             param_1;
        iVar2 = *(int *)(&DAT_00e207ec + DAT_00e20990 * 4);
        iVar1 = DAT_00e20990 * 4;
        *(int *)(&DAT_00e207ec + DAT_00e20990 * 4) = iVar2 + 1;
        *(undefined1 *)(*(int *)(&DAT_00e2079c + iVar1) + iVar2 + 1) = 0;
      }
      else if (*(int *)(&DAT_00e208a0 + DAT_00e20990 * 4) <
               *(int *)(&DAT_00e208f0 + DAT_00e20990 * 4)) {
        FUN_004cbf10(*(int *)(&DAT_00e207ec + DAT_00e20990 * 4) +
                     *(int *)(&DAT_00e2079c + DAT_00e20990 * 4),
                     *(int *)(&DAT_00e2079c + DAT_00e20990 * 4) + 1 +
                     *(int *)(&DAT_00e208a0 + DAT_00e20990 * 4));
        *(undefined1 *)
         (*(int *)(&DAT_00e207ec + DAT_00e20990 * 4) + *(int *)(&DAT_00e2079c + DAT_00e20990 * 4)) =
             param_1;
        *(int *)(&DAT_00e207ec + DAT_00e20990 * 4) = *(int *)(&DAT_00e207ec + DAT_00e20990 * 4) + 1;
        return;
      }
    }
  }
  return;
}
