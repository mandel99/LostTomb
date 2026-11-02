#include "mission_setup.h"
#include "mission_record_actions.h"
#include "actor_mission_init.h"
#include "actor_spawn.h"
#include "camera_bounds.h"
#include "editor_map_reset.h"
#include "map_file_load.h"
#include <windows.h>
#include <mmsystem.h>

#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
extern void FUN_0040f8f0(void *,uint32_t,uint8_t);
extern void FUN_0040f860(const void *,void *,int32_t);
extern int32_t FUN_004cde40(int32_t,int32_t);
extern void FUN_0040f250(void);
extern void FUN_0041df60(void);
extern void FUN_00404330(void);
extern void FUN_00404350(void);
extern void FUN_004051a0(void);
extern void FUN_0046b0e0(void);
extern void FUN_0046efb0(void);
extern void FUN_0046f120(void);
extern void FUN_0046a260(void);
extern void FUN_004b32a0(void);
extern void FUN_004e7e10(void);
extern void FUN_00443ea0(int32_t);
extern void FUN_00444720(void);
extern void FUN_00442800(void);
extern void FUN_004ddec0(void);
extern void FUN_004ddf20(void);
extern void FUN_00430530(uint32_t);
extern void FUN_004cc560(int32_t,uint32_t,uint32_t,uint32_t,uint8_t,uint32_t);
extern void FUN_004e2ab0(void);
extern void FUN_00517a20(void);
extern void FUN_005178b0(void);
extern void FUN_00517370(void);
extern void FUN_00517660(void);
extern void FUN_005178e0(void);
extern uint32_t FUN_00562348(void);

/* Whole540A10/77 instructions. Native's one argument is only pushed for
   4DE820, whose complete body consumes no incoming stack argument. */
void FUN_00540a10(uint32_t unused_path)
{
    uint32_t source=0xe38e74u,destination=0xea184cu,size=0,index=0,difficulty,funds;
    (void)unused_path;
    B(0xe38dd9u)=0;B(0xe38dd8u)=0;B(0xe38dd7u)=0;U(0x6059e8u)=0;
    FUN_004e7d10();FUN_004e7e10();
    U(0xeaac28u)=1;B(0xea624bu)=1;B(0xea624au)=1;
    FUN_004e1770();FUN_00540b50();FUN_0040f250();FUN_0041d730();FUN_0041df60();
    FUN_00404330();FUN_0046a230();FUN_0046b0e0();FUN_0046efb0();FUN_0046f120();
    FUN_0046a260();FUN_004b4760();FUN_004b8c90();FUN_004b58d0();FUN_004b8290();
    FUN_004b32a0();FUN_00451be0();
    if(U(0x605a3cu)==1u){
        U(0x784f10u)=0;FUN_004de430(3);FUN_00443ea0(1);
        FUN_00444720();FUN_00444b10();FUN_004bb090();
    }
    FUN_004de820();
    U(0xeaac28u)=1;
    do{++size;}while(B(source+size-1u));
    difficulty=U(0xe39080u);B(0xea624bu)=1;
    while(size-index>=4u){U(destination+index)=U(source+index);index+=4u;}
    funds=U(0xea6254u);B(0xea624au)=1;
    while(index<size){B(destination+index)=B(source+index);++index;}
    difficulty=U(0x5d1960u+difficulty*4u);B(0xea186du)=1;
    funds=(uint32_t)FUN_004cde40((int32_t)funds,(int32_t)difficulty);
    U(0xea187cu+U(0xeaac28u)*0x49d8u)=funds;
}

/* Whole540B50/80 instructions. The original430530 provider consumes only
   orientation; its native second pushed slot is unused. */
void FUN_00540b50(void)
{
    B(0xe38dd9u)=0;U(0xe38e50u)=0;U(0xe38e54u)=0;
    U(0x78d7dcu)=0;U(0x78d7d8u)=0;U(0x78d7d4u)=0;
    FUN_00481f20();U(0xe94e04u)=0;FUN_00430530(0);
    if(U(0xf53f78u)==1u)FUN_004ddec0();else FUN_004ddf20();
    U(0xbca2bcu)=109;U(0xbca2c0u)=218;U(0xbca2e0u)=0;
    FUN_004ddf90();FUN_004de080();FUN_004cdd60();U(0xe20990u)=0;
    FUN_004cc560(1,0xe38e74u,31,104,1,U(0xf56f84u));
    FUN_004cc560(2,0xe37100u,32,280,1,U(0xf56f9cu));
    FUN_004cc560(4,0xe37078u,32,280,1,U(0xf56f9cu));
    FUN_004cc560(5,0x784914u,24,280,0,U(0xf56f84u));
    FUN_004cc560(6,0x784954u,500,400,0,U(0xf56f84u));
    FUN_004cc560(7,0x7027ecu,256,280,0,U(0xf56f84u));
    FUN_004e2ab0();
}

/* Whole4DE820/131 instructions including tail4051A0. Entry/exit cells
   deliberately publish the native zero-extended low BYTE as a WORD. */
