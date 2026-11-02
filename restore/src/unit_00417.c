#include "menu_input.h"
#define I(a) (*(int32_t *)(uintptr_t)(a))
static int32_t add(int32_t a,int32_t b) {return (int32_t)((uint32_t)a+(uint32_t)b);}
int FUN_00426450(int x,int y,const RestoreMenuButton *buttons,int count)
{
    int index;
    for(index=0;index<count;++index) {
        const RestoreMenuButton *button=buttons+index;
        if(I(0xe38e34)>=add(x,button->left)&&I(0xe38e34)<add(x,button->right)&&
           I(0xe38e30)>=add(y,button->top)&&I(0xe38e30)<add(y,button->bottom)) {
            I(0xe38e28)=I(0xe38e30);I(0xe38e2c)=I(0xe38e34);
            return index+1;
        }
    }
    return 0;
}
