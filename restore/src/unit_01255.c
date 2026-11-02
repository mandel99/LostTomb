#include "restore_types.h"
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
extern undefined4 FUN_00486bf0();

undefined4 FUN_00486cf0(int param_1)

{
  return FUN_00486bf0((param_1 - DAT_00bca2d8) % 0xe4,(param_1 - DAT_00bca2d8) / 0xe4);
}
