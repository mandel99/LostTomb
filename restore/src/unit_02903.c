#include "restore_types.h"
#define DAT_01266a64 (*(undefined *)0x01266a64u)
#define DAT_01266a6c (*(undefined *)0x01266a6cu)
#define DAT_01266a70 (*(undefined *)0x01266a70u)
#define DAT_01266a74 (*(undefined *)0x01266a74u)
#define DAT_01266a78 (*(undefined *)0x01266a78u)
#define DAT_01266a7c (*(undefined *)0x01266a7cu)

undefined4 FUN_00565427(void)

{
  DAT_01266a78 = HeapAlloc(DAT_01266a7c,0,0x140);
  if (DAT_01266a78 == (LPVOID)0x0) {
    return 0;
  }
  DAT_01266a70 = 0;
  DAT_01266a74 = 0;
  DAT_01266a6c = DAT_01266a78;
  DAT_01266a64 = 0x10;
  return 1;
}
