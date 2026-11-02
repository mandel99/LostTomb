#include "restore_types.h"
extern int FUN_004cde60();

undefined1 FUN_0051cd80(int param_1,int param_2,int param_3,int param_4)

{
  int iVar1;
  int iVar2;
  
  if (param_3 < param_1) {
    iVar2 = param_1 - param_3;
  }
  else {
    iVar2 = param_3 - param_1;
  }
  if (param_4 < param_2) {
    iVar1 = param_2 - param_4;
  }
  else {
    iVar1 = param_4 - param_2;
  }
  if (iVar1 < iVar2) {
    iVar2 = FUN_004cde60(iVar2,iVar1);
  }
  else if (iVar2 < iVar1) {
    iVar2 = FUN_004cde60(iVar1,iVar2);
    iVar2 = -iVar2;
  }
  else {
    iVar2 = 100;
  }
  if (param_1 == param_3) {
    if (param_4 < param_2) {
      return false;
    }
    if (param_2 < param_4) {
      return 4;
    }
  }
  if (param_3 < param_1) {
    if (param_2 == param_4) {
      return 6;
    }
    if (param_4 < param_2) {
      if (399 < iVar2) {
        return 6;
      }
      return (iVar2 < -399) - 1U & 7;
    }
    if (399 < iVar2) {
      return 6;
    }
    return (-400 < iVar2) + '\x04';
  }
  if (param_3 <= param_1) {
    return false;
  }
  if (param_2 == param_4) {
    return 2;
  }
  if (param_2 <= param_4) {
    if (399 < iVar2) {
      return 2;
    }
    return (iVar2 < -399) + '\x03';
  }
  if (399 < iVar2) {
    return 2;
  }
  return -400 < iVar2;
}
