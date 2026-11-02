#include "restore_types.h"
#define DAT_00605798 (*(char *)0x00605798u)
#define DAT_006059d0 (*(undefined *)0x006059d0u)
#define DAT_006059d4 (*(undefined *)0x006059d4u)
#define DAT_006059d8 (*(undefined *)0x006059d8u)
#define DAT_006059dc (*(undefined *)0x006059dcu)
#define DAT_006059e0 (*(undefined *)0x006059e0u)
#define DAT_006059e4 (*(undefined *)0x006059e4u)
#define DAT_006059e8 (*(undefined *)0x006059e8u)
#define DAT_00ab623c (*(undefined4 *)0x00ab623cu)
#define DAT_00bc96cc (*(undefined *)0x00bc96ccu)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bca300 (*(undefined *)0x00bca300u)
#define DAT_00bca304 (*(undefined *)0x00bca304u)
#define DAT_00bca308 (*(undefined *)0x00bca308u)
#define DAT_00bca310 (*(undefined *)0x00bca310u)
#define DAT_00e38e60 (*(undefined1 *)0x00e38e60u)
extern undefined4 FUN_00484f60();
extern undefined4 FUN_0051b680();
extern undefined4 FUN_0051c790();

void FUN_00405c20(void)

{
  int iVar1;
  
  if ((DAT_00bca300 != 0) && (DAT_00e38e60 == '\0')) {
    DAT_006059d8 = DAT_00bca308;
    DAT_006059d4 = DAT_00bca308;
    DAT_006059e0 = DAT_00bca304;
    DAT_006059dc = DAT_00bca304;
    DAT_00605798 = 0;
    DAT_006059d0 = DAT_00bca2d8 + DAT_00bca308 * 0xe4 + DAT_00bca304;
    DAT_00bc96cc = (uint)(byte)(&DAT_00ab623c)[DAT_006059d0];
    if (((DAT_00bca310 != 0) || (DAT_006059e8 != 0x88)) && (iVar1 = FUN_00484f60(), iVar1 == 0)) {
      return;
    }
    DAT_006059e4 = 1;
    if (DAT_006059e8 == 5) {
      FUN_0051b680(1,DAT_006059e0,DAT_006059d8,0);
    }
    if (DAT_006059e8 == 6) {
      FUN_0051c790(1,DAT_006059e0,DAT_006059d8);
    }
    if (DAT_006059e8 == 0xa8) {
      FUN_0051c790(1,DAT_006059e0,DAT_006059d8);
    }
    if (DAT_006059e8 == 0xa9) {
      FUN_0051c790(1,DAT_006059e0,DAT_006059d8);
    }
    if (DAT_006059e8 == 8) {
      FUN_0051b680(1,DAT_006059e0,DAT_006059d8,1);
    }
    if (DAT_006059e8 == 7) {
      FUN_0051b680(1,DAT_006059e0,DAT_006059d8,1);
    }
  }
  return;
}
