#include "restore_types.h"
#include "display_configuration.h"
#define DAT_00d3b550 (*(int32_t *)0x00d3b550u)
#define DAT_00d3b554 (*(int32_t *)0x00d3b554u)
#define DAT_00d3b558 (*(int32_t *)0x00d3b558u)
#define DAT_00d3b55c (*(int32_t *)0x00d3b55cu)
#define DAT_00e38d94 (*(int32_t *)0x00e38d94u)
#define DAT_00e38d98 (*(int32_t *)0x00e38d98u)
#define DAT_00e38d9c (*(int32_t *)0x00e38d9cu)
#define DAT_00e38da4 (*(int32_t *)0x00e38da4u)
#define DAT_00e38da8 (*(int32_t *)0x00e38da8u)
#define DAT_00e38dac (*(int32_t *)0x00e38dacu)
#define DAT_00e38db0 (*(int32_t *)0x00e38db0u)
#define DAT_00e38db4 (*(int32_t *)0x00e38db4u)
#define DAT_00e38db8 (*(int32_t *)0x00e38db8u)
#define DAT_00e38e68 (*(int32_t *)0x00e38e68u)
#define DAT_00e39140 (*(HWND *)0x00e39140u)
#define _DAT_00e38d8c (*(undefined4 *)0x00e38d8cu)
#define _DAT_00e38d90 (*(undefined4 *)0x00e38d90u)
#define _DAT_00e390e4 (*(undefined4 *)0x00e390e4u)
#define _DAT_00e390e8 (*(undefined4 *)0x00e390e8u)

void FUN_004cfdc0(undefined4 param_1)

{
  DWORD dwExStyle;
  HMENU pHVar1;
  DWORD dwStyle;
  uint bMenu;
  tagRECT local_10;
  
  restore_display_configure(param_1);
  SetRect(&local_10,0,0,DAT_00e38db0,DAT_00e38dac);
  dwExStyle = GetWindowLongA(DAT_00e39140,-0x14);
  pHVar1 = GetMenu(DAT_00e39140);
  bMenu = (uint)(pHVar1 != (HMENU)0x0);
  dwStyle = GetWindowLongA(DAT_00e39140,-0x10);
  AdjustWindowRectEx(&local_10,dwStyle,bMenu,dwExStyle);
  _DAT_00e390e8 = (int32_t)((uint32_t)DAT_00e38db8 - (uint32_t)DAT_00e38db0) / 2;
  _DAT_00e390e4 = (int32_t)((uint32_t)DAT_00e38db4 - (uint32_t)DAT_00e38dac) / 2;
  SetWindowPos(DAT_00e39140,(HWND)0x0,0,0,local_10.right - local_10.left,
               local_10.bottom - local_10.top,0x16);
  SetWindowPos(DAT_00e39140,(HWND)0xfffffffe,0,0,0,0,0x13);
  UpdateWindow(DAT_00e39140);
  SetFocus(DAT_00e39140);
  return;
}
