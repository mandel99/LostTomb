#include "restore_types.h"
extern void FUN_00560648();

void FUN_0056e208(void)

{
  int unaff_EBP;
  
  if ((*(uint *)(unaff_EBP + -0x10) & 1) != 0) {
    FUN_00560648();
    return;
  }
  return;
}
