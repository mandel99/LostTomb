#include "message_panel_input.h"
void FUN_00425bc0(int32_t x,int32_t y,const void *table,int32_t count,int32_t first)
{
    const RestoreSmallButton *button=(const RestoreSmallButton *)table;int32_t i;
    for(i=0;i<count;++i,++button) {
        int32_t left=mi_add(x,button->left),top=mi_add(y,button->top);
        if(MI_I(0xe38e34)>=left&&MI_I(0xe38e34)<mi_add(left,button->width)&&MI_I(0xe38e30)>=top&&MI_I(0xe38e30)<mi_add(top,button->height)) {
            MI_U(0xe38e2c)=MI_U(0xe38e34);MI_U(0xe38e28)=MI_U(0xe38e30);MI_I(0x7056b8)=mi_add(i,first);return;
        }
    }
}
