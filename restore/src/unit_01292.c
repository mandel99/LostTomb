#include "restore_types.h"
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
extern int FUN_00488080();
extern int FUN_00489050();
extern bool FUN_004890f0();
extern uint FUN_004894a0();

int FUN_00489110(int param_1)

{
  char cVar1;
  int iVar2;
  int iVar3;
  
  iVar2 = FUN_00489050((int)*(short *)((int)&DAT_007999dc + param_1 * 0x108 + 2),
                       (int)*(short *)(&DAT_007999e0 + param_1 * 0x108),3,3);
  cVar1 = FUN_004890f0(param_1);
  if (cVar1 != '\0') {
    cVar1 = FUN_00488080(param_1);
    if (cVar1 != '\0') {
      cVar1 = FUN_004894a0(param_1);
      if (cVar1 == '\0') {
        iVar2 = iVar2 + 0x28;
      }
      else {
        iVar2 = iVar2 + 0x14;
      }
    }
  }
  iVar3 = 99;
  if (iVar2 < 100) {
    iVar3 = iVar2;
  }
  return iVar3;
}
