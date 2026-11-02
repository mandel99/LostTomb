#include "selection_popup.h"
#include "menu_layout.h"
#include "actor_panel_native_context.h"
extern void FUN_004d66f0(int buttons,int *node);
extern void FUN_00536820(int x,int y,int width,int height);
extern void FUN_00536c30(int kind,int x,int y,int width,int height,uint32_t color);
int FUN_004d6740(int x,int y,int buttons,uint32_t count,int hit)
{
    uint32_t mode=(uint32_t)FUN_004d5d70(SP_B(0xe38dda));
    RestoreMenuButton *b;
    (void)count;
    if(!SP_U(0xe94624u+mode*16u)){
        mode=(uint32_t)FUN_004d5d70(SP_B(0xe38dda));
        FUN_004d66f0(buttons,(int *)(uintptr_t)SP_U(0xe9461cu+mode*16u));
        mode=(uint32_t)FUN_004d5d70(SP_B(0xe38dda));
        SP_U(0xe94624u+mode*16u)=1;return 0;
    }
    if(!hit)return 0;
    b=(RestoreMenuButton *)(uintptr_t)((uint32_t)buttons+(uint32_t)hit*28u-28u);
    if(SP_U(0xe94d98)==2){
        int32_t native_x;
        FUN_00536820(0,0,6,6);
        SP_U(0xe94da0)=(uint32_t)(uintptr_t)b;SP_I(0xe94da4)=hit;
        SP_I(0xe94600)=b->left;SP_I(0xe94604)=b->top;
        SP_I(0xe94608)=(int)b->right-b->left;SP_I(0xe9460c)=(int)b->bottom-b->top;
        SP_I(0xe945f8)=hit;SP_U(0xe945fc)=0;
        native_x=(int32_t)((uint32_t)x+(uint32_t)(int32_t)b->left);
        restore_actor_panel_native_highlight((uint32_t)native_x);
        FUN_00536c30(0,native_x,
            (int)((uint32_t)y+(uint32_t)(int32_t)b->top),
            (int)b->right-b->left,(int)b->bottom-b->top,0xffff);
    }
    return hit;
}
