#include "pharaoh/building/construction_site_checks.h"
#include <stddef.h>

_Static_assert(sizeof(PharaohGatehouseImagePart)==16 &&
    offsetof(PharaohGatehouseImagePart,size)==4 &&
    offsetof(PharaohGatehouseImagePart,dy)==8 &&
    offsetof(PharaohGatehouseImagePart,dx)==12 &&
    sizeof(PharaohScenarioEvent)==124 &&
    sizeof(PharaohScenarioPlayerRuntime)==0x49d8,
    "native gatehouse/event/player record strides");

/* Complete prior PlacementSupport C, native004BFF60. Modes other than0/1
 * retain the anchor. Native scans columns before rows (the prior C did not);
 * the two real rectangle queries retain their order and live cursor writes. */
uint32_t pharaoh_gatehouse_site_fits(PharaohMapSurfaceContext *surface,
    const PharaohGatehouseImagePart parts[4],int32_t x,int32_t y,int32_t mode)
{
    for(int32_t index=0;index<4;++index){
        const PharaohGatehouseImagePart *part=&parts[index];
        uint32_t tile_x=(uint32_t)x,tile_y=(uint32_t)y;
        if(mode==0){tile_x+=(uint32_t)part->dx;tile_y+=(uint32_t)part->dy;}
        else if(mode==1){tile_x+=(uint32_t)part->dy;tile_y+=(uint32_t)part->dx;}
        for(int32_t column=0;column<part->size;++column){
            for(int32_t row=0;row<part->size;++row){
                int32_t px=(int32_t)(tile_x+(uint32_t)column);
                int32_t py=(int32_t)(tile_y+(uint32_t)row);
                if((!part->on_road||
                    pharaoh_map_empty_rectangle_fits(surface,px,py,1,1,0x40,0,0))&&
                    !pharaoh_map_empty_rectangle_fits(surface,px,py,1,1,-1,0,0))return 0;
            }
        }
    }
    return 1;
}

/* Complete prior PlacementSupport C, native004C3730. Native table5E00CC
 * matches these128 bytes. Entry-point precedence and signed results matter. */
int32_t pharaoh_temple_complex_can_place(const PharaohTempleComplexSiteContext *c,
    int32_t x,int32_t y)
{
    static const int32_t origins[4][4][2]={
        {{0,-2},{4,2},{2,4},{-2,0}},
        {{-2,0},{0,-2},{4,2},{2,4}},
        {{2,4},{-2,0},{0,-2},{4,2}},
        {{4,2},{2,4},{-2,0},{0,-2}}
    };
    int32_t view=*c->orientation/2;
    uint32_t base_x=(uint32_t)x+(uint32_t)origins[view][*c->construction_orientation][0];
    uint32_t base_y=(uint32_t)y+(uint32_t)origins[view][*c->construction_orientation][1];
    int32_t turn=(*c->construction_orientation-view+4)%4;
    for(int32_t row=0;row<7;++row){
        for(int32_t column=0;column<13;++column){
            uint32_t px,py,tile,terrain;
            switch(turn){
            case 0:px=base_x+(uint32_t)column;py=base_y+(uint32_t)row;break;
            case 1:px=base_x-(uint32_t)row;py=base_y-(uint32_t)column;break;
            case 2:px=base_x-(uint32_t)column;py=base_y-(uint32_t)row;break;
            default:px=base_x+(uint32_t)row;py=base_y+(uint32_t)column;break;
            }
            tile=(uint32_t)c->map->origin+py*228u+px;
            terrain=c->map->terrain[tile];
            if(c->walker_ids[tile])return -1;
            if(pharaoh_construction_area_has_entry_point(c->entry_points,
                (int32_t)px,(int32_t)py,(int32_t)px,(int32_t)py))return -2;
            if(terrain&UINT32_C(0xeeffd77f))return -1;
        }
    }
    return 1;
}

/* Shared literal row/column scan from the two complete prior C owners.
 * No virtual predicate: the original site mask differs from4E5310 by bit0. */
