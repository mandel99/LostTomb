#include "restore_types.h"
#include "video_player.h"
#include "window_messages.h"
#define LAB_0056b6bb (*(undefined1 *)0x0056b6bbu)
#define DAT_006fdc54 RESTORE_CURRENT_VIDEO
#define DAT_00d3b380 (*(int32_t *)0x00d3b380u)
#define DAT_00d3b384 (*(int32_t *)0x00d3b384u)
#define DAT_00d3b494 (*(undefined1 *)0x00d3b494u)
#define DAT_00d3b495 (*(undefined1 *)0x00d3b495u)
#define DAT_00d3b496 (*(undefined1 *)0x00d3b496u)
#define DAT_00d3b497 (*(undefined1 *)0x00d3b497u)
#define DAT_00d3b510 (*(int32_t *)0x00d3b510u)
#define DAT_00d3b518 (*(int32_t *)0x00d3b518u)
#define DAT_00d3b550 (*(int32_t *)0x00d3b550u)
#define DAT_00d3b554 (*(int32_t *)0x00d3b554u)
#define DAT_00d3b558 (*(int32_t *)0x00d3b558u)
#define DAT_00d3b55c (*(int32_t *)0x00d3b55cu)
#define DAT_00e20768 (*(char * *)0x00e20768u)
#define DAT_00e36b94 (*(undefined4 *)0x00e36b94u)
#define DAT_00e38d94 (*(int32_t *)0x00e38d94u)
#define DAT_00e38d98 (*(int32_t *)0x00e38d98u)
#define DAT_00e38d9c (*(int32_t *)0x00e38d9cu)
#define DAT_00e38da1 (*(undefined1 *)0x00e38da1u)
#define DAT_00e38da4 (*(int32_t *)0x00e38da4u)
#define DAT_00e38da8 (*(int32_t *)0x00e38da8u)
#define DAT_00e38dac (*(int32_t *)0x00e38dacu)
#define DAT_00e38db0 (*(int32_t *)0x00e38db0u)
#define DAT_00e38db4 (*(int32_t *)0x00e38db4u)
#define DAT_00e38db8 (*(int32_t *)0x00e38db8u)
#define DAT_00e38e14 (*(int32_t *)0x00e38e14u)
#define DAT_00e38e4c (*(undefined1 *)0x00e38e4cu)
#define DAT_00e38e68 (*(int32_t *)0x00e38e68u)
#define DAT_00e39090 (*(HCURSOR *)0x00e39090u)
#define DAT_00e39094 (*(HCURSOR *)0x00e39094u)
#define DAT_00e39098 (*(HCURSOR *)0x00e39098u)
#define DAT_00e3909c (*(HCURSOR *)0x00e3909cu)
#define DAT_00e390a0 (*(HCURSOR *)0x00e390a0u)
#define DAT_00e390a4 (*(HCURSOR *)0x00e390a4u)
#define DAT_00e390a8 (*(HCURSOR *)0x00e390a8u)
#define DAT_00e390ac (*(HCURSOR *)0x00e390acu)
#define DAT_00e390b0 (*(HCURSOR *)0x00e390b0u)
#define DAT_00e390b4 (*(HCURSOR *)0x00e390b4u)
#define DAT_00e390b8 (*(HCURSOR *)0x00e390b8u)
#define DAT_00e390bc (*(HCURSOR *)0x00e390bcu)
#define DAT_00e390c0 (*(HCURSOR *)0x00e390c0u)
#define DAT_00e390c4 (*(HCURSOR *)0x00e390c4u)
#define DAT_00e390c8 (*(HCURSOR *)0x00e390c8u)
#define DAT_00e390cc (*(HCURSOR *)0x00e390ccu)
#define DAT_00e390d0 (*(HCURSOR *)0x00e390d0u)
#define DAT_00e390d4 (*(HCURSOR *)0x00e390d4u)
#define DAT_00e390d8 (*(HCURSOR *)0x00e390d8u)
#define DAT_00e39140 (*(HWND *)0x00e39140u)
#define DAT_00e39148 (*(int32_t *)0x00e39148u)
#define DAT_00e39170 (*(int32_t *)0x00e39170u)
#define DAT_00e39174 (*(int32_t *)0x00e39174u)
#define DAT_00e39178 (*(int32_t *)0x00e39178u)
#define _DAT_00d3b37c (*(undefined4 *)0x00d3b37cu)
#define _DAT_00d3b508 (*(undefined4 *)0x00d3b508u)
#define _DAT_00e38d8c (*(undefined4 *)0x00e38d8cu)
#define _DAT_00e38d90 (*(undefined4 *)0x00e38d90u)
#define _DAT_00e39144 (*(undefined4 *)0x00e39144u)
#define s_Debug_and_status_report_on_005e0730 (*(char (*)[28])0x005e0730u)
#define s_WinSJBClass_005e0724 (*(char (*)[12])0x005e0724u)
extern void FUN_004cc110();
extern void FUN_004ce6e0();
extern undefined4 FUN_004d0220();
extern void * FUN_00561a1d();
extern void FUN_005623fc();

