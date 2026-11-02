#include "restore_types.h"
#define DAT_00799a18 (*(undefined4 *)0x00799a18u)
#define DAT_00eadd6c (*(undefined *)0x00eadd6cu)
#define DAT_00eadd88 (*(undefined *)0x00eadd88u)
#define DAT_00eadd8c (*(undefined *)0x00eadd8cu)
#define DAT_00eadd90 (*(undefined *)0x00eadd90u)
#define DAT_00f5684c (*(undefined *)0x00f5684cu)
#define DAT_00f56f94 (*(undefined *)0x00f56f94u)
extern uint FUN_004894a0();
extern void FUN_004d6160();
extern void FUN_004fc5c0();
extern int FUN_0053e590();

void FUN_004fc4f0(int param_1)

{
  undefined4 uVar1;
  char cVar2;
  undefined4 uVar3;
  
  uVar1 = DAT_00f56f94;
  cVar2 = FUN_004894a0(DAT_00eadd6c);
  if (cVar2 != '\0') {
    if ((&DAT_00799a18)[DAT_00eadd6c * 0x84] == 0) {
      uVar3 = 5;
    }
    else {
      uVar3 = 6;
    }
    FUN_004d6160(0xb1,uVar3,DAT_00eadd90 + 0x41,param_1 + 10,(DAT_00eadd88 + -4) * 0x10,uVar1,0,
                 DAT_00eadd90,DAT_00eadd8c);
    FUN_0053e590(DAT_00f5684c + 0xe,DAT_00eadd90 + 0x28,param_1 + 6);
    return;
  }
  FUN_004fc5c0(param_1);
  return;
}
