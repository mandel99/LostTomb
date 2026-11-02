#include "financial_updates.h"
#include "town_menu.h"
#include "menu_event_actions.h"
#include "event_publisher.h"
#include "actor_predicate.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define H(a) (*(volatile int16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define I(v) ((int32_t)(uint32_t)(v))
extern int32_t FUN_004cde40(int32_t,int32_t);
static uint32_t player(void){return U(0xeaac28u);}
static uint32_t offset(void){return player()*0x49d8u;}
static uint32_t tribute_floor(int32_t population)
{
    return population>=5001?500u:population>=3001?400u:population>=2001?300u:
        population>=1001?225u:population>=501?150u:50u;
}

/* Whole45A350/47 instructions. Cache the interest rate before the temple
   query, test its full return against1, and reload the owner after queries.
   NEG, percentage multiplication and final stores retain32-bit wrapping. */
void FUN_0045a350(void)
{
    uint32_t owner=player(),off=owner*0x49d8u,rate=U(0x784ddcu),cash,amount;
    if(W(0xea5ce8u+off)){
        if(FUN_004c5120(I(U(0xea5f9cu+off)))==1u){rate-=5u;if(I(rate)<0)rate=0;}
        owner=player();
    }
    off=owner*0x49d8u;cash=U(0xea187cu+off);
    if(I(cash)>=0)return;
    amount=(uint32_t)FUN_004cde40(I(0u-cash),I(rate));amount=(uint32_t)(I(amount)/12);
    off=offset();U(0xea187cu+off)-=amount;U(0xea44e4u+off)+=amount;
}

/* Whole45A5D0/102 instructions. Annual income/expense stay cached across
   both queries; after the percentage call, payment uses the live city and
   its current provisional amount. The closing snapshot reloads the city
   again after45A7B0. No later ledger values replace those cached totals. */
void FUN_0045a5d0(void)
{
    uint32_t off=offset(),income,expense,amount;
    int32_t population;
    U(0xea5cc4u+off)=U(0xea5cc8u+off);U(0xea5cc8u+off)=0;
    U(0xea601cu+off)=U(0xea6020u+off);U(0xea6020u+off)=0;U(0xea5d58u+off)=0;
    income=U(0xea44c0u+off);income+=U(0xea44d8u+off);
    income+=U(0xea601cu+off);income+=U(0xea5cc4u+off);
    expense=U(0xea4500u+off);expense+=U(0xea44e8u+off);expense+=U(0xea44f8u+off);
    expense+=U(0xea448cu+off);expense+=U(0xea44e0u+off);
    if(U(0xea5cbcu+off)==10u){
        U(0xea5d58u+off)=0;U(0xea5d5cu+off)=0;U(0xea5d54u+off)=0;
    }else if(I(U(0xea187cu+off))<=0){
        U(0xea5d58u+off)=1;++U(0xea5d5cu+off);U(0xea5d54u+off)=0;
    }else{
        U(0xea5d5cu+off)=0;population=I(U(0xea1894u+off));
        if(I(income)<=I(expense))U(0xea5d54u+off)=population>=2001?200u:population>=1001?100u:0u;
        else{
            U(0xea5d54u+off)=tribute_floor(population);
            amount=(uint32_t)FUN_004cde40(I(income-expense),25);off=offset();
            if(I(amount)>I(U(0xea5d54u+off)))U(0xea5d54u+off)=amount;
        }
    }
    U(0xea187cu+off)-=U(0xea5d54u+off);expense+=U(0xea5d54u+off);
    FUN_0045a7b0();off=offset();
    U(0xea452cu+off)=U(0xea187cu+off);U(0xea4514u+off)=income;U(0xea451cu+off)=expense;
}

/* Whole45A8C0/27 instructions. Low32-bit signed wage product is divided
   first by10 and then12; the month is a full DWORD and projection wraps. */
void FUN_0045a8c0(void)
{
    uint32_t off=offset(),month=U(0x78d7c0u),product=U(0xea4478u+off)*U(0xea4464u+off);
    uint32_t monthly=(uint32_t)((I(product)/10)/12);
    U(0xea4488u+off)=U(0xea4484u+off);
    U(0xea5f10u+off)=(12u-month)*monthly+U(0xea4484u+off);
}

/* Whole45A930/13 instructions. The optional query can change the owner. */
void FUN_0045a930(void)
{
    uint32_t off=offset();
    if(W(0xea5ce8u+off))(void)FUN_004c5120(I(U(0xea5f9cu+off)));
    off=offset();U(0xea44ecu+off)=U(0xea44e4u+off);
}
/* Whole45A970/5 instructions. */
void FUN_0045a970(void)
{uint32_t off=offset();U(0xea4504u+off)=U(0xea4510u+off);}

