#include "menu_text_glyph_memory.h"
#include "sprite_render.h"
int32_t FUN_004cca60(int32_t font,uint8_t character,int32_t x,int32_t y)
{
    uint32_t glyph=TG_B(0x005e049cu+character),image;int32_t shift;
    TG_U(0x00d3b5c0)=glyph;if(!glyph)return 0;
    image=(uint32_t)font+glyph-1u;
    shift=tg_sub(tg_metric(image,22,TG_I(0x0126517c)),TG_I(0x00d3b500));
    if(shift<0||character<0x60||character==199||character==231)shift=0;
    FUN_0053e590((int32_t)image,x,tg_sub(tg_sub(y,3),shift));
    return tg_metric((uint32_t)font+TG_U(0x00d3b5c0)-1u,20,TG_I(0x0126517c));
}
