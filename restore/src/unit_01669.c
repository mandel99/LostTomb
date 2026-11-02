#include "sprite_libraries.h"
#include <string.h>
int FUN_004cc020(uint8_t character,int font)
{
    int glyph,image;
    int16_t advance;
    if(!character)return 0;
    if(character==32)return 4;
    glyph=((const uint8_t *)0x5e047cu)[character];
    *(int32_t *)0xd3b5c0u=glyph;
    if(!glyph)return 0;
    image=font+glyph-1;
    if(image>=*(int32_t *)0x126517cu||image<0)image=0;
    memcpy(&advance,RESTORE_SPRITE_LIBRARIES->images[image].bytes+20,2);
    return (int)advance+1;
}
