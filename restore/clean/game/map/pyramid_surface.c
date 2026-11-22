#include "pharaoh/map/surface_tiles.h"
#include "pharaoh/core/memory_strings.h"

static const int32_t stage5_offsets[256]={
    13,13,13,13,13,13,13,5,13,13,13,13,13,13,13,13,
    13,13,13,13,13,13,13,13,13,13,13,13,7,13,13,6,
    13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
    13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,6,
    13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
    13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
    13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
    1,13,13,13,13,13,13,13,13,13,13,13,0,13,0,11,
    13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
    13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,6,
    13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
    13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,6,
    13,3,13,13,13,13,13,4,13,13,13,13,13,13,13,4,
    13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,10,
    13,13,13,13,13,13,13,4,13,13,13,13,13,13,13,4,
    13,2,13,2,13,13,13,9,13,2,13,2,0,8,0,12,
};
static const int32_t casing_offsets[256]={
    13,13,13,13,13,0,13,13,13,13,13,13,13,13,13,13,
    13,13,13,13,1,4,13,13,13,13,13,13,13,13,13,13,
    13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
    13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
    13,3,13,13,13,7,13,13,13,13,13,13,13,13,13,13,
    2,6,13,13,5,8,13,13,13,13,13,13,13,13,13,13,
    13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
    13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
    13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
    13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
    13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
    13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
    13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
    13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
    13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
    13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
};
static const int32_t neighbor_offsets[8]={
    -228,-227,1,229,228,227,-1,-229,
};

uint32_t pharaoh_map_building_undo_capture(PharaohMapBuildingUndo *c,int32_t building)
{
    if(building<=0)return 0;
    uint32_t found=0,count=0;*c->count=0;
    for(uint32_t i=0;i<525;++i){
        const int32_t value=c->ids[i];if(value)++count;if(value==building)found=1;
    }
    *c->count=count;if(found)return 0;
    for(uint32_t i=0;i<525;++i)if(!c->ids[i]){
        *c->count=count+1u;
        pharaoh_memory_copy_forward_words64(&c->live[building],&c->backup[i],264);
        c->ids[i]=(int16_t)(uint16_t)building;return 1;
    }
    *c->available=0;return 0;
}

void pharaoh_pyramid_queue_surface_refresh(PharaohMapSurfaceContext *c,int32_t building)
{
    const PharaohBuildingTypeRecord *b=&c->buildings[building];
    if((int16_t)b->building_type!=183)return;
    const uint32_t root=(uint32_t)building-(uint32_t)b->subtype;
    for(int32_t i=0;i<(int32_t)*c->pyramid_queue_count;++i)
        if(c->pyramid_queue[i]==root)return;
    if((int32_t)*c->pyramid_queue_count<4){
        c->pyramid_queue[*c->pyramid_queue_count]=root;++*c->pyramid_queue_count;
    }
}

void pharaoh_pyramid_publish_surface_queue(PharaohMapSurfaceContext *c)
{
    for(int32_t i=0;i<(int32_t)*c->pyramid_queue_count;++i){
        const PharaohBuildingTypeRecord *b=&c->buildings[c->pyramid_queue[i]];
        const int32_t category=b->pyramid_size_variant;
        if(pharaoh_pyramid_has_complex_temple((int32_t)*c->pyramid_style,c->pyramid_complex_offsets,category)){
            const int32_t relative=(int32_t)((uint32_t)b->grid_offset+
                (uint32_t)c->pyramid_complex_offsets[category]-(uint32_t)c->map->origin);
            pharaoh_pyramid_map_segment_update(c,relative%228,relative/228,4,0);
        }
    }
    *c->pyramid_queue_count=0;
}

uint8_t pharaoh_pyramid_set_construction_tile_image(PharaohMapSurfaceContext *c,
    int32_t building,uint32_t tile,int32_t stage,uint8_t action)
{
    const int32_t *table;uint32_t base,variants=1,mask=255,bits=0;
    (void)building;
    switch((uint32_t)stage-3u){
    case 0:base=c->groups[0x2f7].first_image;table=(const int32_t *)c->pyramid_stage3_offset;mask=0;break;
    case 1:
        if(action){
            c->image_ids[tile]=0;
            const int32_t relative=(int32_t)(tile-(uint32_t)c->map->origin),x=relative%228,y=relative/228;
            pharaoh_map_refresh_trees(c,x,y,x,y);
            pharaoh_map_refresh_empty_region(c,x,y,x,y,0,1);
            pharaoh_map_refresh_meadow(c,x,y,x,y);
        }
        return action;
    case 2:base=c->groups[0x2f1].first_image;table=stage5_offsets;variants=8;break;
    case 3:case 5:base=c->groups[0x2f2].first_image;table=casing_offsets;mask=85;break;
    case 4:base=c->groups[0x2f3].first_image;table=casing_offsets;mask=85;break;
    case 6:base=c->groups[0x2f4].first_image;table=casing_offsets;mask=85;break;
    case 7:base=c->groups[0x2f5].first_image;table=casing_offsets;mask=85;break;
    default:return 0;
    }
    for(uint32_t i=0;i<8;++i){
        const uint32_t at=tile+(uint32_t)neighbor_offsets[((uint32_t)*c->orientation+i)&7u];
        const int32_t neighbor=pharaoh_pyramid_effective_tile_stage(c->buildings,c->building_ids,at,
            *c->pyramid_style,c->pyramid_regular,c->pyramid_style_five);
        if(stage==5?(neighbor>=5 || !neighbor):neighbor>=0)bits|=1u<<i;
    }
    bits&=mask;c->image_ids[tile]=(uint32_t)table[bits]+base;
    if(bits==255u && variants>1u)c->image_ids[tile]+=c->random[tile]%variants;
    return 1;
}

