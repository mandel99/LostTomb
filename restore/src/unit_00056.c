#include "restore_types.h"
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd720 (*(undefined4 *)0x00bcd720u)
#define DAT_00bcd724 (*(undefined4 *)0x00bcd724u)
#define DAT_00d05700 (*(undefined *)0x00d05700u)
#define DAT_00d3b198 (*(undefined *)0x00d3b198u)
#define DAT_00e38e60 (*(undefined1 *)0x00e38e60u)
#define DAT_00eadcc8 (*(undefined *)0x00eadcc8u)
#define PTR_LAB_00406c7c (*(unsigned char * *)0x00406c7cu)
extern int FUN_004cde60();

void FUN_00406b40(void)

{
  char cVar1;
  int iVar2;
  int iVar3;
  bool bVar4;
  bool bVar5;
  
  DAT_00d05700 = DAT_00eadcc8;
  if ((0 < DAT_00eadcc8) && (DAT_00e38e60 == '\0')) {
    iVar3 = DAT_00eadcc8 * 0x184;
    iVar2 = FUN_004cde60(*(undefined4 *)(&DAT_00bcd720 + iVar3),
                         *(undefined4 *)(&DAT_00bcd724 + iVar3));
    cVar1 = (&DAT_00bcd630)[iVar3];
    if (((cVar1 != '\v') && (cVar1 != '\x0f')) || (0xf < iVar2)) {
      bVar4 = DAT_00d3b198 < 3;
      bVar5 = false;
      if (DAT_00d3b198 == 3) {
        if (cVar1 == '\v') {
          return;
        }
        if (cVar1 == '\x0f') {
          return;
        }
        if (iVar2 == 100) {
          return;
        }
        bVar4 = false;
        bVar5 = true;
      }
      if (bVar4 || bVar5) {
                    /* WARNING: Could not recover jumptable at 0x00406bcb. Too many branches */
                    /* WARNING: Treating indirect jump as call */
        (*(code *)(&PTR_LAB_00406c7c)[DAT_00d3b198])();
        return;
      }
    }
  }
  return;
}
