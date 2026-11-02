#include "restore_types.h"

undefined1 FUN_004496b0(void)

{
  int in_ECX;
  
  if ((*(char *)(in_ECX + 2) != '\x01') && (*(char *)(in_ECX + 2) != '\x17')) {
    return 0;
  }
  return 1;
}
