#include "pharaoh/map/construction_route_actions.h"
#include "pharaoh/building/construction_commit_support.h"
#include "pharaoh/core/memory_strings.h"

static int32_t signed_bits(uint32_t bits)
{
    return bits<=INT32_MAX?(int32_t)bits:
        (int32_t)((int64_t)bits-INT64_C(0x100000000));
}

static void restore_map(const PharaohConstructionRouteActionsContext *c)
{
    PharaohMapSurfaceContext *const s=c->walk->tiles->surface;
    pharaoh_memory_copy_forward_words64(c->backup->saved_terrain,
        s->map->terrain,0x32c40);
    pharaoh_memory_copy_forward_words64(c->backup->saved_bytes[0],
        c->cleared,0xcb10);
    pharaoh_map_restore_free_tile_images(s,c->backup->saved_images);
}

static uint32_t endpoint_allowed(const PharaohConstructionRouteQueries *q,
    uint32_t tile,uint32_t allow_building)
{
    const uint32_t flags=q->terrain[tile];
    if(flags&8u){
        if(!allow_building) return 0;
        const int32_t id=q->building_ids[tile];
        if(id==0 || q->buildings[id].building_type!=138) return 0;
    }
    return (flags&UINT32_C(0x26345237))==0;
}

static void noncitizen_rect(const PharaohSceneRouting *c,
    int32_t x1,int32_t y1,int32_t x2,int32_t y2)
{
    pharaoh_map_routing_update_land_noncitizen_rect(c->terrain,c->building_ids,
        c->buildings,c->noncitizen,c->geometry->grid_start,c->geometry->width,
        c->geometry->height,x1,y1,x2,y2);
}

void pharaoh_construction_place_road_route(
    const PharaohConstructionRouteActionsContext *c,int32_t preview,
    int32_t x1,int32_t y1,int32_t x2,int32_t y2)
{
    restore_map(c);
    const PharaohConstructionRouteQueries *const q=c->fill->route->queries;
    const uint32_t origin=(uint32_t)*c->fill->route->map_origin;
    *c->walk->tiles->placed_count=0;
    if(!endpoint_allowed(q,origin+(uint32_t)y1*228u+(uint32_t)x1,1)) return;
    if(!endpoint_allowed(q,origin+(uint32_t)y2*228u+(uint32_t)x2,0)) return;
    if(*c->walk->tiles->surface->editor_active){
        if(!pharaoh_map_construction_distances(c->fill->route,x1,y1,0)) return;
    }else if(!pharaoh_map_construction_checked_distances(c->fill,x1,y1,0)) return;
    if(!pharaoh_map_place_routed_building(c->walk,x1,y1,x2,y2,0)) return;
    if(preview) return;
    pharaoh_map_routing_update_citizen_rect(c->routing,x1,y1,x2,y2,1);
    noncitizen_rect(c->routing,x1,y1,x2,y2);
    pharaoh_map_refresh_surrounding_empty(c->walk->tiles->surface,x1,y1,
        signed_bits((uint32_t)x2-(uint32_t)x1));
    *c->map_changed=1;
}

void pharaoh_construction_place_canal_route(
    const PharaohConstructionRouteActionsContext *c,int32_t preview,
    int32_t x1,int32_t y1,int32_t x2,int32_t y2)
{
    restore_map(c);
    const PharaohConstructionRouteQueries *const q=c->fill->route->queries;
    const uint32_t origin=(uint32_t)*c->fill->route->map_origin;
    *c->walk->tiles->placed_count=0;
    if(!endpoint_allowed(q,origin+(uint32_t)y1*228u+(uint32_t)x1,1)) return;
    if(!endpoint_allowed(q,origin+(uint32_t)y2*228u+(uint32_t)x2,1)) return;
    if(*c->walk->tiles->surface->editor_active){
        if(!pharaoh_map_construction_distances(c->fill->route,x1,y1,1)) return;
    }else if(!pharaoh_map_construction_checked_distances(c->fill,x1,y1,1)) return;
    if(!pharaoh_map_place_routed_building(c->walk,x1,y1,x2,y2,3)) return;
    if(preview) return;
    if(x1>x2){const int32_t swap=x1;x1=x2;x2=swap;}
    if(y1>y2){const int32_t swap=y1;y1=y2;y2=swap;}
    pharaoh_map_refresh_water(c->walk->tiles->surface,
        signed_bits((uint32_t)x1-1u),signed_bits((uint32_t)y1-1u),
        signed_bits((uint32_t)x2+1u),signed_bits((uint32_t)y2+1u),1);
    PharaohSceneRouting *const r=c->routing;
    pharaoh_map_routing_update_water_rect(r->terrain,r->tile_direction,r->water,
        r->geometry->grid_start,r->geometry->width,r->geometry->height,x1,y1,x2,y2);
    pharaoh_map_routing_initialize_citizen(r);
    noncitizen_rect(r,x1,y1,x2,y2);
    pharaoh_map_routing_initialize_walls(r);
    *c->map_changed=1;
}
