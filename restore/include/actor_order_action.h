#ifndef RESTORE_ACTOR_ORDER_ACTION_H
#define RESTORE_ACTOR_ORDER_ACTION_H
#include <stdint.h>
#include <stddef.h>
#pragma pack(push,1)
typedef struct RestoreActionGroup {
    uint8_t opaque0[3],marker;int16_t kind;uint8_t opaque6[2];int16_t actors[16];
    uint8_t opaque40[2];int16_t action_tag,health,x,y,target_x,target_y;uint8_t opaque54[10];
    int16_t leader;uint8_t opaque66[6];int16_t variant;uint8_t opaque74[26];
    int16_t locked;uint8_t order_mode,opaque103[3],subtype,opaque107[8],disabled;
    uint8_t opaque116[16];uint32_t direct_origin;uint8_t opaque136[8];
} RestoreActionGroup;
#pragma pack(pop)
typedef char action_group_size[sizeof(RestoreActionGroup)==144?1:-1];
typedef char action_group_actors[offsetof(RestoreActionGroup,actors)==8?1:-1];
typedef char action_group_health[offsetof(RestoreActionGroup,health)==44?1:-1];
typedef char action_group_origin[offsetof(RestoreActionGroup,x)==46?1:-1];
typedef char action_group_leader[offsetof(RestoreActionGroup,leader)==64?1:-1];
typedef char action_group_lock[offsetof(RestoreActionGroup,locked)==100?1:-1];
typedef char action_group_disabled[offsetof(RestoreActionGroup,disabled)==115?1:-1];
typedef char action_group_override[offsetof(RestoreActionGroup,direct_origin)==132?1:-1];
uint32_t FUN_004b6400(int32_t x,int32_t y,int32_t group,uint32_t flags);
uint32_t FUN_004b63d0(int32_t actor);
void FUN_004b7b50(int32_t group,int16_t delta);
void FUN_004b3250(int32_t actor);
/* Original cdecl entry reads one argument. Preserve the accepted public ABI;
   the named callsite contract carries the two known discarded native extras. */
void FUN_004e2970(int32_t message);
void restore_actor_order_message(int32_t message,uint32_t unused1,uint32_t unused2);
#endif
