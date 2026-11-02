#include "restore_types.h"
#define DAT_00bcba4c (*(undefined4 *)0x00bcba4cu)
#define DAT_00bcd602 (*(uint8_t *)0x00bcd602u)
#define DAT_00bcd78a (*(uint8_t *)0x00bcd78au)

void FUN_004d3d00(void)

{
  char cVar1;
  char *pcVar2;
  int *piVar3;
  int iVar4;
  
  piVar3 = &DAT_00bcba4c;
  do {
    cVar1 = (&DAT_00bcd602)[*piVar3 * 0x184];
    if (((cVar1 != 'M') && (cVar1 != '\\')) && (cVar1 != 'e')) {
      *piVar3 = 0;
    }
    piVar3 = piVar3 + 0x24;
  } while ((int)piVar3 < 0xbcd66c);
  iVar4 = 1;
  pcVar2 = (char *)&DAT_00bcd78a;
  do {
    if (((pcVar2[-4] == 'M') && (*pcVar2 == '\x01')) &&
       ((pcVar2[0x2a] != '\x04' && (pcVar2[0x122] != '\0')))) {
      (&DAT_00bcba4c)[(uint)(byte)pcVar2[0x84] * 0x24] = iVar4;
    }
    pcVar2 = pcVar2 + 0x184;
    iVar4 = iVar4 + 1;
  } while ((int)pcVar2 < 0xc8ad46);
  return;
}
