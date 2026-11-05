#include "approach_member_candidates.h"

#define AS_B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define AS_W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define AS_I(a) (*(volatile int32_t *)(uintptr_t)(uint32_t)(a))

typedef char approach_scans_require_pointer32[sizeof(void *) == 4 ? 1 : -1];

/* Native545BE0/5590C0/556430/54FB40 save the incoming ECX receiver.
   These scans call547C10 directly; the earlier three kind scans use the
   receiver's virtual slot instead. Only AL determines acceptance. */
static int32_t find_direct_approach_kind(uint32_t receiver,uint16_t kind,
                                       int32_t actor,int before_state_four)
{
    uint32_t type=0x799af0u;
    int32_t building=1,selected=0;
    do {
        if(selected!=0)return selected;
        if(AS_B(type-16u)==1u && AS_W(type)==kind &&
           (!before_state_four || AS_I(type+80u)<4)) {
            if((uint8_t)restore_actor_member_accept_building(receiver,building,actor,1u)!=0)
                selected=building;
        }
        type+=264u;
        ++building;
    }while((int32_t)type<0x89b6e8);
    return selected;
}

__declspec(noinline) int32_t restore_actor_find_approach_kind229(
    uint32_t receiver,uint32_t x,uint32_t y,int32_t actor)
{
    (void)x;(void)y;
    return find_direct_approach_kind(receiver,229u,actor,0);
}

__declspec(noinline) int32_t restore_actor_find_approach_kind234(
    uint32_t receiver,uint32_t x,uint32_t y,int32_t actor)
{
    (void)x;(void)y;
    return find_direct_approach_kind(receiver,234u,actor,0);
}

__declspec(noinline) int32_t restore_actor_find_approach_kind235(
    uint32_t receiver,uint32_t x,uint32_t y,int32_t actor)
{
    (void)x;(void)y;
    return find_direct_approach_kind(receiver,235u,actor,0);
}

__declspec(noinline) int32_t restore_actor_find_approach_kind236(
    uint32_t receiver,uint32_t x,uint32_t y,int32_t actor)
{
    (void)x;(void)y;
    return find_direct_approach_kind(receiver,236u,actor,1);
}
