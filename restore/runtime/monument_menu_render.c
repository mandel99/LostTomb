#include "monument_menu_render.h"
#include "map_validation.h"
#include "menu_tail_ui.h"
#include "monument_rating.h"
#include "sprite_render.h"
#include <stdlib.h>

#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define S16(a) ((int32_t)(int16_t)W(a))
#define BUTTON(a) ((int16_t *)(uintptr_t)(uint32_t)(a))

extern void FUN_00536820(int32_t,int32_t,int32_t,int32_t);
extern void FUN_004d6250(uint32_t,uint32_t,int32_t,int32_t,uint32_t,uint32_t,uint32_t,int32_t,int32_t);
extern void FUN_004d62f0(uint32_t,uint32_t,int32_t,int32_t,uint32_t,uint32_t,int32_t,int32_t);
extern void FUN_004d6380(uint32_t,uint32_t,int16_t *,uint32_t,uint32_t,int32_t,int32_t,int32_t,int32_t);
extern void FUN_004d6860(uint32_t,uint32_t,const char *,int32_t,int32_t,uint32_t,uint32_t,int32_t,int32_t);
extern void FUN_004cd1a0(uint32_t,int16_t *,uint32_t,int32_t,int32_t);
extern void FUN_004cce90(uint32_t,char,const char *,int32_t,int32_t,int32_t,uint32_t,uint32_t);

/* Whole 40CB40, 59 native instructions. Other legacy callers retain their
   entry; the new renderer uses the explicit twenty-byte value contract. */
uint8_t restore_monument_option_allowed_0040cb40(uint32_t phase,RestoreMonumentMenuOption option)
{
    uint16_t kind=(uint16_t)option.words[0];
    uint16_t subtype=(uint16_t)(option.words[0]>>16);
    if(phase==0u)return 0;
    if(kind==2u&&(subtype==1u||subtype==4u||subtype==2u||subtype==3u))return 1;
    if(phase==3u)return (uint8_t)(kind!=1u&&(subtype==6u||subtype==7u||subtype==5u));
    if(phase==2u)return (uint8_t)(kind!=1u&&(subtype==10u||subtype==11u||subtype==12u||subtype==13u));
    if(phase==4u)return (uint8_t)(kind!=1u&&subtype==14u);
    if(phase==1u&&kind!=1u)return 0;
    return 1;
}

/* Whole 537470..537491: thirteen instructions, signed WORD descending.
   This actual C comparator replaces the original numeric code token. */
static int __cdecl compare_options(const void *first,const void *second)
{
    int16_t a=*(const int16_t *)first,b=*(const int16_t *)second;
    if(a>b)return -1;
    return a<b?1:0;
}

/* Exact active arm of the two-target / thirty-byte 53790C/537914 switch. */
static int resource_row(uint32_t resource)
{
    switch(resource){
    case 1:case 8:case 10:case 13:case 15:case 17:case 18:case 19:
    case 20:case 23:case 24:case 25:case 26:case 28:case 30:return 1;
    default:return 0;
    }
}

/* Complete contiguous 5374A0..53790B, 355 instructions. Coordinates that
   native saves at entry remain distinct from its subsequent global reloads.
   All layout, text, sprite, validation and rating providers are genuine C. */
