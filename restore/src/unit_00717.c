#include "restore_types.h"
#define DAT_005d9a40 (*(undefined *)0x005d9a40u)
#define DAT_0078a634 (*(char *)0x0078a634u)
extern void FUN_0044bc20();
extern void FUN_0044c660();

void FUN_0044ba90(void)

{
  int *in_ECX;
  int *piVar1;
  int iVar2;
  
  iVar2 = 0;
  *(undefined1 *)((int)in_ECX + 0x48b5) = 0;
  *(undefined1 *)(in_ECX + 0x122d) = 0;
  DAT_0078a634 = 0;
  if (0 < *in_ECX) {
    piVar1 = in_ECX + 1;
    do {
      if ((*(byte *)(piVar1 + 10) & 1) == 0) {
        DAT_005d9a40 = iVar2;
        FUN_0044c660();
      }
      iVar2 = iVar2 + 1;
      piVar1 = piVar1 + 0x1f;
    } while (iVar2 < *in_ECX);
  }
  DAT_005d9a40 = 0xffffffff;
  FUN_0044bc20();
  return;
}
