#include "restore_types.h"
#define DAT_007848e2 (*(undefined *)0x007848e2u)
#define DAT_00e950dc (*(undefined *)0x00e950dcu)
extern int FUN_004bd590();
extern void FUN_004e0cf0();

void FUN_004be640(void)

{
  int iVar1;
  int in_ECX;
  
  if ((*(int *)(in_ECX + 0x32fb4) == 5) && (*(char *)(in_ECX + 0x32fbc) != '\0')) {
    DAT_00e950dc = 1;
    iVar1 = FUN_004bd590();
    switch((iVar1 + 0x13) / 0x14) {
    case 0: ;
      FUN_004e0cf0(0x158,0,0,0,0);
      return;
    case 1: ;
      if (DAT_007848e2 == 0) {
        FUN_004e0cf0(0x157,0,0,0,0);
        return;
      }
      FUN_004e0cf0(0x159,0,0,0,0);
      return;
    case 2: ;
      if (DAT_007848e2 == 0) {
        FUN_004e0cf0(0x156,0,0,0,0);
        return;
      }
      FUN_004e0cf0(0x15a,0,0,0,0);
      return;
    case 3: ;
      FUN_004e0cf0(0x15b,0,0,0,0);
      return;
    case 4: ;
      FUN_004e0cf0(0x15c,0,0,0,0);
      return;
    case 5: ;
      FUN_004e0cf0(0x15d,0,0,0,0);
    }
  }
  return;
}