void FUN_005374a0(void)
{
    uint32_t left=U(0xe38da8u),top=U(0xe38da4u);
    uint32_t x,y,font,choice,button,highlight,slot,value,row_y;
    int32_t rating;
    int empty=1;
    FUN_00536820((int32_t)left,(int32_t)top,40,28);
    y=U(0xe38da4u);x=U(0xe38da8u);font=U(0xf56f8cu);
    FUN_004d6250(13,3,(int32_t)left,(int32_t)(top+422u),608,font,0,(int32_t)x,(int32_t)y);
    for(choice=0x784de0u;choice<0x784de6u;choice+=2u){
        uint8_t accepted=FUN_0040c8b0(S16(choice));
        if(accepted){
            uint32_t record=0x5d19c8u+(uint32_t)S16(choice)*16u;
            RestoreMonumentMenuOption option;
            option.words[0]=U(record);
            option.words[1]=U(record+4u);
            option.words[2]=U(record+8u);
            option.words[3]=U(record+12u);
            accepted=restore_monument_option_allowed_0040cb40(U(0x784cbcu)&255u,option);
        }
        if(!accepted)W(choice)=0;
    }
    qsort((void *)0x784de0u,3,2,compare_options);
    choice=0x784de0u;
    for(button=0x5c6f08u,slot=1;button<0x5c6f5cu;button+=28u,choice+=2u,++slot){
        highlight=U(0x7056c8u);font=U(0xf56f8cu);
        FUN_004cd1a0(1,BUTTON(button),highlight==slot,0,0);
        y=U(0xe38da4u);x=U(0xe38da8u);value=(uint32_t)S16(choice);
        FUN_004d6380(198,value,BUTTON(button),font,0,4,4,(int32_t)x,(int32_t)y);
    }
    y=U(0xe38da4u);x=U(0xe38da8u);
    FUN_004d63d0(1,x+10u,y+166u,270,100,0,x,y);
    y=U(0xe38da4u);x=U(0xe38da8u);font=U(0xf56f8cu);
    FUN_004d6250(199,11,(int32_t)(x+10u),(int32_t)(y+190u),270,font,0,(int32_t)x,(int32_t)y);
    rating=restore_monument_rating_int32(1);
    y=U(0xe38da4u);x=U(0xe38da8u);font=U(0xf56fb4u);
    FUN_004cce90((uint32_t)rating,0x40,(const char *)0x7027d4u,
                 (int32_t)(x+10u),(int32_t)(y+214u),270,font,0);
    highlight=U(0x7056c8u);
    B(0x784c3eu)=(uint8_t)(rating!=0);
    U(0x784c2cu)=(uint32_t)rating;
    FUN_004cd1a0(1,BUTTON(0x5c7100u),highlight==19u,0,0);
    y=U(0xe38da4u);x=U(0xe38da8u);font=U(0xf56f8cu);value=U(0x784cbcu)&255u;
    FUN_004d6380(311,value,BUTTON(0x5c7100u),font,0,4,4,(int32_t)x,(int32_t)y);
    for(choice=0x784de0u;choice<0x784de6u;choice+=2u){
        uint32_t record=0x5d19c8u+(uint32_t)S16(choice)*16u;
        uint16_t kind=W(record);
        if(kind==1u)empty=0;
        else if(kind==2u){
            uint16_t subtype=W(record+2u);
            if(subtype==4u||subtype==8u||subtype==10u||subtype==11u||subtype==12u||subtype==13u)empty=0;
        }
    }
    if(empty){
        for(slot=0;slot<36u;++slot)U(0x784de8u+slot*4u)=0;
        return;
    }
    y=U(0xe38da4u);x=U(0xe38da8u);font=U(0xf56f8cu);
    FUN_004d62f0(44,213,(int32_t)(left+290u),(int32_t)(top+400u),font,0,(int32_t)x,(int32_t)y);
    row_y=top+29u;button=0x5c6f5cu;highlight=4;
    for(slot=0;slot<36u;++slot){
        uint8_t available;
        if(!resource_row(slot))continue;
        value=U(0xf571fcu)+slot;
        (void)restore_sprite_draw(RESTORE_SPRITE_LIBRARIES,(int32_t)value,
                                  (int32_t)(left+350u),(int32_t)(row_y-4u));
        value=U(0xf571fcu)+slot;
        (void)restore_sprite_draw(RESTORE_SPRITE_LIBRARIES,(int32_t)value,
                                  (int32_t)(left+600u),(int32_t)(row_y-4u));
        y=U(0xe38da4u);x=U(0xe38da8u);font=U(0xf56f8cu);
        FUN_004d62f0(23,slot,(int32_t)(left+450u),(int32_t)row_y,font,0,(int32_t)x,(int32_t)y);
        value=U(0x7056c8u);
        FUN_004cd1a0(1,BUTTON(button),value==highlight,0,0);
        /* Both original callsites invoke the same AL query exactly once.
           Preserve the conditional DWORD read at the second callsite. */
        if(slot==28u||U(0xd3b198u)==10u)available=FUN_00442870(slot);
        else available=FUN_00442870(slot);
        if(!available){
            y=U(0xe38da4u);x=U(0xe38da8u);font=U(0xf56f8cu);
            FUN_004d62f0(18,6,(int32_t)(left+385u),(int32_t)row_y,font,0,(int32_t)x,(int32_t)y);
            U(0x784de8u+slot*4u)=0;
        }else{
            value=U(0x784de8u+slot*4u);
            if((int32_t)value>3200)U(0x784de8u+slot*4u)=3200;
            y=U(0xe38da4u);x=U(0xe38da8u);font=U(0xf56f8cu);
            value=U(0x784de8u+slot*4u);
            FUN_004d6860(value,0x40,(const char *)0x57c048u,(int32_t)(left+385u),
                         (int32_t)row_y,font,0,(int32_t)x,(int32_t)y);
        }
        ++highlight;button+=28u;row_y+=25u;
    }
}
