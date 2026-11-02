#include "mission_city_commit.h"
#include "city_prepare.h"
#include "mission_loaders.h"
#include "camera_bounds.h"
#include "map_record_refresh.h"
#include <stdlib.h>

#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define S16(a) ((int32_t)(int16_t)W(a))

extern void FUN_0040f8f0(void *,uint32_t,uint8_t);
extern void FUN_00486e70(void);
extern void FUN_004860d0(void);
extern void FUN_0051f780(int32_t,int32_t,uint32_t,uint32_t);
extern void FUN_004e59a0(void);
extern void FUN_00442660(void);
extern void FUN_004d4040(void);

static void remember_random(void)
{
    uint32_t index=U(0xd3b1c4u);
    U(0xd3b1c8u+index*4u)=U(0xd3b374u);
    ++index;
    U(0xd3b1c4u)=index;
    if((int32_t)index>=100)U(0xd3b1c4u)=0;
}

static void shift_random(uint32_t *high,uint32_t *low)
{
    uint32_t a=*high,b=*low,i;
    for(i=0;i<31u;++i){
        uint32_t feedback_a=((a>>4)^a)&1u;
        uint32_t feedback_b=((b>>4)^b)&1u;
        a>>=1;b>>=1;
        if(feedback_a)a|=0x40000000u;
        if(feedback_b)b|=0x40000000u;
    }
    *high=a;*low=b;
}

/* Complete 123-instruction range selector. Negative parameter words are
   native sentinels. An actual zero IDIV divisor remains an explicit fault;
   no successful replacement value is manufactured for that native #DE. */
void restore_mission_range_004487f0(uint32_t self)
{
    int32_t first=S16(self+2u),second,third;
    uint32_t a,b;
    if(first>=0){
        if(S16(self+4u)<0){W(self)=(uint16_t)first;return;}
    }else{
        second=S16(self+4u);
        if(second>=0){
            third=S16(self+6u);
            if(third>=0&&third-second+1==1){W(self)=(uint16_t)first;return;}
        }
    }
    remember_random();
    a=U(0xd3b384u);b=U(0xd3b380u);
    shift_random(&a,&b);
    U(0xd3b384u)=a;
    U(0xd3b378u)=a&7u;
    U(0xd3b380u)=b;
    U(0xd3b364u)=b&0x7fffu;
    U(0xd3b370u)=a&0x7fffu;
    U(0xd3b374u)=a&0x7fu;
    U(0xd3b368u)=b&0x7fu;
    U(0xd3b36cu)=b&7u;
    first=S16(self+2u);
    if(first<0){
        int32_t divisor,remainder;
        second=S16(self+4u);third=S16(self+6u);
        divisor=third-second;
        if(divisor==0)abort();
        remainder=(int32_t)(a&0x7fffu)%divisor;
        W(self)=(uint16_t)(remainder+(uint32_t)second);
        return;
    }
    second=S16(self+4u);
    third=second<0?1:(S16(self+6u)>=0?3:2);
    switch((a&0x7fffu)%(uint32_t)third){
    case 0: W(self)=(uint16_t)first;return;
    case 1: W(self)=(uint16_t)second;return;
    default: W(self)=W(self+6u);return;
    }
}

/* Complete 155-instruction ECX method. Keep its first RNG update partial:
   the other derived RNG globals are written only after the second shift. */
