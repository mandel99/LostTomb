#include "restore_types.h"
#define DAT_0078d7bc (*(undefined *)0x0078d7bcu)
#define _DAT_007848d8 (*(undefined4 *)0x007848d8u)
extern void FUN_00448a90();

void FUN_0044bc20(void)

{
  uint uVar1;
  char cVar2;
  int *in_ECX;
  uint *puVar3;
  int iVar4;
  
  iVar4 = 0;
  if (0 < *in_ECX) {
    puVar3 = (uint *)(in_ECX + 0xb);
    do {
      uVar1 = *puVar3;
      if (((uVar1 >> 2 & 1) != 0) && (*(char *)((int)puVar3 + 10) == '\0')) {
        if ((uVar1 & 2) == 0) {
          if (((uVar1 >> 4 & 1) != 0) &&
             (cVar2 = *(char *)((int)puVar3 + -0x25) + -1, *(char *)((int)puVar3 + -0x25) = cVar2,
             cVar2 == '\0')) {
            *puVar3 = *puVar3 & 0xfffffffb;
            FUN_00448a90();
          }
        }
        else {
          *puVar3 = uVar1 & 0xfffffffb;
          FUN_00448a90();
          *(short *)(puVar3 + -5) = (short)puVar3[-5] + ((short)DAT_0078d7bc - (short)_DAT_007848d8)
          ;
        }
      }
      iVar4 = iVar4 + 1;
      puVar3 = puVar3 + 0x1f;
    } while (iVar4 < *in_ECX);
  }
  return;
}
