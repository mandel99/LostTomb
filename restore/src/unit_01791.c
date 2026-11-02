#include "graphics_session.h"
#include <string.h>
/* 31 logical shifts of both original feedback registers. History is written
   before advancing, including rows above the 30-line drawing boundary. */
static uint32_t advance_random(void)
{
    uint32_t a=GFX_U(0xd3b384),b=GFX_U(0xd3b380);
    uint32_t index=GFX_U(0xd3b1c4),next=index+1u;
    int i;
    GFX_U(0xd3b1c8u+index*4u)=GFX_U(0xd3b374);
    GFX_U(0xd3b1c4)=(int32_t)next>=100?0:next;
    for(i=0;i<31;++i) {
        a=(a>>1)|(((a^(a>>4))&1u)<<30);
        b=(b>>1)|(((b^(b>>4))&1u)<<30);
    }
    GFX_U(0xd3b384)=a; GFX_U(0xd3b380)=b;
    GFX_U(0xd3b374)=a&127u; GFX_U(0xd3b370)=a&32767u; GFX_U(0xd3b378)=a&7u;
    GFX_U(0xd3b364)=b&32767u; GFX_U(0xd3b368)=b&127u; GFX_U(0xd3b36c)=b&7u;
    return a&32767u;
}
void FUN_004d0650(RestoreSurfaceContext *self,int buffer)
{
    uint16_t *pixels;
    int x,y,width,height,limit;
    int32_t brightness;
    if(self->steps<=0||self->format==-1) return;
    --self->steps;
    brightness=self->brightness;
    if(self->randomize) brightness=(int32_t)((uint32_t)brightness+(advance_random()&15u)-8u);
    width=self->width; height=self->height;
    /* Native buffer selectors are 0/1. Keep the original 32-bit slot address. */
    memcpy(&pixels,(void *)((uintptr_t)self+36u+(uint32_t)buffer*4u),sizeof(pixels));
    limit=GFX_I(GFX_U(0xf53f78)?0xf53f60u:0xf53f64u);
    if(!self->effect) {
        if(self->format!=0&&self->format!=1) return;
        for(y=0;y<height;++y) for(x=0;x<width;++x,++pixels) {
            if(x<limit&&y>=30) {
                uint32_t value=*pixels;
                /* Both original branches shift red by 11, including RGB555. */
                int32_t average=(int32_t)(((value>>11)+(value&31u))>>1);
                uint32_t level=(uint32_t)FUN_004d0630((int32_t)((uint32_t)average+(uint32_t)brightness),0,31);
                *pixels=(uint16_t)((((level<<5)|level)<<(self->format==1?6:5))|level);
            }
        }
    } else {
        for(y=0;y<height;++y) {
            uint32_t noise=advance_random();
            for(x=0;x<width;++x,++pixels) {
                if(x<limit&&y>=30) {
                    uint32_t value=*pixels;
                    if((((uint32_t)x+noise)&127u)==0) *pixels=self->format==0?0x7fffu:0xffffu;
                    else if(self->format==0) *pixels=(uint16_t)(((value>>1)&0x3defu)|(value&0x8000u));
                    else {
                        value=(value&0xffe0u)|((value>>1)&15u);
                        value=(value&0xf81fu)|((value>>1)&0x3e0u);
                        *pixels=(uint16_t)(((value>>1)&0x7800u)|(value&0x7ffu));
                    }
                }
            }
        }
    }
}
