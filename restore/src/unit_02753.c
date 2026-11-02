#include "restore_types.h"
#define DAT_005d9a18 (*(undefined4 *)0x005d9a18u)
#define DAT_005d9a1a (*(undefined4 *)0x005d9a1au)
extern undefined4 FUN_00560bfd();
extern int FUN_00560c26();
extern void FUN_00560d96();
extern undefined4 FUN_00561a36();

void FUN_00560b4c(int param_1)

{
  uint uVar1;
  byte bVar2;
  int iVar3;
  int *in_ECX;
  undefined1 local_18 [12];
  undefined2 local_c;
  undefined1 uStack_a;
  undefined1 local_9;
  ushort local_8;
  undefined1 uStack_6;
  undefined1 local_5;
  
  local_c = DAT_005d9a18;
  uStack_a = DAT_005d9a1a;
  local_9 = 0;
  local_8 = 0;
  uStack_6 = 0;
  local_5 = 0;
  iVar3 = FUN_00560bfd();
  if (iVar3 != 0) {
    if (param_1 != 0) {
      uVar1 = *(uint *)((int)in_ECX + *(int *)(*in_ECX + 4) + 0x24);
      if ((uVar1 & 0x60) == 0) {
        if ((0 < param_1) && ((uVar1 & 0x400) != 0)) {
          local_8 = CONCAT11(RESTORE_SLICE1(local_8,1),0x2b);
        }
      }
      else {
        if ((uVar1 & 0x40) == 0) {
          local_c = CONCAT11(0x6f,(undefined1)local_c);
        }
        else {
          bVar2 = ~(byte)(uVar1 >> 4);
          local_c = CONCAT11(bVar2,(undefined1)local_c) & 0x20ff | 0x5800;
          local_8 = CONCAT11(bVar2,(undefined1)local_8) & 0x20ff | 0x5800;
        }
        if ((*(byte *)((int)in_ECX + *(int *)(*in_ECX + 4) + 0x24) & 0x80) != 0) {
          local_8 = CONCAT11(RESTORE_SLICE1(local_8,1),0x30);
        }
      }
    }
    FUN_00561a36(local_18,&local_c,param_1);
    FUN_00560d96(&local_8,local_18);
    FUN_00560c26();
  }
  return;
}
