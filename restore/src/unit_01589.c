#include "placement_preview.h"
#include "temple_upgrade.h"
#include "menu_prepare_integer.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define I(v) ((int32_t)(uint32_t)(v))
static uint32_t player_offset(void){return U(0xeaac28u)*0x49d8u;}

/* Whole4C2F50: additions211/212 to a linked temple complex. Counts,
   projections and sprite ids are DWORDs; map ownership, kind, links and
   tint are WORDs. Cache the same rotation/base inputs as the native code. */
void restore_temple_complex_upgrade_preview(int32_t tool)
{
    int32_t screen_x=0,screen_y=0,rotation=I(U(0xbca2e0u))/2,count=I(U(0x5cfce0u)),root,index,type;
    uint32_t p,x,y,base,table,cell,terrain,sprite_base,map_base,record,image,variant,dx=0,dy=0;
    int blocked;
    FUN_004f0ba0(I(U(0xbca304u)),I(U(0xbca308u)),&screen_x,&screen_y);
    x=(uint32_t)screen_x;y=(uint32_t)screen_y+U(0x70e44cu);
    p=player_offset();blocked=W(0xea5ce8u+p)==0;base=U(0xbca300u);
    root=I(FUN_00475b50((uint32_t)(int32_t)(int16_t)W(0xb4e6fcu+base*2u)));
    record=(uint32_t)root*264u;type=(int32_t)(int16_t)W(0x7999e8u+record);
    if(type<65||type>69)blocked=1;
    p=player_offset();if((int16_t)W(0xea5cfau+p)<2)blocked=1;
    if(FUN_004c52d0(I(U(0xea5f9cu+p)),tool==211?1u:2u))blocked=1;
    if(I(U(0xea187cu+player_offset()))<=-5000)blocked=1;
    if(blocked){
        table=0x5cfe10u+(uint32_t)rotation*144u;
        for(index=0;index<count;++index){
            uint32_t offset=(uint32_t)index*8u;
            uint32_t px=U(0x5cfcf0u+offset)+x,py=U(0x5cfcf4u+offset)-U(0x70e44cu)+y;
            cell=base+U(table+(uint32_t)index*4u);
            terrain=(uint32_t)(int32_t)(int16_t)W(0xb350dcu+cell*2u)|(U(0xaf598cu+cell*4u)&0xeefed77fu);
            (void)FUN_0053f4f0(I(U(0xf56be4u)),I(px),I(py),(blocked||terrain)?0xfffff863u:0x1fe3u);
        }
        W(0x70e120u)=0;return;
    }
    if(U(0xbca2e0u)==2u)dx=2;
    else if(U(0xbca2e0u)==4u){dx=2;dy=2;}
    else if(U(0xbca2e0u)==6u)dy=2;
    W(0x70e120u)=0x1fe3u;
    sprite_base=U(tool==211?0xf56d8cu:0xf56d84u);map_base=U(0xbca2d8u);
    while(root){
        record=(uint32_t)root*264u;
        cell=map_base+(uint32_t)(int32_t)(int16_t)W(0x7999deu+record)
            +(uint32_t)(int32_t)(int16_t)W(0x7999e0u+record)*228u;
        image=U(0xb7482cu+cell*4u);
        if(image==sprite_base||image==sprite_base+3u){
            variant=tool==211?(image==sprite_base?2u:3u):(image==sprite_base?0u:1u);
            FUN_004f0ba0(I((uint32_t)(int32_t)(int16_t)W(0x7999deu+record)+dx),
                I((uint32_t)(int32_t)(int16_t)W(0x7999e0u+record)+dy),&screen_x,&screen_y);
            (void)FUN_0053e590(I(U(0xf56dacu)+variant),screen_x,screen_y);
            (void)FUN_0053f250(I(U(0xf56dacu)+variant),screen_x,screen_y);
            break;
        }
        root=(int32_t)(int16_t)W(0x799a12u+record);
    }
    W(0x70e120u)=0;
}
void FUN_004c2f50(int32_t tool){restore_temple_complex_upgrade_preview(tool);}
