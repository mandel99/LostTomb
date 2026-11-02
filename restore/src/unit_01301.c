#include "restore_types.h"
#define DAT_00799b34 (*(undefined4 *)0x00799b34u)
extern int FUN_00488030();
extern longlong FUN_0056133c();

void FUN_00489840(void)

{
  char cVar1;
  short sVar2;
  short *psVar3;
  int iVar4;
  
  iVar4 = 1;
  psVar3 = (short *)&DAT_00799b34;
  do {
    cVar1 = FUN_00488030(iVar4);
    if (cVar1 != '\0') {
      sVar2 = FUN_0056133c();
      *psVar3 = sVar2 * *psVar3;
    }
    psVar3 = psVar3 + 0x84;
    iVar4 = iVar4 + 1;
  } while ((int)psVar3 < 0x89b72c);
  return;
}
