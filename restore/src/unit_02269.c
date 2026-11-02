#include "restore_types.h"
#define DAT_006fdc88 (*(undefined *)0x006fdc88u)
#define DAT_00e38dc8 (*(char *)0x00e38dc8u)
#define DAT_00e38dc9 (*(char *)0x00e38dc9u)
#define DAT_00eadce8 (*(undefined4 *)0x00eadce8u)
#define DAT_00eadd84 (*(undefined *)0x00eadd84u)
#define DAT_00eadd8c (*(undefined *)0x00eadd8cu)
#define DAT_00eadd90 (*(undefined *)0x00eadd90u)
#define PTR_DAT_005f78f0 (*(unsigned char * *)0x005f78f0u)
extern void FUN_00425730();

void FUN_00515bf0(void)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  iVar3 = DAT_00eadd84 * 0x10;
  iVar2 = 0;
  DAT_00e38dc9 = 1;
  iVar1 = 0;
  do {
    if (((&DAT_00eadce8)[iVar2] != 0) && ((&DAT_006fdc88)[iVar2] != '\0')) {
      FUN_00425730(iVar1 + 0x28 + DAT_00eadd90,DAT_00eadd8c + iVar3 + -0x28,
                   (&PTR_DAT_005f78f0)[iVar2],1);
      iVar1 = iVar1 + 0x1b;
    }
    iVar2 = iVar2 + 1;
  } while (iVar2 < 0xf);
  DAT_00e38dc8 = 0;
  return;
}
