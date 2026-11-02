#include "menu_input.h"
#define B(a) (*(uint8_t *)(uintptr_t)(a))
#define I(a) (*(int32_t *)(uintptr_t)(a))
static int32_t add(int32_t a,int32_t b) {return (int32_t)((uint32_t)a+(uint32_t)b);}

int FUN_004261d0(int x,int y,RestoreMenuButton *button,int count)
{
    int index;
    for(index=0;index<count;++index) {
        int side=0;
        if(I(0xe38e34)<add(x,button->left)||I(0xe38e34)>=add(x,button->right)||
           I(0xe38e30)<add(y,button->top)||I(0xe38e30)>=add(y,button->bottom)) goto next;
        switch(button->trigger) {
        case 1:
            if(B(0xe38e03)||B(0xe38e02)) side=1;
            else if(B(0xe38df7)) side=2;
            else goto next;
            break;
        case 3:
            if(B(0xe38e04)) side=1;
            else if(B(0xe38df8)) side=2;
            else goto next;
            break;
        case 4:
            if(B(0xe38e03)||B(0xe38e02)) side=1;
            else goto next;
            break;
        case 2:
            if(!B(0xe38e00)&&!B(0xe38e03)&&!B(0xe38e02)&&!B(0xe38df4)&&
               !B(0xe38df7)&&!B(0xe38df6)) goto next;
            if(B(0xe38e03)||B(0xe38e02)||B(0xe38df7)||B(0xe38df6)) I(0xd3b3c8)=0;
            else if(!I(0xd3b3ac)) {
                /* Native 00426316 advances the index but retains this record. */
                continue;
            }
            if(B(0xe38e00)) side=1;
            else if(B(0xe38df4)) side=2;
            break;
        default: goto next;
        }
        button->activated=1;
        I(0xd3b198)=button->primary;I(0xd3b194)=button->secondary;
        if(side) restore_menu_callback(side==1?button->on_left:button->on_right);
        return index+1;
next:  ++button;
    }
    return 0;
}
