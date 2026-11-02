#include "rich_message_text.h"
void FUN_004c7fd0(int32_t link,int32_t left,int32_t right,int32_t y)
{
    uint32_t row;
    if(RT_I(0xd3a6ec)>=50)return;
    row=0xd3a6f8u+RT_U(0xd3a6f0)*20u;
    RT_I(row)=rt_sub(left,2);RT_I(row+4)=rt_add(right,2);
    RT_I(row+8)=rt_sub(y,1);RT_I(row+12)=rt_add(y,13);RT_I(row+16)=link;
    RT_U(0xd3a6f0)+=1;RT_U(0xd3a6ec)+=1;
}
