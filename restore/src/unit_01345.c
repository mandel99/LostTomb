#include "actor_city_refresh.h"
#include "actor_walking.h"
#define REFRESH_U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define REFRESH_W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define REFRESH_B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define REFRESH_H(a) ((int32_t)(int16_t)REFRESH_W(a))

/* Complete existing native480060 radius loop in src/unit_01171.c; its
   child uses restore_map_find_unmarked_tile. Full EAX is always0 or1. */
extern uint32_t FUN_00480060(uint32_t x,uint32_t y,uint32_t size,int32_t max_radius);

/* Whole497B90,98 instructions. Preserve the existing scan and both repair
   branches in their original source owner. Repairs: BYTE map/kind/action,
   388-byte actors, signed WORD building IDs, 264-byte buildings, DWORD map
   offset/current actor, and the native loads on both sides of callbacks. */
void restore_actor_refresh_sentries_on_marked_tiles(void)
{
    uint32_t cursor=0xbcd7d6u,index=1u,building,coord_x,coord_y,base,record;
    uint16_t x,y;

    do {
        if(REFRESH_B(cursor-0x50u)==42u &&
           REFRESH_B(0xebab20u+REFRESH_U(cursor-0x3au))!=0u){
            coord_y=(uint32_t)REFRESH_H(cursor-0x44u);
            coord_x=(uint32_t)REFRESH_H(cursor-0x46u);
            if(FUN_00480060(coord_x,coord_y,1u,2)==0u){
                REFRESH_B(cursor-0x22u)=8u;
                FUN_004b5420((int32_t)index);
                building=(uint32_t)REFRESH_H(cursor)*264u;
                x=REFRESH_W(0x7999deu+building);
                y=REFRESH_W(0x7999e0u+building);
                REFRESH_W(cursor-0x46u)=x;
                REFRESH_W(cursor-0x2eu)=x;
                REFRESH_W(cursor-0x44u)=y;
                REFRESH_W(cursor-0x2cu)=y;
                coord_y=(uint32_t)REFRESH_H(cursor-0x44u);
                base=REFRESH_U(0xbca2d8u);
                coord_x=(uint32_t)REFRESH_H(cursor-0x46u);
                REFRESH_U(cursor-0x3au)=base+coord_x+coord_y*228u;
                FUN_004b52f0((int32_t)index);
                REFRESH_B(cursor-0x22u)=8u;
                FUN_004b3250((int32_t)index);
            }else{
                FUN_004b3250((int32_t)REFRESH_U(0xd056f8u));
                REFRESH_B(cursor-0x21u)=0u;
                FUN_004b5420((int32_t)index);
                coord_x=REFRESH_U(0xd3b4e4u);
                coord_y=REFRESH_U(0xd3b4e0u);
                REFRESH_W(cursor-0x46u)=(uint16_t)coord_x;
                REFRESH_W(cursor-0x42u)=(uint16_t)coord_x;
                REFRESH_W(cursor-0x44u)=(uint16_t)coord_y;
                REFRESH_W(cursor-0x40u)=(uint16_t)coord_y;
                REFRESH_W(cursor-0x10u)=(uint16_t)(coord_x*15u);
                REFRESH_W(cursor-0x0eu)=(uint16_t)(coord_y*15u);
                base=REFRESH_U(0xbca2d8u);
                REFRESH_U(cursor-0x3au)=base+coord_x+coord_y*228u;
                FUN_004b52f0((int32_t)index);
                record=REFRESH_U(0xd056f8u)*388u;
                REFRESH_B(0xbcd630u+record)=11u;
                x=REFRESH_W(0xbcd624u+record);
                REFRESH_W(0xbcd61cu+record)=x;
                y=REFRESH_W(0xbcd626u+record);
                REFRESH_W(0xbcd61eu+record)=y;
            }
        }
        cursor+=388u;
        ++index;
    }while((int32_t)cursor<(int32_t)0xc8ad92u);
}

void FUN_00497b90(void)
{
    restore_actor_refresh_sentries_on_marked_tiles();
}
