#include "city_spawn_helpers.h"
#include "map_refresh_postpass6.h"
#include "road_place.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define S(a) ((int32_t)(int16_t)W(a))

/* Native00467310: retain the cached tile across the terrain query. */
void restore_city_refresh_masked_building_image(int32_t building)
{
    uint32_t record=(uint32_t)building*264u,tile,found,image;
    int32_t x,y;
    int8_t fertility;
    if(B(0x7999d8u+record)!=1u)return;
    y=S(0x7999e0u+record);x=S(0x7999deu+record);tile=U(0x7999e4u+record);
    found=FUN_004816b0(x,y,2,0x80u);
    B(0x799a0du+record)=(uint8_t)(found!=0);
    if(W(0x799a18u+record)==0)return;
    if(B(0x799a0du+record)==0)return;
    fertility=(int8_t)B(0xac2d4cu+tile);image=U(0xf56644u);
    if(fertility>=30)image+=2u;
    x=S(0x7999deu+record);y=S(0x7999e0u+record);
    FUN_0047bf50((uint16_t)building,x,y,2,2,image,8);
}
