#include "pharaoh/walker/tile_chain.h"
#include "pharaoh/runtime/platform_measure_speed.h"

#include <stddef.h>

_Static_assert(sizeof(PharaohWalkerQueryRecord)==388 &&
    offsetof(PharaohWalkerQueryRecord,next_walker_id_on_same_tile)==8 &&
    offsetof(PharaohWalkerQueryRecord,grid_offset)==0x20 &&
    offsetof(PharaohWalkerQueryRecord,tile_chain_ordinal)==0x8a,
    "native tile-chain walker fields");

/* Reused MenuDragActorWalkLink C. The native no-operation child is real RET. */
void pharaoh_walker_attach_to_tile(PharaohWalkerTileChainContext *c,int32_t id)
{
    PharaohWalkerQueryRecord *const walker=&c->walkers[id];
    if(walker->grid_offset>=0){
        int32_t tail=c->grid_heads[walker->grid_offset];
        walker->tile_chain_ordinal=0;
        if(tail==0){
            c->grid_heads[walker->grid_offset]=(int16_t)id;
            pharaoh_actor_walking_no_operation();
            return;
        }
        walker->tile_chain_ordinal=1;
        int32_t next=c->walkers[tail].next_walker_id_on_same_tile;
        while(next!=0){
            tail=next;
            if(tail==id)goto complete;
            walker->tile_chain_ordinal=(uint8_t)(walker->tile_chain_ordinal+1u);
            next=c->walkers[tail].next_walker_id_on_same_tile;
        }
        if(walker->tile_chain_ordinal>20)walker->tile_chain_ordinal=20;
        /* Possible original bug: an existing one-element head equal to id
         * becomes self-linked. Keep the native order and lack of a guard. */
        c->walkers[tail].next_walker_id_on_same_tile=(int16_t)id;
    }
complete:
    pharaoh_actor_walking_no_operation();
}

/* Reused MenuDragActorWalkUnlink C. */
void pharaoh_walker_detach_from_tile(PharaohWalkerTileChainContext *c,int32_t id)
{
    PharaohWalkerQueryRecord *const walker=&c->walkers[id];
    const int32_t tile=walker->grid_offset;
    if(tile<0){pharaoh_actor_walking_no_operation();return;}
    int32_t previous=c->grid_heads[tile];
    if(previous==0){pharaoh_actor_walking_no_operation();return;}
    if(previous==id){
        c->grid_heads[tile]=walker->next_walker_id_on_same_tile;
        walker->next_walker_id_on_same_tile=0;
        pharaoh_actor_walking_no_operation();return;
    }
    int32_t next=c->walkers[previous].next_walker_id_on_same_tile;
    while(next!=id){
        if(previous==0)return;
        previous=next;
        /* Native visits record zero before testing previous on the next
         * iteration. Do not replace this with an earlier end-of-list test. */
        next=c->walkers[previous].next_walker_id_on_same_tile;
    }
    c->walkers[previous].next_walker_id_on_same_tile=
        walker->next_walker_id_on_same_tile;
    walker->next_walker_id_on_same_tile=0;
    pharaoh_actor_walking_no_operation();
    pharaoh_actor_walking_no_operation();
}

/* Native004B53B0. The old Unit01452 algorithm used Undefined4 DAT pointer
 * units for native byte/word fields. Bind qualified record/grid types here. */
void pharaoh_walker_refresh_tile_ordinal(PharaohWalkerTileChainContext *c,int32_t id)
{
    PharaohWalkerQueryRecord *const walker=&c->walkers[id];
    walker->tile_chain_ordinal=0;
    int32_t current=c->grid_heads[walker->grid_offset];
    while(current!=0){
        if(current==id)return;
        walker->tile_chain_ordinal=(uint8_t)(walker->tile_chain_ordinal+1u);
        current=c->walkers[current].next_walker_id_on_same_tile;
    }
    if(walker->tile_chain_ordinal>20)walker->tile_chain_ordinal=20;
}
