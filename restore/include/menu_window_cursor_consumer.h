#ifndef RESTORE_MENU_WINDOW_CURSOR_CONSUMER_H
#define RESTORE_MENU_WINDOW_CURSOR_CONSUMER_H
#include <stdint.h>
#include <stddef.h>
typedef struct RestoreOrderActor {
    uint8_t opaque0[8];int16_t next;uint8_t kind,opaque11[6],direction,opaque18[2];
    int16_t x,y;uint8_t opaque24[12];int16_t target_x,target_y;
    uint8_t opaque40[8];int16_t secondary_x,secondary_y;uint8_t opaque52[4];
    uint8_t state,opaque57;int16_t route;uint8_t opaque60[36];int16_t tag;
    uint8_t opaque98[48],action_tag,opaque147[157],can_special,opaque305[43];
    uint32_t order_mode;uint8_t opaque352[36];
} RestoreOrderActor;
typedef char order_actor_size[sizeof(RestoreOrderActor)==388?1:-1];
typedef char order_actor_next[offsetof(RestoreOrderActor,next)==8?1:-1];
typedef char order_actor_kind[offsetof(RestoreOrderActor,kind)==10?1:-1];
typedef char order_actor_direction[offsetof(RestoreOrderActor,direction)==17?1:-1];
typedef char order_actor_xy[offsetof(RestoreOrderActor,x)==20?1:-1];
typedef char order_actor_target[offsetof(RestoreOrderActor,target_x)==36?1:-1];
typedef char order_actor_secondary[offsetof(RestoreOrderActor,secondary_x)==48?1:-1];
typedef char order_actor_state[offsetof(RestoreOrderActor,state)==56?1:-1];
typedef char order_actor_route[offsetof(RestoreOrderActor,route)==58?1:-1];
typedef char order_actor_tag[offsetof(RestoreOrderActor,tag)==96?1:-1];
typedef char order_actor_action_tag[offsetof(RestoreOrderActor,action_tag)==146?1:-1];
typedef char order_actor_special[offsetof(RestoreOrderActor,can_special)==304?1:-1];
typedef char order_actor_mode[offsetof(RestoreOrderActor,order_mode)==348?1:-1];
static RestoreOrderActor *restore_order_actor(uint32_t id)
{return (RestoreOrderActor *)(uintptr_t)(0xbcd5f8u+id*388u);}
void FUN_0040ba40(void);
int32_t FUN_004b5520(uint32_t tile);
void FUN_004b3250(int32_t id);
uint8_t FUN_0051cd80(int32_t x,int32_t y,int32_t target_x,int32_t target_y);
int32_t FUN_004cde60(int32_t value,int32_t divisor);
int32_t FUN_0040c180(int32_t x,int32_t y,int32_t target_x,int32_t target_y,int32_t *out_x,int32_t *out_y);
int32_t FUN_0040bda0(int32_t x,int32_t y,int32_t target_x,int32_t target_y,int32_t *out_x,int32_t *out_y);
uint32_t FUN_004b6400(int32_t x,int32_t y,int32_t group,uint32_t flags);
#endif
