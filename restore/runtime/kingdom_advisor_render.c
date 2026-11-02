#include "kingdom_advisor_render.h"
#include "town_menu.h"
#include "menu_event_actions.h"
#include "menu_text_glyph.h"
#include "menu_tail_ui.h"
#include "menu_training_render.h"
#include "sprite_render.h"
#include <stdlib.h>
extern void FUN_004e7db0(void);
extern const uint8_t *FUN_00447840(void);
extern int32_t FUN_004449d0(int32_t city);
extern void FUN_00536820(int32_t,int32_t,int32_t,int32_t);
extern void FUN_004d62f0(uint32_t,uint32_t,int32_t,int32_t,uint32_t,uint32_t,int32_t,int32_t);
extern void FUN_004d6160(uint32_t,uint32_t,int32_t,int32_t,uint32_t,uint32_t,uint32_t,int32_t,int32_t);
extern void FUN_004d6860(int32_t,uint32_t,const char *,int32_t,int32_t,uint32_t,uint32_t,int32_t,int32_t);
extern void FUN_004ccc30(uint32_t,uint32_t,int32_t,int32_t,int32_t,uint32_t,uint32_t);
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define H(a) ((int32_t)*(volatile int16_t *)(uintptr_t)(uint32_t)(a))
#define I(v) ((int32_t)(uint32_t)(v))
#define EMPTY ((const char *)(uintptr_t)0x57c048u)

static int32_t px(uint32_t offset){return I(U(0xf53f0cu)+offset);}
static int32_t py(uint32_t offset){return I(U(0xf53f08u)+offset);}
static int32_t player_value(uint32_t field){return I(U(field+U(0xeaac28u)*0x49d8u));}
static void label(uint32_t group,uint32_t item,uint32_t x,uint32_t y,uint32_t font)
{FUN_004d62f0(group,item,px(x),py(y),U(font),0,I(U(0xe38da8u)),I(U(0xe38da4u)));}
static void number(int32_t value,uint32_t x,uint32_t y,uint32_t font)
{FUN_004d6860(value,0x40,EMPTY,px(x),py(y),U(font),0,I(U(0xe38da8u)),I(U(0xe38da4u)));}
static void paragraph(uint32_t item,uint32_t x,uint32_t y,uint32_t width,uint32_t font)
{FUN_004d6160(0x34,item,px(x),py(y),width,U(font),0,I(U(0xe38da8u)),I(U(0xe38da4u)));}
static void row_background(uint32_t row)
{FUN_004d63d0(0,(uint32_t)px(0x26),(uint32_t)py(row+0x60),0x230,0x28,0,U(0xe38da8u),U(0xe38da4u));}

