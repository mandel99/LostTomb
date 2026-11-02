#include "restore_types.h"
#define DAT_00e38dca (*(byte *)0x00e38dcau)
#define DAT_00e38e00 (*(char *)0x00e38e00u)
#define DAT_00e38e30 (*(undefined *)0x00e38e30u)
#define DAT_00e38e34 (*(undefined *)0x00e38e34u)
#define DAT_00e950f4 (*(undefined *)0x00e950f4u)
#define DAT_00e950f8 (*(undefined *)0x00e950f8u)
#define DAT_00e950fc (*(undefined *)0x00e950fcu)
#define DAT_00e95100 (*(undefined *)0x00e95100u)
#define DAT_00e95114 (*(undefined *)0x00e95114u)
#define DAT_00e95118 (*(undefined *)0x00e95118u)
#define DAT_00e9511c (*(undefined *)0x00e9511cu)
#define DAT_00e95120 (*(undefined *)0x00e95120u)
extern int FUN_004cde40();
extern int FUN_004cde60();

undefined4 FUN_004e2090(void)

{
  int iVar1;
  undefined4 uVar2;
  int iVar3;
  
  if (DAT_00e9511c < 1) {
    return 0;
  }
  if (DAT_00e38e00 == '\0') {
    return 0;
  }
  iVar3 = DAT_00e950f8 * 0x10 + DAT_00e95100;
  iVar1 = DAT_00e950f4 * 0x10 + -0x34;
  if (DAT_00e38e34 < iVar3 + 1) {
    return 0;
  }
  if (iVar3 + 0x29 < DAT_00e38e34) {
    return 0;
  }
  if (DAT_00e38e30 < DAT_00e950fc + 0x1a) {
    return 0;
  }
  if (DAT_00e950fc + 0x1a + iVar1 < DAT_00e38e30) {
    return 0;
  }
  iVar3 = (DAT_00e38e30 - DAT_00e950fc) + -0xb;
  if (iVar1 < iVar3) {
    iVar3 = iVar1;
  }
  uVar2 = FUN_004cde60(iVar3,iVar1);
  DAT_00e95120 = FUN_004cde40(DAT_00e9511c,uVar2);
  DAT_00e95114 = iVar3 + -0x19;
  DAT_00e95118 = 1;
  if (DAT_00e95114 < 0) {
    DAT_00e95114 = 0;
  }
  DAT_00e38dca = 1;
  return 1;
}
