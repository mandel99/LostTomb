#include "restore_types.h"
#define DAT_00799a8c (*(undefined4 *)0x00799a8cu)
#define DAT_00bcd7d6 (*(undefined4 *)0x00bcd7d6u)

void FUN_004b4710(void)

{
  char cVar1;
  short sVar2;
  short *psVar3;
  
  psVar3 = &DAT_00bcd7d6;
  do {
    if (((char)psVar3[-0x26] == '\x01') &&
       (((cVar1 = (char)psVar3[-0x28], cVar1 == '\x19' || (cVar1 == 'M')) || (cVar1 == 'N')))) {
      sVar2 = *psVar3;
      *psVar3 = 0;
      *(undefined1 *)(psVar3 + -0x28) = 0x43;
      psVar3[-0x12] = 0;
      *(undefined2 *)(&DAT_00799a8c + sVar2 * 0x108) = 0;
    }
    psVar3 = psVar3 + 0xc2;
  } while ((int)psVar3 < 0xc8ad92);
  return;
}
