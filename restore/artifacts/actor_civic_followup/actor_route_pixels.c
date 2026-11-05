/* Ordinary route-pixel advancement. Original RAW/frame routes remain until
   the independent physical/caller scope is qualified at integration. */
#include <stdint.h>
#define MOVE_U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define MOVE_W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define MOVE_B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define MOVE_H(a) ((int32_t)(int16_t)MOVE_W(a))
typedef char movement_pointer32[sizeof(void *)==4?1:-1];
extern void FUN_004b5420(int32_t actor),FUN_004b52f0(int32_t actor);
extern void FUN_004b4180(int32_t actor),FUN_004b43a0(int32_t actor),FUN_004b43e0(int32_t actor);

__declspec(noinline) int32_t restore_actor_advance_route_pixels(int32_t actor,int32_t steps)
{
    uint32_t offset=(uint32_t)actor*388u,step=0,grid_base,tile;
    int32_t arrived=0,remaining_y,remaining_x,pixel_x,pixel_y,coordinate_y,coordinate_x;
    uint8_t phase,terrain_timer;

    FUN_004b5420(actor);
    while((int32_t)step<steps){
        phase=MOVE_B(0xbcd614u+offset);
        MOVE_B(0xbcd614u+offset)=phase?(uint8_t)(phase-1u):0;
        remaining_y=MOVE_H(0xbcd64cu+offset);
        remaining_x=MOVE_H(0xbcd64au+offset);
        if(remaining_y+remaining_x<=0){arrived=1;break;}

        FUN_004b4180(actor);
        if(MOVE_B(0xbcd650u+offset)==2u){
            FUN_004b43a0(actor);
            if(MOVE_H(0xbcd64eu+offset)>=0){
                MOVE_W(0xbcd64au+offset)=(uint16_t)(MOVE_W(0xbcd64au+offset)-1u);
                FUN_004b43e0(actor);
            }
        }else{
            FUN_004b43e0(actor);
            if(MOVE_H(0xbcd64eu+offset)>=0){
                MOVE_W(0xbcd64cu+offset)=(uint16_t)(MOVE_W(0xbcd64cu+offset)-1u);
                FUN_004b43a0(actor);
            }
        }
        ++step;
    }

    pixel_x=MOVE_H(0xbcd642u+offset);
    MOVE_W(0xbcd60cu+offset)=(uint16_t)(pixel_x/15);
    /* X publication precedes the fresh pixelY read. Native loads gridbase
       after that read but before publishing Y; do not share a cached pair. */
    pixel_y=MOVE_H(0xbcd644u+offset);coordinate_y=pixel_y/15;
    grid_base=MOVE_U(0xbca2d8u);
    MOVE_W(0xbcd60eu+offset)=(uint16_t)coordinate_y;
    coordinate_x=MOVE_H(0xbcd60cu+offset);
    tile=grid_base+(uint32_t)coordinate_x+(uint32_t)coordinate_y*228u;
    MOVE_U(0xbcd618u+offset)=tile;
    if((MOVE_B(0xaf598cu+tile*4u)&8u)!=0u)MOVE_B(0xbcd638u+offset)=8;
    else{
        terrain_timer=MOVE_B(0xbcd638u+offset);
        if(terrain_timer!=0u)MOVE_B(0xbcd638u+offset)=(uint8_t)(terrain_timer-1u);
    }
    FUN_004b52f0(actor);
    return arrived;
}
