#include "editor_brush_validation.h"
uint8_t FUN_004ecea0(int32_t tool)
{
    if(EBV_U(0x5e4ef4u)==5)return 0;
    return EBV_U(0x5f5d08u+(uint32_t)tool*4u)!=0;
}
