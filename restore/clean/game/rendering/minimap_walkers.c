#include "pharaoh/rendering/minimap_walkers.h"
#include "pharaoh/building/type_queries.h"
#include <stddef.h>

_Static_assert(sizeof(PharaohMinimapFormationStatus)==144,"native formation status stride");
_Static_assert(offsetof(PharaohWalkerQueryRecord,herd_target_id_60)==0x60 &&
    offsetof(PharaohWalkerQueryRecord,action_state)==0x38,"native aliased classifier fields");

/* Unit00580 reused. Native0043EE10 defines full EAX0..3: the Ghidra bool
 * prototype loses cases2/3 and is not the result contract. WORD+60 is a
 * signed formation index in this path, despite its other overlay name. */
int32_t pharaoh_minimap_walker_land_kind(const PharaohMinimapWalkers *c,int32_t id)
{
    while(id!=0) {
        const PharaohWalkerQueryRecord *w=&c->walkers[id];
        const uint8_t type=w->type;
        if(type>=11 && type<=13) return c->formation_status[w->herd_target_id_60].value<0;
        if((type>=43 && type<=57) || type==99 || (type==41 && w->action_state==11)) return 2;
        if((uint8_t)pharaoh_building_type_is_004ea9c0_group(type)!=0) return 3;
        id=w->next_walker_id_on_same_tile;
    }
    return 0;
}

/* Unit00578 reused, native0043E790. The overlay is reloaded after the
 * matching callback; it shares original state and may not be snapshotted. */
int32_t pharaoh_minimap_walker_other_kind(const PharaohMinimapWalkers *c,int32_t id)
{
    while(id!=0) {
        if(*c->overlay==51) return 0;
        if(c->walker_matches_overlay(c->user_data,id)!=0) return 1;
        if(*c->overlay==0) {
            const uint8_t type=c->walkers[id].type;
            if(type==22 || type==23 || type==24 || type==107 || type==106 || type==109) return 1;
        }
        id=c->walkers[id].next_walker_id_on_same_tile;
    }
    return 0;
}

static int32_t land(void *p,int32_t id) {return pharaoh_minimap_walker_land_kind(p,id);}
static int32_t ships(void *p,int32_t id)
{const PharaohMinimapWalkers *c=p;return pharaoh_minimap_walker_ship_kind(c->walkers,id);}
static int32_t other(void *p,int32_t id) {return pharaoh_minimap_walker_other_kind(p,id);}
void pharaoh_minimap_bind_walkers(PharaohMinimap *m,PharaohMinimapWalkers *c)
{m->walker_land_kind=land;m->walker_ship_kind=ships;m->walker_other_kind=other;m->walker_user_data=c;}
static void pair(void *p,uint32_t color,int32_t x,int32_t y)
{pharaoh_graphics_set_pixel_pair_raw(p,x,y,color);}
void pharaoh_minimap_bind_pixel_pair(PharaohMinimap *m,PharaohGraphicsPrimitives *p)
{m->draw_pair=pair;m->pair_user_data=p;}
