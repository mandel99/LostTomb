#include "restore_types.h"
#define DAT_0060492c (*(undefined4 *)0x0060492cu)
#define DAT_00604930 (*(undefined4 *)0x00604930u)
#define DAT_00604934 (*(undefined4 *)0x00604934u)

void FUN_0056187e(void)

{
  undefined4 in_EAX;
  int unaff_EBP;
  
  DAT_00604930 = *(undefined4 *)(unaff_EBP + 8);
  DAT_0060492c = in_EAX;
  DAT_00604934 = unaff_EBP;
  return;
}
