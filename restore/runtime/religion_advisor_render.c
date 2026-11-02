#include "religion_advisor_render.h"
#include "religion_updates.h"
#include "city_updates.h"
#include "menu_building_overlay.h"
#include "sprite_render.h"
#include <stdint.h>
extern void FUN_00536820(int32_t,int32_t,int32_t,int32_t);
extern void FUN_004cd680(int32_t,int32_t,int32_t,int32_t,int32_t);
extern void FUN_004d62f0(uint32_t,uint32_t,int32_t,int32_t,uint32_t,uint32_t,int32_t,int32_t);
extern void FUN_004d6250(uint32_t,uint32_t,int32_t,int32_t,uint32_t,uint32_t,uint32_t,int32_t,int32_t);
extern void FUN_004d6160(uint32_t,uint32_t,int32_t,int32_t,uint32_t,uint32_t,uint32_t,int32_t,int32_t);
extern void FUN_004d65b0(int32_t,int32_t,int32_t,int32_t,int32_t,int32_t);
extern void FUN_004cce90(int32_t,uint32_t,const char *,int32_t,int32_t,int32_t,uint32_t,uint32_t);
extern void FUN_004ccc30(uint32_t,uint32_t,int32_t,int32_t,int32_t,uint32_t,uint32_t);
extern void FUN_004d6860(int32_t,uint32_t,const char *,int32_t,int32_t,uint32_t,uint32_t,int32_t,int32_t);
extern void FUN_004d63d0(uint32_t,int32_t,int32_t,int32_t,int32_t,uint32_t,int32_t,int32_t);
#include "pixel_access.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define I(v) ((int32_t)(uint32_t)(v))
#define EMPTY ((const char *)(uintptr_t)0x57c048u)
static uint32_t player_offset(void){return U(0xeaac28u)*0x49d8u;}
static int32_t value(uint32_t field){return I(U(field+player_offset()));}
static int32_t mood(uint32_t field,uint32_t god){return (int8_t)B(field+player_offset()+god);}
static int32_t x(uint32_t dx){return I(U(0xf53f0cu)+dx);}
static int32_t y(uint32_t dy){return I(U(0xf53f08u)+dy);}
static void label(uint32_t group,uint32_t item,uint32_t dx,uint32_t dy,uint32_t font)
{FUN_004d62f0(group,item,x(dx),y(dy),U(font),0,I(U(0xe38da8u)),I(U(0xe38da4u)));}
static void center(uint32_t group,uint32_t item,uint32_t dx,uint32_t dy,uint32_t width,uint32_t font)
{FUN_004d6250(group,item,x(dx),y(dy),width,U(font),0,I(U(0xe38da8u)),I(U(0xe38da4u)));}
static void paragraph(uint32_t group,uint32_t item,uint32_t dx,uint32_t dy,uint32_t width,uint32_t font)
{FUN_004d6160(group,item,x(dx),y(dy),width,U(font),0,I(U(0xe38da8u)),I(U(0xe38da4u)));}
static void aligned(int32_t amount,uint32_t dx,uint32_t dy)
{FUN_004cce90(amount,0x40,EMPTY,x(dx),y(dy),50,U(0xf56f9cu),0);}
static void query_number(uint32_t god,int32_t tier,uint32_t dx,uint32_t dy)
{
    /* Seven numeric-render arguments are already on the native stack when
       the genuine two-argument classification query executes. */
    int32_t px=x(dx),py=y(dy);uint32_t font=U(0xf56f9cu);
    uint32_t kind=FUN_004c06e0(god,tier);
    FUN_004cce90(I(U(0x705af0u+kind*4u)),0x40,EMPTY,px,py,50,font,0);
}
static void grid(uint32_t dx,uint32_t dy,int32_t width,int32_t height)
{FUN_004d65b0(x(dx),y(dy),width,height,I(U(0xe38da8u)),I(U(0xe38da4u)));}
static void box(uint32_t type,uint32_t dx,uint32_t dy,int32_t width,int32_t height,uint32_t selected)
{FUN_004d63d0(type,x(dx),y(dy),width,height,selected,I(U(0xe38da8u)),I(U(0xe38da4u)));}
static void shade(uint32_t dx,uint32_t dy,int32_t width,int32_t height)
{FUN_004cf090(x(dx),y(dy),width,height,0);}

