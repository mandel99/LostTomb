#include "restore_types.h"
#define DAT_00b350dc (*(undefined4 *)0x00b350dcu)
#define DAT_00bcd600 (*(undefined4 *)0x00bcd600u)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd618 (*(undefined4 *)0x00bcd618u)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)

__declspec(noinline) int FUN_004b56d0(int param_1)

{
  byte bVar1;
  byte cached_state;
  short sVar2;
  int iVar3;
  int iVar4;
  
  sVar2 = *(volatile short *)(0x00b350dcu +
       (unsigned int)*(volatile int *)(0x00bcd618u + (unsigned int)param_1 * 0x184u) * 2u);
  do {
    iVar3 = (int)sVar2;
    if (iVar3 < 1) {
      return 0;
    }
    iVar4 = iVar3 * 0x184;
    cached_state = *(volatile byte *)(0x00bcd630u + (unsigned int)iVar4);
    if (cached_state != '\x04') {
      bVar1 = *(volatile byte *)(0x00bcd602u + (unsigned int)iVar4);
      if ((0x2a < bVar1) && (bVar1 < 0x3a)) {
        return iVar3;
      }
      if ((bVar1 == 0x29) && (cached_state == '\v')) {
        return iVar3;
      }
      if (bVar1 == 0x52) {
        return iVar3;
      }
      if (bVar1 == 0x53) {
        return iVar3;
      }
      if (bVar1 == 0x68) {
        return iVar3;
      }
      if (bVar1 == 0x66) {
        return iVar3;
      }
      if (bVar1 == 0x67) {
        return iVar3;
      }
      if (bVar1 == 0x54) {
        return iVar3;
      }
      if (bVar1 == 0x6d) {
        return iVar3;
      }
      if (bVar1 == 99) {
        return iVar3;
      }
      if (bVar1 == 100) {
        return iVar3;
      }
      if (bVar1 == 0x65) {
        return iVar3;
      }
      if (bVar1 == 0x5d) {
        return iVar3;
      }
      if (bVar1 == 0x5c) {
        return iVar3;
      }
    }
    sVar2 = *(volatile short *)(0x00bcd600u + (unsigned int)iVar4);
  } while( true );
}
