#include "restore_types.h"
#define DAT_00bcb9c0 (*(undefined *)0x00bcb9c0u)
#define DAT_00bcb9f2 (*(undefined4 *)0x00bcb9f2u)
#define DAT_00bcba3f (*(undefined4 *)0x00bcba3fu)
#define DAT_00d3b194 (*(undefined *)0x00d3b194u)
#define DAT_00d3b198 (*(undefined *)0x00d3b198u)
#define DAT_00e38e60 (*(undefined1 *)0x00e38e60u)
#define DAT_00eadccc (*(undefined *)0x00eadcccu)
extern void FUN_00406720();

void FUN_00407110(void)

{
  int iVar1;
  int iVar2;
  
  iVar1 = DAT_00bcb9c0;
  if ((DAT_00bcb9c0 == 0) || (iVar2 = DAT_00bcb9c0, DAT_00bcb9c0 != DAT_00eadccc)) {
    iVar2 = DAT_00eadccc;
  }
  if (DAT_00e38e60 == '\0') {
    iVar2 = iVar2 * 0x90;
    (&DAT_00bcba3f)[iVar2] = '\x01' - (&DAT_00bcba3f)[iVar2];
    switch(*(undefined2 *)(&DAT_00bcb9f2 + iVar2)) {
    case 1: ;
    case 2: ;
      DAT_00d3b198 = 0;
      break;
    case 3: ;
    case 4: ;
      DAT_00d3b198 = 1;
      break;
    case 0xf: ;
      DAT_00d3b198 = 2;
    }
    if (DAT_00d3b194 == 1) {
      DAT_00eadccc = iVar1;
      FUN_00406720();
      return;
    }
  }
  return;
}