void restore_mission_record_00448a90(uint32_t self)
{
    uint32_t a,b,i;
    U(self+0x28u)&=0xfffffffbu;
    restore_mission_range_004487f0(self+4u);
    restore_mission_range_004487f0(self+12u);
    restore_mission_range_004487f0(self+20u);
    restore_mission_range_004487f0(self+28u);
    restore_mission_range_004487f0(self+88u);
    a=(uint16_t)(B(self+3u)+W(self+20u));
    B(self+0x57u)=0;
    W(self+0x30u)=0;
    W(self+0x2eu)=(uint16_t)a;
    W(self+0x34u)=0xffffu;
    W(self+0x64u)=0xffffu;
    a=B(self+2u);
    B(self+0x32u)=0;
    B(self+0x33u)=0;
    B(self+0x63u)=0;
    if(a==3u){
        for(i=0;i<4u;++i){
            W(self+0x40u+i*4u)=W(0x784c60u);
            W(self+0x42u+i*4u)=W(0x784c62u);
        }
    }
    remember_random();
    a=U(0xd3b384u);b=U(0xd3b380u);
    shift_random(&a,&b);
    U(0xd3b384u)=a;
    U(0xd3b374u)=a&0x7fu;
    U(0xd3b380u)=b;
    B(self+0x61u)=(uint8_t)((a&0x7fffu)%5u+4u);
    remember_random();
    a=U(0xd3b384u);b=U(0xd3b380u);
    shift_random(&a,&b);
    U(0xd3b384u)=a;
    U(0xd3b378u)=a&7u;
    U(0xd3b364u)=b&0x7fffu;
    U(0xd3b370u)=a&0x7fffu;
    U(0xd3b380u)=b;
    U(0xd3b374u)=a&0x7fu;
    U(0xd3b368u)=b&0x7fu;
    U(0xd3b36cu)=b&7u;
    B(self+0x62u)=(uint8_t)(a&1u);
}

/* Complete 29-instruction leaf: snapshot signed DWORD row count once. */
void restore_mission_terrain_004771c0(void)
{
    int32_t rows=(int32_t)U(0xbca2d4u);
    uint32_t row=0;
    while((int32_t)row<rows){
        uint32_t right=U(0xbc9f24u+row*4u);
        uint32_t left=U(0xbc9b94u+row*4u);
        uint32_t cell=U(0xbc9804u+row*4u);
        if((int32_t)right<(int32_t)left){
            uint32_t remaining=left-right;
            uint32_t address=0xaf598cu+cell*4u;
            do{
                uint32_t value=U(address);
                if(value&0x10u)U(address)=(value&0xffffffefu)|1u;
                address+=4u;
            }while(--remaining);
        }
        ++row;
    }
}

/* Complete 14-instruction leaf, 264-byte building records. */
uint32_t restore_mission_building_count_0046f190(void)
{
    uint32_t remaining=U(0x78d790u),record=0x799af0u,result=0;
    if((int32_t)remaining<1)return 0;
    do{
        if(B(record-16u)==1u&&W(record)==0xb6u)++result;
        record+=264u;
    }while(--remaining);
    return result;
}

/* Complete 87-instruction mission-to-city setup. These external calls are
   the real canonical algorithms, not local substitutes for their bodies. */
void FUN_004d5980(void)
{
    uint32_t i,record,offset,first,second;
    FUN_0040f8f0((void *)0xb7482cu,0x32c40u,0);
    restore_mission_terrain_004771c0();
    FUN_004bd360(RESTORE_LOAD_SETTINGS);
    if((int32_t)U(0xe945c0u)<145){
        B(0xbca30cu)=1;
        FUN_00486e70();
    }
    for(i=0;i<0x32c4u;++i)U(0xa43eacu+i*4u)=0xffffffffu;
    first=U(0xbca2c0u);second=U(0xbca2d0u);
    FUN_0051f780(0,0,second,first);
    FUN_004860d0();
    second=U(0xbca2d4u)-1u;first=U(0xbca2d0u)-1u;
    FUN_004be160(RESTORE_CITY_WORLD,0,0,(int32_t)first,(int32_t)second);
    i=0;record=0x785d74u;
    if((int32_t)U(0x785d70u)>0){
        do{
            restore_mission_record_00448a90(record);
            first=U(0x785d70u);
            ++i;record+=124u;
        }while((int32_t)i<(int32_t)first);
    }
    for(i=0;i<150u;++i)W(0x785d74u+i*124u)=(uint16_t)i;
    FUN_004e59a0();
    offset=U(0xeaac28u)*0x49d8u;
    W(0xea6084u+offset)=0;
    U(0xea6010u+offset)=0xffffffffu;
    U(0xea6014u+offset)=0xffffffffu;
    FUN_00442660();
    FUN_004426c0();
    for(record=0x77f748u;record<0x7843d8u;record+=98u){
        if(B(record+1u)&&B(record)==1u)B(record+0x1bu)=0;
    }
    for(record=0x77de6fu;record<0x77f747u;record+=106u)B(record)=0;
    U(0xe945c0u)=160u;
    FUN_004ddf90();
    U(0xbcb9b0u)=restore_mission_building_count_0046f190();
    FUN_004d4040();
}
