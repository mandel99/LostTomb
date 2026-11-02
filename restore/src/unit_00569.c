#include "placement_preview.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define I(v) ((int32_t)(uint32_t)(v))

static int complex_blocked(uint32_t tile,int actors)
{
    uint32_t object;
    if(U(0xaf598cu+tile*4u)&0xeeffd77fu)return 1;
    object=U(0xd05720u+tile*4u);if(object&&B(object+24u)==0)return 1;
    return actors&&W(0xb350dcu+tile*2u)!=0;
}
static void complex_second(uint32_t rotation,uint32_t sx,uint32_t sy,
                           uint32_t height,int invalid,uint32_t *x,uint32_t *y)
{
    *x=U(0x5d2804u+rotation*8u)*U(0x70e45cu)+U(0x5d27e4u+rotation*8u)*U(0x70e458u)+U(0x5d2824u+rotation*4u)+sx;
    *y=U(0x5d27e8u+rotation*8u)*U(0x70e448u)+sy+
        (U(0x5d2808u+rotation*8u)-(invalid?0u:1u))*height;
}
static void complex_pair(uint32_t offset,uint32_t x,uint32_t y)
{
    /* Each call reloads the image base, as does the native member sequence. */
    (void)FUN_0053e590(I(U(0xf5662cu)+offset),I(x),I(y));
    (void)FUN_0053f250(I(U(0xf5662cu)+offset),I(x),I(y));
}
static void complex_decoration(uint32_t x,uint32_t primary_y)
{
    uint32_t kind=U(0x6059e8u),offset;
    if(kind==40u)offset=3;else if(kind==45u)offset=4;else if(kind==44u)offset=2;else return;
    (void)FUN_0053e590(I(U(0xf5662cu)+offset),I(U(0x70e45cu)+4u+x),I(U(0x70e44cu)-5u+primary_y));
}

/* Whole43D720: complex building preview, both footprint groups and painter
   orders. Counts/coordinates are DWORDs, actor cells/tint WORDs, objects are
   full pointers. Table offsets are native byte offsets, scaled only once. */
void FUN_0043d720(void)
{
    int32_t screen_x=0,screen_y=0,count,second_count;
    uint32_t sx,sy,height,rotation,table,base,second_base,i,x,y;
    int all_invalid,blocked;
    FUN_004f0ba0(I(U(0xbca304u)),I(U(0xbca308u)),&screen_x,&screen_y);
    height=U(0x70e44cu);sx=(uint32_t)screen_x;sy=(uint32_t)screen_y+height;
    all_invalid=I(U(0xbcb9b4u))>=6||I(U(0xea187cu+U(0xeaac28u)*0x49d8u))<=-5000;
    blocked=all_invalid;
    count=I(U(0x5cfcd4u+U(0x5d0b50u)*4u));rotation=(uint32_t)(I(U(0xbca2e0u))/2);
    table=0x5cfe10u+rotation*144u;
    for(i=0;I(i)<count;++i)if(complex_blocked(U(table+i*4u)+U(0xbca300u),1))blocked=1;
    second_base=U(0x5cdf78u+rotation*4u)+U(0xbca300u);
    second_count=I(U(0x5cfcd4u+U(0x5d0b14u)*4u));
    for(i=0;I(i)<second_count;++i)if(complex_blocked(U(table+i*4u)+second_base,1))blocked=1;
    if(blocked){
        base=U(0xbca300u);
        for(i=0;I(i)<count;++i){
            x=U(0x5cfcf0u+i*8u)+sx;y=U(0x5cfcf4u+i*8u)-height+sy;
            (void)FUN_0053f4f0(I(U(0xf56be4u)),I(x),I(y),
                (complex_blocked(U(table+i*4u)+base,0)||all_invalid)?0xfffff863u:0x1fe3u);
            height=U(0x70e44cu);
        }
        rotation=(uint32_t)(I(U(0xbca2e0u))/2);
        second_base=U(0x5cdf78u+rotation*4u)+U(0xbca300u);
        complex_second(rotation,sx,sy,height,1,&x,&y);
        second_count=I(U(0x5cfcd4u+U(0x5d0b14u)*4u));table=0x5cfe10u+rotation*144u;
        for(i=0;I(i)<second_count;++i){
            uint32_t px=U(0x5cfcf0u+i*8u)+x,py=y-height+U(0x5cfcf4u+i*8u);
            (void)FUN_0053f4f0(I(U(0xf56be4u)),I(px),I(py),
                (complex_blocked(U(table+i*4u)+second_base,1)||all_invalid)?0xfffff863u:0x1fe3u);
            height=U(0x70e44cu);
        }
        return;
    }
    if(U(0xbca2e0u)!=0&&U(0xbca2e0u)!=6){
        complex_second(rotation,sx,sy,height,0,&x,&y);
        W(0x70e120u)=0x1fe3u;complex_pair(1,x,y);
        y=sy-U(0x70e44cu);complex_pair(0,sx,y);complex_decoration(sx,y);
    }else{
        y=sy-height;W(0x70e120u)=0x1fe3u;
        complex_pair(0,sx,y);complex_decoration(sx,y);
        rotation=(uint32_t)(I(U(0xbca2e0u))/2);
        complex_second(rotation,sx,sy,U(0x70e44cu),0,&x,&y);complex_pair(1,x,y);
    }
    W(0x70e120u)=0;
}
