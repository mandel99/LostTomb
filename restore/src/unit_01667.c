#include "menu_text_glyph_memory.h"
int32_t FUN_004cbf30(const uint8_t *text,int32_t font)
{
    uint32_t width=0,i;int32_t space,spacing,count=TG_I(0x0126517c);
    if(font==TG_I(0x00f56fb4)||font==TG_I(0x00f56fbc)){space=10;spacing=1;}
    else if(font==TG_I(0x00f56f84)){space=4;spacing=1;}
    else{space=6;spacing=0;}
    for(i=0;i<10000&&text[i];++i){
        uint8_t character=text[i];uint32_t glyph;
        if(character==32){width+=(uint32_t)space;continue;}
        glyph=TG_B(0x005e047cu+character);TG_U(0x00d3b5c0)=glyph;
        if(glyph)width+=(uint32_t)(tg_metric((uint32_t)font+glyph-1u,20,count)+spacing);
    }
    return(int32_t)width;
}
