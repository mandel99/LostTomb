#include "menu_window_cursor_consumer.h"
#define B(a) (*(uint8_t *)(uintptr_t)(a))
#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define I(a) (*(int32_t *)(uintptr_t)(a))
static uint32_t tile(void){return U(0xbca2d8)+U(0xbca2cc)*228u+U(0xbca2c8);}
static void finish(void){B(0xe38dda)=1;B(0xe38dca)=1;U(0xd05708)=0;}
void FUN_0040ba40(void)
{
    int32_t id,x,y,other_x,other_y,group;RestoreOrderActor *actor;
    if(B(0xe38e60))return;
    id=I(0xd05700);
    if(id<=0){B(0xe38dda)=1;B(0xe38dca)=1;return;}
    actor=restore_order_actor((uint32_t)id);
    if(actor->state==8||!U(0xbca2c4)){B(0xe38dda)=1;B(0xe38dca)=1;return;}
    if(U(0xd05708)){
        uint32_t cursor=U(0xe390dc);
        if(cursor==U(0xe390c8))actor->state=12;
        if(cursor==U(0xe390c4))actor->state=13;
    }
    switch(actor->state){
    case 9:case 11:case 20:
        (void)FUN_0040c180(actor->x,actor->y,I(0xbca2c8),I(0xbca2cc),&x,&y);
        actor->target_x=(int16_t)x;actor->target_y=(int16_t)y;
        FUN_004b3250(id);B(0xe38dca)=1;return;
    case 10:
        (void)FUN_0040c180(actor->x,actor->y,I(0xbca2c8),I(0xbca2cc),&x,&y);
        actor->target_x=(int16_t)x;actor->target_y=(int16_t)y;
        actor->direction=FUN_0051cd80(actor->x,actor->y,x,y);
        actor->order_mode=9;FUN_004b3250(id);finish();return;
    case 12:
        (void)FUN_0040c180(actor->x,actor->y,I(0xbca2c8),I(0xbca2cc),&x,&y);
        group=FUN_004b5520(tile());
        if(!group){actor->target_x=(int16_t)x;actor->target_y=(int16_t)y;}
        else{
            (void)FUN_0040bda0(I(0xbca2c8),I(0xbca2cc),x,y,&other_x,&other_y);
            if(FUN_004b6400(other_x,other_y,group,0)){
                actor->action_tag=(uint8_t)group;
                actor->target_x=(int16_t)x;actor->target_y=(int16_t)y;FUN_004b3250(id);
            }
        }
        finish();return;
    case 13:
        if(!actor->can_special)return;
        if(!(U(0xaf598cu+tile()*4u)&4u)){
            (void)FUN_0040c180(actor->x,actor->y,I(0xbca2c8),I(0xbca2cc),&x,&y);
            actor->target_x=(int16_t)x;actor->target_y=(int16_t)y;
            actor->secondary_x=(int16_t)U(0xbca2c8);actor->secondary_y=(int16_t)U(0xbca2cc);
            FUN_004b3250(id);
        }
        finish();return;
    default:return;
    }
}
