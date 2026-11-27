#include "pharaoh/map/ferry_routing.h"
#include <stddef.h>

_Static_assert(sizeof(PharaohBuildingTypeRecord)==264 &&
    offsetof(PharaohBuildingTypeRecord,linked_ferry_building_id)==196 &&
    offsetof(PharaohBuildingTypeRecord,runtime_value_ca)==202 &&
    sizeof(PharaohFerryQueueRecord)==224 &&
    sizeof(PharaohWalkerQueryRecord)==388 &&
    offsetof(PharaohWalkerQueryRecord,action_state_before_6_7)==16 &&
    offsetof(PharaohWalkerQueryRecord,action_state)==56,
    "native ferry queues and actor fields");

void pharaoh_ferry_queue_release(PharaohFerryContext *c,int32_t building)
{
    const uint32_t id=c->buildings[building].runtime_value_ca;
    PharaohRingQueue *const queue=&c->queues[id].queue;
    if(pharaoh_ring_queue_size(queue)!=0){
        if(pharaoh_ring_queue_size(queue)>0){
            int32_t iteration=0;
            do{
                const int32_t actor=pharaoh_ring_queue_pop(queue);
                c->walkers[actor].action_state=c->walkers[actor].action_state_before_6_7;
                pharaoh_walker_detach_slot_link(c->walkers,c->slot_walkers,actor);
                ++iteration;
            }while(iteration<pharaoh_ring_queue_size(queue));
        }
        pharaoh_ring_queue_clear(queue);
    }
}

void pharaoh_ferry_queues_release(PharaohFerryContext *c,int32_t building)
{
    const int32_t linked=c->buildings[building].linked_ferry_building_id;
    pharaoh_ferry_queue_release(c,building);
    if(linked>0) pharaoh_ferry_queue_release(c,linked);
}
