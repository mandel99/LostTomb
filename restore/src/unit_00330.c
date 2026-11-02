#include "restore_types.h"
#define DAT_007999e4 (*(undefined4 *)0x007999e4u)
#define DAT_007999fe (*(undefined4 *)0x007999feu)
#define DAT_00799a00 (*(undefined4 *)0x00799a00u)
#define DAT_00799af0 (*(undefined4 *)0x00799af0u)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd61c (*(undefined4 *)0x00bcd61cu)
#define DAT_00bcd61e (*(undefined4 *)0x00bcd61eu)
#define DAT_00bcd620 (*(undefined4 *)0x00bcd620u)
#define DAT_00bcd63c (*(undefined4 *)0x00bcd63cu)
#define DAT_00ea5ba8 (*(undefined4 *)0x00ea5ba8u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define PTR_DAT_005c2750 (*(unsigned char * *)0x005c2750u)

int FUN_00420350(undefined4 param_1,undefined4 param_2,int param_3)

{
  undefined4 uVar1;
  undefined **ppuVar2;
  int iVar3;
  short *psVar4;
  
  ppuVar2 = &PTR_DAT_005c2750;
  while ((((*(short *)(*ppuVar2 + 0x12) != 0 ||
           (ppuVar2[-2] !=
            (undefined *)(int)*(short *)((int)&DAT_00ea5ba8 + DAT_00eaac28 * 0x49d8 + 2))) ||
          (ppuVar2[-1] != (undefined *)(uint)(byte)(&DAT_00bcd602)[param_3 * 0x184])) ||
         (*(int *)(*ppuVar2 + 0xc) < 1))) {
    ppuVar2 = ppuVar2 + 4;
    if (0x5c28af < (int)ppuVar2) {
      return 0;
    }
  }
  psVar4 = &DAT_00799af0;
  iVar3 = 1;
  while (((char)psVar4[-8] != '\x01' || (*psVar4 != 0xd2))) {
    psVar4 = psVar4 + 0x84;
    iVar3 = iVar3 + 1;
    if (0x89b6e7 < (int)psVar4) {
      return 0;
    }
  }
  param_3 = param_3 * 0x184;
  *(undefined2 *)(&DAT_00bcd61c + param_3) = (&DAT_007999fe)[iVar3 * 0x84];
  uVar1 = *(undefined4 *)(&DAT_007999e4 + iVar3 * 0x108);
  *(undefined2 *)(&DAT_00bcd61e + param_3) = (&DAT_00799a00)[iVar3 * 0x84];
  *(undefined4 *)(&DAT_00bcd620 + param_3) = uVar1;
  *(undefined2 *)(&DAT_00bcd63c + param_3) = 0;
  return iVar3;
}
