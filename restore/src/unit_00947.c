#include "city_disaster_fields.h"
#include "building_destruction.h"
#include "building_footprint.h"
#include "building_removal.h"
#include "map_commit_connect.h"
#include "map_commit_costs.h"
#include "mission_unlock.h"

#define FIRE_U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define FIRE_W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define FIRE_B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define FIRE_H(a) ((int32_t)(int16_t)FIRE_W(a))
#define FIRE_I(a) ((int32_t)(uint32_t)(a))

/* Whole469730. Retain the phase's signed WORD domain, WORD active-ID list,
   DWORD list/ring counters, and the two cached 31-step random states. Each
   label corresponds to a native candidate/reload phase, preserving which
   failed selector calls reload the global random states. */
void restore_city_tick_burning_fields(void)
{
    uint32_t limit,id=1u,phase_address=0x799b2au,list_index=0u;
    uint32_t changed=0u,random_a,random_b,bit_a,bit_b,steps;
    uint32_t total,ring_index,previous,direction,next_direction,cell,tile,size;
    uint16_t phase;
    int32_t neighbor,coord_x,coord_y;

    limit=FIRE_U(0x78d790u);
    FIRE_U(0x78d800u)=0u;
    FIRE_U(0x78d7fcu)=0u;
    FIRE_U(0x78d804u)=0u;
    if(FIRE_I(limit)<1)return;
    random_a=FIRE_U(0xd3b384u);
    random_b=FIRE_U(0xd3b380u);
    for(;;){
        if(FIRE_B(phase_address-0x4au)!=1u)goto next_building;
        if(FIRE_W(phase_address-0x3au)!=99u)goto next_building;
        if((int16_t)FIRE_W(phase_address)<0)FIRE_W(phase_address)=0u;
        phase=(uint16_t)(FIRE_W(phase_address)+1u);
        FIRE_W(phase_address)=phase;
        if((int16_t)phase>32){
            FIRE_B(phase_address-0x4au)=4u;
            size=FIRE_B(phase_address-0x47u);
            FIRE_U(0xbc9694u)=0u;
            coord_x=FIRE_H(phase_address-0x44u);
            coord_y=FIRE_H(phase_address-0x42u);
            FUN_0047c2b0(FIRE_I(id),coord_x,coord_y,FIRE_I(size),FIRE_I(size));
            changed=1u;
            goto reload_random;
        }
        if(FIRE_B(phase_address+0x73u)!=0u)goto next_building;
        total=FIRE_U(0x78d804u);
        FIRE_W(0x78dd90u+list_index*2u)=(uint16_t)id;
        total+=1u;
        list_index+=1u;
        FIRE_U(0x78d804u)=total;
        FIRE_U(0x78d7fcu)=list_index;
        if(FIRE_I(list_index)>=500)FIRE_U(0x78d7fcu)=499u;
        if(FIRE_B(0x784cb0u)==2u){
            if((phase&3u)!=0u)goto next_building;
        }else{
            if((phase&7u)!=0u)goto next_building;
        }
        ring_index=FIRE_U(0xd3b1c4u);
        total=FIRE_U(0xd3b374u);
        FIRE_U(0xd3b1c8u+ring_index*4u)=total;
        ring_index+=1u;
        FIRE_U(0xd3b1c4u)=ring_index;
        if(FIRE_I(ring_index)>=100)FIRE_U(0xd3b1c4u)=0u;
        steps=31u;
        do{
            bit_a=((random_a>>4)^random_a)&1u;
            bit_b=((random_b>>4)^random_b)&1u;
            random_a>>=1;
            random_b>>=1;
            if(bit_a!=0u)random_a|=0x40000000u;
            if(bit_b!=0u)random_b|=0x40000000u;
        }while(--steps!=0u);
        FIRE_U(0xd3b374u)=random_a&0x7fu;
        FIRE_U(0xd3b370u)=random_a&0x7fffu;
        FIRE_U(0xd3b378u)=random_a&7u;
        FIRE_U(0xd3b364u)=random_b&0x7fffu;
        FIRE_U(0xd3b380u)=random_b;
        FIRE_U(0xd3b368u)=random_b&0x7fu;
        FIRE_U(0xd3b384u)=random_a;
        FIRE_U(0xd3b36cu)=random_b&7u;
        if(((uint32_t)FIRE_B(phase_address+3u)&3u)!=(random_a&3u))goto next_building;

        direction=FIRE_U(0x78d808u);
        previous=direction-1u;
        if(FIRE_I(previous)<0)previous=7u;
        next_direction=direction+1u;
        if(FIRE_I(next_direction)>7)next_direction=0u;
        cell=FIRE_U(phase_address-0x3eu);
        tile=FIRE_U(0x5f80fcu+direction*4u)+cell;
        neighbor=FIRE_H(0xb4e6fcu+tile*2u);
        if(neighbor!=0&&FIRE_B(0x799a24u+(uint32_t)neighbor*264u)==0u){
            if(restore_building_allows_disaster_spread(neighbor)!=0u)goto destroy_neighbor;
            random_a=FIRE_U(0xd3b384u);
            random_b=FIRE_U(0xd3b380u);
        }
        tile=FIRE_U(0x5f80fcu+previous*4u)+cell;
        neighbor=FIRE_H(0xb4e6fcu+tile*2u);
        if(neighbor!=0&&FIRE_B(0x799a24u+(uint32_t)neighbor*264u)==0u){
            if(restore_building_allows_disaster_spread(neighbor)!=0u)goto destroy_neighbor;
            random_a=FIRE_U(0xd3b384u);
            random_b=FIRE_U(0xd3b380u);
        }
        tile=FIRE_U(0x5f80fcu+next_direction*4u)+cell;
        neighbor=FIRE_H(0xb4e6fcu+tile*2u);
        if(neighbor==0||FIRE_B(0x799a24u+(uint32_t)neighbor*264u)!=0u)goto next_building;
        if(restore_building_allows_disaster_spread(neighbor)==0u)goto reload_random;
destroy_neighbor:
        FUN_0046d100(neighbor,0u);
        FUN_004695d0(neighbor,1);
        FUN_0053b090(18);
        changed=1u;
reload_random:
        random_a=FIRE_U(0xd3b384u);
        random_b=FIRE_U(0xd3b380u);
next_building:
        limit=FIRE_U(0x78d790u);
        id+=1u;
        phase_address+=264u;
        if(FIRE_I(id)>FIRE_I(limit))break;
        list_index=FIRE_U(0x78d7fcu);
    }
    if(changed!=0u){FUN_00517370();FUN_00517660();}
}

void FUN_00469730(void)
{restore_city_tick_burning_fields();}
