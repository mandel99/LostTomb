#include "service_advisor_render.h"
#include "menu_list_scroll.h"
#include "menu_training_render.h"
#include "sprite_render.h"
#include "religion_updates.h"
#include "city_updates.h"
extern void FUN_00536820(int32_t,int32_t,int32_t,int32_t);
extern void FUN_004d62f0(uint32_t,uint32_t,int32_t,int32_t,uint32_t,uint32_t,int32_t,int32_t);
extern void FUN_004d6250(uint32_t,uint32_t,int32_t,int32_t,uint32_t,uint32_t,uint32_t,int32_t,int32_t);
extern void FUN_004d6160(uint32_t,uint32_t,int32_t,int32_t,uint32_t,uint32_t,uint32_t,int32_t,int32_t);
extern void FUN_004d6860(int32_t,uint32_t,const char *,int32_t,int32_t,uint32_t,uint32_t,int32_t,int32_t);
extern void FUN_004cce90(int32_t,uint32_t,const char *,int32_t,int32_t,int32_t,uint32_t,uint32_t);
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define I(v) ((int32_t)(uint32_t)(v))
#define EMPTY ((const char *)(uintptr_t)0x57c048u)
static int32_t x(uint32_t dx){return I(U(0xf53f0cu)+dx);}
static int32_t y(uint32_t dy){return I(U(0xf53f08u)+dy);}
static int32_t value(uint32_t field){return I(U(field+U(0xeaac28u)*0x49d8u));}
static void label(uint32_t group,uint32_t item,uint32_t dx,uint32_t dy,uint32_t font)
{FUN_004d62f0(group,item,x(dx),y(dy),U(font),0,I(U(0xe38da8u)),I(U(0xe38da4u)));}
static void paragraph(uint32_t group,uint32_t item,uint32_t dx,uint32_t dy)
{FUN_004d6160(group,item,x(dx),y(dy),512,U(0xf56f8cu),0,I(U(0xe38da8u)),I(U(0xe38da4u)));}
static void centered(uint32_t group,uint32_t item,uint32_t dy)
{FUN_004d6250(group,item,x(420),y(dy),200,U(0xf56f9cu),0,I(U(0xe38da8u)),I(U(0xe38da4u)));}
static void aligned(int32_t amount,uint32_t dx,uint32_t dy,int32_t width,uint32_t font)
{FUN_004cce90(amount,0x40,EMPTY,x(dx),y(dy),width,U(font),0);}
static void number(int32_t amount,uint32_t dy)
{FUN_004d6860(amount,0x40,EMPTY,x(280),y(dy),U(0xf56f9cu),0,I(U(0xe38da8u)),I(U(0xe38da4u)));}
static int opening(uint32_t height,uint32_t icon,uint32_t group)
{
    uint32_t phase=5u-U(0xf53f48u);
    U(0xf53f0cu)=U(0xe38da8u);U(0xf53f08u)=U(0xe38da4u);
    U(0xf53f04u)=40;U(0xf53f00u)=height;
    if(I(phase)>=5)return 0;
    if(I(phase)>0){
        FUN_004cd680(I(U(0xe38da8u)+phase*16u),I(U(0xe38da4u)+phase*8u),
                     I((40u-phase*2u)*16u),I((height-phase*2u)*16u),0);return 0;
    }
    FUN_00536820(I(U(0xe38da8u)),I(U(0xe38da4u)),40,I(height));
    (void)FUN_0053e590(I(U(0xf5681cu)+icon),x(10),y(10));
    label(group,0,60,12,0xf56fb4u);return 1;
}
static uint32_t coverage_text(int32_t coverage,uint32_t empty,uint32_t base,uint32_t full)
{return coverage==0?empty:coverage<100?(uint32_t)(coverage/10)+base:full;}

/* Whole527BF0: full active-building counts, three DWORD coverage records,
   native population percentage, and the real multiline text cursors. */
void FUN_00527bf0(void)
{
    static const uint32_t kinds[3]={206,49,47},coverage_indices[3]={10,8,11},items[3]={39,40,42};
    uint32_t index,dy,item,height;int32_t population,coverage,risk;
    if(!opening(22,6,0x38))return;
    item=value(0xea1894u)<200?16u:(uint32_t)(value(0xea1888u)/10)+17u;
    paragraph(0x38,item,60,46);
    label(0x38,3,180,94,0xf56f8cu);label(0x38,4,290,94,0xf56f8cu);label(0x38,5,478,94,0xf56f8cu);
    FUN_004d65b0(x(32),y(108),36,4,I(U(0xe38da8u)),I(U(0xe38da4u)));
    for(index=0,dy=112;index<3u;++index,dy+=20u){
        label(0x38,items[index],40,dy,0xf56f9cu);
        aligned(I(U(0x70573cu+kinds[index]*4u)),150,dy,100,0xf56f9cu);
        coverage=I(U(0x78d5f0u+coverage_indices[index]*4u));U(0xe20778u)=0;
        population=coverage?value(0xea1894u):0;
        if(coverage&&population){
            /* Eight already-pushed numeric arguments belong to4D6860,
               not to the two-argument percentage query in between. */
            int32_t px=x(280),py=y(dy),origin_x=I(U(0xe38da8u)),origin_y=I(U(0xe38da4u));
            uint32_t font=U(0xf56f9cu);
            int32_t amount=FUN_004cde40(population,coverage);
            FUN_004d6860(amount,0x40,EMPTY,px,py,font,0,origin_x,origin_y);
        }else number(0,dy);
        label(0x38,6,U(0xe20778u)+280u,dy,0xf56f9cu);
        coverage=I(U(0x78d5f0u+coverage_indices[index]*4u));
        centered(0x38,coverage_text(coverage,2,43,53),dy);
    }
    U(0xe20778u)=0;U(0xe20774u)=0;
    aligned(I(U(0x7057f4u)),60,194,5,0xf56f8cu);
    paragraph(0x38,59,U(0xe20778u)+57u,194);
    U(0xe20778u)=0;risk=value(0xea5fc0u);
    item=risk==0?54u:risk<3?55u:risk<6?56u:risk<=8?57u:58u;
    height=U(0xe20774u);paragraph(0x38,item,60,height+194u);
    U(0xe20774u)=height;paragraph(0x38,60,U(0xe20778u)+57u,height+194u);
    U(0xe20778u)=0;aligned(value(0xea41e0u),60,250,5,0xf56f8cu);
    paragraph(0x38,61,U(0xe20778u)+57u,250);
    switch((uint32_t)value(0xea5c8cu)){
    case 1:item=7u+(value(0xea5bb8u)!=0);break;
    case 2:item=9u+(value(0xea5bb4u)!=0);break;
    case 3:item=11u+(value(0xea5bbcu)!=0);break;
    case 4:item=13u+(value(0xea5fb8u)!=0);break;
    default:item=15;break;
    }
    paragraph(0x38,item,60,275);
}

