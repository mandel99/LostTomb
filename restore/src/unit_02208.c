#include "restore_types.h"
#define DAT_00784c38 (*(undefined *)0x00784c38u)
#define DAT_00eadb28 (*(undefined4 *)0x00eadb28u)
extern int FUN_004ea280();

void FUN_004f6ad0(void)

{
  DAT_00eadb28 = FUN_004ea280(DAT_00784c38);
  return;
}
