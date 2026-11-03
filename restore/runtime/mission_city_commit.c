#include "mission_city_commit.h"
#include "event_parameters.h"
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

/* Mission loading and simulation share the explicit C providers. */
void restore_mission_range_004487f0(uint32_t self)
{(void)restore_event_parameter_choose(self,0);}
void restore_mission_record_00448a90(uint32_t self)
{(void)restore_event_record_reset(self);}

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
