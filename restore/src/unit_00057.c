#include "restore_types.h"
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd720 (*(undefined4 *)0x00bcd720u)
#define DAT_00bcd724 (*(undefined4 *)0x00bcd724u)
#define DAT_00bcd72a (*(undefined4 *)0x00bcd72au)
#define DAT_00bcd754 (*(undefined4 *)0x00bcd754u)
#define DAT_00d05704 (*(undefined *)0x00d05704u)
#define DAT_00d0570c (*(undefined *)0x00d0570cu)
#define DAT_00d3b198 (*(undefined *)0x00d3b198u)
#define DAT_00e38dca (*(byte *)0x00e38dcau)
#define DAT_00e38dda (*(char *)0x00e38ddau)
#define DAT_00e38e60 (*(undefined1 *)0x00e38e60u)
#define DAT_00eadcc4 (*(undefined *)0x00eadcc4u)
extern void FUN_004a45d0();
extern int FUN_004cde60();

void FUN_00406c90(void)

{
  char cVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  
  iVar2 = DAT_00eadcc4;
  if ((DAT_00e38e60 == '\0') && (DAT_00d05704 = DAT_00eadcc4, 0 < DAT_00eadcc4)) {
    iVar4 = DAT_00eadcc4 * 0x184;
    iVar3 = FUN_004cde60(*(undefined4 *)(&DAT_00bcd720 + iVar4),
                         *(undefined4 *)(&DAT_00bcd724 + iVar4));
    cVar1 = (&DAT_00bcd630)[iVar4];
    if ((((cVar1 != '\r') && (cVar1 != '\x0f')) || (0xf < iVar3)) &&
       (((DAT_00d3b198 != 3 || (((cVar1 != '\r' && (cVar1 != '\x0f')) && (iVar3 != 100)))) &&
        (*(short *)(&DAT_00bcd72a + iVar4) < 0)))) {
      switch((int)(DAT_00d3b198)) {
      case 0: ;
        (&DAT_00bcd630)[iVar4] = 0x14;
        DAT_00d0570c = 1;
        DAT_00e38dda = 0x20;
        DAT_00e38dca = 1;
        return;
      case 1: ;
        (&DAT_00bcd630)[iVar4] = 0x17;
        *(undefined4 *)(&DAT_00bcd754 + iVar4) = 0x16;
        DAT_00d0570c = 1;
        DAT_00e38dda = 0x20;
        DAT_00e38dca = 1;
        return;
      case 2: ;
        (&DAT_00bcd630)[iVar4] = 10;
        *(undefined4 *)(&DAT_00bcd754 + iVar4) = 0x10;
        DAT_00d0570c = 0;
        DAT_00e38dda = 1;
        DAT_00e38dca = 1;
        return;
      case 3: ;
        DAT_00d0570c = 0;
        FUN_004a45d0(iVar2);
        DAT_00e38dda = 0x20;
        DAT_00e38dca = 1;
      }
    }
  }
  return;
}
