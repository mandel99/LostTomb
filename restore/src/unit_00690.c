#include "event_description.h"
#include "editor_event_detail_actions.h"
#include "text_selection.h"
#include "legacy_format.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define S16(a) ((int32_t)(int16_t)W(a))
#define TEXT(a) ((const char *)(uintptr_t)(uint32_t)(a))

/* Reuse the reviewed nullary member leaves: explicit ECX, full EAX for
   table indices or AL for predicates. None reads the incoming stack. */
static uint32_t query(uint32_t target,uint32_t event)
{
    uint32_t registers[8]={0},flags=2,return_slot;
    registers[1]=event;registers[4]=(uint32_t)(uintptr_t)&return_slot;
    restore_event_detail_raw(target,registers,&flags);
    return registers[0];
}
static void append(char *destination,const char *source)
{
    while(*destination)++destination;
    do{*destination++=*source;}while(*source++);
}

/* Whole448F60,1025 bytes/330 instructions. Native local buffers, selected
   text restoration, dependency scan and40-character display cap. */
const char *restore_event_description(const RestoreTownRecord *event)
{
    uint32_t self=(uint32_t)(uintptr_t)event,saved=U(0xe20768u),label,i,record;
    int32_t group,lower,upper,dependency=-1;
    int16_t selections[3]={-1,-1,-1};
    const char *names[3];
    char number[20],trigger[52],quantity[40],choices[200];
    FUN_004cc110(25,B(self+3u));label=U(0xe20768u);
    FUN_004cc110(20,0);FUN_004cc110(20,0);
    group=(int32_t)query(0x4495c0u,self);
    if(group>=0){
        selections[0]=(int16_t)W(self+6u);selections[1]=(int16_t)W(self+8u);
        selections[2]=(int16_t)W(self+10u);
        for(i=0;i<3u;++i){
            if(selections[i]<(int32_t)query(0x4493d0u,self))U(0xe20768u)=0x7027d4u;
            else FUN_004cc110(group,selections[i]);
            names[i]=TEXT(U(0xe20768u));
        }
    }
    U(0xe20768u)=saved;
    if(!(B(self+40u)&1u)){
        if(!(U(self+40u)&16u))FUN_00561a36(trigger,TEXT(0x5da1b4u),TEXT(label));
        else FUN_00561a36(trigger,TEXT(0x5da1b8u));
    }else{
        for(i=0,record=0x785d74u;(int32_t)i<(int32_t)U(0x785d70u);++i,record+=124u){
            if(((query(0x4495f0u,record)&255u)&&W(record+36u)==W(self))||
               ((query(0x4496b0u,record)&255u)&&W(record+38u)==W(self))||
               ((query(0x4496b0u,record)&255u)&&W(record+82u)==W(self))||
               ((query(0x449600u,record)&255u)&&W(record+84u)==W(self))){
                dependency=(int32_t)i;break;
            }
        }
        FUN_00561a36(trigger,TEXT(dependency>=0?0x5da1bcu:0x5da1c4u),dependency);
    }
    lower=S16(self+22u);
    if(lower<0){upper=S16(self+26u);lower=S16(self+24u);}
    else{upper=lower;if(S16(self+24u)>=0)lower=upper=-1;}
    FUN_00561a36(number,TEXT(0x5da1b0u),lower);append(trigger,number);
    if(lower<upper){FUN_00561a36(number,TEXT(0x5da1acu),upper);append(trigger,number);}
    if(FUN_00449620(event)){
        lower=S16(self+14u);
        if(lower<0){lower=S16(self+16u);upper=S16(self+18u);}
        else{upper=lower;if(S16(self+16u)>=0)lower=upper=-1;}
        if(lower<upper)FUN_00561a36(quantity,TEXT(0x5da1a4u),lower,upper);
        else FUN_00561a36(quantity,TEXT(0x5da1a0u),lower);
    }else quantity[0]=0;
    choices[0]=' ';choices[1]=0;
    for(i=0;i<3u;++i)if(selections[i]>0){
        if(choices[1])append(choices,TEXT(0x5da19cu));
        append(choices,names[i]);
    }
    if(query(0x4493b0u,self)&255u)
        FUN_00561a36((char *)0x785c40u,TEXT(0x5da194u),trigger,quantity,choices);
    else FUN_00561a36((char *)0x785c40u,TEXT(0x5da18cu),trigger,quantity);
    B(0x785c68u)=0;
    return TEXT(0x785c40u);
}
const char *FUN_00448f60(const RestoreTownRecord *event)
{return restore_event_description(event);}
