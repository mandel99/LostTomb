#include "city_spawn_helpers.h"
#include "road_place.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define S(a) ((int32_t)(int16_t)W(a))

/* Native004673C0: logical AL output; do not claim a full EAX result. */
uint8_t restore_city_refresh_fertility_building_image(int32_t building)
{
    uint32_t record=(uint32_t)building*264u,tile,image,side;
    int32_t x,y;
    uint8_t fertile;
    if(B(0x7999d8u+record)!=1u)return 0;
    tile=U(0x7999e4u+record);
    fertile=(uint8_t)((int8_t)B(0xac2d4cu+tile)>30);
    image=U(fertile?0xf56584u:0xf564ccu);
    side=B(0x7999dbu+record);
    y=S(0x7999e0u+record);x=S(0x7999deu+record);
    FUN_0047bf50((uint16_t)building,x,y,(int32_t)side,(int32_t)side,image,8);
    return fertile;
}
