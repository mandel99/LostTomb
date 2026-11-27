#include "pharaoh/building/construction_commit.h"
#include "pharaoh/core/memory_strings.h"
#include "pharaoh/rendering/sprite_raw_blit.h"
#include "pharaoh/map/surface_queries.h"
#include "pharaoh/events/event_queries.h"
#include "pharaoh/monuments/obelisk_queries.h"
#include "pharaoh/runtime/platform_measure_speed.h"

static int32_t commit_add(int32_t a,int32_t b)
{return (int32_t)((uint32_t)a+(uint32_t)b);}
static int32_t commit_sub(int32_t a,uint32_t b)
{return (int32_t)((uint32_t)a-b);}

static void clear_preview(const PharaohConstructionCommitContext *c)
{
    pharaoh_graphics_and_bytes((uint32_t *)(void *)
        c->allocation->surface->property_flags,0xcb10,0xaf);
}
static void reject(const PharaohConstructionCommitContext *c,int32_t message)
{
    pharaoh_city_warning_post(c->basic->warnings,message);
    clear_preview(c);
}
static void redraw_area(const PharaohConstructionCommitContext *c,int32_t x,int32_t y)
{
    uint32_t radius=*c->size;
    pharaoh_map_routing_update_citizen_rect(c->routing,
        commit_sub(x,radius),commit_sub(y,radius),
        (int32_t)((uint32_t)x+radius),(int32_t)((uint32_t)y+radius),1);
    radius=*c->size;
    const PharaohSceneRouting *r=c->routing;
    pharaoh_map_routing_update_land_noncitizen_rect(r->terrain,r->building_ids,
        r->buildings,r->noncitizen,r->geometry->grid_start,
        r->geometry->width,r->geometry->height,
        commit_sub(x,radius),commit_sub(y,radius),
        (int32_t)((uint32_t)x+radius),(int32_t)((uint32_t)y+radius));
}
static int32_t area_exists(const PharaohConstructionCommitContext *c,
    int32_t x,int32_t y,int32_t size,int32_t radius,uint32_t mask)
{
    PharaohMapSurfaceContext *s=c->allocation->surface;
    PharaohMapAreaCursor cursor={*s->area_x,*s->area_y,0};
    const int32_t result=pharaoh_map_terrain_exists_in_area_with(
        s->map,x,y,size,radius,mask,&cursor);
    *s->area_x=cursor.x;*s->area_y=cursor.y;return result;
}
static int32_t area_all(const PharaohConstructionCommitContext *c,
    int32_t x,int32_t y,int32_t size,int32_t radius,uint32_t mask)
{
    PharaohMapSurfaceContext *s=c->allocation->surface;
    PharaohMapAreaCursor cursor={*s->area_x,*s->area_y,0};
    const int32_t result=pharaoh_map_terrain_all_in_area_are(
        s->map,x,y,size,radius,mask,&cursor);
    *s->area_x=cursor.x;*s->area_y=cursor.y;return result;
}
static uint32_t actual_rule(uint32_t terrain,void *data)
{return (*(const PharaohConstructionTerrainRule *)data)(terrain);}
static int32_t region_rule(const PharaohConstructionCommitContext *c,
    int32_t x,int32_t y,int32_t size,int32_t radius,
    PharaohConstructionTerrainRule rule)
{
    return pharaoh_map_region_matches_predicate(c->allocation->surface->map,
        x,y,size,radius,actual_rule,&rule,
        c->allocation->surface->area_x,c->allocation->surface->area_y);
}
static int32_t withdraw_city(const PharaohConstructionCommitContext *c,
    int32_t player,int32_t resource,int32_t amount)
{
    PharaohStorageWithdrawContext storage=*c->storage;
    storage.withdrawn=c->cities[player].withdrawn;
    storage.stored=c->cities[player].stored;
    storage.city_cursor=&c->cities[player].storage_cursor;
    return pharaoh_storage_withdraw_city(&storage,resource,amount);
}
static void charge_city(const PharaohConstructionCommitContext *c,
    int32_t player,int32_t amount)
{
    PharaohConstructionCommitCity *city=&c->cities[player];
    const uint8_t active=city->player_active;
    const int32_t editor=(int32_t)*c->basic->editor_active;
    /* The existing finance API is normalized. Its complete C body touches
     * only these two ledger members, has no callbacks, and gates all reads.
     * Marshal the actual live fields rather than maintaining a second city. */
    PharaohFinanceLedger ledger;
    if(active==1 && editor!=1) {
        ledger.treasury=city->treasury;
        ledger.construction_current_year=city->construction_current_year;
    }
    pharaoh_finance_process_construction(&ledger,active,editor,amount);
    if(active==1 && editor!=1) {
        city->treasury=ledger.treasury;
        city->construction_current_year=ledger.construction_current_year;
    }
}

