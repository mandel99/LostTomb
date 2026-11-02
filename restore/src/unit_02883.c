#include "restore_types.h"
#define DAT_00576000 (*(undefined *)0x00576000u)
#define DAT_0057607c (*(undefined4 *)0x0057607cu)
#define DAT_00576080 (*(undefined4 *)0x00576080u)
#define DAT_00576094 (*(undefined4 *)0x00576094u)
#define PTR_FUN_00604940 (*(unsigned char * *)0x00604940u)
extern void FUN_005647a8();

void FUN_005646c0(void)

{
  if (PTR_FUN_00604940 != (undefined *)0x0) {
    (*(code *)PTR_FUN_00604940)();
  }
  FUN_005647a8(&DAT_00576080,&DAT_00576094);
  FUN_005647a8(&DAT_00576000,&DAT_0057607c);
  return;
}