/* Whole45A990/34 instructions: every ledger sum wraps at32 bits. */
void FUN_0045a990(void)
{
    uint32_t off=offset(),income=U(0xea6020u+off),expense;
    income+=U(0xea5cc8u+off);income+=U(0xea44bcu+off);income+=U(0xea44d4u+off);
    U(0xea4518u+off)=income;
    expense=U(0xea5fc8u+off);expense+=U(0xea44f4u+off);expense+=U(0xea4504u+off);
    expense+=U(0xea44ecu+off);expense+=U(0xea44fcu+off);
    expense+=U(0xea4488u+off);expense+=U(0xea44dcu+off);U(0xea4520u+off)=expense;
    U(0xea4524u+off)=U(0xea4514u+off)-U(0xea451cu+off);
    U(0xea4528u+off)=U(0xea4518u+off)-U(0xea4520u+off);
    U(0xea4530u+off)=U(0xea452cu+off)+U(0xea4528u+off);
}

/* Whole45A7B0/62 instructions. Native publishes the provisional amount,
   calls the percentage helper, then clears the live city's amount to zero.
   Keep those transient stores and the post-query owner reload. */
void FUN_0045a7b0(void)
{
    uint32_t off=offset(),value,address;
    int32_t population;
    if(I(U(0xea187cu+off))<=0){U(0xea5d50u+off)=0;U(0xea5d50u+off)=0;return;}
    value=U(0xea4528u+off);population=I(U(0xea1894u+off));
    if(I(value)<=0){
        if(population>=2001){U(0xea5d50u+off)=200;U(0xea5d50u+off)=0;return;}
        if(population>=1001){U(0xea5d50u+off)=100;U(0xea5d50u+off)=0;return;}
        U(0xea5d50u+off)=0;U(0xea5d50u+off)=0;return;
    }
    U(0xea5d50u+off)=tribute_floor(population);
    value=(uint32_t)FUN_004cde40(I(U(0xea4528u+off)),25);
    address=0xea5d50u+offset();
    if(I(value)>I(U(address)))U(address)=value;
    U(address)=0;
}

/* Whole46E840/9 instructions, including the wrapped result's sign test. */
void FUN_0046e840(void)
{
    uint32_t off=offset(),value=U(0xea5b40u+off)-3u;
    U(0xea5b40u+off)=value;if(I(value)<0)U(0xea5b40u+off)=0;
    U(0xea5ca8u+off)=8;
}

/* Whole4460B0/130 instructions. The initial treasury and second ledger
   address are cached across their respective percentage calls. Later owner
   loads and the event's explicit receiver follow their native call sites. */
void FUN_004460b0(void)
{
    uint32_t owner=player(),off=owner*0x49d8u,address,amount,record=0,flags;
    RestoreTownRecord *town;
    if(I(U(0xea187cu+off))>=0){
        U(0xea609cu+off)=0;U(0xea60a0u+off)=0;U(0xea5d0cu+off)=0;
    }else if(!U(0xea5d08u+off)){
        address=0xea187cu+off;
        amount=(uint32_t)FUN_004cde40(I(U(0x784c94u)),I(U(0x5d1960u+U(0xe39080u)*4u)));
        U(address)+=amount;
        address=0xea5cc8u+offset();
        amount=(uint32_t)FUN_004cde40(I(U(0x784c94u)),I(U(0x5d1960u+U(0xe39080u)*4u)));
        U(address)+=amount;
        FUN_0045a990();FUN_0045a7b0();
        owner=player();off=owner*0x49d8u;amount=U(0x784c94u);U(0xea5d08u+off)=1;
        if(I(amount)>0){
            U(0xe950dcu)=1;
            if(U(0xea5cbcu+off)==10u)FUN_004e0cf0(117,0,0,0,0);
            else{FUN_004e0cf0(115,0,0,0,0);FUN_0046e840();}
            owner=player();
        }
    }else if(!U(0xea5d0cu+off)){
        U(0xea5d0cu+off)=1;U(0xe950dcu)=1;FUN_004e0cf0(116,0,0,0,0);owner=player();
    }
    if(U(0xea5b48u+owner*0x49d8u))return;
    town=(RestoreTownRecord *)FUN_0044bb90(0,0);
    while(town){
        record=(uint32_t)(uintptr_t)town;flags=U(record+40u);
        if(flags&16u)break;
        town=(RestoreTownRecord *)FUN_0044bb90((const int16_t *)town,0);
    }
    if(!town||(U(record+40u)&4u))return;
    amount=H(record+4u)==3?171u:181u;
    W(record+22u)=0;W(record+24u)=UINT16_MAX;W(record+26u)=UINT16_MAX;
    restore_event_schedule_0044c490(town,H(record),(int16_t)amount);
    /* Whole44EF90/5 instructions: clear BYTE enabled before flagging the
       same record, then4460B0 publishes the deferred month marker. */
    flags=U(record+40u);B(record+50u)=0;U(record+40u)=flags|4u;B(record+3u)=12;
}
