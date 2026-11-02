#include "restore_types.h"
#include <string.h>

char *FUN_005169c0(void)
{
  char *destination = (char *)(uintptr_t)0x00eadea4u;
  strcpy(destination, (const char *)(uintptr_t)0x005e0494u);
  strcat(destination, (const char *)(uintptr_t)0x00e38e74u);
  return destination;
}
