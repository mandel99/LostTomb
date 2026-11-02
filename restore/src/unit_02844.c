#include "restore_types.h"
#define LAB_00561a90 (*(undefined1 *)0x00561a90u)
#define DAT_00570620 (*(undefined4 *)0x00570620u)
#define DAT_01266400 (*(undefined *)0x01266400u)
#define DAT_01267aa4 (*(undefined *)0x01267aa4u)
#define _DAT_0126643c (*(undefined4 *)0x0126643cu)
#define _DAT_01266440 (*(undefined4 *)0x01266440u)
#define _DAT_01266444 (*(undefined4 *)0x01266444u)
#define _DAT_01266448 (*(undefined4 *)0x01266448u)
extern undefined4 FUN_00413ed0();
extern void FUN_00563105();
extern void FUN_005646c0();
extern void FUN_005646ed();
extern undefined4 FUN_005653eb();
extern void FUN_00566a03();
extern LONG FUN_005677b8();
extern byte * FUN_0056793c();
extern void FUN_00567994();
extern void FUN_00567a4d();
extern LPSTR FUN_00567c9a();
#define stack0xffffff88 (restore_stack[32648])

void FUN_00562fea(void)

{
  unsigned char restore_stack[65536];
  DWORD DVar1;
  int iVar2;
  undefined4 uVar3;
  uint uVar4;
  HMODULE pHVar5;
  undefined4 uVar6;
  _STARTUPINFOA local_60;
  undefined1 *local_1c;
  undefined4 *local_18;
  void *pvStack_14;
  undefined1 *puStack_10;
  undefined *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &DAT_00570620;
  puStack_10 = &LAB_00561a90;
  pvStack_14 = ExceptionList;
  local_1c = &stack0xffffff88;
  ExceptionList = &pvStack_14;
  DVar1 = GetVersion();
  _DAT_01266448 = DVar1 >> 8 & 0xff;
  _DAT_01266444 = DVar1 & 0xff;
  _DAT_01266440 = _DAT_01266444 * 0x100 + _DAT_01266448;
  _DAT_0126643c = DVar1 >> 0x10;
  iVar2 = FUN_005653eb(0);
  if (iVar2 == 0) {
    FUN_00563105(0x1c);
  }
  local_8 = 0;
  FUN_00566a03();
  DAT_01267aa4 = GetCommandLineA();
  DAT_01266400 = FUN_00567c9a();
  FUN_00567a4d();
  FUN_00567994();
  FUN_005646c0();
  local_60.dwFlags = 0;
  GetStartupInfoA(&local_60);
  uVar3 = FUN_0056793c();
  if ((local_60.dwFlags & 1) == 0) {
    uVar4 = 10;
  }
  else {
    uVar4 = (uint)local_60.wShowWindow;
  }
  uVar6 = 0;
  pHVar5 = GetModuleHandleA((LPCSTR)0x0);
  uVar3 = FUN_00413ed0(pHVar5,uVar6,uVar3,uVar4);
  FUN_005646ed(uVar3);
  FUN_005677b8(*(undefined4 *)*local_18,local_18);
  return;
}
