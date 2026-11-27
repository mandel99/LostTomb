#include "pharaoh/core/projectile_direction.h"
#include "pharaoh/core/percentage_scale.h"

#if !defined(_MSC_VER) || !defined(_M_IX86)
#error Native signed divide-fault materialization requires MSVC Win32.
#endif

/* Reuse solved4CDE60 logic. Its sole signed-IDIV overflow has exact operands
 * INT_MIN/-1; execute that actual hardware operation instead of inventing a
 * zero direction on a fault. A continuing handler retains the CPU result. */
static int32_t scale_ratio_live(int32_t value,int32_t denominator)
{
    const PharaohPercentageScaleResult scaled=pharaoh_scale_value_to_percent(value,denominator);
    if(scaled.status==PHARAOH_PERCENTAGE_SCALE_VALUE)return scaled.value;
    int32_t quotient;
    __asm {
        mov eax,80000000h
        cdq
        mov ecx,denominator
        idiv ecx
        mov quotient,eax
    }
    return quotient;
}

/* Whole0051CC00. Current/old ordinary C algorithm, explicit wrapped SUB/NEG. */
uint8_t pharaoh_projectile_direction(int32_t x1,int32_t y1,int32_t x2,int32_t y2)

{
  int32_t iVar1;
  int32_t iVar2;

  if (x2 < x1) {
    iVar2 = (int32_t)((uint32_t)x1-(uint32_t)x2);
  }
  else {
    iVar2 = (int32_t)((uint32_t)x2-(uint32_t)x1);
  }
  if (y2 < y1) {
    iVar1 = (int32_t)((uint32_t)y1-(uint32_t)y2);
  }
  else {
    iVar1 = (int32_t)((uint32_t)y2-(uint32_t)y1);
  }
  if (iVar1 < iVar2) {
    iVar2 = scale_ratio_live(iVar2,iVar1);
  }
  else if (iVar2 < iVar1) {
    iVar2 = scale_ratio_live(iVar1,iVar2);
    iVar2 = (int32_t)(0u-(uint32_t)iVar2);
  }
  else {
    iVar2 = 100;
  }
  if (x1 == x2) {
    if (y2 < y1) {
      return 0;
    }
    if (y1 < y2) {
      return 8;
    }
  }
  if (x2 < x1) {
    if (y1 == y2) {
      return 0xc;
    }
    if (y1 <= y2) {
      if (499 < iVar2) {
        return 0xc;
      }
      if (199 < iVar2) {
        return 0xb;
      }
      if (-200 < iVar2) {
        return 10;
      }
      return (uint8_t)((-500 < iVar2) + 8);
    }
    if (499 < iVar2) {
      return 0xc;
    }
    if (199 < iVar2) {
      return 0xd;
    }
    if (-200 < iVar2) {
      return 0xe;
    }
    return (uint8_t)(((iVar2 < -499) - 1U) & 0xf);
  }
  if (x2 <= x1) {
    return 0;
  }
  if (y1 == y2) {
    return 4;
  }
  if (y2 < y1) {
    if (499 < iVar2) {
      return 4;
    }
    if (199 < iVar2) {
      return 3;
    }
    if (-200 < iVar2) {
      return 2;
    }
    return -500 < iVar2;
  }
  if (499 < iVar2) {
    return 4;
  }
  if (199 < iVar2) {
    return 5;
  }
  if (-200 < iVar2) {
    return 6;
  }
  return (uint8_t)((iVar2 < -499) + 7);
}
