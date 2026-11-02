#include "restore_types.h"
#define DAT_00605984 (*(undefined *)0x00605984u)
#define DAT_00e38da4 (*(undefined *)0x00e38da4u)
#define DAT_00e38da8 (*(undefined *)0x00e38da8u)
#define DAT_00f53f08 (*(undefined *)0x00f53f08u)
#define DAT_00f53f0c (*(undefined *)0x00f53f0cu)
#define DAT_00f56f8c (*(undefined *)0x00f56f8cu)
#define DAT_00f56fb4 (*(undefined *)0x00f56fb4u)
extern void FUN_004cd680();
extern void FUN_004cf090();
extern void FUN_004d6250();
extern void FUN_00536820();

void FUN_00521800(void)

{
  int iVar1;
  int iVar2;
  
  FUN_00536820(DAT_00f53f0c + 0xa0,DAT_00f53f08 + 0xb0,0x14,9);
  FUN_004d6250(0x32,0x19,DAT_00f53f0c + 0xa0,DAT_00f53f08 + 0xb9,0x140,DAT_00f56fb4,0,DAT_00e38da8,
               DAT_00e38da4);
  iVar2 = 0;
  iVar1 = 0;
  do {
    FUN_004cd680(iVar1 + 0xb2 + DAT_00f53f0c,DAT_00f53f08 + 0xdd,0x1b,0x1b,0);
    FUN_004d6250(0x32,iVar2 + 0x1b,iVar1 + 0xb2 + DAT_00f53f0c,DAT_00f53f08 + 0xe0,0x1b,DAT_00f56fb4
                 ,0,DAT_00e38da8,DAT_00e38da4);
    if (DAT_00605984 <= iVar2) {
      FUN_004cf090(iVar1 + 0xb3 + DAT_00f53f0c,DAT_00f53f08 + 0xde,0x19,0x19,1);
    }
    iVar1 = iVar1 + 0x20;
    iVar2 = iVar2 + 1;
  } while (iVar1 < 0x120);
  FUN_004cd680(DAT_00f53f0c + 0xb4,DAT_00f53f08 + 0x100,0x118,0x19,0);
  FUN_004d6250(0x32,0x1a,DAT_00f53f0c + 0xb4,DAT_00f53f08 + 0x107,0x118,DAT_00f56f8c,0,DAT_00e38da8,
               DAT_00e38da4);
  FUN_004d6250(0xd,3,DAT_00f53f0c + 0xa0,DAT_00f53f08 + 0x128,0x140,DAT_00f56f8c,0,DAT_00e38da8,
               DAT_00e38da4);
  return;
}
