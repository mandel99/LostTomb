#include "menu_text_glyph_memory.h"
void FUN_004cc770(const uint8_t *text,int32_t x,int32_t y,int32_t font,uint16_t color)
{
    int32_t space,spacing=0;uint8_t character;
    if(font==TG_I(0x00f56fb4)){space=8;TG_I(0x00d3b500)=23;}
    else if(font==TG_I(0x00f56fbc)){space=8;TG_I(0x00d3b500)=24;}
    else if(font==TG_I(0x00f56f84)){space=4;spacing=1;TG_I(0x00d3b500)=9;}
    else{space=6;TG_I(0x00d3b500)=11;}
    TG_W(0x00d3b51c)=color;character=*text;
    for(;;){
        uint8_t previous=character,draw_character=character;int32_t advance=0;
        if(!character){
            if(!TG_U(0x00e20788)){
                TG_U(0x00e2078c)=4;TG_U(0x00e20788)=1;
                TG_U(0x00e20790)=TG_U(0x00e20778);TG_U(0x00e20794)=TG_U(0x00e20774);
            }
            TG_U(0x00e20780)=0;TG_I(0x00e20778)=tg_add(TG_I(0x00e20778),space);
            TG_U(0x00e2077c)=0;TG_B(0x00e20799)=0;TG_U(0x00d3b504)=0;return;
        }
        if(character==35){
            if(TG_U(0x00e20780)==1)draw_character=(uint8_t)FUN_004ccb00();
            else draw_character=32;
        }
        if(draw_character==95)draw_character=32;
        if(draw_character>31){
            uint8_t glyph=(uint8_t)(draw_character-32);advance=space;
            if(TG_B(0x005e049cu+glyph)){
                if(!TG_U(0x00d3b510)&&TG_U(0x00d3b518)){
                    TG_W(0x00d3b51c)=0;FUN_004cca60(font,glyph,x,tg_sub(y,1));
                    TG_W(0x00d3b51c)=0;FUN_004cca60(font,glyph,x,tg_add(y,1));
                }
                else if(!TG_U(0x00d3b510)&&TG_U(0x00d3b514)){
                    TG_W(0x00d3b51c)=0;FUN_004cca60(font,glyph,tg_add(x,1),tg_add(y,1));
                    TG_W(0x00d3b51c)=color;
                    advance=tg_add(FUN_004cca60(font,glyph,tg_sub(x,1),y),spacing);
                    goto positioned;
                }
                TG_W(0x00d3b51c)=color;advance=tg_add(FUN_004cca60(font,glyph,x,y),spacing);
            }
positioned:
            if(TG_U(0x00e20784)==TG_U(0x00e207ecu+TG_U(0x00e20990)*4u)&&!TG_U(0x00e20788)){
                TG_U(0x00e20790)=TG_U(0x00e20778);TG_U(0x00e20794)=TG_U(0x00e20774);
                TG_U(0x00e20788)=1;TG_I(0x00e2078c)=advance;
            }
            x=tg_add(x,advance);TG_I(0x00e20778)=tg_add(TG_I(0x00e20778),advance);
        }
        if(!TG_U(0x00e2077c)){++text;character=*text;}
        else character=(uint8_t)FUN_004ccb00();
        TG_U(0x00e20784)+=1;
        if(!TG_B(0x00e20799)&&TG_B(0x00e20798)&&previous==character&&(character==32||character==95)){
            TG_I(0x00e20778)=tg_sub(TG_I(0x00e20778),advance);x=tg_sub(x,advance);
        }
    }
}
