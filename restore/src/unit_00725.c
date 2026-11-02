#include "earthquake_event.h"
#include "mission_unlock.h"
#include "road_refresh.h"
#include "surface_passes.h"
#include "map_commit_connect.h"
#include "map_commit_costs.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
extern void FUN_0046d100(int32_t building,int32_t mode);
extern void FUN_004695d0(int32_t building,int32_t mode);
extern void FUN_00476530(void);
extern void FUN_0047b570(void);
extern void FUN_0048c0e0(int32_t x,int32_t y,int32_t width,int32_t height);

/* Whole44BD50. Cache the cell, but reload its signed building ID after
   demolition callbacks. Only the building's state BYTE is replaced. */
void restore_earthquake_destroy_tile(int32_t x,int32_t y)
{
    uint32_t cell=U(0xbca2d8u)+(uint32_t)x+(uint32_t)y*228u;
    int32_t building=(int16_t)W(0xb4e6fcu+cell*2u);
    if(building){
        FUN_0046d100(building,0);FUN_004695d0(building,1);
        FUN_0053b090(17);
    }
    building=(int16_t)W(0xb4e6fcu+cell*2u);
    if(building)B(0x7999d8u+(uint32_t)building*264u)=5;
    U(0xaf598cu+cell*4u)=0;
    FUN_004798e0(x,y,1);FUN_00476530();FUN_0047b570();
    FUN_00475720(0,0,(int32_t)(U(0xbca2d0u)-1u),
        (int32_t)(U(0xbca2d4u)-1u),1);
    FUN_00516f90((int32_t)((uint32_t)x-1u),(int32_t)((uint32_t)y-1u),
        (int32_t)((uint32_t)x+1u),(int32_t)((uint32_t)y+1u),1);
    FUN_005173a0((int32_t)((uint32_t)x-1u),(int32_t)((uint32_t)y-1u),
        (int32_t)((uint32_t)x+1u),(int32_t)((uint32_t)y+1u));
    FUN_005178e0();FUN_0048c0e0(x,y,1,1);
}
void FUN_0044bd50(int32_t x,int32_t y){restore_earthquake_destroy_tile(x,y);}
