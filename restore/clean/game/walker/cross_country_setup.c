#include "pharaoh/walker/cross_country_setup.h"
#include "pharaoh/core/projectile_direction.h"
#include "pharaoh/map/terrain_queries.h"

_Static_assert(offsetof(PharaohWalkerQueryRecord,cross_country_setup)==0x4e,"cross-country target");
_Static_assert(offsetof(PharaohWalkerQueryRecord,cross_country_setup.error)==0x56,"cross-country error");
_Static_assert(offsetof(PharaohWalkerQueryRecord,cross_country_setup.axis)==0x58,"cross-country axis");

/* Reuse current/old MenuDragActorMove.c. WORD stores and signed comparisons
 * are distinct: the error arithmetic observes only the low WORD result. */
void pharaoh_walker_cross_country_setup(PharaohWalkerQueryRecord *walker,
    int32_t from_x,int32_t from_y,int32_t target_x,int32_t target_y,int32_t mode)
{
    uint32_t dx,dy,error;
    uint8_t direction;
    int32_t sx,sy;
    walker->cross_country_setup.target_x=(int16_t)target_x;
    walker->cross_country_setup.target_y=(int16_t)target_y;
    dx=from_x>target_x?(uint32_t)from_x-(uint32_t)target_x:
        (uint32_t)target_x-(uint32_t)from_x;
    walker->cross_country_setup.delta_x=(int16_t)dx;
    dy=from_y>target_y?(uint32_t)from_y-(uint32_t)target_y:
        (uint32_t)target_y-(uint32_t)from_y;
    walker->cross_country_setup.delta_y=(int16_t)dy;
    dx=(uint16_t)walker->cross_country_setup.delta_x;
    dy=(uint16_t)walker->cross_country_setup.delta_y;
    if((int16_t)dx<(int16_t)dy)error=dx*2u-dy;
    else if((int16_t)dx>(int16_t)dy)error=dy*2u-dx;
    else error=0;
    walker->cross_country_setup.error=(int16_t)error;
    if(mode){
        walker->direction=pharaoh_projectile_direction(from_x,from_y,target_x,target_y);
    }else{
        direction=(uint8_t)pharaoh_map_general_direction(from_x,from_y,target_x,target_y);
        walker->direction=direction;
        sx=walker->cross_country_setup.delta_x;sy=walker->cross_country_setup.delta_y;
        if(sy>sx*2){
            if(direction==1||direction==7)walker->direction=0;
            else if(direction==3||direction==5)walker->direction=4;
        }
        sy=walker->cross_country_setup.delta_y;sx=walker->cross_country_setup.delta_x;
        if(sx>sy*2){
            direction=walker->direction;
            if(direction==1||direction==3)walker->direction=2;
            else if(direction==5||direction==7)walker->direction=6;
        }
    }
    sy=walker->cross_country_setup.delta_y;sx=walker->cross_country_setup.delta_x;
    walker->cross_country_setup.axis=sy>sx?2:1;
}
