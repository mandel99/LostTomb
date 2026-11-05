#include "city_migration_actions.h"
#include "building_footprint.h"
#include "mission_unlock.h"
#include "map_commit_connect.h"
#include "map_commit_costs.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define H(a) ((int32_t)*(volatile int16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))

/* Whole469F80, original-owner repair: preserve the existing complete scan
   and removal algorithm, correcting generated field widths and array scales.
   The count is cached for the scan. The result DWORD is cached before state4
   and all callbacks; each rectangle reloads y, x and size after the callback. */
uint32_t restore_city_remove_first_building_kind(int32_t kind)
{
    uint32_t count=U(0x78d790u),index=1u,cursor=0x799af0u,offset,result,size;
    uint32_t x,y;
    if((int32_t)count<1)return 0;
    do {
        if(B(cursor-0x10u)==1u&&H(cursor)==kind){
            offset=index*264u;
            U(0xbc9694u)=0;
            result=U(0x7999e4u+offset);
            B(0x7999d8u+offset)=4;
            size=B(0x7999dbu+offset);
            x=(uint32_t)H(0x7999deu+offset);
            y=(uint32_t)H(0x7999e0u+offset);
            FUN_0047c2b0((int32_t)index,(int32_t)x,(int32_t)y,(int32_t)size,(int32_t)size);
            FUN_0053b090(0x11);
            y=(uint32_t)H(0x7999e0u+offset);
            x=(uint32_t)H(0x7999deu+offset);
            size=B(0x7999dbu+offset);
            FUN_00516f90((int32_t)(x-size),(int32_t)(y-size),(int32_t)(x+size),(int32_t)(y+size),1);
            y=(uint32_t)H(0x7999e0u+offset);
            x=(uint32_t)H(0x7999deu+offset);
            size=B(0x7999dbu+offset);
            FUN_005173a0((int32_t)(x-size),(int32_t)(y-size),(int32_t)(x+size),(int32_t)(y+size));
            return result;
        }
        ++index;cursor+=264u;
    }while((int32_t)index<=(int32_t)count);
    return 0;
}

uint32_t FUN_00469f80(int32_t kind)
{return restore_city_remove_first_building_kind(kind);}
