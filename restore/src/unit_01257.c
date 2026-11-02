#include "restore_types.h"
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00bca30c (*(char *)0x00bca30cu)
extern void FUN_00478420();
extern void FUN_00487450();
extern void FUN_004877f0();

void FUN_00486e70(void)

{
  if (DAT_00bca30c != '\0') {
    FUN_00478420();
    FUN_00487450(0,0,DAT_00bca2d0 + -1,DAT_00bca2d4 + -1);
    FUN_004877f0(0,0,DAT_00bca2d0 + -1,DAT_00bca2d4 + -1);
    DAT_00bca30c = '\0';
  }
  return;
}
