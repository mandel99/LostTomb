#include "restore_types.h"
#define DAT_00e94620 (*(undefined4 *)0x00e94620u)
#define DAT_00e94d9c (*(int32_t *)0x00e94d9cu)
extern void FUN_004d5dd0();
extern void FUN_004d5e70();
extern void FUN_004d69b0();

void FUN_004d5d90(void)

{
  undefined4 *puVar1;
  
  puVar1 = &DAT_00e94620;
  do {
    puVar1[-1] = 0;
    *puVar1 = 0;
    puVar1[-2] = 0;
    puVar1[1] = 1;
    puVar1 = puVar1 + 4;
  } while ((int)puVar1 < 0xe94da0);
  DAT_00e94d9c = 1;
  FUN_004d5dd0();
  FUN_004d5e70();
  FUN_004d69b0();
  return;
}
