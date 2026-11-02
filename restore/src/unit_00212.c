#include "restore_types.h"
#define DAT_00579654 (*(undefined *)0x00579654u)
#define DAT_00579670 (*(undefined *)0x00579670u)
#define DAT_0057968c (*(undefined *)0x0057968cu)
#define DAT_005796a8 (*(undefined *)0x005796a8u)
#define DAT_005796c4 (*(undefined *)0x005796c4u)
#define DAT_005796e0 (*(undefined *)0x005796e0u)
#define DAT_005796fc (*(undefined *)0x005796fcu)
#define DAT_00579718 (*(undefined *)0x00579718u)
#define DAT_00579734 (*(undefined *)0x00579734u)
#define DAT_00579750 (*(undefined *)0x00579750u)
#define DAT_0057976c (*(undefined *)0x0057976cu)
#define DAT_006fdcb4 (*(undefined1 *)0x006fdcb4u)
#define DAT_006fdcc4 (*(undefined1 *)0x006fdcc4u)
#define DAT_006fdcd0 (*(undefined1 *)0x006fdcd0u)
#define DAT_006fdcdc (*(undefined1 *)0x006fdcdcu)
#define DAT_007027c7 (*(char *)0x007027c7u)
#define DAT_007027c8 (*(char *)0x007027c8u)
#define DAT_007027c9 (*(char *)0x007027c9u)
#define DAT_007027ca (*(char *)0x007027cau)
#define DAT_007027cb (*(char *)0x007027cbu)
#define DAT_007027cc (*(char *)0x007027ccu)
#define DAT_007027cd (*(char *)0x007027cdu)
#define DAT_007027ce (*(char *)0x007027ceu)
#define DAT_007027cf (*(char *)0x007027cfu)
#define DAT_007027d0 (*(char *)0x007027d0u)
#define DAT_007027d1 (*(char *)0x007027d1u)
#define DAT_00784d2a (*(undefined *)0x00784d2au)
#define DAT_00784d2c (*(undefined *)0x00784d2cu)
#define DAT_00784d2e (*(undefined *)0x00784d2eu)
#define DAT_00e38ea8 (*(undefined1 *)0x00e38ea8u)
#define DAT_00e38ebc (*(undefined *)0x00e38ebcu)
#define DAT_00e950dc (*(undefined *)0x00e950dcu)
extern void FUN_00417ae0();
extern void FUN_004e0cf0();

void FUN_00417b40(void)

{
  bool bVar1;
  bool bVar2;
  bool bVar3;
  undefined2 local_20;
  undefined2 local_1e;
  short local_10;
  
  if (DAT_00e38ebc != 0) {
    return;
  }
  if ((DAT_007027c7 == '\0') && (DAT_00579654 == DAT_00e38ea8)) {
    FUN_00417ae0(0xf);
  }
  if ((DAT_007027c8 == '\0') && (DAT_00579670 == DAT_00e38ea8)) {
    FUN_00417ae0(0x10);
  }
  if ((DAT_007027c9 == '\0') && (DAT_0057968c == DAT_00e38ea8)) {
    FUN_00417ae0(0x11);
  }
  if ((DAT_007027ca == '\0') && (DAT_005796a8 == DAT_00e38ea8)) {
    FUN_00417ae0(0x12);
  }
  if ((DAT_007027cb == '\0') && (DAT_005796c4 == DAT_00e38ea8)) {
    FUN_00417ae0(0x13);
  }
  if ((DAT_007027cc == '\0') && (DAT_005796e0 == DAT_00e38ea8)) {
    FUN_00417ae0(0x14);
  }
  if ((DAT_007027cd == '\0') && (DAT_005796fc == DAT_00e38ea8)) {
    FUN_00417ae0(0x15);
  }
  if ((DAT_007027ce == '\0') && (DAT_00579718 == DAT_00e38ea8)) {
    FUN_00417ae0(0x16);
  }
  if ((DAT_007027cf == '\0') && (DAT_00579734 == DAT_00e38ea8)) {
    FUN_00417ae0(0x17);
  }
  if ((DAT_007027d0 == '\0') && (DAT_00579750 == DAT_00e38ea8)) {
    FUN_00417ae0(0x18);
  }
  if ((DAT_007027d1 == '\0') && (DAT_0057976c == DAT_00e38ea8)) {
    FUN_00417ae0(0x19);
  }
  if (DAT_006fdcb4 != '\0') {
    return;
  }
  if ((DAT_006fdcc4 == '\0') || (DAT_00784d2a == 0)) {
    bVar1 = false;
  }
  else {
    bVar1 = true;
  }
  if ((DAT_006fdcd0 == '\0') || (DAT_00784d2c == 0)) {
    bVar2 = false;
  }
  else {
    bVar2 = true;
  }
  if ((DAT_006fdcdc == '\0') || (DAT_00784d2e == 0)) {
    bVar3 = false;
  }
  else {
    bVar3 = true;
  }
  if (((!bVar1) && (!bVar2)) && (!bVar3)) {
    DAT_006fdcb4 = 1;
    return;
  }
  local_1e = 0x248;
  local_20 = 0x249;
  if (bVar1) {
    if (bVar2) {
      if (bVar3) {
        local_10 = 0x250;
      }
      else {
        local_10 = 0x24d;
      }
      goto LAB_00417d98;
    }
    if (bVar3) {
      local_10 = 0x24e;
      goto LAB_00417d98;
    }
  }
  else if ((bVar2) && (bVar3)) {
    local_10 = 0x24f;
    goto LAB_00417d98;
  }
  if (bVar1) {
    local_10 = 0x24a;
  }
  else {
    local_10 = 0x24c - (ushort)bVar2;
  }
LAB_00417d98: ;
  DAT_00e950dc = 1;
  FUN_004e0cf0(0x83,0,0,&local_20,0);
  DAT_006fdcb4 = 1;
  return;
}
