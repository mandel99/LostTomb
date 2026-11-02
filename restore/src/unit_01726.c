#include "restore_types.h"
#include "sprite_render.h"
#define DAT_00d3b530 (*(undefined1 *)0x00d3b530u)
#define DAT_00d3b531 (*(undefined1 *)0x00d3b531u)
#define DAT_00d3b538 (*(int32_t *)0x00d3b538u)
#define DAT_00d3b550 (*(int32_t *)0x00d3b550u)
#define DAT_00d3b554 (*(int32_t *)0x00d3b554u)
#define DAT_00d3b5bc (*(int16_t *)0x00d3b5bcu)
#define DAT_00d3b5d4 (*(int32_t *)0x00d3b5d4u)
#define DAT_00d3b5dc (*(int32_t *)0x00d3b5dcu)
#define _DAT_00d3b534 (*(int32_t *)0x00d3b534u)

void FUN_004cdb80(void)

{
  int iVar1;
  
  iVar1 = DAT_00d3b5dc;
  _DAT_00d3b534 = (int)DAT_00d3b5bc;
  DAT_00d3b530 = 0;
  DAT_00d3b538 = 0;
  if (DAT_00d3b5bc < 1) {
LAB_004cdc21: ;
    DAT_00d3b530 = 5;
    DAT_00d3b5d4 = 0;
  }
  else {
    if (DAT_00d3b5dc < DAT_00d3b554) {
      if (_DAT_00d3b534 + DAT_00d3b5dc <= DAT_00d3b554) goto LAB_004cdc21;
      DAT_00d3b5dc = DAT_00d3b554;
      DAT_00d3b538 = DAT_00d3b554 - iVar1;
      DAT_00d3b530 = 3;
      iVar1 = (DAT_00d3b550 - DAT_00d3b554) + DAT_00d3b538;
      if (iVar1 < _DAT_00d3b534) {
LAB_004cdbe2: ;
        _DAT_00d3b534 = iVar1;
      }
    }
    else if (DAT_00d3b550 - _DAT_00d3b534 < DAT_00d3b5dc) {
      if (DAT_00d3b5dc < DAT_00d3b550) {
        DAT_00d3b530 = 4;
        iVar1 = DAT_00d3b550 - DAT_00d3b5dc;
        goto LAB_004cdbe2;
      }
      goto LAB_004cdc21;
    }
    DAT_00d3b5d4 = _DAT_00d3b534 - DAT_00d3b538;
  }
  if (DAT_00d3b531 == '\x05') {
    DAT_00d3b530 = 5;
  }
  return;
}
