#include "restore_types.h"
#define DAT_00bcd606 (*(undefined4 *)0x00bcd606u)
#define DAT_00bcd609 (*(undefined4 *)0x00bcd609u)
#define DAT_00d056f8 (*(undefined *)0x00d056f8u)
extern void FUN_004a5680();
extern void FUN_004b3250();

void FUN_004aec80(undefined4 param_1,undefined4 *param_2)

{
  char cVar1;
  
  FUN_004a5680(param_1,0);
  *param_2 = 1;
  cVar1 = (&DAT_00bcd609)[DAT_00d056f8 * 0x184];
  if (cVar1 == '\t') {
    FUN_004b3250(DAT_00d056f8);
    return;
  }
  if ((cVar1 == '\n') || (cVar1 == '\b')) {
    (&DAT_00bcd606)[DAT_00d056f8 * 0x184] = 2;
  }
  return;
}
