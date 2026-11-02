#include "editor_event_render.h"
#include "event_description.h"
#include "editor_event_list_actions.h"
#include "editor_event_detail_actions.h"
#include "menu_resource_render.h"
#include "menu_event_actions.h"
#include "menu_record_queries.h"
#include "menu_building_overlay.h"
#include "ui_rect_text.h"
#include <stdlib.h>
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define S16(a) ((int32_t)(int16_t)W(a))
#define RECT(a) ((const int16_t *)(uintptr_t)(uint32_t)(a))
#define SUFFIX ((const char *)0x57c048u)
extern void FUN_00536820(int32_t,int32_t,int32_t,int32_t);
extern void FUN_004cd1a0(uint32_t,const int16_t *,uint32_t,int32_t,int32_t);
extern void FUN_004cd240(uint32_t,uint32_t,const int16_t *,uint32_t,uint32_t,int32_t,int32_t);
extern void FUN_004ccbb0(uint32_t,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t);
extern void FUN_004ccb30(uint32_t,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t);
extern void FUN_004cce00(uint32_t,uint32_t,const char *,uint32_t,uint32_t,uint32_t,uint32_t);

/* Complete existing C member graphs, with an explicit receiver and one
   optional DWORD argument. The reviewed query paths need at most76 local
   bytes;128 words leave bounded space for both native return/argument slots. */
static uint32_t property(uint32_t self,uint32_t target,int32_t argument)
{
    uint32_t stack[128]={0},registers[8]={0},flags=2,sp=(uint32_t)(uintptr_t)(stack+96);
    uint32_t cleanup=(target==0x449550u||target==0x4496d0u||target==0x44c0d0u)?4u:0u;
    registers[1]=self;registers[4]=sp;stack[97]=(uint32_t)argument;
    if(target==0x449370u||target==0x449380u)
        restore_resource_render_raw(target,registers,&flags);
    else if(target==0x4496d0u||target==0x44c0d0u)
        restore_event_actions_raw(target,registers,&flags);
    else restore_event_detail_raw(target,registers,&flags);
    if(registers[4]!=sp+4u+cleanup)abort();
    return registers[0];
}
static int has(uint32_t self,uint32_t target){return (property(self,target,0)&255u)!=0;}
static void panel(uint32_t rect,uint32_t selection,uint32_t style)
{FUN_004cd1a0(style,RECT(rect),U(0x7056c8u)==selection,0,0);}
static void box_text(uint32_t rect,uint32_t group,uint32_t entry,int32_t dy)
{FUN_004cd240(group,entry,RECT(rect),U(0xf56f8cu),0,4,dy);}
static void label(uint32_t entry,uint32_t x,uint32_t y)
{FUN_004ccbb0(44,entry,U(0xe38da8u)+x,U(0xe38da4u)+y,U(0xf56f8cu),0);}
static void clipped(int32_t value,uint32_t separator,uint32_t rect)
{FUN_004cd2e0((uint32_t)value,separator,SUFFIX,RECT(rect),U(0xf56f8cu),0,4,7);}
static void range(uint32_t self,uint32_t offset,int32_t *low,int32_t *high)
{
    *low=S16(self+offset);
    if(*low<0){*low=S16(self+offset+2u);*high=S16(self+offset+4u);}
    else if(S16(self+offset+2u)<0)*high=*low;
    else *low=*high=-1;
}
static void range_row(uint32_t entry,uint32_t x,uint32_t y,int32_t low,int32_t high,
                      uint32_t first,uint32_t second,uint32_t first_selection)
{
    label(entry,x,y);panel(first,first_selection,0);clipped(low,64,first);
    label(0x91u,x==0xa0u?0x180u:0x1e8u,y);
    panel(second,first_selection+1u,0);clipped(high,64,second);
}
static void date_row(uint32_t self,uint32_t entry,uint32_t rect,uint32_t selection,
                     uint32_t y,int32_t value)
{
    FUN_004ccb30(44,entry,U(0xe38da8u)+40u,U(0xe38da4u)+y,100,U(0xf56f8cu),0);
    panel(rect,selection,0);clipped(value,43,rect);
    if(B(self+40u)&1u)box_text(rect,44,0x96u,24);
    else FUN_004cd340(1,(uint32_t)S16(0x7848d8u)+(uint32_t)value,RECT(rect),U(0xf56f8cu),0,4,24);
}
static void dependency_row(uint32_t self,uint32_t title,uint32_t offset,uint32_t y,
                            uint32_t rect,uint32_t selection,uint32_t action_rect,uint32_t action_offset)
{
    uint32_t group=44,entry=0x69u,target;
    label(title,0x8cu,y);panel(rect,selection,0);
    FUN_004cd290((uint32_t)S16(self+offset),64,SUFFIX,RECT(rect),U(0xf56f8cu),0,4,7);
    if(property(self,0x4496d0u,S16(self+offset))&255u){
        target=property(self,0x44c0d0u,S16(self+offset));group=156;entry=B(target+2u);
    }
    FUN_004ccbb0(group,entry,U(0xe38da8u)+0x19au,U(0xe38da4u)+y,U(0xf56f8cu),0);
    panel(action_rect,selection==14u?27u:selection+13u,0);
    box_text(action_rect,299,(uint32_t)B(self+action_offset)+7u,7);
}

/* Entire44AAF0,3873 bytes/1237 instructions. The selected record remains
   cached even when removal or a panel callback changes global selection. */
