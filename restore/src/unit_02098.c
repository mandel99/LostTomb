#include "restore_types.h"
#define DAT_00799a3c (*(undefined4 *)0x00799a3cu)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd606 (*(undefined4 *)0x00bcd606u)
#define DAT_00bcd608 (*(undefined4 *)0x00bcd608u)
#define DAT_00bcd620 (*(undefined4 *)0x00bcd620u)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
extern undefined4 FUN_004ef100();

uint FUN_004eea10(int param_1)

{
  short sVar1;
  uint uVar2;
  uint uVar3;
  char cVar4;
  int iVar5;
  
  sVar1 = (&DAT_00799a3c)[param_1 * 0x84];
  uVar2 = 0;
  if (sVar1 == 0) goto switchD_004eea79_caseD_3;
  iVar5 = sVar1 * 0x184;
  uVar2 = CONCAT31((int3)(char)((ushort)sVar1 >> 8),(&DAT_00bcd606)[iVar5]);
  if ((&DAT_00bcd606)[iVar5] == '\0') {
LAB_004eeac2: ;
    (&DAT_00799a3c)[param_1 * 0x84] = 0;
  }
  else {
    cVar4 = (&DAT_00bcd630)[iVar5];
    if ((cVar4 == '\x06') || (cVar4 == '\a')) {
      cVar4 = (&DAT_00bcd608)[iVar5];
    }
    uVar2 = (byte)(&DAT_00bcd602)[iVar5] - 0x4f;
    switch((int)(uVar2)) {
    case 0: ;
      uVar3 = uVar2;
      if (((cVar4 == '\t') || (cVar4 == '\f')) || (cVar4 == '\n')) {
LAB_004eeadd: ;
        return CONCAT31((int3)(uVar3 >> 8),1);
      }
      break;
    case 1: ;
      if (((cVar4 == '\n') || (cVar4 == '\r')) ||
         ((cVar4 == '\x0f' || (((cVar4 == '\f' || (cVar4 == '\x0e')) || (cVar4 == '\v')))))) {
LAB_004eeb3b: ;
        return CONCAT31((int3)(uVar2 >> 8),1);
      }
      break;
    case 2: ;
      uVar2 = FUN_004ef100(param_1);
      if (uVar2 == 0) {
        uVar3 = 0;
        if (((((cVar4 == '\t') || (cVar4 == '\r')) || (cVar4 == '\x0f')) ||
            ((cVar4 == '\v' || (cVar4 == '\x0e')))) || (cVar4 == '\x10')) goto LAB_004eeadd;
        if (cVar4 != '\n') break;
        uVar2 = *(uint *)(&DAT_00bcd620 + iVar5);
        if (*(short *)(&DAT_00b4e6fc + uVar2 * 2) == param_1) goto LAB_004eeb3b;
      }
      goto LAB_004eeac2;
    case 6: ;
      if (((((cVar4 == '\x0f') || (cVar4 == '\x10')) || (cVar4 == '\x11')) ||
          ((((cVar4 == '\x1e' || (cVar4 == '\x12')) ||
            ((cVar4 == '\x13' || ((cVar4 == '\x14' || (cVar4 == '\x15')))))) || (cVar4 == '\x1d'))))
         || (cVar4 == '\x16')) {
        return CONCAT31((int3)(uVar2 >> 8),1);
      }
    }
  }
switchD_004eea79_caseD_3: ;
  return uVar2 & 0xffffff00;
}
