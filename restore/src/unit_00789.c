#include "restore_types.h"
#define DAT_005f5d08 (*(undefined4 *)0x005f5d08u)
#define DAT_00799ba8 (*(undefined4 *)0x00799ba8u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00ea5ba8 (*(undefined4 *)0x00ea5ba8u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define DAT_00eaae90 (*(undefined *)0x00eaae90u)
extern void FUN_004ed360();

uint FUN_00451b10(void)

{
  short sVar1;
  uint in_EAX;
  uint uVar2;
  int iVar3;
  short *psVar4;
  
  psVar4 = &DAT_00799ba8;
  do {
    if ((char)psVar4[-100] == '\x01') {
      sVar1 = psVar4[-0x5c];
      uVar2 = CONCAT22((short)(in_EAX >> 0x10),sVar1);
      in_EAX = uVar2;
      if (sVar1 == 0xb7) {
        if (*psVar4 < 1) {
          iVar3 = (*(int *)(&DAT_005f5d08 + (uint)*(byte *)(psVar4 + -0x3a) * 4) +
                  *(int *)(psVar4 + -0x5e)) - DAT_00bca2d8;
          FUN_004ed360(iVar3 % 0xe4,iVar3 / 0xe4,3,0);
          uVar2 = 0;
          in_EAX = DAT_00eaae90;
          if (DAT_00eaae90 == 0) goto LAB_00451bba;
        }
      }
      else if ((sVar1 == 0xd2) && (0xf < *(short *)((int)&DAT_00ea5ba8 + DAT_00eaac28 * 0x49d8 + 2))
              ) {
LAB_00451bba: ;
        return CONCAT31((int3)(uVar2 >> 8),1);
      }
    }
    psVar4 = psVar4 + 0x84;
    if (0x89b79f < (int)psVar4) {
      return in_EAX & 0xffffff00;
    }
  } while( true );
}
