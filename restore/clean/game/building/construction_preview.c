#include "pharaoh/building/construction_preview.h"
#include "pharaoh/rendering/sprite_raw_blit.h"

static int32_t area_query(const PharaohConstructionPreviewContext *c,
    int32_t x,int32_t y,int32_t size,int32_t radius,uint32_t mask,uint8_t all)
{
    PharaohMapSurfaceContext *s=c->commit->allocation->surface;
    PharaohMapAreaCursor cursor={*s->area_x,*s->area_y,0};
    const int32_t result=all?pharaoh_map_terrain_all_in_area_are(
        s->map,x,y,size,radius,mask,&cursor):
        pharaoh_map_terrain_exists_in_area_with(s->map,x,y,size,radius,mask,&cursor);
    *s->area_x=cursor.x;*s->area_y=cursor.y;
    return result;
}
static uint32_t rule_call(uint32_t terrain,void *data)
{return (*(const PharaohConstructionTerrainRule *)data)(terrain);}
static uint32_t floodplain_count(const PharaohConstructionPreviewContext *c,
    int32_t x,int32_t y,int32_t size,uint32_t required)
{
    PharaohMapSurfaceContext *s=c->commit->allocation->surface;
    return pharaoh_map_region_terrain_count_reached(s->map,x,y,size,0,0x80,
        (int32_t)required,s->area_x,s->area_y);
}

