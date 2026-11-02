#include "menu_record_queries.h"

void FUN_00442500(void)
{
    uint32_t record=0x0077f749u;
    int32_t count=0;
    volatile int32_t *selected=(volatile int32_t *)(uintptr_t)0x00775918u;
    *selected=0; /* The original writes a DWORD, including on the empty path. */
    while(*(volatile uint8_t *)(uintptr_t)record>1u){
        record+=0x62u;
        ++count;
        if((int32_t)record>=0x007843d9)break;
    }
    *selected=count;
    if(count>=200)*selected=199;
}
