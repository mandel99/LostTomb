#include "restore_types.h"
#define DAT_00799a2e (*(undefined4 *)0x00799a2eu)

uint FUN_00496550(undefined4 param_1,int param_2,int param_3,int param_4,int param_5,int param_6)

{
  uint uVar1;
  short sVar2;
  uint uVar3;
  int iVar4;
  
  iVar4 = 800;
  if (param_6 != 0) {
    iVar4 = 600;
  }
  param_5 = (iVar4 - (short)(&DAT_00799a2e)[param_6 + param_3 * 0x84]) - param_5;
  if (param_5 < 1) {
    return 0;
  }
  uVar1 = param_5 / 100 + 1;
  sVar2 = (&DAT_00799a2e)[param_2 + param_4 * 0x84];
  if (sVar2 < 800) {
    if (sVar2 < 700) {
      if (sVar2 < 600) {
        if (sVar2 < 500) {
          if (sVar2 < 400) {
            if (sVar2 < 300) {
              if (sVar2 < 200) {
                if (sVar2 < 100) {
                  uVar3 = (uint)(0 < sVar2);
                }
                else {
                  uVar3 = 1;
                }
              }
              else {
                uVar3 = 2;
              }
            }
            else {
              uVar3 = 3;
            }
          }
          else {
            uVar3 = 4;
          }
        }
        else {
          uVar3 = 5;
        }
      }
      else {
        uVar3 = 6;
      }
    }
    else {
      uVar3 = 7;
    }
  }
  else {
    uVar3 = 8;
  }
  if ((int)uVar1 < (int)uVar3) {
    uVar3 = uVar1;
  }
  return uVar3 & ((int)uVar3 < 1) - 1;
}