/* Native004ED360, complete modes1..6; mode1 requires real render endpoints. */
static uint32_t flags(const PharaohMapSurfaceContext *c,uint32_t cell){return (c->map->terrain[cell]);}
static int outside(const PharaohMapSurfaceContext *c,uint32_t x,uint32_t y,uint32_t extra)
{
    uint32_t wy=(c->geometry->diamond_y_min)+y+extra,wx=(c->geometry->diamond_x_min)+x+extra;
    return (uint8_t)pharaoh_map_grid_size_reaches_outside(c->geometry,(int32_t)wx,(int32_t)wy,3)!=0;
}
static void draw(PharaohMapSurfaceContext *c,uint32_t x,uint32_t y,uint32_t samples,uint32_t mask)
{
    int32_t sx,sy;uint32_t i;
    c->render->project_tile(c->render->user_data,(int32_t)x,(int32_t)y,&sx,&sy);
    for(i=0;(int32_t)i<(int32_t)samples;++i){
        uint32_t yy=(c->preview_offsets[i][1])+(uint32_t)sy,xx=(c->preview_offsets[i][0])+(uint32_t)sx;
        c->render->draw_masked_sprite(c->render->user_data,(int32_t)(c->groups[0x206].first_image),(int32_t)xx,(int32_t)yy,mask);
    }
}
static void TerrainOr(PharaohMapSurfaceContext *c,uint32_t cell)
{
    (c->map->terrain[cell] = flags(c,cell)|0x10000000u);
    (c->map->terrain[cell+1u] = (c->map->terrain[cell+1u])|0x10000000u);
    (c->map->terrain[cell+228u] = (c->map->terrain[cell+228u])|0x10000000u);
    (c->map->terrain[cell+229u] = (c->map->terrain[cell+229u])|0x10000000u);
}
static void TerrainAnd(PharaohMapSurfaceContext *c,uint32_t cell)
{
    (c->map->terrain[cell] = flags(c,cell)&0x03052e84u);
    (c->map->terrain[cell+1u] = (c->map->terrain[cell+1u])&0x03052e84u);
    (c->map->terrain[cell+228u] = (c->map->terrain[cell+228u])&0x03052e84u);
    (c->map->terrain[cell+229u] = (c->map->terrain[cell+229u])&0x03052e84u);
}
static int missing(PharaohMapSurfaceContext *c,uint32_t cell,uint32_t Code)
{
    if((c->building_ids[cell]))return 0;
    (c->pyramid_placement->missing_tile = cell);(c->pyramid_placement->missing_type = Code);return 1;
}
static int matches(PharaohMapSurfaceContext *c,uint32_t cell)
{
    if((uint32_t)(int32_t)(c->building_ids[cell])!=(c->pyramid_placement->target_building))return 0;
    (c->pyramid_placement->found = 1);return 1;
}
void pharaoh_pyramid_map_segment_update(PharaohMapSurfaceContext *c,int32_t inputX,int32_t inputY,int32_t mode,uint32_t mask)
{
    uint32_t x=(uint32_t)inputX,y=(uint32_t)inputY,base=(c->map->origin),cell=base+y*228u+x;
    uint32_t reject=0xeeffd76eu,terminal=0xfbffff7bu,samples,value,record;
    (c->pyramid_placement->found = 0);(c->pyramid_placement->step = 1);(c->pyramid_placement->size = 2);
    (c->pyramid_placement->terminal_tile = 0);(c->pyramid_placement->span = 0);(c->pyramid_placement->missing_tile = 0);
    (c->pyramid_placement->anchor_tile = cell);(c->pyramid_placement->missing_type = 0);
    if(mode==3||mode==4||mode==5||mode==6){reject=0xeeffd766u;terminal=0xebffff73u;}
    if(outside(c,x,y,0)||outside(c,x,y,1))return;
    if(((c->map->terrain[cell+1u])&reject)||((c->map->terrain[cell+228u])&reject)||((c->map->terrain[cell+229u])&reject))return;
    samples=(*c->preview_count);
    if(mode==1)draw(c,x,y,samples,mask);
    else if(mode==2){
        TerrainOr(c,cell);value=(c->groups[0x2f7].first_image);
        (c->image_ids[cell] = value);(c->image_ids[cell+1u] = value);
        (c->image_ids[cell+228u] = value);(c->image_ids[cell+229u] = value);
    }else if(mode==3){if(missing(c,cell,0xda))return;}
    else if(mode==4){
        if(!(c->building_ids[cell])){
            (c->map->terrain[cell] = flags(c,cell)|0x10000000u);
            (c->map->terrain[cell+1u] = (c->map->terrain[cell+1u])|0x10000000u);
            (c->map->terrain[cell+228u] = (c->map->terrain[cell+228u])|0x10000000u);
            record=(c->map->terrain[cell+229u])|0x10000000u;value=(c->groups[0x2f7].first_image);
            (c->image_ids[cell] = value);(c->image_ids[cell+1u] = value);(c->image_ids[cell+228u] = value);
            (c->map->terrain[cell+229u] = record);(c->image_ids[cell+229u] = value);
        }
    }else if(mode==5){
        record=(uint32_t)(int32_t)(c->building_ids[cell]);
        if(record){pharaoh_map_building_undo_capture(c->undo,(int32_t)record);(*c->snapshot_calls = (*c->snapshot_calls)+1u);(c->buildings[record].state = 6);}
        else TerrainAnd(c,cell);
    }else if(mode==6){if(matches(c,cell))return;}
    cell+=(c->pyramid_placement->step)*2u;x+=2u;
    if(outside(c,x,y,0))return;
    for(;;){
        if(outside(c,x,y,1))return;
        if(((c->map->terrain[cell+1u])&reject)||((c->map->terrain[cell+228u])&reject)||((c->map->terrain[cell+229u])&reject))break;
        if(mode==1)draw(c,x,y,samples,mask);
        else if(mode==2)TerrainOr(c,cell);
        else if(mode==4){if(!(c->building_ids[cell]))TerrainOr(c,cell);}
        else if(mode==3){if(missing(c,cell,0xdb))return;}
        else if(mode==5){
            record=(uint32_t)(int32_t)(c->building_ids[cell]);
            if(record){pharaoh_map_building_undo_capture(c->undo,(int32_t)record);(*c->snapshot_calls = (*c->snapshot_calls)+1u);(c->buildings[record].state = 6);}
            else TerrainAnd(c,cell);
        }else if(mode==6){if(matches(c,cell))return;}
        value=(c->pyramid_placement->span)+1u;(c->pyramid_placement->span = value);
        if((int32_t)value>20)return;
        if((int32_t)((c->geometry->diamond_y_min)+y)>=(int32_t)(c->geometry->diamond_y_max))return;
        cell+=(c->pyramid_placement->step)*2u;x+=2u;
        if(outside(c,x,y,0))return;
    }
    if(!(c->pyramid_placement->span))return;
    value=(c->map->terrain[cell+1u]);
    if((value&terminal)||((c->map->terrain[cell+228u])&reject))return;
    record=(c->map->terrain[cell+229u]);
    if((record&terminal)||!(value&4u)||!(record&4u))return;
    if(outside(c,x,y,0)||outside(c,x,y,1))return;
    if(mode==1){samples=(*c->preview_count);draw(c,x,(uint32_t)inputY,samples,mask);(c->pyramid_placement->terminal_tile = cell);return;}
    if(mode==2||(mode==4&&!(c->building_ids[cell]))){
        value=flags(c,cell);(c->pyramid_placement->terminal_tile = cell);(c->map->terrain[cell] = value|0x10000000u);
        (c->map->terrain[cell+1u] = (c->map->terrain[cell+1u])|0x10000000u);
        (c->map->terrain[cell+228u] = (c->map->terrain[cell+228u])|0x10000000u);
        (c->map->terrain[cell+229u] = (c->map->terrain[cell+229u])|0x10000000u);
        value=(c->groups[0x2f7].first_image);(c->image_ids[cell] = value);(c->image_ids[cell+228u] = value);return;
    }
    if(mode==3){if(missing(c,cell,0xdc))return;}
    else if(mode==5){
        record=(uint32_t)(int32_t)(c->building_ids[cell]);
        if(record){
            pharaoh_map_building_undo_capture(c->undo,(int32_t)record);value=(*c->snapshot_calls)+1u;
            (c->pyramid_placement->terminal_tile = cell);(*c->snapshot_calls = value);(c->buildings[record].state = 6);return;
        }
        value=flags(c,cell)&0x03052e84u;(c->pyramid_placement->terminal_tile = cell);(c->map->terrain[cell] = value);
        (c->map->terrain[cell+1u] = (c->map->terrain[cell+1u])&0x03052e84u);
        (c->map->terrain[cell+228u] = (c->map->terrain[cell+228u])&0x03052e84u);
        (c->map->terrain[cell+229u] = (c->map->terrain[cell+229u])&0x03052e84u);return;
    }else if(mode==6){if(matches(c,cell))return;}
    (c->pyramid_placement->terminal_tile = cell);
}
