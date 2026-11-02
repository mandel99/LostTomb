#include "restore_types.h"

void FUN_004d4040(void)
{
  unsigned int selector = *(unsigned int *)(uintptr_t)0x00eaac28u;
  unsigned int offset = selector * 0x49d8u;

  *(unsigned int *)(uintptr_t)(0x00ea41fcu + offset) = 0;
  *(unsigned int *)(uintptr_t)(0x00ea4200u + offset) = 0;
  *(unsigned char *)(uintptr_t)(0x00ea4204u + offset) = 0;
  memset((void *)(uintptr_t)(0x00ea4205u + offset),0,0x24u);
}
