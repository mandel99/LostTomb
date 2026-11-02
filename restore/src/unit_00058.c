#include "restore_types.h"
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd720 (*(undefined4 *)0x00bcd720u)
#define DAT_00bcd724 (*(undefined4 *)0x00bcd724u)
#define DAT_00bcd72a (*(undefined4 *)0x00bcd72au)
#define DAT_00d0570c (*(undefined *)0x00d0570cu)
#define DAT_00d3b198 (*(undefined *)0x00d3b198u)
#define DAT_00e38dca (*(byte *)0x00e38dcau)
#define DAT_00e38dda (*(char *)0x00e38ddau)
#define DAT_00e38e60 (*(undefined1 *)0x00e38e60u)
#define DAT_00eadcc4 (*(undefined *)0x00eadcc4u)
extern bool FUN_004a4300();
extern void FUN_004bc670();
extern int FUN_004cde60();

void FUN_00406de0(void)

{
  int iVar1;
  char cVar2;
  int iVar3;
  int iVar4;
  
  iVar1 = DAT_00eadcc4;
  if ((0 < DAT_00eadcc4) && (DAT_00e38e60 == '\0')) {
    iVar4 = DAT_00eadcc4 * 0x184;
    iVar3 = FUN_004cde60(*(undefined4 *)(&DAT_00bcd720 + iVar4),
                         *(undefined4 *)(&DAT_00bcd724 + iVar4));
    if ((((&DAT_00bcd630)[iVar4] != '\r') && ((&DAT_00bcd630)[iVar4] != '\x0f')) || (0xf < iVar3)) {
      if ((DAT_00d3b198 == 4) && (cVar2 = FUN_004a4300(iVar1), cVar2 != '\0')) {
        return;
      }
      if (*(short *)(&DAT_00bcd72a + iVar4) < 0) {
        if (DAT_00d3b198 == 4) {
          DAT_00d0570c = 0;
          FUN_004bc670(iVar1);
        }
        DAT_00e38dda = 1;
        DAT_00e38dca = 1;
      }
    }
  }
  return;
}
