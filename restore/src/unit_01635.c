#include "rich_message_text.h"
int32_t FUN_004c8680(int32_t font,uint8_t glyph,int32_t x,int32_t y,int32_t calculate,int32_t flags)
{
    int32_t index,offset;RT_U(0xd3b5c0)=RT_B(0x5e049cu+glyph);if(!RT_U(0xd3b5c0))return 0;
    index=rt_sub(rt_add(font,RT_I(0xd3b5c0)),1);offset=rt_sub(rt_metric(index,22),flags?16:11);
    if(offset<0||glyph<96||glyph==199||glyph==231)offset=0;
    if(!calculate)FUN_0053e590(index,x,rt_sub(y,offset));
    /* The sprite endpoint may change the shared glyph; native reloads it after draw. */
    return rt_metric(rt_sub(rt_add(font,RT_I(0xd3b5c0)),1),20);
}