PharaohPercentageScaleStatus pharaoh_construction_preview(
    const PharaohConstructionPreviewContext *c,int32_t x1,int32_t y1,
    int32_t x2,int32_t y2,int32_t tool)
{
    const PharaohConstructionCommitContext *p=c->commit;
    PharaohBuildingAllocationContext *allocation=p->allocation;
    PharaohMapSurfaceContext *s=allocation->surface;
    PharaohBuildingTypeRecord *buildings=s->buildings;
    uint32_t *cost=c->connection->preview_cost;
    uint32_t base_cost,size,width,mask=UINT32_MAX,value,cell,stage,source;
    uint8_t allow=0;
    *cost=0;*p->credit=0;
    if(tool==0 || p->cities[*allocation->current_player].treasury<=-5000)
        return PHARAOH_PERCENTAGE_SCALE_VALUE;
    pharaoh_graphics_and_bytes((uint32_t *)(void *)s->property_flags,0xcb10,0xaf);
    base_cost=(uint32_t)p->models[tool].cost;
    size=allocation->definitions[tool].size;
    *cost=base_cost;*p->size=size;width=size;
    switch(tool) {
    case 5:pharaoh_construction_place_road_route(p->routes,1,x1,y1,x2,y2);goto scale;
    case 6:pharaoh_construction_place_wall_strip(p->walls,1,x1,y1,x2,y2,0x4000);goto scale;
    case 7:
        (void)pharaoh_construction_plan_connection(c->connection,1,x1,y1,x2,y2);
        *s->channels_refresh_pending=1;
        pharaoh_map_refresh_channel_region(s,0,0,
            (int32_t)((uint32_t)s->map->width-1u),
            (int32_t)((uint32_t)s->map->height-1u),0);
        *p->footprint->placement_ok=0;break;
    case 8:pharaoh_construction_place_canal_route(p->routes,1,x1,y1,x2,y2);goto scale;
    case 9:pharaoh_construction_request_demolition(p->demolition,1,x1,y1,x2,y2);goto scale;
    case 10: {
        const PharaohPercentageScaleStatus status=
            pharaoh_construction_build_housing_rectangle(p->housing,1,x1,y1,x2,y2);
        if(status!=PHARAOH_PERCENTAGE_SCALE_VALUE)return status;
        goto scale;
    }
    case 30:(void)pharaoh_entertainment_build_bandstand(p->entertainment,1,x2,y2);goto scale;
    case 31:(void)pharaoh_entertainment_build_booth(p->entertainment,1,x2,y2);goto scale;
    case 33:(void)pharaoh_entertainment_build_pavilion(p->entertainment,1,x2,y2);goto scale;
    case 38:pharaoh_construction_paint_paved_roads(p->paint,x1,y1,x2,y2);goto scale;
    case 39:pharaoh_construction_paint_gardens(p->paint,x1,y1,x2,y2);goto scale;
    case 40:case 44:case 45:
        if(*p->native_bcb9b4>=6)return PHARAOH_PERCENTAGE_SCALE_VALUE;
        (void)pharaoh_construction_footprint_is_free(p->footprint,3,x2,y2,UINT32_MAX,1);break;
    case 50:if(c->cities[*allocation->current_player].native_4600)break;goto draw;
    case 56:width=3;mask=0xffffffbf;goto draw;
    case 58:case 170:case 200:case 201:width=2;mask=0xffffffbf;goto draw;
    case 65:case 66:case 67:case 68:case 69:case 156:case 157:case 158:case 159:case 160:
        (void)pharaoh_construction_footprint_is_free(p->footprint,3,x2,y2,UINT32_MAX,1);break;
    case 72:case 193:width=3;goto draw;
    case 74:case 75:case 182:
        value=(uint32_t)pharaoh_map_water_determine_orientation_size3(p->water_orientation,x2,y2,1);
        if(!value)*p->footprint->placement_ok=1;
        if(tool==182) {
            if(*p->native_bcb9b0>=6)return PHARAOH_PERCENTAGE_SCALE_VALUE;
            if(!pharaoh_map_water_determine_orientation_size3(p->water_orientation,x2,y2,1))
                *p->footprint->placement_ok=1;
        }
        break;
    case 76:case 90:case 181:
        if(!pharaoh_map_water_determine_orientation_size2(p->footprint,x2,y2,1,0))
            *p->footprint->placement_ok=1;
        break;
    case 77:case 78:case 79:
        if(p->cities[*allocation->current_player].mansion_placed)break;
        if(p->requirements->floodplain_count) {
            if(!floodplain_count(c,x2,y2,(int32_t)size,p->requirements->floodplain_count))
                return PHARAOH_PERCENTAGE_SCALE_VALUE;
            width=*p->size;
        }
        goto draw;
    case 82:case 83:
        value=p->bridge->state->length;
        if((int32_t)value>1)*cost=value*base_cost;
        break;
    case 84:case 85:case 86:case 87:case 187:case 188:case 189:
        if(p->cities[*allocation->current_player].palace_placed)break;goto draw;
    case 94:if(p->cities[*allocation->current_player].academy_placed)break;goto draw;
    case 95:if((int32_t)*p->native_705c6c>0)break;goto draw;
    case 136:
        if(!pharaoh_map_water_determine_orientation_size2(p->footprint,x2,y2,1,0))
            *p->footprint->placement_ok=1;
        cell=(uint32_t)s->map->origin+(uint32_t)y2*228u+(uint32_t)x2;
        if(!pharaoh_map_is_ferry_landing_at_offset(c->ferry_landings,(int32_t)cell))break;
        stage=*allocation->ferry_state;
        if(stage==0) {
            *c->ferry_link_accepted=0;
            if(buildings[s->building_ids[cell]].linked_ferry_building_id<0) {
                *allocation->ferry_state=2;*c->ferry_link_accepted=1;
                *allocation->first_ferry=(uint32_t)(int32_t)s->building_ids[
                    (uint32_t)s->map->origin+(uint32_t)y2*228u+(uint32_t)x2];
            }
        } else if(stage==1) {
            const int32_t id=s->building_ids[cell];
            source=*allocation->first_ferry;*c->ferry_link_accepted=0;
            if(buildings[id].linked_ferry_building_id<0) {
                const uint32_t source_orientation=(buildings[source].industry_orientation*2u+4u)&7u;
                const uint32_t target_orientation=buildings[id].industry_orientation*2u;
                const uint32_t source_size=buildings[source].size;
                const int32_t source_y=buildings[source].y,source_x=buildings[source].x;
                int32_t count=0;
                if(pharaoh_construction_ferry_route_is_valid(c->ferry_route,x2,y2,
                    source_x,source_y,source_size,(int32_t)target_orientation,
                    (int32_t)source_orientation,0,&count)) {
                    value=(uint32_t)(int32_t)s->building_ids[cell];source=*allocation->first_ferry;
                    *allocation->ferry_state=3;*c->ferry_link_accepted=1;
                    buildings[value].linked_ferry_building_id=(int32_t)source;
                    buildings[source].linked_ferry_building_id=(int32_t)value;
                    /* Native486020 ignores the pushed value; reads BC9800. */
                    pharaoh_construction_ferry_landing_placed(allocation->ferry_state,
                        (const int32_t *)allocation->first_ferry,p->basic->warnings,s->undo);
                }
            }
        }
        break;
    case 168:pharaoh_construction_place_wall_strip(p->walls,1,x1,y1,x2,y2,0x404000);goto scale;
    case 169:pharaoh_construction_place_wall_strip(p->walls,1,x1,y1,x2,y2,0x804000);goto scale;
    case 171:case 202:pharaoh_gatehouse_mark_construction(p->footprint,x2,y2,*allocation->rotation);break;
    case 207:width=3;mask=0xffffffbf;goto draw;
    case 208:width=5;mask=0xffffffbf;goto draw;
    case 209:(void)pharaoh_entertainment_build_festival_square(p->entertainment,1,x2,y2);goto scale;
    case 210:(void)pharaoh_construction_footprint_is_free(p->footprint,6,x2,y2,UINT32_MAX,1);break;
    case 213:case 222:case 225:case 227:case 228:case 229:case 230:case 234:case 235:case 236:break;
    default: {
        allow=(uint8_t)pharaoh_building_type_is_0046fde0_group((uint32_t)tool);
        if(!allow && area_query(c,x2,y2,(int32_t)*p->size,0,0x10000,1))
            return PHARAOH_PERCENTAGE_SCALE_VALUE;
        if(p->requirements->terrain_10800) {
            if(!area_query(c,x2,y2,3,1,0x10800,0))return PHARAOH_PERCENTAGE_SCALE_VALUE;
            break;
        }
        const PharaohConstructionTerrainRule rule=*p->terrain_rule;
        if(rule) {
            if(!pharaoh_map_region_matches_predicate(s->map,x2,y2,2,1,
                rule_call,(void *)&rule,s->area_x,s->area_y))return PHARAOH_PERCENTAGE_SCALE_VALUE;
            break;
        }
        if(p->requirements->forest) {
            if(!area_query(c,x2,y2,2,1,1,0))return PHARAOH_PERCENTAGE_SCALE_VALUE;break;
        }
        if(p->requirements->water_10004) {
            if(!area_query(c,x2,y2,2,3,4,0))return PHARAOH_PERCENTAGE_SCALE_VALUE;break;
        }
        if(p->requirements->wall) {
            if(!area_query(c,x2,y2,2,0,0x4000,1))return PHARAOH_PERCENTAGE_SCALE_VALUE;break;
        }
        if(p->requirements->road) {
            if(!area_query(c,x2,y2,1,0,0x40,1))return PHARAOH_PERCENTAGE_SCALE_VALUE;
            allow=0;mask=0xffffffbf;width=*p->size;goto draw;
        }
        if(p->requirements->floodplain_count) {
            if(!floodplain_count(c,x2,y2,(int32_t)*p->size,p->requirements->floodplain_count))
                return PHARAOH_PERCENTAGE_SCALE_VALUE;
            break;
        }
        if(p->requirements->water_40000) {
            if(!area_query(c,x2,y2,2,1,0x40000,0))return PHARAOH_PERCENTAGE_SCALE_VALUE;break;
        }
        if(tool==183)break;
        width=*p->size;goto draw;
    }
    }
    goto finish;
scale:
    value=*p->count;if((int32_t)value>=0)*cost=value * *cost;
    goto finish;
draw:
    pharaoh_construction_mark_footprint(p->footprint,(int32_t)width,x2,y2,mask,0,allow);
finish:
    *cost-=*p->credit;
    return PHARAOH_PERCENTAGE_SCALE_VALUE;
}
