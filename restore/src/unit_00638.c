#include "menu_record_queries.h"

int32_t FUN_00444920(int32_t value)
{
    uint32_t record=0x0077de56u;
    int32_t index=0;
    do {
        if(*(volatile uint8_t *)(uintptr_t)(record-0x56u)!=0u &&
           (int32_t)*(volatile int16_t *)(uintptr_t)record==value)return index;
        record+=0x6au;
        ++index;
    } while((int32_t)record<0x0077f798);
    return 0;
}
