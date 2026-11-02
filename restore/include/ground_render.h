#ifndef RESTORE_GROUND_RENDER_H
#define RESTORE_GROUND_RENDER_H
#include <stdint.h>
#include "sprite_render.h"
#include "graphics_53f250.h"
#include "game_diagnostics.h"
#define GR_U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define GR_W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define GR_B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define GR_S(v) ((int32_t)(uint32_t)(v))
void FUN_00430ef0(void);
void FUN_004316f0(void);
void FUN_00430e60(uint32_t x,uint32_t y,uint32_t mode,uint32_t actor_parameter);
void FUN_0053b670(void);
uint32_t FUN_0053b830(uint32_t type);
uint32_t FUN_0047a1c0(int32_t x);
void FUN_0051f780(void);
void FUN_00436b60(uint32_t);
void FUN_00439740(uint32_t);
void FUN_0041d410(int32_t,int32_t);
void FUN_00431ce0(int32_t,int32_t);
void FUN_004321a0(int32_t,int32_t,uint32_t,uint8_t);
void FUN_00432620(int32_t,int32_t);
void FUN_00432d30(int32_t,int32_t,int32_t,uint32_t);
uint32_t FUN_00431690(int32_t building);
void FUN_00432920(int32_t building);
void FUN_00433760(int32_t,int32_t,uint32_t);
void FUN_00434aa0(int32_t,int32_t,uint32_t,uint32_t);
void FUN_00434e10(int32_t,int32_t);
/* The original boundary call pushes column and row, but its complete body
   consumes only the column. The no-op similarly consumes no arguments.
   Keep the extra evaluated inputs visible to the verification adapters. */
#ifdef GROUND_RENDER_VERIFY
uint32_t restore_ground_boundary_at(int32_t column,int32_t row);
void restore_ground_empty_at(int32_t x,int32_t y);
#else
static __inline uint32_t restore_ground_boundary_at(int32_t column,int32_t row){(void)row;return FUN_0047a1c0(column);}
static __inline void restore_ground_empty_at(int32_t x,int32_t y){(void)x;(void)y;FUN_0051f780();}
#endif
static __inline int ground_footprint(uint32_t f,uint32_t x,uint32_t y,uint32_t *px,uint32_t *py)
{
    switch(f){
    case 0:*px=x;*py=y;return 1;
    case 1:*py=y-GR_U(0x70e44c);*px=x+GR_U(0x70e45c)+1u;return 1;
    case 2:*py=y-GR_U(0x70e448);*px=x+GR_U(0x70e458)+2u;return 1;
    case 3:*py=y-GR_U(0x70e444);*px=x+GR_U(0x70e454)+3u;return 1;
    case 4:*py=y-GR_U(0x70e440);*px=x+GR_U(0x70e450)+4u;return 1;
    case 5:*py=y-GR_U(0x70e440)-GR_U(0x70e44c);*px=x+GR_U(0x70e450)+GR_U(0x70e45c)+5u;return 1;
    default:return 0; /* Native unsupported-footprint branch skips the draw. */
    }
}
static __inline int ground_neighbor_occludes(uint32_t tile)
{
    uint32_t offset,actor,visited=0;int32_t id;
    if(GR_S(tile)>=0x0fff0000)return 0;
    offset=(uint32_t)(int32_t)(int16_t)GR_W(0xb4e6fcu+tile*2u)*264u;
    if(GR_W(0x7999e8u+offset)==183&&GR_B(0x799a2eu+offset)!=0&&GR_U(0x799a40u+offset)!=0&&GR_B(0x799a31u+offset)<=3)return 1;
    id=(int16_t)GR_W(0xb350dcu+tile*2u);
    while(id>0){
        /* A valid cell can visit at most1999 actor slots. Capture corruption
           instead of leaving the UI in an unreportable infinite loop. */
        if(id>=2000||++visited>=2000u)restore_game_actor_grid_failure(tile,id);
        actor=(uint32_t)id*388u;
        if(GR_B(0xbcd65du+actor)==0){switch(GR_B(0xbcd602u+actor)){case 13:case 45:case 50:case 56:case 57:return 1;default:break;}}
        id=(int16_t)GR_W(0xbcd600u+actor);
    }
    return 0;
}
#endif
