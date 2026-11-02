#include "restore_types.h"
#include <string.h>

char *FUN_00516a20(void)
{
  char *destination = (char *)(uintptr_t)0x00eadda0u;
  strcpy(destination, (const char *)(uintptr_t)0x005e0494u);
  strcat(destination, (const char *)(uintptr_t)0x00e38e74u);
  strcat(destination, (const char *)(uintptr_t)0x005f80e8u);
  return destination;
}
