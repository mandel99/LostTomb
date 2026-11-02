#include "restore_types.h"
#define DAT_0126672c (*(undefined *)0x0126672cu)
#define DAT_01266840 (*(undefined4 *)0x01266840u)
extern uint * FUN_005685e0();

byte * FUN_0056b104(byte *param_1,uint param_2)

{
  ushort uVar1;
  byte *pbVar2;
  
  if (DAT_0126672c == 0) {
    pbVar2 = (byte *)FUN_005685e0(param_1,param_2);
    return pbVar2;
  }
  while( true ) {
    uVar1 = (ushort)*param_1;
    if (uVar1 == 0) break;
    if ((*(byte *)((int)&DAT_01266840 + uVar1 + 1) & 4) == 0) {
      pbVar2 = param_1;
      if (param_2 == uVar1) break;
    }
    else {
      pbVar2 = param_1 + 1;
      if (param_1[1] == 0) {
        return (byte *)0x0;
      }
      if (param_2 == CONCAT11(*param_1,param_1[1])) {
        return param_1;
      }
    }
    param_1 = pbVar2 + 1;
  }
  return (byte *)(~-(uint)(param_2 != uVar1) & (uint)param_1);
}