undefined4 FUN_004cf960(HINSTANCE param_1)

{
  ATOM AVar1;
  HWND pHVar2;
  BOOL BVar3;
  int nHeight;
  int nWidth;
  void *pvVar4;
  undefined4 uVar5;
  HMENU hMenu;
  LPVOID lpParam;
  WNDCLASSA local_34;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_0056b6bb;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  FUN_005623fc(&DAT_00e36b94,100);
  DAT_00d3b384 = 0x54657687;
  DAT_00d3b380 = 0x72641663;
  _DAT_00d3b37c = 0x34518632;
  DAT_00d3b494 = 0;
  DAT_00d3b495 = 0;
  DAT_00d3b496 = 0;
  DAT_00d3b497 = 0;
  DAT_00e38e4c = 0;
  DAT_00d3b518 = 0;
  _DAT_00d3b508 = 0;
  DAT_00d3b510 = 0;
  DAT_00e38e14 = 0;
  DAT_00e38da1 = 1;
  if (DAT_00e38e68 == 2) {
    DAT_00d3b558 = 800;
    DAT_00d3b550 = 600;
    DAT_00e38db0 = 800;
  }
  else if (DAT_00e38e68 == 3) {
    DAT_00d3b558 = 0x400;
    DAT_00d3b550 = 0x300;
    DAT_00e38db0 = 0x400;
  }
  else {
    DAT_00d3b558 = 0x280;
    DAT_00d3b550 = 0x1e0;
    DAT_00e38db0 = 0x280;
  }
  DAT_00e38da8 = (DAT_00d3b558 + -0x280) / 2;
  DAT_00e38da4 = (DAT_00d3b550 + -0x1e0) / 2;
  DAT_00e38d94 = DAT_00d3b558 * 2;
  _DAT_00e38d8c = DAT_00e38d94 * DAT_00d3b550;
  DAT_00d3b55c = 0;
  DAT_00d3b554 = 0;
  _DAT_00e38d90 = DAT_00d3b550;
  DAT_00e38d98 = DAT_00d3b550;
  DAT_00e38d9c = DAT_00d3b558;
  DAT_00e38dac = DAT_00d3b550;
  DAT_00e38db8 = GetSystemMetrics(0);
  DAT_00e38db4 = GetSystemMetrics(1);
  FUN_004ce6e0(s_Debug_and_status_report_on_005e0730,0,0);
  CoInitialize((LPVOID)0x0);
  FUN_004cc110(9,0);
  pHVar2 = FindWindowA(s_WinSJBClass_005e0724,DAT_00e20768);
  if (pHVar2 != (HWND)0x0) {
    BVar3 = IsIconic(pHVar2);
    if (BVar3 != 0) {
      OpenIcon(pHVar2);
    }
    SetForegroundWindow(pHVar2);
    SendMessageA(pHVar2,7,0,0);
    SendMessageA(pHVar2,0x18,1,0);
    SendMessageA(pHVar2,0x1c,1,0);
    ExceptionList = local_c;
    return 0;
  }
  local_34.style = 0;
  local_34.lpfnWndProc = restore_window_proc;
  local_34.cbClsExtra = 0;
  local_34.cbWndExtra = 0;
  local_34.hInstance = param_1;
  local_34.hIcon = LoadIconA(param_1,(LPCSTR)0x8d);
  local_34.hCursor = (HCURSOR)0x0;
  local_34.hbrBackground = GetStockObject(4);
  local_34.lpszMenuName = (LPCSTR)0x0;
  local_34.lpszClassName = s_WinSJBClass_005e0724;
  AVar1 = RegisterClassA(&local_34);
  if (AVar1 == 0) {
    ExceptionList = local_c;
    return 0;
  }
  DAT_00e390d8 = LoadCursorA(param_1,(LPCSTR)0x66);
  DAT_00e390d4 = LoadCursorA(param_1,(LPCSTR)0x88);
  DAT_00e390d0 = LoadCursorA(param_1,(LPCSTR)0x82);
  DAT_00e390bc = LoadCursorA(param_1,(LPCSTR)0x93);
  DAT_00e390c4 = LoadCursorA(param_1,(LPCSTR)0x90);
  DAT_00e390c8 = LoadCursorA(param_1,(LPCSTR)0x91);
  DAT_00e39094 = LoadCursorA(param_1,(LPCSTR)0x92);
  DAT_00e390cc = LoadCursorA(param_1,(LPCSTR)0x8e);
  DAT_00e390b8 = LoadCursorA(param_1,(LPCSTR)0x96);
  DAT_00e390b0 = LoadCursorA(param_1,(LPCSTR)0x95);
  DAT_00e390b4 = LoadCursorA(param_1,(LPCSTR)0x94);
  DAT_00e390ac = LoadCursorA(param_1,(LPCSTR)0x99);
  DAT_00e390a8 = LoadCursorA(param_1,(LPCSTR)0x98);
  DAT_00e390a4 = LoadCursorA(param_1,(LPCSTR)0x9b);
  DAT_00e390a0 = LoadCursorA(param_1,(LPCSTR)0x9a);
  DAT_00e3909c = LoadCursorA(param_1,(LPCSTR)0x9c);
  DAT_00e39098 = LoadCursorA(param_1,(LPCSTR)0x9d);
  DAT_00e390c0 = LoadCursorA(param_1,(LPCSTR)0x9f);
  DAT_00e39090 = LoadCursorA(param_1,(LPCSTR)0xa0);
  lpParam = (LPVOID)0x0;
  _DAT_00e39144 = param_1;
  hMenu = (HMENU)0x0;
  pHVar2 = (HWND)0x0;
  nHeight = GetSystemMetrics(1);
  nWidth = GetSystemMetrics(0);
  DAT_00e39140 = CreateWindowExA(0x40000,s_WinSJBClass_005e0724,DAT_00e20768,0x90080000,0,0,nWidth,
                                 nHeight,pHVar2,hMenu,param_1,lpParam);
  if (DAT_00e39140 == (HWND)0x0) {
    ExceptionList = local_c;
    return 0;
  }
  UpdateWindow(DAT_00e39140);
  SetFocus(DAT_00e39140);
  DAT_00e39178 = 0;
  DAT_00e39174 = 0;
  DAT_00e39148 = 0;
  DAT_00e39170 = 0;
  pvVar4 = FUN_00561a1d(0x1c);
  local_4 = 0;
  if (pvVar4 == (void *)0x0) {
    DAT_006fdc54 = 0;
  }
  else {
    DAT_006fdc54 = FUN_004133d0((RestoreVideoPlayer *)pvVar4,DAT_00e39140);
  }
  local_4 = 0xffffffff;
  uVar5 = FUN_004d0220();
  ExceptionList = local_c;
  return uVar5;
}