void FUN_004de820(void)
{
    uint32_t enabled=0,x,y,offset,value,base,year;
    uint16_t first_x,first_y;
    U(0xeaac28u)=1;FUN_004de600();
    FUN_0040f860((const void *)0x7848d8u,(void *)0x7848d8u,0x638);
    if(U(0x605a3cu)==0){
        x=W(0x784c8cu);y=W(0x784c8eu);
        if(x!=0xffffu&&y!=0xffffu&&W(0x784c90u)!=0xffffu&&W(0x784c92u)!=0xffffu)enabled=1;
        FUN_004bbd40((int16_t)x,(int16_t)y,(int32_t)enabled);
    }
    FUN_005178b0();FUN_00517370();FUN_00517660();FUN_005178e0();
    first_x=W(0x784c64u);
    if(first_x!=0xffffu){
        first_y=W(0x784c66u);
        if(first_y!=0xffffu)goto first_ready;
    }
    first_x=(uint16_t)(U(0xbca2d0u)-1u);
    value=U(0xbca2d4u);W(0x784c64u)=first_x;
    first_y=(uint16_t)((int32_t)value/2);W(0x784c66u)=first_y;
first_ready:
    offset=U(0xeaac28u)*0x49d8u;
    W(0xea3fe8u+offset)=B(0x784c64u);
    y=B(0x784c66u);W(0xea3feau+offset)=(uint16_t)y;
    x=(uint32_t)(int32_t)(int16_t)W(0xea3fe8u+offset);
    base=U(0xbca2d8u);U(0xea3fecu+offset)=base+x+y*228u;
    if(W(0x784c68u)==0xffffu||W(0x784c6au)==0xffffu){
        W(0x784c68u)=first_x;W(0x784c6au)=first_y;
    }
    W(0xea3ff0u+offset)=B(0x784c68u);
    y=B(0x784c6au);W(0xea3ff2u+offset)=(uint16_t)y;
    x=(uint32_t)(int32_t)(int16_t)W(0xea3ff0u+offset);
    value=U(0xe39080u);U(0xea3ff4u+offset)=base+x+y*228u;
    y=U(0x7848f8u);value=U(0x5d1960u+value*4u);
    value=(uint32_t)FUN_004cde40((int32_t)y,(int32_t)value);
    offset=U(0xeaac28u)*0x49d8u;
    U(0xea187cu+offset)=value;U(0xea452cu+offset)=value;
    year=(uint32_t)(int32_t)(int16_t)W(0x7848d8u);
    value=U(0x784c48u);U(0x78d7bcu)=year;
    if(value)year+=U(0x784c4cu);
    else if(U(0x784c50u))year+=U(0x784c54u);
    else year+=1000000u;
    U(0x78d7b8u)=year;
    U(0x784f10u)=(uint32_t)(int32_t)(int16_t)W(0x7848dcu);
    FUN_00444720();FUN_00444b10();
    value=U(0x784f10u)<<5;
    x=(uint32_t)(int32_t)(int16_t)W(0x7843d8u+value);
    y=(uint32_t)(int32_t)(int16_t)W(0x7843dau+value);
    U(0x775910u)=x;U(0x77590cu)=y;
    FUN_00442800();FUN_00404350();FUN_004051a0();
}

/* Whole4E7D10/31,4E7E20/6,4E9EE0/25 instructions. */
void FUN_004e7d10(void)
{
    FUN_0040f8f0((void *)0xea1870u,0x93b0u,0);FUN_004e7e20();
    W(0xea186cu)=0;U(0xea625cu)=50;U(0xea6260u)=50;
    U(0xea6258u)=60;U(0xea8e54u)=30;U(0xea8e50u)=30;
    U(0xea6268u)=3;U(0xea6250u)=9;U(0xeaa50cu)=1;U(0xeaa510u)=1;
    U(0xea8804u)=0;U(0xea8808u)=0;U(0xeaa5d4u)=1;U(0xeaa73cu)=1;
    U(0xeaa7bcu)=0;U(0xeaa7c0u)=0;U(0xeaa7c4u)=0;FUN_004e9ee0();
}
void FUN_004e7e20(void)
{
    uint8_t enabled=B(0xea624bu);U(0xea1820u)=0;
    if(enabled)U(0xea1820u)=1;
}
/* Whole46A230/8,4B58D0/124,4B8290/15,451BE0/25,46A080/12. */
void FUN_0046a230(void)
{
    FUN_0040f8f0((void *)0x7999d8u,0x101d00u,0);
    U(0x78d78cu)=0;U(0x78d7b4u)=0;
}
void FUN_004b58d0(void)
{
    uint32_t address=0xd056e0u;
    do{uint32_t value=FUN_00562348();U(address)=value&15u;address-=4u;}while(address>=0xd05640u);
}
void FUN_004b8290(void)
{
    uint32_t address=0xbcb9c8u;
    do{FUN_0040f8f0((void *)(uintptr_t)address,0x90u,0);address+=0x90u;}while(address<0xbcd5e8u);
    U(0xbcb9bcu)=0;U(0xbcb9b8u)=0;U(0xbcb9b4u)=0;
}
void FUN_00451be0(void)
{
    uint32_t address;
    U(0x78d7e0u)=0;U(0x78d7ccu)=0;U(0x78d7c8u)=0;U(0x78d7c4u)=0;
    U(0x78d7c0u)=0;U(0x78d7bcu)=0x832;U(0x78d7b0u)=0;
    U(0x78d7a8u)=0;U(0x78d7a4u)=0;U(0x78d7a0u)=0;
    U(0x78d5c4u)=0;U(0x78d5c0u)=0;U(0x78d78cu)=0;FUN_0046a080();
    for(address=0x78d3ecu;address<0x78d4b8u;address+=4u)U(address)=0;
    for(address=0x78d4b8u;address<0x78d584u;address+=4u)U(address)=0;
}
void FUN_0046a080(void)
{
    uint32_t address=0x78d5ecu;
    do{U(address)=0;address-=4u;}while(address>=0x78d5c8u);
}

