/* Whole logical movement leaf bodies. Original RAW/frame physical outputs
   remain available; these void APIs promise their fixed-width RAM effects. */
#include <stdint.h>
#define LEAF_W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define LEAF_B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
typedef char movement_leaves_pointer32[sizeof(void *)==4?1:-1];

__declspec(noinline) void restore_actor_update_route_error(int32_t actor)
{
    uint32_t offset=(uint32_t)actor*388u;
    uint8_t axis=LEAF_B(0xbcd650u+offset);
    uint16_t error=LEAF_W(0xbcd64eu+offset),component,other;
    if(axis==1u){
        component=LEAF_W(0xbcd64cu+offset);
        if((int16_t)error<0)error=(uint16_t)((uint32_t)error+(uint32_t)component*2u);
        else{
            other=LEAF_W(0xbcd64au+offset);
            component=(uint16_t)(component-other);
            error=(uint16_t)((uint32_t)error+(uint32_t)component*2u);
        }
        LEAF_W(0xbcd64eu+offset)=error;
        /* DEC reads the current WORD again after the error publication. */
        LEAF_W(0xbcd64au+offset)=(uint16_t)(LEAF_W(0xbcd64au+offset)-1u);
    }else{
        component=LEAF_W(0xbcd64au+offset);
        if((int16_t)error<0)error=(uint16_t)((uint32_t)error+(uint32_t)component*2u);
        else{
            other=LEAF_W(0xbcd64cu+offset);
            component=(uint16_t)(component-other);
            error=(uint16_t)((uint32_t)error+(uint32_t)component*2u);
        }
        LEAF_W(0xbcd64eu+offset)=error;
        LEAF_W(0xbcd64cu+offset)=(uint16_t)(LEAF_W(0xbcd64cu+offset)-1u);
    }
}

/* The two54-byte native bodies are identical except for four field
   displacements. Capture each WORD once; the old source reread the target. */
static void movement_step_route_pixel(uint32_t current_address,uint32_t target_address)
{
    uint16_t current,target;
    current=LEAF_W(current_address);
    target=LEAF_W(target_address);
    if((int16_t)current<(int16_t)target)LEAF_W(current_address)=(uint16_t)(current+1u);
    else if((int16_t)current>(int16_t)target)LEAF_W(current_address)=(uint16_t)(current-1u);
}
__declspec(noinline) void restore_actor_step_route_pixel_y(int32_t actor)
{
    uint32_t offset=(uint32_t)actor*388u;
    movement_step_route_pixel(0xbcd644u+offset,0xbcd648u+offset);
}
__declspec(noinline) void restore_actor_step_route_pixel_x(int32_t actor)
{
    uint32_t offset=(uint32_t)actor*388u;
    movement_step_route_pixel(0xbcd642u+offset,0xbcd646u+offset);
}
