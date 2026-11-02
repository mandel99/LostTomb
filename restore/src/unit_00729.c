#include "restore_types.h"
#define DAT_007770f1 (*(undefined1 *)0x007770f1u)
#define DAT_007770f2 (*(undefined4 *)0x007770f2u)
extern int FUN_004449d0();
extern int FUN_00447430();
extern int FUN_004477d0();
extern uint FUN_00449390();

undefined4 FUN_0044c110(void)

{
  char cVar1;
  int iVar2;
  undefined4 uVar3;
  int in_ECX;
  undefined1 local_1;
  
  if (*(char *)(in_ECX + 2) == '\x02') {
    iVar2 = (int)*(short *)(in_ECX + 0x58);
    if ((-1 < iVar2) && (iVar2 < 0x32)) {
      if ((byte)(&DAT_007770f1)[iVar2 * 0x144] < 2) {
        (&DAT_007770f2)[iVar2 * 0x144] = 0;
      }
      if ((&DAT_007770f2)[iVar2 * 0x144] != '\0') {
        uVar3 = FUN_004477d0();
        return uVar3;
      }
    }
  }
  else {
    cVar1 = FUN_00449390();
    if (cVar1 != '\0') {
      uVar3 = FUN_004449d0((int)*(short *)(in_ECX + 0x1c));
      FUN_00447430(uVar3,&local_1);
      uVar3 = FUN_004477d0();
      return uVar3;
    }
  }
  return 0;
}
