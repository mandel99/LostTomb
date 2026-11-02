#include "restore_types.h"
extern void FUN_005613e3();

void FUN_005614ab(void)

{
  int unaff_EBP;
  
  if (*(int *)(unaff_EBP + -0x20) == 0) {
    FUN_005613e3(*(undefined4 *)(unaff_EBP + 8),*(undefined4 *)(unaff_EBP + 0xc),
                 *(undefined4 *)(unaff_EBP + -0x1c),*(undefined4 *)(unaff_EBP + 0x18));
  }
  return;
}
