#include "menu_text_glyph_memory.h"
void FUN_004cc720(const uint8_t *text,int32_t x,int32_t y,int32_t width,int32_t font,uint16_t color)
{
    int32_t measured=FUN_004cbf30(text,font),offset=tg_sub(width,measured)/2;
    TG_I(0x00e20770)=offset;
    if(offset<0)TG_I(0x00e20770)=0;
    FUN_004cc770(text,tg_add(TG_I(0x00e20770),x),y,font,color);
}
