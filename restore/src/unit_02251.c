#include "restore_types.h"
#define DAT_005c8d38 (*(undefined4 *)0x005c8d38u)
#define DAT_00eadd8c (*(undefined *)0x00eadd8cu)
#define DAT_00eadd90 (*(undefined *)0x00eadd90u)
extern int FUN_00425c40();
extern void FUN_00425e80();

void FUN_00504630(void)

{
  FUN_00425e80(DAT_00eadd90 + 0xe2,DAT_00eadd8c + -0x25,&DAT_005c8d38,2,10);
  FUN_00425c40(DAT_00eadd90 + 0xe2,DAT_00eadd8c + -0x25,&DAT_005c8d38,2);
  return;
}