/* Whole4CDD60/69 instructions. The31-step registers are retained locally
   through all100 rounds; native publishes the two generator words last. */
void FUN_004cdd60(void)
{
    uint32_t output=U(0xd3b374u),low=U(0xd3b380u),high=U(0xd3b384u),index=0,round;
    for(round=0;round<100u;++round){
        uint32_t bit;U(0xd3b1c8u+index*4u)=output;
        ++index;if((int32_t)index>=100)index=0;
        for(bit=0;bit<31u;++bit){
            uint32_t a=((high>>4)^high)&1u,b=((low>>4)^low)&1u;
            high>>=1;low>>=1;
            if(a)high|=0x40000000u;if(b)low|=0x40000000u;
        }
        U(0xd3b378u)=high&7u;U(0xd3b370u)=high&0x7fffu;
        U(0xd3b364u)=low&0x7fffu;U(0xd3b368u)=low&0x7fu;
        U(0xd3b36cu)=low&7u;output=high&0x7fu;U(0xd3b374u)=output;
    }
    U(0xd3b1c4u)=index;U(0xd3b384u)=high;U(0xd3b380u)=low;
}

/* Whole41D730/332 instructions. These63 ordered native five-store groups
   include the late record703988 publication. No inferred defaults or full
   record memset replaces the original sparse configuration writes. */
void FUN_0041d730(void)
{
    static const struct {uint32_t address,kind;} records[]={
        {0x702988u,0x1eu},
        {0x702a08u,0x22u},
        {0x702a88u,0x26u},
        {0x702b08u,0x2au},
        {0x702b88u,0x2eu},
        {0x702c08u,0x32u},
        {0x702c88u,0x33u},
        {0x702d08u,0x34u},
        {0x702d88u,0x35u},
        {0x702e08u,0x36u},
        {0x702e88u,0x37u},
        {0x702f08u,0x38u},
        {0x702f88u,0x39u},
        {0x703008u,0x3au},
        {0x703088u,0x3eu},
        {0x703108u,0x3fu},
        {0x703188u,0x40u},
        {0x703208u,0x41u},
        {0x703288u,0x42u},
        {0x703308u,0x43u},
        {0x703388u,0x44u},
        {0x703408u,0x45u},
        {0x703488u,0x46u},
        {0x703508u,0x4au},
        {0x703588u,0x4eu},
        {0x703608u,0x4fu},
        {0x703688u,0x50u},
        {0x703708u,0x51u},
        {0x703788u,0x52u},
        {0x703808u,0x53u},
        {0x703888u,0x57u},
        {0x703908u,0x59u},
        {0x703a08u,0x5du},
        {0x703a88u,0x5fu},
        {0x703b08u,0x61u},
        {0x703b88u,0x62u},
        {0x703c08u,0x63u},
        {0x703c88u,0x64u},
        {0x703d08u,0x65u},
        {0x703d88u,0x66u},
        {0x703e08u,0x6au},
        {0x703e88u,0x6eu},
        {0x703f08u,0x6fu},
        {0x703f88u,0x70u},
        {0x704008u,0x71u},
        {0x704088u,0x72u},
        {0x704108u,0x73u},
        {0x704188u,0x74u},
        {0x704208u,0x75u},
        {0x704288u,0x76u},
        {0x704308u,0x77u},
        {0x704388u,0x78u},
        {0x704408u,0x79u},
        {0x704488u,0x7au},
        {0x704508u,0x7bu},
        {0x704588u,0x7cu},
        {0x704608u,0x7du},
        {0x704688u,0x7eu},
        {0x704708u,0x7fu},
        {0x704788u,0x80u},
        {0x704808u,0x84u},
        {0x704888u,0x85u},
        {0x703988u,0x5bu},

    };
    uint32_t address,index,now=timeGetTime();
    U(0x7028fcu)=now;FUN_0040f8f0((void *)0x702900u,0x2300u,0);
    now=U(0x7028fcu);
    for(address=0x7029d0u;address<0x704c50u;address+=128u)U(address)=now;
    for(index=0;index<63u;++index){
        address=records[index].address;
        U(address)=200;U(address+28u)=1;U(address+32u)=records[index].kind;
        U(address+64u)=1;U(address+76u)=30000;
    }
}
