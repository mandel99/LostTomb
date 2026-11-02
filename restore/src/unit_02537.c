#include <stdint.h>
#include <stdbool.h>
#include "mission_loaders.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
extern void FUN_004de140(void);
extern bool FUN_005409d0(uint32_t);
extern void FUN_00540a10(uint32_t);
extern int32_t FUN_004cde40(int32_t,int32_t);
extern void FUN_00404350(void);
extern void FUN_004051a0(void);
extern void FUN_004e1770(void);
extern void FUN_00418bf0(void);
extern void FUN_00489b40(void);

/* Native NUL-inclusive REP MOVSD/MOVSB sequence, including DWORD store
   order. The real source paths supply the original bounded filenames. */
static uint32_t copy_name(uint32_t destination,uint32_t source)
{
    uint32_t size=0,i=0;
    do{++size;}while(B(source+size-1u));
    while(size-i>=4u){U(destination+i)=U(source+i);i+=4u;}
    while(i<size){B(destination+i)=B(source+i);++i;}
    return destination+size-1u;
}

/* Complete41B9D0/16 instructions: genuine campaign receiver6FDD08,
   one argument/RET4. Keep the legacy implicit-ECX alias unpromoted. */
static int32_t campaign_index(int32_t mission)
{
    uint32_t count=U(0x6ff608u),i=0,record=0x6fdd14u;
    if((int32_t)count>0){
        do{if(U(record)==(uint32_t)mission)return (int32_t)i;
           ++i;record+=64u;}while((int32_t)i<(int32_t)count);
    }
    return -1;
}

/* Complete5405B0/275 instructions. Player records use byte offsets49D8,
   not a C pointer scaled a second time. No synthesized scenario defaults. */
void FUN_005405b0(char *name,char force_campaign)
{
    char path[100];
    uint32_t end,saved_mode,saved_intro,offset,value,loaded,table,index;
    int8_t mission,kind;uint8_t branch,variant,previous;
    end=copy_name((uint32_t)(uintptr_t)path,0x57626cu);
    saved_mode=U(0xe38eb4u);
    (void)copy_name(end,(uint32_t)(uintptr_t)name);
    mission=(int8_t)B(0xe38ea8u);branch=B(0xe38ea9u);
    kind=(int8_t)B(0x7027b4u);saved_intro=B(0xe38eb8u);
    variant=B(0xe38eabu);previous=B(0xe38eaau);
    B(0x6fdcb4u)=1;FUN_004de140();
    if(U(0xe38ebcu)!=0&&!force_campaign){
        if(!FUN_005409d0((uint32_t)(uintptr_t)path)){
            B(0xe38ddau)=1;B(0xe38dcau)=1;return;
        }
        FUN_00540a10((uint32_t)(uintptr_t)path);
    }else{
        (void)copy_name(0xea184cu,0xe38e74u);
        if(!FUN_004d58b0(0,0))return;
        offset=U(0xeaac28u)*0x49d8u;
        value=U(0x5d1960u+U(0xe39080u)*4u);
        U(0xe38ebcu)=0;
        value=(uint32_t)FUN_004cde40((int32_t)U(0xea187cu+offset),(int32_t)value);
        offset=U(0xeaac28u)*0x49d8u;U(0xea187cu+offset)=value;
        (void)copy_name(0xea184cu,0xe38e74u);
    }
    U(0xe38eb8u)=saved_intro;
    B(0xe38ea8u)=(uint8_t)mission;B(0xe38ea9u)=branch;
    B(0x7027b4u)=(uint8_t)kind;U(0xe38eb4u)=saved_mode;
    if(kind==3){
        variant=(uint8_t)campaign_index(mission);B(0xe38eabu)=variant;
        if((int8_t)variant<0)B(0xe38eabu)=0;
        mission=(int8_t)B(0xe38ea8u);B(0xe38eaau)=255;
    }else{B(0xe38eabu)=variant;B(0xe38eaau)=previous;}
    value=U(0x5d1988u+U(0xe39080u)*4u);loaded=U(0xe38ebcu);
    offset=U(0xeaac28u)*0x49d8u;
    U(0xe38eb0u)=value;U(0xea5b48u+offset)=value;
    U(0xea5cc0u+offset)=U(0xe38eb4u);
    if(loaded)U(0xea5cc0u+offset)=0;
    value=(uint32_t)(int32_t)(int16_t)W(0x784b62u);
    U(0xea5cbcu+offset)=value;U(0xea450cu+offset)=value;
    if((int32_t)value>10)U(0xea450cu+offset)=10;
    value=U(0xea450cu+offset);U(0x78d830u)=0;U(0x78d828u)=0;
    U(0xea4508u+offset)=U(0x5d0320u+value*4u);
    U(0xea5f90u+offset)=1;U(0x78d824u)=1;U(0x78d82cu)=1;
    U(0x78d820u)=1;U(0x78d818u)=1;U(0x78d81cu)=1;
    U(0x78d814u)=1;U(0x78d810u)=1;U(0xea5f94u+offset)=1;
    if(!loaded){
        if(!mission)U(0x78d830u)=1;
        if(mission<=4)U(0x78d828u)=1;
    }
    if(mission<=10&&!loaded){
        U(0x6fdc88u)=0;U(0x6fdc8cu)=0;U(0x6fdc90u)=0;
        W(0x6fdc94u)=0;B(0x6fdc96u)=0;
        for(table=0x5794bcu,index=0;table<0x579794u;table+=28u,++index){
            uint8_t enabled=(uint8_t)(mission>(int32_t)U(table-12u));
            uint32_t count=U(table),items;
            B(0x7027b8u+index)=enabled;
            if((int32_t)count>0){
                items=U(table+4u);
                do{value=U(items);items+=4u;--count;B(0x6fdc88u+value)=enabled;}while(count);
            }
        }
    }else{
        for(index=0;index<6u;++index)U(0x7027b8u+index*4u)=0x01010101u;
        U(0x6fdc88u)=0x01010101u;U(0x6fdc8cu)=0x01010101u;U(0x6fdc90u)=0x01010101u;
        W(0x6fdc94u)=0x0101;W(0x7027d0u)=0x0101;B(0x6fdc96u)=1;
    }
    FUN_00404350();FUN_004051a0();FUN_004e1770();FUN_00418bf0();
    loaded=U(0xe38ebcu);offset=U(0xeaac28u)*0x49d8u;
    if(!loaded){U(0xea5b48u+offset)=U(0xe38eb0u);U(0xea5cc0u+offset)=U(0xe38eb4u);}
    U(0xea6018u+offset)=U(0xe39080u);
    B(0x784cbdu)=(uint8_t)(B(0x784cbdu)==1);
    FUN_00489b40();
}
