#include "restore_types.h"
#define DAT_012666a8 (*(undefined4 *)0x012666a8u)
#define DAT_012666aa (*(undefined4 *)0x012666aau)
#define DAT_012666ac (*(undefined4 *)0x012666acu)
#define DAT_012666c8 (*(undefined4 *)0x012666c8u)
#define _DAT_012666c8 (*(undefined4 *)0x012666c8u)
#define _DAT_012666cc (*(undefined4 *)0x012666ccu)
#define _DAT_012666d0 (*(undefined4 *)0x012666d0u)
#define _DAT_012666d4 (*(undefined4 *)0x012666d4u)
extern void FUN_00568dec();
extern undefined4 FUN_0056a47e();
#define stack0x00000004 (restore_stack[32772])

undefined * FUN_00568d88(void)

{
  unsigned char restore_stack[65536];
  undefined4 local_10;
  undefined4 uStack_c;
  undefined2 uStack_8;
  
  FUN_00568dec(&local_10,&stack0x00000004);
  _DAT_012666d0 = FUN_0056a47e(local_10,uStack_c,uStack_8,0x11,0,&DAT_012666a8);
  _DAT_012666c8 = (int)DAT_012666aa;
  _DAT_012666cc = (int)DAT_012666a8;
  _DAT_012666d4 = &DAT_012666ac;
  return &DAT_012666c8;
}
