#include "restore_types.h"
#define DAT_00e92e1c (*(uint32_t *)0x00e92e1cu)
#define DAT_01265188 (*(uint32_t *)0x01265188u)
#define DAT_0126518c (*(int32_t *)0x0126518cu)
#define DAT_01265194 (*(undefined4 *)0x01265194u)
#define _DAT_01265190 (*(undefined4 *)0x01265190u)
extern undefined4 FUN_0055c000();
extern int FUN_005660ed();

undefined4 FUN_0055bcf0(undefined4 param_1,undefined4 *param_2,uint param_3)

{
  int iVar1;
  uint uVar2;
  
  DAT_01265188 = DAT_00e92e1c;
  if (1200000 < (int)param_3) {
    return 0;
  }
  _DAT_01265190 = param_3;
  iVar1 = FUN_005660ed(param_1,&DAT_0126518c,4);
  if (iVar1 == 0) {
    return 0;
  }
  if (DAT_0126518c == -0x80000000) {
    iVar1 = FUN_005660ed(param_1,param_2,param_3);
    if (iVar1 == 0) {
      return 0;
    }
  }
  else if (DAT_0126518c == 0) {
    for (uVar2 = param_3 >> 2; uVar2 != 0; uVar2 = uVar2 - 1) {
      *param_2 = 0;
      param_2 = param_2 + 1;
    }
    for (param_3 = param_3 & 3; param_3 != 0; param_3 = param_3 - 1) {
      *(undefined1 *)param_2 = 0;
      param_2 = (undefined4 *)((int)param_2 + 1);
    }
  }
  else {
    iVar1 = FUN_005660ed(param_1,DAT_01265188,DAT_0126518c);
    if (iVar1 == 0) {
      return 0;
    }
    iVar1 = FUN_0055c000(&DAT_01265194,DAT_01265188,DAT_0126518c,param_2,param_3);
    if (iVar1 == 0) {
      return 0;
    }
  }
  return 1;
}