static PharaohPercentageScaleStatus construction_warnings(
    const PharaohConstructionCommitContext *c,int32_t tool,int32_t x,
    int32_t y,int32_t footprint)
{
    const PharaohPercentageScaleStatus status=
        pharaoh_construction_warning_food_supply(c->basic,tool);
    if(status!=PHARAOH_PERCENTAGE_SCALE_VALUE)return status;
    pharaoh_construction_warning_workers(c->basic,tool);
    pharaoh_construction_warning_granary(c->basic,tool);
    pharaoh_construction_warning_booth(c->basic,tool);
    pharaoh_construction_warning_bandstand(c->basic,tool);
    pharaoh_construction_warning_pavilion(c->basic,tool);
    pharaoh_construction_warning_fort(c->basic,tool);
    pharaoh_construction_warning_military(c->military,tool);
    pharaoh_construction_warning_recruiter(c->basic,tool);
    pharaoh_construction_warning_tower(c->access,tool,x,y,footprint);
    pharaoh_construction_warning_well(c->basic,tool,x,y);
    pharaoh_actor_walking_no_operation();
    pharaoh_construction_warning_brewery(c->resources,tool);
    pharaoh_construction_warning_weaver(c->resources,tool);
    pharaoh_construction_warning_jeweler(c->resources,tool);
    pharaoh_construction_warning_potter(c->resources,tool);
    pharaoh_construction_warning_papyrus(c->resources,tool);
    pharaoh_construction_warning_bricks(c->resources,tool);
    pharaoh_construction_warning_lamps(c->resources,tool);
    pharaoh_construction_warning_paint(c->resources,tool);
    pharaoh_construction_warning_cattle(c->resources,tool);
    pharaoh_construction_warning_senet(c->resources,tool);
    pharaoh_construction_warning_zoo(c->resources,tool);
    pharaoh_construction_warning_papyrus_users(c->resources,tool);
    pharaoh_construction_warning_mortuary(c->resources,tool);
    pharaoh_construction_warning_carpenters(c->resources,tool);
    pharaoh_construction_warning_artisans(c->resources,tool);
    pharaoh_construction_warning_pyramid(c->resources,tool);
    pharaoh_construction_warning_sandstone(c->resources,tool);
    pharaoh_construction_warning_ptolemaic(c->resources,tool);
    pharaoh_construction_warning_venue_schools(c->resources,tool);
    pharaoh_construction_warning_road_access(c->access,tool,x,y,footprint);
    return PHARAOH_PERCENTAGE_SCALE_VALUE;
}

