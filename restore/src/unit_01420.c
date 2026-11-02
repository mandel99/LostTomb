#include "restore_types.h"
#define DAT_005f80fc (*(undefined4 *)0x005f80fcu)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bcd609 (*(undefined4 *)0x00bcd609u)
#define DAT_00bcd60c (*(undefined4 *)0x00bcd60cu)
#define DAT_00bcd60e (*(undefined4 *)0x00bcd60eu)
#define DAT_00bcd61c (*(undefined4 *)0x00bcd61cu)
#define DAT_00bcd61e (*(undefined4 *)0x00bcd61eu)
#define DAT_00bcd640 (*(undefined4 *)0x00bcd640u)
#define DAT_00bcd641 (*(undefined4 *)0x00bcd641u)
extern undefined4 FUN_0051cba0();

void FUN_004b21d0(int param_1)

{
  uint uVar1;
  uint uVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  
  iVar5 = param_1 * 0x184;
  iVar6 = (int)(short)(&DAT_00bcd60c)[param_1 * 0xc2] +
          (short)(&DAT_00bcd60e)[param_1 * 0xc2] * 0xe4 + DAT_00bca2d8;
  uVar1 = FUN_0051cba0((int)(short)(&DAT_00bcd60c)[param_1 * 0xc2],
                       (int)(short)(&DAT_00bcd60e)[param_1 * 0xc2],
                       (int)*(short *)(&DAT_00bcd61c + iVar5),(int)*(short *)(&DAT_00bcd61e + iVar5)
                      );
  uVar1 = uVar1 & 0xff;
  if (7 < uVar1) {
    uVar1 = 0;
  }
  iVar3 = 0;
  iVar4 = 0;
  uVar2 = uVar1;
  do {
    if (((uVar2 & 1) == 0) &&
       ((*(byte *)(&DAT_00af598c + (&DAT_005f80fc)[uVar2] + iVar6) & 0x40) != 0)) goto LAB_004b226e;
    uVar2 = uVar2 + 1;
    if (7 < (int)uVar2) {
      uVar2 = 0;
    }
    iVar4 = iVar4 + 1;
    iVar3 = iVar3 + 1;
  } while (iVar3 < 8);
  uVar2 = 0;
LAB_004b226e: ;
  iVar3 = 0;
  do {
    if (((uVar1 & 1) == 0) &&
       ((*(byte *)(&DAT_00af598c + (&DAT_005f80fc)[uVar1] + iVar6) & 0x40) != 0)) goto LAB_004b229b;
    uVar1 = uVar1 - 1;
    if ((int)uVar1 < 0) {
      uVar1 = 7;
    }
    iVar3 = iVar3 + 1;
  } while (iVar3 < 8);
  uVar1 = 4;
LAB_004b229b: ;
  if (iVar4 <= iVar3) {
    (&DAT_00bcd609)[iVar5] = (char)uVar2;
    (&DAT_00bcd640)[iVar5] = 2;
    (&DAT_00bcd641)[iVar5] = 5;
    return;
  }
  (&DAT_00bcd609)[iVar5] = (char)uVar1;
  (&DAT_00bcd640)[iVar5] = 0xfe;
  (&DAT_00bcd641)[iVar5] = 5;
  return;
}