/* Whole528360: native school/library capacities wrap at32 bits. Coverage
   and requirement decisions use full signed DWORD comparisons. */
void FUN_00528360(void)
{
    uint32_t index,dy,item,field;int32_t school,library;
    if(!opening(16,7,0x39))return;
    U(0xe20778u)=0;
    label(0x39,1,180,86,0xf56f8cu);label(0x39,2,290,86,0xf56f8cu);label(0x39,3,478,86,0xf56f8cu);
    FUN_004d65b0(x(32),y(100),36,3,I(U(0xe38da8u)),I(U(0xe38da4u)));
    for(index=0,dy=105;index<2u;++index,dy+=20u){
        field=0x705808u+index*8u;
        label(0x39,index+28u,40,dy,0xf56f9cu);
        aligned(I(U(field)),150,dy,100,0xf56f9cu);
        U(0xe20778u)=0;number(I(U(field)*(index?800u:300u)),dy);
        label(0x39,index?6u:4u,U(0xe20778u)+280u,dy,0xf56f9cu);
        centered(0x39,coverage_text(I(U(0x78d604u+index*4u)),7,8,18),dy);
    }
    switch((uint32_t)value(0xea5c94u)){
    case 1:item=19u+(value(0xea5b88u)!=0);break;
    case 2:item=21u+(value(0xea5b8cu)!=0);break;
    case 3:item=23;break;
    default:
        if(!value(0xea5b88u))item=24;
        else{
            school=I(U(0x78d604u));library=I(U(0x78d608u));
            if(!value(0xea5b8cu))item=25u+(school<100);
            else item=school<=library?26u:27u;
        }
        break;
    }
    paragraph(0x39,item,60,180);
}

/* Whole528950. Both genuine city refreshes precede even the opening gate;
   each staffed count is captured before its row's presentation callbacks. */
void FUN_00528950(void)
{
    static const uint32_t kinds[5]={31,30,33,32,226};
    static const uint32_t items[5]={47,48,49,50,56};
    static const uint32_t fields[5]={0xea5bd0u,0xea5bd8u,0xea5be0u,0xea5be8u,0xea5d38u};
    static const uint32_t capacities[5]={400,700,1200,5000,7500};
    uint32_t index,dy,item;int32_t staffed,coverage,count;
    FUN_004e9890(0);FUN_004550e0();
    if(!opening(19,8,0x3a))return;
    label(0x3a,1,180,42,0xf56f8cu);label(0x3a,55,180,62,0xf56f8cu);
    label(0x3a,2,260,62,0xf56f8cu);label(0x3a,3,340,62,0xf56f8cu);label(0x3a,4,480,62,0xf56f8cu);
    FUN_004d65b0(x(32),y(76),36,8,I(U(0xe38da8u)),I(U(0xe38da4u)));
    for(index=0,dy=80;index<5u;++index,dy+=24u){
        staffed=value(fields[index]);label(0x3a,items[index],40,dy,0xf56f9cu);
        aligned(I(U(0x70573cu+kinds[index]*4u)),150,dy,100,0xf56f9cu);
        aligned(staffed,230,dy,100,0xf56f9cu);U(0xe20778u)=0;
        FUN_004d6860(I(U(0x70573cu+kinds[index]*4u)*capacities[index]),0x40,EMPTY,
                     x(340),y(dy),U(0xf56f9cu),0,I(U(0xe38da8u)),I(U(0xe38da4u)));
        label(0x3a,5,U(0xe20778u)+340u,dy,0xf56f9cu);
        coverage=I(U(0x78d5f0u+index*4u));
        FUN_004d6250(0x3a,coverage_text(coverage,35,36,46),x(470),y(dy),100,
                     U(0xf56f9cu),0,I(U(0xe38da8u)),I(U(0xe38da4u)));
    }
    if(value(0xea5b7cu)<value(0xea5bf8u))item=10;
    else if(value(0xea5b7cu)==0)item=7u+(value(0xea5bf4u)!=0);
    else{
        count=value(0xea5bf0u);
        item=count==0?9u:count==1?11u:count==2?12u:count==3?13u:14u;
    }
    paragraph(0x3a,item,60,220);
}
