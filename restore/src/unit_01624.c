#include "message_panel.h"
#include "sprite_render.h"
#define I(a) (*(int32_t *)(uintptr_t)(a))
extern int FUN_004cde40(int,int),FUN_004cde60(int,int);
void FUN_004c72b0(void)
{
    int32_t percentage,offset;
    if(I(0xd3b134)<=0)return;
    if(I(0xd3b130)<=0)percentage=0;
    else if(I(0xd3b130)<I(0xd3b134))percentage=FUN_004cde60(I(0xd3b130),I(0xd3b134));
    else percentage=100;
    offset=FUN_004cde40((int32_t)((uint32_t)I(0xd3b148)*16u-77u),percentage);
    if(I(0xd3b12c))offset=I(0xd3b128);
    FUN_0053e590(I(0xf56494)+39,I(0xd3b14c)*16+6+I(0xd3b154),I(0xd3b150)+26+offset);
}
