#include "restore_types.h"
#define DAT_00799ae0 (*(undefined4 *)0x00799ae0u)
#define DAT_00799af0 (*(undefined4 *)0x00799af0u)
extern int FUN_00542ad0();

uint FUN_00542ee0(void)

{
  uint in_EAX;
  short *psVar1;
  char *pcVar2;
  int iVar3;
  
  iVar3 = 1;
  psVar1 = &DAT_00799af0;
  pcVar2 = &DAT_00799ae0;
  do {
    if ((*pcVar2 == '\x01') && (*psVar1 == 0xd5)) {
      in_EAX = FUN_00542ad0(iVar3,0);
      if (in_EAX == 100) {
        return 1;
      }
      iVar3 = iVar3 + 99;
      pcVar2 = pcVar2 + 0x6618;
      psVar1 = psVar1 + 0x330c;
    }
    pcVar2 = pcVar2 + 0x108;
    iVar3 = iVar3 + 1;
    psVar1 = psVar1 + 0x84;
  } while ((int)pcVar2 < 0x89b6d8);
  return in_EAX & 0xffffff00;
}