/* Whole5292B0: phase/hover are DWORDs; festival eligibility is a WORD. */
void FUN_005292b0(void)
{
    uint32_t offset;
    if(I(U(0xf53f48u))<=5)return;
    offset=player_offset();if(U(0xea5d68u+offset))return;
    box(0,102,348,300,20,W(0xea6084u+offset)!=0&&U(0x7056c8u)==1);
}

/* Whole528F10: idle/queued festival descriptions, signed month arithmetic,
   and the real foreground renderer. All player/layout reloads are retained. */
void FUN_00528f10(void)
{
    int32_t state,months,limit;uint32_t item,base;
    grid(48,320,34,6);
    (void)FUN_0053e590(I(U(0xf55c5cu)+15u),x(460),y(323));
    U(0xe20778u)=0;
    FUN_004ccc30(8,4,value(0xea5bfcu),x(112),y(328),U(0xf56f9cu),0);
    label(0x3a,15,U(0xe20778u)+112u,328,0xf56f9cu);
    state=value(0xea5d68u);
    if(state){
        if(state==1){limit=2;base=10;}
        else if(state==2){limit=3;base=20;}
        else{limit=4;base=31;}
        months=value(0xea5d6cu);U(0xe20778u)=0;
        label(0x3a,months>limit?53u:34u,102,352,0xf56f9cu);
        months=value(0xea5d6cu);item=base+(uint32_t)months-1u;
        if(months==1)label(0x3a,54,U(0xe20778u)+102u,352,0xf56f9cu);
        else label(0xa0,(uint32_t)(I((uint32_t)months+U(0x78d7c0u))%12),U(0xe20778u)+102u,352,0xf56f9cu);
    }else if(value(0xea5d98u)&&value(0xea5d9cu)){
        center(0x3a,52,102,352,300,0xf56f9cu);
        months=value(0xea5bfcu);item=months<2?7u:8u+(months>6);
    }else{
        center(0x3a,16,102,352,300,0xf56f9cu);
        months=value(0xea5bfcu);
        item=months<2?0u:months<7?1u:months<13?2u:months<19?3u:months<25?4u:5u+(months>30);
    }
    paragraph(0x127,item,56,373,380,0xf56f9cu);
    FUN_005292b0();
}

/* Whole529530: preserve the third row's saved cursor and disable all
   three costs at treasury<=-5000, independently of the celebration flag. */
void FUN_00529530(void)
{
    uint32_t row,dy,saved_cursor=0;
    for(row=0,dy=224;row<3u;++row,dy+=30u){
        box(1,102,dy-8u,430,26,U(0x7056c8u)==6u+row);
        U(0xe20778u)=0;label(0x3a,31u+row,110,dy,0xf56f8cu);
        if(row==2)saved_cursor=U(0xe20778u);
        FUN_004ccc30(8,0,value(0xea5d74u+row*4u),x(U(0xe20778u)+110u),y(dy),U(0xf56f8cu),0);
    }
    U(0xe20778u)+=saved_cursor;
    FUN_004d6860(value(0xea5d80u),0x40,EMPTY,x(U(0xe20778u)+120u),y(284),U(0xf56f8cu),0,
                 I(U(0xe38da8u)),I(U(0xe38da4u)));
    label(0x17,15,U(0xe20778u)+120u,284,0xf56f8cu);
    if(value(0xea187cu)<=-5000){shade(104,218,426,22);shade(104,248,426,22);shade(104,278,426,22);}
    else if(value(0xea5d84u))shade(104,278,426,22);
}

/* Whole529320: each unavailable god advances a matching selection before
   drawing; the following popup title uses the resulting full signed value. */
void FUN_00529320(void)
{
    uint32_t god,dx,offset;int32_t selected;
    FUN_00536820(x(48),y(48),34,20);
    for(god=0,dx=0;god<5u;++god,dx+=100u){
        if(W(0x7848e2u+god*2u)){
            if((uint32_t)value(0xea5d60u)==god){
                box(1,dx+66u,92,90,100,1);
                (void)FUN_0053e590(I(U(0xf55c5cu)+21u+god),x(dx+70u),y(96));
            }else (void)FUN_0053e590(I(U(0xf55c5cu)+16u+god),x(dx+70u),y(96));
        }else{
            offset=player_offset();if(U(0xea5d60u+offset)==god)++U(0xea5d60u+offset);
            (void)FUN_0053e590(I(U(0xf55c5cu)+16u+god),x(dx+70u),y(96));
            shade(dx+75u,101,72,82);
        }
    }
    selected=value(0xea5d60u);
    if(selected<5)center(0x3a,(uint32_t)selected+25u,48,60,544,0xf56fb4u);
    FUN_00529530();U(0xe20778u)=0;
    label(0x3a,(uint32_t)value(0xea5d64u)+30u,180,322,0xf56f8cu);
}

