#include "family_menu.h"
#include "menu_building_overlay.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define POINT(a) ((const int16_t *)(uintptr_t)(uint32_t)(a))
extern void FUN_00536820(int32_t,int32_t,int32_t,int32_t);
extern void FUN_004cd1a0(uint32_t,const int16_t *,uint32_t,int32_t,int32_t);
extern void FUN_004d6380(uint32_t,uint32_t,const int16_t *,uint32_t,uint32_t,int32_t,int32_t,int32_t,int32_t);
/* Complete520D00:42 catalog rows plus close, with two unselectable headings.
   Native hover has priority over the selected-name highlight. */
void restore_family_draw_name_choices(void)
{
    uint32_t resolution=U(0xe38e68u),x,y,rect,index,font;
    if(resolution==1u){x=110;y=30;}
    else if(resolution==2u){x=190;y=90;}
    else{x=302;y=174;}
    FUN_00536820((int32_t)x,(int32_t)y,26,28);
    for(rect=0x5c4358u,index=0;rect<0x5c47f0u;rect+=28u,++index){
        if(rect==0x5c4358u||rect==0x5c45a4u)font=U(0xf56f8cu);
        else if(U(0x7056c8u)==index+1u)font=U(0xf56facu);
        else if(U(0x6059fcu)==index)font=U(0xf56fa4u);
        else font=U(0xf56f8cu);
        FUN_004d6200(197,index,POINT(rect),font,0,115,30,(int32_t)U(0xe38da8u),(int32_t)U(0xe38da4u));
    }
    FUN_004cd1a0(0,POINT(0x5c47f0u),U(0x7056c8u)==43u,110,30);
    FUN_004d6380(18,3,POINT(0x5c47f0u),U(0xf56f8cu),0,114,34,(int32_t)U(0xe38da8u),(int32_t)U(0xe38da4u));
}
void FUN_00520d00(void){restore_family_draw_name_choices();}
