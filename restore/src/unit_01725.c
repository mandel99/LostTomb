#include "restore_types.h"
#include "sprite_render.h"
#define DAT_00d3b531 (*(undefined1 *)0x00d3b531u)
#define DAT_00d3b53c (*(int32_t *)0x00d3b53cu)
#define DAT_00d3b540 (*(int32_t *)0x00d3b540u)
#define DAT_00d3b544 (*(int32_t *)0x00d3b544u)
#define DAT_00d3b54c (*(int32_t *)0x00d3b54cu)
#define DAT_00d3b558 (*(int32_t *)0x00d3b558u)
#define DAT_00d3b55c (*(int32_t *)0x00d3b55cu)
#define DAT_00d3b5be (*(int16_t *)0x00d3b5beu)
#define DAT_00d3b5e0 (*(int32_t *)0x00d3b5e0u)
#define DAT_00e38d9c (*(int32_t *)0x00e38d9cu)
#define _DAT_00d3b548 (*(int32_t *)0x00d3b548u)

void FUN_004cdad0(void)

{
  int iVar1;
  
  iVar1 = (int)DAT_00d3b5be;
  DAT_00d3b531 = 0;
  DAT_00d3b54c = 0;
  _DAT_00d3b548 = iVar1;
  if (DAT_00d3b5be < 1) {
LAB_004cdb6f: ;
    DAT_00d3b531 = 5;
    DAT_00d3b544 = 0;
  }
  else {
    if (DAT_00d3b5e0 < DAT_00d3b55c) {
      if (iVar1 + DAT_00d3b5e0 <= DAT_00d3b55c) goto LAB_004cdb6f;
      DAT_00d3b531 = 1;
      DAT_00d3b54c = DAT_00d3b55c - DAT_00d3b5e0;
      DAT_00d3b5e0 = DAT_00d3b55c;
    }
    else if (DAT_00d3b558 - iVar1 < DAT_00d3b5e0) {
      if (DAT_00d3b558 <= DAT_00d3b5e0) goto LAB_004cdb6f;
      _DAT_00d3b548 = DAT_00d3b558 - DAT_00d3b5e0;
      DAT_00d3b531 = 2;
    }
    DAT_00d3b544 = _DAT_00d3b548 - DAT_00d3b54c;
  }
  DAT_00d3b540 = (iVar1 - DAT_00d3b544) * 2;
  DAT_00d3b53c = (DAT_00e38d9c - DAT_00d3b544) * 2;
  return;
}
