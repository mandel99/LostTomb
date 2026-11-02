#include "menu_text_glyph_memory.h"
uint32_t FUN_004ccb00(void)
{
    uint32_t index=TG_U(0x00e2077c);uint8_t character=TG_B(0x00e34338u+index);
    if(character)TG_U(0x00e2077c)=index+1u;
    else{TG_U(0x00e2077c)=0;character=32;}
    return(index&0xffffff00u)|character;
}