void restore_editor_event_detail_draw(void)
{
    uint32_t selected=U(0x78a61cu),self,entry,font,i,rect;
    int32_t low,high,group;int16_t choices[3];
    if((int32_t)selected<0||(int32_t)selected>=(int32_t)U(0x785d70u)){
        FUN_00448cf0();return;
    }
    self=0x785d74u+selected*124u;
    if(!self){FUN_00448cf0();return;}
    if(!B(self+2u)){
        (void)restore_event_list_erase_00448e30(0x785d70u,(int32_t)selected);
        FUN_00448cf0();
    }
    if(has(self,0x449690u)&&B(self+54u)>=5u)B(self+54u)=0;
    FUN_00536820((int32_t)U(0xe38da8u),(int32_t)U(0xe38da4u),38,25);
    U(0xe20778u)=0;
    FUN_004ccbb0(44,0x8bu,U(0xe38da8u)+40u,U(0xe38da4u)+20u,U(0xf56fb4u),0);
    FUN_004cce00((uint32_t)S16(self),64,SUFFIX,U(0xe20778u)+U(0xe38da8u)+40u,
                  U(0xe38da4u)+20u,U(0xf56fb4u),0);
    panel(0x5d9e20u,1,1);box_text(0x5d9e20u,156,B(self+2u),7);
    if(has(self,0x4496b0u)){
        panel(0x5da050u,21,1);box_text(0x5da050u,44,0xa2u-(B(self+86u)!=0),7);
    }
    panel(0x5d9ffcu,18,1);
    entry=(B(self+40u)&1u)?0x9fu:(B(self+40u)&2u)?0x9eu:(B(self+40u)&16u)?0xa0u:0x9du;
    box_text(0x5d9ffcu,44,entry,7);
    if(has(self,0x449640u)){
        panel(0x5da0c0u,25,1);font=U(0xf56f8cu);entry=B(self+96u);
        group=(int32_t)property(self,0x449b30u,0);
        FUN_004cd240((uint32_t)group,entry,RECT(0x5da0c0u),font,0,4,7);
    }
    if(!(U(self+40u)&16u)){
        if(!(B(self+40u)&1u)){
            panel(0x5d9e3cu,2,0);if(B(self+3u)<12u)box_text(0x5d9e3cu,25,B(self+3u),7);
        }
        range(self,22,&low,&high);
        date_row(self,0x8du,0x5d9e58u,3,100,low);
        date_row(self,0x8eu,0x5d9e74u,4,173,high);
    }
    if(has(self,0x449370u)){
        range(self,30,&low,&high);entry=0x8fu+(uint32_t)has(self,0x449380u);
        range_row(entry,0xa0u,104,low,high,0x5d9f38u,0x5d9f54u,11);
    }
    if(FUN_00449620((const RestoreTownRecord *)(uintptr_t)self)){
        entry=0x92u+(B(self+2u)==3u);range(self,14,&low,&high);
        range_row(entry,0x153u,148,low,high,0x5d9ee4u,0x5d9f00u,8);
    }
    if(has(self,0x449630u)){
        range(self,90,&low,&high);
        range_row(0x20u,0x153u,186,low,high,0x5da06cu,0x5da088u,22);
    }
    if(has(self,0x4493b0u)){
        choices[0]=(int16_t)W(self+6u);choices[1]=(int16_t)W(self+8u);choices[2]=(int16_t)W(self+10u);
        for(i=0,rect=0x5d9e90u;i<3u;++i,rect+=28u)panel(rect,i+5u,0);
        if((int32_t)property(self,0x4495c0u,0)>=0){
            for(i=0,rect=0x5d9e90u;i<3u;++i,rect+=28u){
                if(property(self,0x449550u,choices[i])&255u){
                    font=U(0xf56f8cu);group=(int32_t)property(self,0x4495c0u,0);
                    FUN_004cd240((uint32_t)group,(uint32_t)(int32_t)choices[i],RECT(rect),font,0,4,7);
                }
            }
        }
        if(B(self+2u)==1u)label(0xd5u,0x122u,168);
    }
    if(has(self,0x4495d0u)){
        panel(0x5da018u,19,0);U(0xe20778u)=0;
        FUN_004cd290((uint32_t)S16(self+44u),64,SUFFIX,RECT(0x5da018u),U(0xf56f8cu),0,4,7);
        FUN_004cd1f0(44,S16(self+44u)==1?0x8cu:0x96u,RECT(0x5da018u),U(0xf56f8cu),0,
                      (int32_t)(U(0xe20778u)+4u),4);
    }
    if(has(self,0x449630u)){
        panel(0x5da034u,20,0);FUN_004cd1f0(36,(uint32_t)S16(self+56u),RECT(0x5da034u),U(0xf56f8cu),0,4,7);
    }
    if(has(self,0x449660u)){
        label(0xa3u,0x184u,218);panel(0x5da0dcu,26,0);clipped(B(self+54u),64,0x5da0dcu);
    }
    if(has(self,0x449690u)){
        label(0xd4u,0x184u,218);panel(0x5da0dcu,26,0);box_text(0x5da0dcu,157,B(self+54u),7);
    }
    if(has(self,0x4495f0u))
        dependency_row(self,has(self,0x4496b0u)?0x97u:0x9cu,36,248,0x5d9f8cu,14,0x5da0f8u,106);
    if(has(self,0x4496b0u))
        dependency_row(self,0x98u,38,280,0x5d9fa8u,15,0x5da114u,107);
    if(has(self,0x4496b0u))
        dependency_row(self,B(self+2u)==23u?0x9au:0x99u,82,312,0x5d9fc4u,16,0x5da130u,108);
    if(has(self,0x449600u))
        dependency_row(self,0x9bu,84,344,0x5d9fe0u,17,0x5da14cu,109);
}
void FUN_0044aaf0(void){restore_editor_event_detail_draw();}