/* Complete Native004728A0; whole Unit01022 C algorithm reused. */
PharaohPercentageScaleStatus pharaoh_construction_commit(
    const PharaohConstructionCommitContext *c,uint32_t player,int32_t orientation,int32_t firstX,
                        int32_t firstY,int32_t lastX,int32_t lastY,int32_t tool)
{
    uint8_t allowFloodplain=(uint8_t)pharaoh_building_type_is_0046fde0_group((uint32_t)tool);
    uint32_t cityAtDispatch,terrainMask=UINT32_MAX,queryResult,city_index;
    uint32_t amount,resource,required,cell;
    uint32_t *spent;
    PharaohConstructionTerrainRule rule;
    PharaohPercentageScaleStatus status;
    int32_t footprint,Building=0,query,warning,kind,contiguous;
    int skipInitialization=0;

    (*c->basic->warnings->redraw)=0;
    (*c->cost)=0;
    (*c->credit)=0;
    if(tool==0)return PHARAOH_PERCENTAGE_SCALE_VALUE;
    cityAtDispatch=(*c->allocation->current_player);
    if((int32_t)c->cities[cityAtDispatch].treasury<=-5000) {
        clear_preview(c);
        pharaoh_city_warning_post(c->basic->warnings,3);
        return PHARAOH_PERCENTAGE_SCALE_VALUE;
    }
    if(tool!=9&&pharaoh_walker_enemy_near_building_site(c->walkers,firstX,firstY,lastX,lastY)!=0) {
        switch(tool) {
        case 6:case 0xa8:case 0xa9:case 5:case 8:
        case 0x26:case 0x27:
            pharaoh_memory_copy_forward_words64(c->backup->saved_terrain,c->allocation->surface->map->terrain,0x32c40);
            pharaoh_memory_copy_forward_words64(c->backup->saved_bytes[0],c->allocation->surface->canal_water,0xcb10);
            if(tool==0x26||tool==0x27) {
                pharaoh_memory_copy_forward_words64(c->backup->saved_bytes[1],c->allocation->surface->property_flags,0xcb10);
                pharaoh_memory_copy_forward_words64(c->backup->saved_bytes[2],c->allocation->surface->edge_flags,0xcb10);
            }
            pharaoh_map_restore_free_tile_images(c->allocation->surface,c->backup->saved_images);
            break;
        case 0x52:case 0x53:c->bridge->state->length=0;break;
        default:clear_preview(c);break;
        }
        pharaoh_city_warning_post(c->basic->warnings,0x32);
        return PHARAOH_PERCENTAGE_SCALE_VALUE;
    }
    if(tool!=9)cityAtDispatch=(*c->allocation->current_player);
    if(tool==0xcf||tool==0xd0) {
        if(pharaoh_obelisk_first_unfinished(c->allocation->surface->buildings,c->allocation->players,(const int32_t *)c->allocation->current_player)>0) {
            reject(c,0x58);
            return PHARAOH_PERCENTAGE_SCALE_VALUE;
        }
        cityAtDispatch=(*c->allocation->current_player);
    }
    (*c->native_bc9720)=0;
    (*c->cost)=(uint32_t)c->models[tool].cost;

    switch(tool) {
    case 9:
        pharaoh_construction_request_demolition(c->demolition,0,firstX,firstY,lastX,lastY);
        goto multiplyCount;
    case 6:case 0xa8:case 0xa9:
        if((uint8_t)pharaoh_construction_area_has_entry_point(c->entry_points,firstX,firstY,lastX,lastY))goto protectedRegion;
        terrainMask=tool==6?0x4000u:tool==0xa8?0x404000u:0x804000u;
        pharaoh_construction_place_wall_strip(c->walls,0,firstX,firstY,lastX,lastY,terrainMask);
        goto multiplyCount;
    case 5:
        if((uint8_t)pharaoh_construction_area_has_entry_point(c->entry_points,firstX,firstY,lastX,lastY))goto protectedRegion;
        pharaoh_construction_place_road_route(c->routes,0,firstX,firstY,lastX,lastY);
        goto multiplyCount;
    case 0x26:
        pharaoh_construction_paint_paved_roads(c->paint,firstX,firstY,lastX,lastY);
        goto multiplyCount;
    case 0x1f:case 0x1e:case 0x21:case 0xd1:
        if(tool==0xd1&&c->cities[cityAtDispatch].festival_placed!=0) {
            pharaoh_city_warning_post(c->basic->warnings,4);
            return PHARAOH_PERCENTAGE_SCALE_VALUE;
        }
        footprint=tool==0x1f?1:tool==0x1e?2:tool==0x21?3:4;
        if((uint8_t)pharaoh_construction_area_has_entry_point(c->entry_points,firstX,firstY,commit_add(firstX,footprint),commit_add(firstY,footprint)))goto protectedRegion;
        if(tool==0x1f)queryResult=pharaoh_entertainment_build_booth(c->entertainment,0,lastX,lastY);
        else if(tool==0x1e)queryResult=pharaoh_entertainment_build_bandstand(c->entertainment,0,lastX,lastY);
        else if(tool==0x21)queryResult=pharaoh_entertainment_build_pavilion(c->entertainment,0,lastX,lastY);
        else queryResult=pharaoh_entertainment_build_festival_square(c->entertainment,0,lastX,lastY);
        if(queryResult==0) {
            if(tool==0x1f)clear_preview(c);
            pharaoh_city_warning_post(c->basic->warnings,tool==0xd1?0xd7:0x4d);
            return PHARAOH_PERCENTAGE_SCALE_VALUE;
        }
        if(tool!=0xd1)pharaoh_construction_warning_venue_schools(c->resources,tool);
        goto finish;
    case 0x27:
        if((uint8_t)pharaoh_construction_area_has_entry_point(c->entry_points,firstX,firstY,lastX,lastY))goto protectedRegion;
        pharaoh_construction_paint_gardens(c->paint,firstX,firstY,lastX,lastY);
        (*c->cost)=(*c->count)*(*c->cost);
        redraw_area(c,lastX,lastY);
        goto finish;
    case 0x52:case 0x53:
        pharaoh_map_place_bridge(c->bridge,lastX,lastY,tool==0x53);
        if((int32_t)c->bridge->state->length<2) {
            pharaoh_city_warning_post(c->basic->warnings,0x19);
            return PHARAOH_PERCENTAGE_SCALE_VALUE;
        }
        (*c->cost)=c->bridge->state->length*(*c->cost);
        goto finish;
    case 8:
        pharaoh_construction_place_canal_route(c->routes,0,firstX,firstY,lastX,lastY);
        goto multiplyCount;
    case 10:
        /* The pinned original performs this guard as well as the housing
         * commit's individual-cell checks. */
        if((uint8_t)pharaoh_construction_area_has_entry_point(c->entry_points,firstX,firstY,lastX,lastY))goto protectedRegion;
        status=pharaoh_construction_build_housing_rectangle(c->housing,0,firstX,firstY,lastX,lastY); if(status!=PHARAOH_PERCENTAGE_SCALE_VALUE)return status;
        goto multiplyCount;
    default:break;
    }

    footprint=(int32_t)(uint32_t)c->allocation->definitions[tool].size;
    (*c->size)=(uint32_t)footprint;
    switch(tool) {
    case 0x3a:case 0xaa:case 0xab:case 200:case 0xc9:case 0xca:case 0x38:
        terrainMask=0xffffffbfu;break;
    case 0x3b:case 0xac:case 0xad:terrainMask=0xff3fbfffu;break;
    case 0x8a:terrainMask=8;break;
    case 0x48:footprint=3;break;
    default:break;
    }
    if((uint8_t)pharaoh_construction_area_has_entry_point(c->entry_points,firstX,firstY,commit_add(firstX,commit_add(footprint,-1)),commit_add(firstY,commit_add(footprint,-1))))goto protectedRegion;
    if(tool!=0xab&&tool!=0xca) {
        if(orientation==6)lastY=commit_add(lastY,commit_sub(1,(uint32_t)footprint));
        else if(orientation==4) {
            lastX=commit_add(lastX,commit_sub(1,(uint32_t)footprint));
            lastY=commit_add(lastY,commit_sub(1,(uint32_t)footprint));
        } else if(orientation==2)lastX=commit_add(lastX,commit_sub(1,(uint32_t)footprint));
    }

    switch(tool) {
    case 0x4c:case 0xb5:
        if(pharaoh_map_water_determine_orientation_size2(c->footprint,lastX,lastY,0,0)) {
            pharaoh_city_warning_post(c->basic->warnings,0x19);return PHARAOH_PERCENTAGE_SCALE_VALUE;
        }
        goto extraChecks;
    case 7:
        if(pharaoh_map_water_determine_orientation_size2(c->footprint,lastX,lastY,0,1)) {
            reject(c,0x19);return PHARAOH_PERCENTAGE_SCALE_VALUE;
        }
        tool=0x5a;
        cell=c->allocation->surface->map->origin+(uint32_t)lastY*228u+(uint32_t)lastX;
        Building=pharaoh_building_allocate(c->allocation,player,0x5a,lastX,lastY,0,0);
        pharaoh_construction_warning_workers(c->basic,0x5a);
        pharaoh_construction_warning_well(c->basic,0x5a,lastX,lastY);
        (void)pharaoh_map_building_undo_capture(c->allocation->surface->undo,Building);
        {
            uint32_t height=c->allocation->surface->map->height-1u;
            uint32_t width=c->allocation->surface->map->width-1u;
            c->allocation->surface->canal_water[cell]=0;
            pharaoh_map_refresh_channel_region(c->allocation->surface,0,0,(int32_t)width,(int32_t)height,0);
        }
        redraw_area(c,lastX,lastY);
        goto allocate;
    case 0xab:case 0xca:
        if(!(uint8_t)pharaoh_gatehouse_site_fits(c->allocation->surface,c->gatehouse_parts,lastX,lastY,(int32_t)(*c->allocation->rotation))) {
            clear_preview(c);pharaoh_city_warning_post(c->basic->warnings,2);return PHARAOH_PERCENTAGE_SCALE_VALUE;
        }
        goto extraChecks;
    case 0xb6:case 0x4a:case 0x4b:case 0xc0:
        if((tool==0x4b||tool==0xc0)&&(int32_t)(*c->native_705c1c)>=10) {
            pharaoh_city_warning_post(c->basic->warnings,0xce);return PHARAOH_PERCENTAGE_SCALE_VALUE;
        }
        if(pharaoh_map_water_determine_orientation_size3(c->water_orientation,lastX,lastY,0)) {pharaoh_city_warning_post(c->basic->warnings,0x19);return PHARAOH_PERCENTAGE_SCALE_VALUE;}
        if(!pharaoh_dock_is_connected_to_open_water(c->dock,lastX,lastY)) {pharaoh_city_warning_post(c->basic->warnings,0x38);return PHARAOH_PERCENTAGE_SCALE_VALUE;}
        if(tool!=0xb6)goto extraChecks;
        if((int32_t)(*c->native_bcb9b0)>=6) {pharaoh_city_warning_post(c->basic->warnings,0x54);return PHARAOH_PERCENTAGE_SCALE_VALUE;}
        goto allocate;
    case 0x88:
        if(!(*c->native_bca314)) {
            if(!(*c->native_bca315))pharaoh_city_warning_post(c->basic->warnings,0x44);
            return PHARAOH_PERCENTAGE_SCALE_VALUE;
        }
        if(pharaoh_map_water_determine_orientation_size2(c->footprint,lastX,lastY,0,0)) {pharaoh_city_warning_post(c->basic->warnings,0x19);return PHARAOH_PERCENTAGE_SCALE_VALUE;}
        goto allocate;
    case 0xe5:
        queryResult=pharaoh_small_royal_tomb_can_place(c->monument_site,c->small_tomb.placement,(*c->selected_cell));
        if(!(uint8_t)queryResult)goto invalidComplex;
        queryResult=pharaoh_small_royal_tomb_has_incomplete(c->small_tomb.progress_object,c->allocation->surface->buildings,c->small_tomb.progress);
        goto existingLargeComplex;
    case 0xea:
        queryResult=pharaoh_medium_royal_tomb_can_place(c->monument_site,c->medium_tomb.placement,(*c->selected_cell));
        if(!(uint8_t)queryResult)goto invalidComplex;
        queryResult=pharaoh_medium_royal_tomb_has_incomplete(c->medium_tomb.progress_object,c->allocation->surface->buildings,c->medium_tomb.progress);
        goto existingLargeComplex;
    case 0xeb:
        queryResult=pharaoh_large_royal_tomb_can_place(c->monument_site,c->large_tomb.placement,(*c->selected_cell));
        if(!(uint8_t)queryResult)goto invalidComplex;
        queryResult=pharaoh_large_royal_tomb_has_incomplete(c->large_tomb.progress_object,c->allocation->surface->buildings,c->large_tomb.progress);
        goto existingLargeComplex;
    case 0xec:
        queryResult=pharaoh_grand_royal_tomb_can_place(c->monument_site,c->grand_tomb.placement,(*c->selected_cell));
        if(!(uint8_t)queryResult)goto invalidComplex;
        queryResult=pharaoh_grand_royal_tomb_has_incomplete(c->grand_tomb.progress_object,c->allocation->surface->buildings,c->grand_tomb.progress);
        goto existingLargeComplex;
    case 0xe4:
        queryResult=pharaoh_lighthouse_can_place(c->monument_site,c->lighthouse.placement,(*c->selected_cell));
        if(!(uint8_t)queryResult) {warning=0xf9;goto invalidComplexMessage;}
        queryResult=pharaoh_lighthouse_has_incomplete(c->lighthouse.progress_object,c->allocation->surface->buildings,c->lighthouse.progress);
        goto existingSmallComplex;
    case 0xe6:
        queryResult=pharaoh_abu_simbel_can_place(c->monument_site,c->abu_simbel.placement,(*c->selected_cell));
        if(!(uint8_t)queryResult)goto invalidComplex;
        /* 00401430 forwards to the shared existing-building query. Its AL is
         * real; the upper EAX bits are those from this exact preceding check. */
        queryResult=pharaoh_abu_simbel_has_incomplete(c->abu_simbel.progress_object,c->allocation->surface->buildings,c->abu_simbel.progress);
        goto existingSmallComplex;
    default:break;
    }
    if(!pharaoh_map_empty_rectangle_fits(c->allocation->surface,lastX,lastY,footprint,footprint,(int32_t)terrainMask,0,allowFloodplain)) {
        reject(c,2);return PHARAOH_PERCENTAGE_SCALE_VALUE;
    }
    if(tool==0x35||tool==0xcf||tool==0xd0) {
        if((*c->editor_mode)!=1) {
            uint32_t city=(*c->allocation->current_player);
            amount=tool==0x35?500u:tool==0xcf?10000u:20000u;
            resource=tool==0x35?0x17u:0x1au;
            spent=tool==0x35?c->spent_23:c->spent_26;

            if((int32_t)c->cities[city].stored[resource]<(int32_t)amount) {
                reject(c,tool==0x35?0x4f:tool==0xcf?0x55:0x56);return PHARAOH_PERCENTAGE_SCALE_VALUE;
            }
            (void)withdraw_city(c,(int32_t)city,(int32_t)resource,(int32_t)amount);
            (*spent)=(*spent)+amount;
        }
        goto allocate;
    }
    if(tool==0xa1) {
        rule=(*c->terrain_rule);
        if(rule&&!region_rule(c,lastX,lastY,2,1,rule)) {
            pharaoh_city_warning_post(c->basic->warnings,0x16);return PHARAOH_PERCENTAGE_SCALE_VALUE;
        }
        if(!(uint8_t)pharaoh_city_buildings_has_treasury(c->allocation->surface->buildings,(int32_t)*c->allocation->highest_id))pharaoh_city_warning_post(c->basic->warnings,0x3c);
        goto allocate;
    }
    /* Exactly one requirement branch runs; the order is the native priority. */
    warning=0;
    if(c->requirements->terrain_10800) {
        if(!area_exists(c,lastX,lastY,3,1,0x10800u))warning=0x14;
    } else if((rule=(*c->terrain_rule))!=0) {
        if(!region_rule(c,lastX,lastY,2,1,rule))warning=0x16;
    } else if(c->requirements->forest) {
        if(!area_exists(c,lastX,lastY,2,1,1))warning=0x17;
    } else if(c->requirements->water_40000) {
        if(!area_exists(c,lastX,lastY,2,1,0x40000u))warning=0x3a;
    } else if(c->requirements->water_10004) {
        if(!area_exists(c,lastX,lastY,2,3,0x10004u))warning=0x15;
    } else if(c->requirements->wall) {
        if(!area_all(c,lastX,lastY,2,0,0x4000u))warning=0x31;
    } else if(c->requirements->road) {
        /* Cache the cell before the callback, as EBX does in the original. */
        cell=c->allocation->surface->map->origin+(uint32_t)lastY*228u+(uint32_t)lastX;
        if(!area_all(c,lastX,lastY,1,0,0x40u))warning=0x42;
        else if(c->bridge->tile_direction[cell])warning=2;
    } else if((required=c->requirements->floodplain_count)!=0) {
        int32_t size=(int32_t)(uint32_t)c->allocation->definitions[tool].size;
        if(!pharaoh_map_region_terrain_count_reached(c->allocation->surface->map,lastX,lastY,size,0,0x80u,(int32_t)required,c->allocation->surface->area_x,c->allocation->surface->area_y))warning=0x4c;
    }
    if(warning) {pharaoh_city_warning_post(c->basic->warnings,warning);return PHARAOH_PERCENTAGE_SCALE_VALUE;}

extraChecks:
    if(tool==0x28||tool==0x2c||tool==0x2d) {
        if(!pharaoh_map_empty_rectangle_fits(c->allocation->surface,commit_add(lastX,3),commit_add(lastY,-1),4,4,(int32_t)terrainMask,0,allowFloodplain)) {
            pharaoh_city_warning_post(c->basic->warnings,2);return PHARAOH_PERCENTAGE_SCALE_VALUE;
        }
        if((int32_t)(*c->native_bcb9b4)>=6) {pharaoh_city_warning_post(c->basic->warnings,0x34);return PHARAOH_PERCENTAGE_SCALE_VALUE;}
    }
    switch(tool) {
    case 0xd2:
        if(c->cities[(*c->allocation->current_player)].sphinx_stage) {pharaoh_city_warning_post(c->basic->warnings,4);return PHARAOH_PERCENTAGE_SCALE_VALUE;}
        if(!pharaoh_map_empty_rectangle_fits(c->allocation->surface,commit_add(lastX,6),lastY,6,6,(int32_t)terrainMask,0,allowFloodplain)) {
            pharaoh_city_warning_post(c->basic->warnings,2);return PHARAOH_PERCENTAGE_SCALE_VALUE;
        }
        if(!pharaoh_map_empty_rectangle_fits(c->allocation->surface,commit_add(lastX,12),lastY,6,6,(int32_t)terrainMask,0,allowFloodplain)) {
            pharaoh_city_warning_post(c->basic->warnings,2);return PHARAOH_PERCENTAGE_SCALE_VALUE;
        }
        if((uint8_t)pharaoh_construction_area_has_entry_point(c->entry_points,commit_add(lastX,6),lastY,commit_add(lastX,11),commit_add(firstY,5)))
            goto protectedRegion;
        if((uint8_t)pharaoh_construction_area_has_entry_point(c->entry_points,commit_add(lastX,12),lastY,commit_add(lastX,17),commit_add(firstY,5)))
            goto protectedRegion;
        goto allocate;
    case 0xd5:
        queryResult=pharaoh_sun_temple_can_place(c->monument_site,c->sun_parts,(*c->selected_cell));
        if(!(uint8_t)queryResult) {pharaoh_city_warning_post(c->basic->warnings,2);return PHARAOH_PERCENTAGE_SCALE_VALUE;}
        queryResult=pharaoh_sun_temple_has_completed(c->sun_progress);
        if((uint8_t)queryResult) {pharaoh_city_warning_post(c->basic->warnings,0x5b);return PHARAOH_PERCENTAGE_SCALE_VALUE;}
        amount=22000;
        warning=0x5a;
        goto specialResource;
    case 0xe1:
        queryResult=pharaoh_library_can_place(c->monument_site,c->library.placement,(*c->selected_cell));
        if(!(uint8_t)queryResult) {pharaoh_city_warning_post(c->basic->warnings,2);return PHARAOH_PERCENTAGE_SCALE_VALUE;}
        queryResult=pharaoh_library_has_incomplete(c->library.progress_object,c->allocation->surface->buildings,c->library.progress);
        if((uint8_t)queryResult) {pharaoh_city_warning_post(c->basic->warnings,0xf3);return PHARAOH_PERCENTAGE_SCALE_VALUE;}
        goto allocate;
    case 0xe3:
        queryResult=pharaoh_caesareum_can_place(c->monument_site,c->caesareum.placement,(*c->selected_cell));
        if(!(uint8_t)queryResult) {pharaoh_city_warning_post(c->basic->warnings,2);return PHARAOH_PERCENTAGE_SCALE_VALUE;}
        queryResult=pharaoh_caesareum_has_incomplete(c->caesareum.progress_object,c->allocation->surface->buildings,c->caesareum.progress);
        goto existingLargeComplex;
    case 0xde:
        cell=c->allocation->surface->map->origin+(uint32_t)firstY*228u+(uint32_t)firstX;
        if(!pharaoh_mausoleum_can_place(c->monument_site,c->mausoleum,c->allocation->surface->orientation,(int32_t)cell)) {pharaoh_city_warning_post(c->basic->warnings,2);return PHARAOH_PERCENTAGE_SCALE_VALUE;}
        amount=24000;
        warning=0xbe;
        goto specialResource;
    default:break;
    }
    if(tool>=0x41&&tool<=0x45) {
        if(c->cities[(*c->allocation->current_player)].temple_placed)goto existingUnique;
        query=pharaoh_temple_complex_can_place(c->temple_site,lastX,lastY);
        if(query==-1) {reject(c,2);return PHARAOH_PERCENTAGE_SCALE_VALUE;}
        if(query==-2)goto protectedRegion;
    }
    if(tool==0xd3||tool==0xd4) {
        query=pharaoh_building_follow_link(c->allocation->surface->buildings,c->allocation->surface->building_ids[(*c->selected_cell)]);
        city_index=(*c->allocation->current_player);
        if(!c->cities[city_index].temple_placed) {reject(c,0x49);return PHARAOH_PERCENTAGE_SCALE_VALUE;}
        kind=c->allocation->surface->buildings[query].building_type;
        if(kind<0x41||kind>0x45) {reject(c,0x4b);return PHARAOH_PERCENTAGE_SCALE_VALUE;}
        if(c->cities[city_index].temple_count<2) {reject(c,0x4e);return PHARAOH_PERCENTAGE_SCALE_VALUE;}
        if(pharaoh_temple_upgrade_present(c->allocation->surface->buildings,(int32_t)c->cities[city_index].temple_building,tool==0xd3?1u:2u)) {
            reject(c,0x4a);return PHARAOH_PERCENTAGE_SCALE_VALUE;
        }
        goto allocate;
    }
    city_index=(*c->allocation->current_player);
    if((tool==0xbd||tool==0xbc||tool==0xbb)&&c->cities[city_index].palace_placed)goto existingUnique;
    if((tool==0x4f||tool==0x4e||tool==0x4d)&&c->cities[city_index].mansion_placed)goto existingUnique;
    if(tool==0x56) {
        if(!c->cities[city_index].palace_placed) {reject(c,0x59);return PHARAOH_PERCENTAGE_SCALE_VALUE;}
    } else if(tool==0x5f) {
        if((int32_t)(*c->native_705c6c)>0)goto existingUnique;
        (*c->native_705c6c)=1;
    } else if(tool==0x5e) {
        if(c->cities[city_index].academy_placed)goto existingUnique;
    } else if(tool==0xb7) {
        uint32_t failureReason=(uint32_t)firstX;
        cell=c->allocation->surface->map->origin+(uint32_t)lastY*228u+(uint32_t)lastX;
        if(!(uint8_t)pharaoh_pyramid_preview_is_allowed(c->pyramid_preview,(int32_t)cell,(int32_t)(*c->pyramid_tool),&failureReason)) {
            pharaoh_city_warning_post(c->basic->warnings,failureReason==2?0xd6:0xd5);return PHARAOH_PERCENTAGE_SCALE_VALUE;
        }
    }
    goto allocate;

specialResource:
    if((*c->editor_mode)==0) {
        uint32_t city=(*c->allocation->current_player);
        if((int32_t)c->cities[city].stored[30]<(int32_t)amount) {
            pharaoh_city_warning_post(c->basic->warnings,warning);return PHARAOH_PERCENTAGE_SCALE_VALUE;
        }
        (void)withdraw_city(c,(int32_t)city,0x1e,(int32_t)amount);
        (*c->spent_30)=(*c->spent_30)+amount;
    }
    goto allocate;

existingLargeComplex:
    if((uint8_t)queryResult) {pharaoh_city_warning_post(c->basic->warnings,0xf5);return PHARAOH_PERCENTAGE_SCALE_VALUE;}
    goto allocate;
existingSmallComplex:
    if((uint8_t)queryResult) {pharaoh_city_warning_post(c->basic->warnings,0xf4);return PHARAOH_PERCENTAGE_SCALE_VALUE;}
    goto allocate;
invalidComplex:
    warning=0x110;
invalidComplexMessage:
    pharaoh_city_warning_post(c->basic->warnings,warning);
    pharaoh_city_warning_post(c->basic->warnings,0x111);
    return PHARAOH_PERCENTAGE_SCALE_VALUE;
existingUnique:
    reject(c,4);
    return PHARAOH_PERCENTAGE_SCALE_VALUE;
protectedRegion:
    reject(c,0x3d);
    return PHARAOH_PERCENTAGE_SCALE_VALUE;

allocate:
    status=construction_warnings(c,tool,lastX,lastY,footprint);
    if(status!=PHARAOH_PERCENTAGE_SCALE_VALUE)return status;
    (*c->placement_byte)=0;
    kind=tool;
    contiguous=0;
    switch(tool) {
    case 0x28:case 0x2c:case 0x2d:kind=0x39;break;
    case 0xb7:contiguous=0x90;break;
    case 0xd5:contiguous=100;skipInitialization=1;break;
    case 0xe6:contiguous=0x10;skipInitialization=1;break;
    case 0xe1:contiguous=0x19;skipInitialization=1;break;
    case 0xe3:contiguous=9;skipInitialization=1;break;
    case 0xe5:contiguous=0xa7;skipInitialization=1;break;
    case 0xea:contiguous=0x9e;skipInitialization=1;break;
    case 0xeb:contiguous=0x1b5;skipInitialization=1;break;
    case 0xec:contiguous=0x201;skipInitialization=1;break;
    case 0xe4:contiguous=2;skipInitialization=1;break;
    case 0xde:contiguous=0x74;skipInitialization=1;break;
    default:break;
    }
    if(tool!=0x5a)
        Building=pharaoh_building_allocate(c->allocation,player,(uint32_t)kind,lastX,lastY,contiguous,0);
    if(Building==0)return PHARAOH_PERCENTAGE_SCALE_VALUE;
    if(!skipInitialization)(void)pharaoh_map_building_undo_capture(c->allocation->surface->undo,Building);
    if(Building<1)return PHARAOH_PERCENTAGE_SCALE_VALUE;
    pharaoh_building_construction_add_to_map(c->creation,player,tool,Building,lastX,lastY,(int32_t)(*c->size),1,0);
    if(tool==0x88)pharaoh_construction_ferry_landing_placed(c->allocation->ferry_state,&Building,c->basic->warnings,c->allocation->surface->undo);
    goto finish;

multiplyCount:
    (*c->cost)=(*c->count)*(*c->cost);
finish:
    pharaoh_map_routing_initialize_citizen(c->routing);
    pharaoh_map_routing_initialize_noncitizen(c->routing);
    pharaoh_map_routing_initialize_walls(c->routing);
    pharaoh_construction_alert_formations(c->alert,lastX,lastY);
    queryResult=tool==0x88&&(*c->allocation->ferry_state)==0?0:(*c->cost);
    queryResult-=(*c->credit);
    (*c->cost)=queryResult;
    charge_city(c,(int32_t)player,(int32_t)queryResult);
    if(tool!=0x38&&tool!=9)pharaoh_game_undo_finish_build((*c->cost),c->undo_cursor,c->undo_ticks,c->undo_ready,c->redraw);
    pharaoh_monument_rebuild_player_slots(c->slots);
    return PHARAOH_PERCENTAGE_SCALE_VALUE;
}
