#include "selection_popup.h"
void FUN_00403c00(uint32_t x,uint32_t y,int32_t count,uint32_t group,
    uint32_t first,uint32_t byte_destination,uint32_t word_destination,
    uint32_t dword_destination,uint32_t post_action,uint32_t storage,
    uint32_t preserve_mapping,uint32_t completion,uint32_t alternate_mode,
    int32_t rows,uint32_t disable_edges)
{
    int32_t i;
    if(count>78)count=78;
    if(rows<=0)rows=26;
    SP_U(0x605954)=x;SP_U(0x605938)=dword_destination;
    SP_U(0x60594c)=y;SP_U(0x605930)=byte_destination;
    SP_U(0x605934)=word_destination;SP_U(0x605944)=group;
    SP_U(0x60592c)=storage;SP_U(0x60593c)=post_action;
    SP_U(0x605940)=first;SP_B(0x6057f0)=(uint8_t)disable_edges;
    SP_U(0x605a50)=completion;SP_I(0x605948)=count;
    SP_I(0x6057ec)=(int32_t)((uint32_t)count+(uint32_t)rows-1u)/rows;
    SP_I(0x605950)=rows;
    if(!(uint8_t)preserve_mapping)
        for(i=0;i<count;++i)SP_I(0x6057f4u+(uint32_t)i*4u)=i;
    SP_U(0xe38ddc)=20;SP_U(0x605958)=SP_B(0xe38dda);
    SP_B(0xe38dda)=(uint8_t)alternate_mode?0x48:0x0f;
    for(i=0;i<count;++i){
        RestoreMenuButton *b=(RestoreMenuButton *)(uintptr_t)(0x704dc8u+(uint32_t)i*28u);
        uint32_t col=(uint32_t)(i/rows)*200u,row=(uint32_t)(i%rows)*20u;
        b->left=(int16_t)(col+5u);b->top=(int16_t)(row+14u);
        b->right=(int16_t)(col+201u);b->bottom=(int16_t)(row+32u);
        b->on_left=0x403950;b->on_right=0x51f780;
        b->reserved16=0;b->activated=0;b->reserved18=0;b->trigger=3;
        b->primary=i;b->secondary=0;
    }
}