static uint8_t site_area(const PharaohMonumentPlacementContext *c,
    uint32_t x,uint32_t y,int32_t size)
{
    *c->current_tile=(uint32_t)*c->origin+y*228u+x;
    for(int32_t row=0;row<size;++row){
        for(int32_t column=0;column<size;++column){
            if(pharaoh_map_grid_size_reaches_outside(c->geometry,
                (int32_t)((uint32_t)*c->world_x+(uint32_t)column+x),
                (int32_t)((uint32_t)*c->world_y+(uint32_t)row+y),3))return 0;
            uint32_t tile=*c->current_tile+c->footprints[row][column].grid_offset;
            if(c->terrain[tile]&UINT32_C(0xfeffd76e))return 0;
        }
    }
    return 1;
}

/* Complete CityFootprintValidation C, native004DEF00. Orientation is read
 * once per part; the actual descriptor provider supplies all116 pieces. */
uint8_t pharaoh_mausoleum_can_place(const PharaohMonumentPlacementContext *c,
    const PharaohMausoleumProjectionContext *projection,
    const int32_t *orientation,uint32_t cell)
{
    int32_t relative=(int32_t)(cell-(uint32_t)*c->origin);
    uint32_t x=(uint32_t)(relative%228),y=(uint32_t)(relative/228);
    for(int32_t index=0;index<116;++index){
        PharaohMausoleumPartDescriptor part;
        int32_t view=*orientation;
        pharaoh_mausoleum_part_describe(projection,&part,index,view,view);
        if(!site_area(c,x+(uint32_t)part.dx,y+(uint32_t)part.dy,part.size))return 0;
    }
    return 1;
}

/* Complete SunArchitecture50 C, native00541AE0. Descriptor base601DD8,
 * not its size-based overlay601DE0. Every part writes the shared cursor. */
uint8_t pharaoh_sun_temple_can_place(const PharaohMonumentPlacementContext *c,
    const PharaohMonumentImagePart parts[100],uint32_t cell)
{
    int32_t relative=(int32_t)(cell-(uint32_t)*c->origin);
    uint32_t x=(uint32_t)(relative%228),y=(uint32_t)(relative/228);
    for(int32_t index=0;index<100;++index){
        if(!site_area(c,x+(uint32_t)parts[index].dx,
            y+(uint32_t)parts[index].dy,parts[index].size))return 0;
    }
    return 1;
}

/* Complete MonumentPlacementQueries loop, native00542EE0. Native callers
 * test AL; upper EAX is incidental. A matched incomplete temple skips99 IDs. */
uint8_t pharaoh_sun_temple_has_completed(PharaohComplexProgressContext *c)
{
    for(int32_t id=1;id<4000;++id){
        const PharaohBuildingTypeRecord *building=&c->buildings[id];
        if(building->state==1&&building->building_type==213){
            if(pharaoh_sun_temple_progress(c,id,NULL)==100)return 1;
            id+=99;
        }
    }
    return 0;
}

/* Complete MapCommitTownWarning C, native004E2E20. Real event iteration and
 * running-event predicate retain ID/search mutations and calendar semantics. */
void pharaoh_construction_warning_military(const PharaohConstructionMilitaryWarnings *c,
    int32_t tool)
{
    PharaohScenarioEvent *event;
    if(*c->editor_active==1)return;
    if(!((tool>=65&&tool<=69)||(tool>=60&&tool<=64)||(tool>=140&&tool<=149))){
        switch(tool){
        case 30:case 31:case 32:case 33:case 34:case 35:case 36:case 55:
        case 77:case 78:case 79:case 81:case 86:case 138:case 167:
        case 184:case 187:case 188:case 189:case 226:break;
        default:return;
        }
    }
    event=pharaoh_scenario_invasion_next(c->events,*c->event_count,NULL,false,
        c->invasion_search_flag,c->current_event_index);
    while(event&&!pharaoh_event_is_running(event,c->calendar))
        event=pharaoh_scenario_invasion_next(c->events,*c->event_count,event,false,
            c->invasion_search_flag,c->current_event_index);
    if(!event)return;
    if(*c->invasion_warning_shown)return;
    const uint8_t *player=(const uint8_t *)&c->players[*c->current_player];
    if(player[0x4668])return;
    pharaoh_city_warning_post(c->warnings,104);
}
