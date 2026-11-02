#include "restore_types.h"
#define DAT_00bcd790 (*(undefined4 *)0x00bcd790u)

undefined4 FUN_004b5830(int param_1,int param_2,int param_3,int param_4)

{
  byte bVar1;
  int iVar2;
  int iVar3;
  short *psVar4;
  int iVar5;
  int iVar6;
  
  psVar4 = &DAT_00bcd790;
  do {
    if (((char)psVar4[-3] == '\x01') &&
       (((bVar1 = *(byte *)(psVar4 + -5), 0x2a < bVar1 && (bVar1 < 0x3a)) || (bVar1 == 99)))) {
      iVar3 = (int)*psVar4;
      if (param_1 < iVar3) {
        iVar6 = iVar3 - param_1;
      }
      else {
        iVar6 = param_1 - iVar3;
      }
      iVar2 = (int)psVar4[1];
      if (param_2 < iVar2) {
        iVar5 = iVar2 - param_2;
      }
      else {
        iVar5 = param_2 - iVar2;
      }
      if ((iVar6 < 0xd) && (iVar5 < 0xd)) {
        return 1;
      }
      if (param_3 < iVar3) {
        iVar3 = iVar3 - param_3;
      }
      else {
        iVar3 = param_3 - iVar3;
      }
      if (param_4 < iVar2) {
        iVar2 = iVar2 - param_4;
      }
      else {
        iVar2 = param_4 - iVar2;
      }
      if ((iVar3 < 0xd) && (iVar2 < 0xd)) {
        return 1;
      }
    }
    psVar4 = psVar4 + 0xc2;
    if (0xc8ad4b < (int)psVar4) {
      return 0;
    }
  } while( true );
}
