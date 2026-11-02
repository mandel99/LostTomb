#include "restore_types.h"
extern float10 FUN_00567517();

void FUN_0056760b(void)

{
  ushort in_FPUStatusWord;
  ushort unaff_retaddr;
  
  if (((unaff_retaddr != 0x27f) && ((unaff_retaddr & 0x20) != 0)) &&
     ((in_FPUStatusWord & 0x20) != 0)) {
    FUN_00567517();
    return;
  }
  return;
}