static int32_t months_margin(const RestoreTownRecord *record)
{
    uint32_t stack[64],r[8]={0},flags=2,sp=(uint32_t)(uintptr_t)(stack+48);
    stack[48]=0;r[1]=(uint32_t)(uintptr_t)record;r[4]=sp;
    restore_event_actions_raw(0x44c9a0u,r,&flags);
    if(r[4]!=sp+4u||r[3]||r[5]||r[6]||r[7])abort();
    return I(r[0]);
}
static int bulk(int32_t kind)
{return kind==24||kind==25||kind==26||kind==30||kind==35;}
static void royal_row(const RestoreTownRecord *record,uint32_t row)
{
    uint32_t p=(uint32_t)(uintptr_t)record,item;
    int32_t country=FUN_004449d0(H(p+0x1cu)),strength=H(p+12u);
    int32_t margin=months_margin(record),city;
    int active;
    row_background(row);
    city=H(p+0x1cu);active=city>=0&&city<50&&B(0x7770f0u+(uint32_t)city*324u)==2;
    (void)FUN_0053e590(I(U(0xf56b04u)+(uint32_t)active),px(0x32),py(row+0x6a));
    U(0xe20778u)=0;
    label(0x34,active?0x50u:0x48u,0x50,row+0x66,0xf56f9cu);
    item=B(0x77de03u+(uint32_t)country*106u);
    label(B(0xe39088u)?0x15u:0xc3u,item,U(0xe20778u)+0x50u,row+0x66u,0xf56f9cu);
    U(0xe20778u)=0;
    label(0x34,strength<46?73u:strength>88?75u:74u,0x50,row+0x78,0xf56f9cu);
    FUN_004ccc30(8,4,margin,px(U(0xe20778u)+0x50u),py(row+0x78u),U(0xf56f9cu),0);
}
static void goods_row(const RestoreTownRecord *record,uint32_t row)
{
    uint32_t p=(uint32_t)(uintptr_t)record,item;
    int32_t kind=H(p+4u),quantity=H(p+12u),available,months;
    if(!bulk(kind)&&kind!=28&&kind!=10&&kind!=36)quantity=I((uint32_t)quantity*100u);
    row_background(row);
    (void)FUN_0053e590(I(U(0xf571fcu)+(uint32_t)(kind==36?21:kind)),px(0x28),py(row+100u));
    U(0xe20778u)=0;number(quantity,0x42,row+0x66,0xf56f9cu);
    item=(uint32_t)kind;
    if(bulk(kind)||((kind==28||kind==10)&&quantity>1))item+=0x36u;
    label(0x17,item,U(0xe20778u)+0x42u,row+0x66u,0xf56f9cu);
    /* Four delayed native arguments before44C100 belong to this numeric
       renderer. The member getter itself consumes no stack argument. */
    months=FUN_0044c100(record);
    FUN_004ccc30(8,4,months,px(0x136),py(row+0x66u),U(0xf56f9cu),0);
    label(0xc,2,U(0xe20778u)+0x136u,row+0x66u,0xf56f9cu);
    U(0xe20778u)=0;
    if(kind==36){
        number(player_value(0xea187cu),0x28,row+0x78,0xf56f9cu);
        label(0x34,0x2c,U(0xe20778u)+0x28u,row+0x78u,0xf56f9cu);
        available=player_value(0xea187cu);
    }else{
        available=I(U(0xea6160u+U(0xeaac28u)*0x49d8u+(uint32_t)kind*4u));
        if(bulk(kind)||kind==28||kind==10)available/=100;
        number(available,0x28,row+0x78,0xf56f9cu);
        label(0x34,0x2b,U(0xe20778u)+0x28u,row+0x78u,0xf56f9cu);
    }
    label(0x34,available<quantity?0x30u:0x2fu,U(0xe20778u)+0x50u,row+0x78u,0xf56f9cu);
}

/* Whole523EA0: panel animation, heading, all request kinds and empty state. */
void FUN_00523ea0(void)
{
    uint32_t phase,rows=0,row=0;
    RestoreTownRecord *record;
    FUN_004e7db0();
    phase=5u-U(0xf53f48u);U(0xf53f0cu)=U(0xe38da8u);U(0xf53f08u)=U(0xe38da4u);
    U(0xf53f04u)=40;U(0xf53f00u)=27;
    if(I(phase)>=5)return;
    if(I(phase)>0){
        FUN_004cd680(I(U(0xe38da8u)+phase*16u),I(U(0xe38da4u)+phase*8u),
                     I((40u-phase*2u)*16u),I((27u-phase*2u)*16u),0);return;
    }
    FUN_00536820(I(U(0xe38da8u)),I(U(0xe38da4u)),40,27);
    (void)FUN_0053e590(I(U(0xf5681cu)+2u),px(10),py(10));
    U(0xe20778u)=0;label(0x34,0x51,0x3c,12,0xf56fb4u);
    FUN_004cc770(FUN_00447840(),px(U(0xe20778u)+0x3cu),py(12),I(U(0xf56fb4u)),0);
    U(0xe20778u)=0;label(0x34,0,0x3c,0x2c,0xf56f8cu);
    number(player_value(0xea5b48u),U(0xe20778u)+0x3cu,0x2c,0xf56f8cu);
    paragraph((uint32_t)(player_value(0xea5b48u)/5)+0x16u,0x3c,0x3c,0x220,0xf56f8cu);
    FUN_004d65b0(px(0x20),py(0x5a),0x24,0xe,I(U(0xe38da8u)),I(U(0xe38da4u)));
    record=restore_town_trade_next(RESTORE_TOWNS,0);
    while(record){
        if((uint8_t)FUN_0044d4c0(record)){
            if(row>=210u)break;
            if(H((uint32_t)(uintptr_t)record+4u)==37)royal_row(record,row);
            else goods_row(record,row);
            ++rows;row+=42u;
        }
        record=restore_town_trade_next(RESTORE_TOWNS,(const int16_t *)record);
    }
    if(!rows)paragraph(0x15,0x40,0xa0,0x200,0xf56f9cu);
}
