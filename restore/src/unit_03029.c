#include "restore_types.h"
#define DAT_01266710 (*(undefined *)0x01266710u)
#define DAT_01266740 (*(undefined4 *)0x01266740u)
#define DAT_01266840 (*(undefined4 *)0x01266840u)
#define DAT_01266944 (*(undefined *)0x01266944u)
extern int FUN_0056947e();

uint FUN_0056b363(uint param_1)

{
  uint uVar1;
  uint uVar2;
  int iVar3;
  uint uVar4;
  undefined1 uVar5;
  undefined2 local_8;
  
  uVar4 = param_1;
  if (param_1 < 0x100) {
    if ((*(byte *)((int)&DAT_01266840 + param_1 + 1) & 0x20) == 0x20) {
      uVar4 = (uint)(byte)(&DAT_01266740)[param_1];
    }
  }
  else {
    uVar5 = (undefined1)param_1;
    uVar2 = param_1 >> 8;
    uVar1 = param_1 >> 8;
    param_1 = CONCAT13(uVar5,CONCAT12((char)uVar1,(undefined2)param_1));
    if (((*(byte *)((int)&DAT_01266840 + (uVar2 & 0xff) + 1) & 4) != 0) &&
       (iVar3 = FUN_0056947e(DAT_01266944,0x200,(int)&param_1 + 2,2,&local_8,2,DAT_01266710,1),
       iVar3 != 0)) {
      uVar4 = (uint)CONCAT11((undefined1)local_8,RESTORE_SLICE1(local_8,1));
    }
  }
  return uVar4;
}
