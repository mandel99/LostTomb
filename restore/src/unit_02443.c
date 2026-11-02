#include "city_top_chrome.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(a))
extern int FUN_0053e590(uint32_t image,uint32_t x,uint32_t y);
void FUN_00536670(void)
{
    uint32_t width=U(0x00e38db0u),x;
    if(width==640)x=(uint32_t)-360;
    else if(width==800)x=(uint32_t)-240;
    else if(width==1024)x=0;
    else return;
    /* Native reads the editor mode in each resolution branch. Both outcomes
       select the same image and position; retain the full-width access. */
    (void)U(0x00605a3cu);
    FUN_0053e590(U(0x00f567e4u)+8u,x,0);
}
