#include "rich_message_text.h"
void FUN_004c8590(const char *text,int32_t x,int32_t y,int32_t calculate,int32_t flags)
{
    const uint8_t *p=(const uint8_t *)text;uint8_t c=*p++;int32_t highlighted=0,font,advance;
    while(c) {
        if(c=='@') {
            int32_t link=FUN_004cc070((const char *)p),width;
            c=*p++;while(c>='0'&&c<='9')c=*p++;
            width=FUN_004c8480((const char *)p-1);highlighted=rt_add(RT_I(0xe20760),1);
            FUN_004c7fd0(link,x,rt_add(x,width),y);
        }
        if(c>=' ') {
            uint8_t glyph=(uint8_t)(c-32);
            font=RT_I(0xd3b168);if(highlighted>0){--highlighted;font=RT_I(0xd3b164);}
            if(!RT_B(0x5e049cu+glyph))advance=6;
            else {
                advance=FUN_004c8680(font,glyph,x,y,calculate,flags);
                if(font==RT_I(0xf56f84))advance=rt_add(advance,2);
            }
            x=rt_add(x,advance);
        }
        c=*p++;
    }
}
