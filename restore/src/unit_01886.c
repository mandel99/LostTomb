#include "camera_bounds.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
void FUN_004de080(void)
{
    uint32_t left=U(0xe94dd0),top=U(0xe94dc8);
    uint32_t right=U(0xe94dcc),bottom=U(0xe94dc4),y,height;
    int changed=0;
    if(U(0x6fdc50)!=0 && U(0x605a3c)==1) {
        left-=2u;top-=5u;right+=3u;bottom+=5u;
    }
    if((int32_t)U(0xbca2bc)<(int32_t)left) {
        U(0xbca2bc)=left;changed=1;
    }
    if((int32_t)U(0xbca2c0)<(int32_t)top) {
        U(0xbca2c0)=top;changed=1;
    }
    if((int32_t)(FUN_004ddea0()+U(0xbca2bc))>(int32_t)right) {
        U(0xbca2bc)=right-FUN_004ddea0();changed=1;
    }
    height=FUN_004ddeb0();
    y=U(0xbca2c0);
    if((int32_t)(height+y)>(int32_t)bottom) {
        y=bottom-FUN_004ddeb0();changed=1;
    }
    if(changed && U(0x605a3c)==1) B(0xe38dca)=1;
    /* Native mask also clears the upper half, even without a clamp. */
    U(0xbca2c0)=y&0xfffeu;
}