/* Whole5298E0: full religion page, all five gods and diagnostic rows,
   genuine temple counts/least-happy query, and complete festival composition. */
void FUN_005298e0(void)
{
    uint32_t phase=5u-U(0xf53f48u),god,dy,j,item,offset;int32_t selected,need;
    static const uint32_t columns[4]={260,330,390,460};
    U(0xf53f0cu)=U(0xe38da8u);U(0xf53f08u)=U(0xe38da4u);
    U(0xf53f04u)=40;U(0xf53f00u)=27;
    if(I(phase)>=5)return;
    if(I(phase)>0){FUN_004cd680(x(phase*16u),y(phase*8u),I((40u-phase*2u)*16u),I((27u-phase*2u)*16u),0);return;}
    FUN_00536820(I(U(0xe38da8u)),I(U(0xe38da4u)),40,27);
    (void)FUN_0053e590(I(U(0xf5681cu)+9u),x(10),y(10));
    label(0x3b,0,60,12,0xf56fb4u);center(0x3b,5,180,32,65,0xf56f8cu);
    label(0x1c,150,325,46,0xf56f8cu);label(0x3b,1,255,46,0xf56f8cu);
    center(0x3b,2,180,46,65,0xf56f8cu);label(0x3b,3,490,46,0xf56f8cu);
    U(0xe20778u)=0;center(0x3b,6,390,18,65,0xf56f8cu);
    center(0x3b,8,390,32,65,0xf56f8cu);center(0x3b,7,390,46,65,0xf56f8cu);
    grid(32,60,36,13);U(0xeadd44u)=0;
    for(god=0,dy=0;god<5u;++god,dy+=40u){
        FUN_004d6200(0x9d,god,(const int16_t *)(uintptr_t)(0x5c59e8u+god*28u),U(0xf56f9cu),0,0,0,
                     I(U(0xe38da8u)),I(U(0xe38da4u)));
        if(!U(0xeadd44u)){
            label(0xbb,(uint32_t)(int32_t)(int16_t)W(0x7848e2u+god*2u),100,dy+67u,0xf56f9cu);
            label(0x9e,god,40,dy+87u,0xf56f94u);
            if(W(0x7848e2u+god*2u)){
                query_number(god,0,260,dy+66u);query_number(god,2,330,dy+66u);
                aligned(value(0xea5c50u+god*4u),390,dy+66u);
                U(0xe20778u)=0;
                label(0x3b,(uint32_t)(mood(0xea5c0au,god)/10)+20u,450,dy+66u,0xf56f9cu);
                for(j=0;I(j)<mood(0xea5c14u,god)/10;++j)
                    (void)FUN_0053e590(I(U(0xf56794u)),x(U(0xe20778u)+450u+j*10u),y(dy+62u));
                for(j=0;I(j)<mood(0xea6086u,god)/10;++j)
                    (void)FUN_0053e590(I(U(0xf56464u)),x(U(0xe20778u)+450u+j*10u),y(dy+62u));
            }else for(j=0;j<4u;++j)center(0x3b,37,columns[j],dy+66u,50,0xf56f9cu);
            if(W(0x784db0u+god*2u)==1)query_number(god,1,200,dy+66u);
            else center(0x3b,37,200,dy+66u,50,0xf56f9cu);
        }else{
            aligned(mood(0xea5c14u,god),210,dy+66u);aligned(mood(0xea6086u,god),245,dy+66u);
            aligned(mood(0xea5c00u,god),310,dy+66u);aligned(mood(0xea5c0au,god),360,dy+66u);
        }
    }
    (void)FUN_004e9f50();offset=player_offset();selected=I(U(0xea5c78u+offset));
    if(selected>0&&(int8_t)B(0xea5c13u+offset+(uint32_t)selected)>4)item=(uint32_t)selected+14u;
    else{
        need=I(U(0xea5c90u+offset));
        if(need==1)item=9u+(U(0xea5bccu+offset)!=0);
        else if(need==2)item=11;
        else if(need==3)item=12;
        else item=U(0xea5bccu+offset)?(uint32_t)selected+14u:13u;
    }
    paragraph(0x3b,item,60,272,512,0xf56f8cu);FUN_00528f10();
}
