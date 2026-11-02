#include "restore_types.h"
#define DAT_00bcd609 (*(undefined4 *)0x00bcd609u)
#define DAT_00bcd60c (*(undefined4 *)0x00bcd60cu)
#define DAT_00bcd60e (*(undefined4 *)0x00bcd60eu)
#define DAT_00bcd614 (*(undefined4 *)0x00bcd614u)
#define DAT_00bcd61c (*(undefined4 *)0x00bcd61cu)
#define DAT_00bcd61e (*(undefined4 *)0x00bcd61eu)
#define DAT_00bcd642 (*(undefined4 *)0x00bcd642u)
#define DAT_00bcd644 (*(undefined4 *)0x00bcd644u)
#define DAT_00bcd652 (*(undefined4 *)0x00bcd652u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern void FUN_004b4220();
extern int FUN_004b4800();
extern void FUN_004b5210();
extern ushort FUN_0051cae0();

int FUN_004b45c0(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined2 param_4,
                short param_5,int param_6)

{
  undefined1 uVar1;
  int iVar2;
  int iVar3;
  
  iVar2 = FUN_004b4800(DAT_00eaac28,param_6,param_2,param_3,0);
  if (iVar2 != 0) {
    FUN_004b5210(param_1);
    if (param_6 == 0x3d) {
      (&DAT_00bcd614)[iVar2 * 0x184] = 0x3c;
    }
    else {
      (&DAT_00bcd614)[iVar2 * 0x184] = 10;
    }
    iVar3 = iVar2 * 0x184;
    (&DAT_00bcd652)[iVar2 * 0xc2] = (short)param_1;
    *(undefined2 *)(&DAT_00bcd61c + iVar3) = param_4;
    *(short *)(&DAT_00bcd61e + iVar3) = param_5;
    FUN_004b4220(iVar2,(int)(short)(&DAT_00bcd642)[iVar2 * 0xc2],
                 (int)(short)(&DAT_00bcd644)[iVar2 * 0xc2],*(short *)(&DAT_00bcd61c + iVar3) * 0xf,
                 param_5 * 0xf,1);
    uVar1 = FUN_0051cae0((int)(short)(&DAT_00bcd60c)[iVar2 * 0xc2],
                         (int)(short)(&DAT_00bcd60e)[iVar2 * 0xc2],
                         (int)*(short *)(&DAT_00bcd61c + iVar3),
                         (int)*(short *)(&DAT_00bcd61e + iVar3));
    (&DAT_00bcd609)[iVar3] = uVar1;
    return iVar2;
  }
  return 0;
}
