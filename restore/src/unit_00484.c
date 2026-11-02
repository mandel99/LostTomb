#include "restore_types.h"
#define DAT_00a5d4cc (*(undefined4 *)0x00a5d4ccu)

undefined4 FUN_004339e0(int param_1)

{
  byte bVar1;
  
  bVar1 = (&DAT_00a5d4cc)[param_1];
  if (bVar1 == 0) {
    return 0;
  }
  if (bVar1 < 10) {
    return 1;
  }
  if (bVar1 < 0x14) {
    return 2;
  }
  if (bVar1 < 0x1e) {
    return 3;
  }
  if (bVar1 < 0x28) {
    return 4;
  }
  if (bVar1 < 0x32) {
    return 5;
  }
  if (bVar1 < 0x3c) {
    return 6;
  }
  if (bVar1 < 0x46) {
    return 7;
  }
  if (bVar1 < 0x50) {
    return 8;
  }
  return 9;
}
