#include "pharaoh/map/surface_tiles.h"
#include "pharaoh/map/surface_queries.h"
#include "pharaoh/core/x87_numeric.h"

_Static_assert(sizeof(void *)!=4 || offsetof(PharaohFloodTile,is_floodplain)==24,
    "native flood object flag is BYTE+18");

static const int32_t neighbors[8]={-228,-227,1,229,228,227,-1,-229};
static uint32_t image_group(const PharaohMapSurfaceContext *c,uint32_t group)
{return c->groups[group].first_image;}

/* Complete typed reuse of Unit01117, native0047C140. */
void pharaoh_map_place_region_images(PharaohMapSurfaceContext *c,
    int32_t building,int32_t x,int32_t y,int32_t width,int32_t height,
    uint32_t image,uint32_t flags)
{
    uint32_t frame=0;
    if(x<0 || y<0 ||
        (int32_t)((uint32_t)x+(uint32_t)width-1u)>=c->map->width ||
        (int32_t)((uint32_t)y+(uint32_t)height-1u)>=c->map->height){
        *c->placement_failed=1;return;
    }
    const uint32_t base=(uint32_t)c->map->origin+(uint32_t)y*228u+(uint32_t)x;
    *c->current_tile=base;
    for(int32_t row=0;row<height;++row)for(int32_t col=0;col<width;++col){
        const uint32_t tile=base+c->footprints[row][col].grid_offset;
        c->property_flags[tile]&=0xe0u;
        c->edge_flags[tile]|=0x40u;
        c->map->terrain[tile]=(c->map->terrain[tile]&0x03052e80u)|flags;
        c->building_ids[tile]=(int16_t)(uint16_t)building;
        c->image_ids[tile]=image+frame++;
    }
}

/* Complete typed reuse of Unit01122, native0047C880. */
int32_t pharaoh_map_empty_rectangle_fits(PharaohMapSurfaceContext *c,
    int32_t x,int32_t y,int32_t width,int32_t height,int32_t mask,
    int32_t required_image,uint8_t allow_floodplain)
{
    uint32_t seen_flood=0,seen_dry=0;
    if(x<3 || y<3 ||
        (int32_t)((uint32_t)x+(uint32_t)width-1u)>=(int32_t)((uint32_t)c->map->width-3u) ||
        (int32_t)((uint32_t)y+(uint32_t)height-1u)>=(int32_t)((uint32_t)c->map->height-3u))return 0;
    *c->current_tile=(uint32_t)c->map->origin+(uint32_t)y*228u+(uint32_t)x;
    for(int32_t row=0;row<height;++row)for(int32_t col=0;col<width;++col){
        const uint32_t tile=*c->current_tile+c->footprints[row][col].grid_offset;
        if(pharaoh_map_grid_size_reaches_outside(c->geometry,(int32_t)tile%228,(int32_t)tile/228,3))return 0;
        uint32_t flags=c->map->terrain[tile]&((uint32_t)mask|0x10010000u);
        if(c->flood_tiles[tile] && !c->flood_tiles[tile]->is_floodplain)return 0;
        if(flags&0x10000u){if(seen_dry)return 0;seen_flood=1;}
        else{if(seen_flood)return 0;seen_dry=1;}
        flags&=allow_floodplain ? 0xfefed77fu : 0xfeffd77fu;
        if(flags || c->blocked_grid[tile])return 0;
        if(required_image && c->image_ids[tile]!=(uint32_t)required_image-1u)return 0;
    }
    return 1;
}

static int neighbors_blocked(const PharaohMapSurfaceContext *c,int32_t tile)
{
    for(int32_t i=0;i<8;++i)
        if(c->map->terrain[tile+neighbors[i]]&0xeeffd77fu)return 1;
    return 0;
}

static int retain_building(PharaohMapSurfaceContext *c,int32_t tile)
{
    const int32_t id=c->building_ids[tile];
    const PharaohBuildingTypeRecord *b=&c->buildings[id];
    const int32_t type=(int16_t)b->building_type;
    if(type==0xb7){
        const int32_t stage=pharaoh_pyramid_effective_tile_stage(c->buildings,
            c->building_ids,(uint32_t)tile,*c->pyramid_style,c->pyramid_regular,c->pyramid_style_five);
        return stage>=11 || (stage>=3 && pharaoh_pyramid_set_construction_tile_image(c,id,(uint32_t)tile,stage,0));
    }
    if(type==0xd5 || type==0xe3 || type==0xe5 || type==0xea ||
        type==0xeb || type==0xec || type==0xe4){
        if(b->monument_stage)return 1;
        return c->buildings[id-b->subtype].monument_stage!=0;
    }
    if(type==0xe6)return c->buildings[id-b->subtype].abu_simbel_stage>0;
    if(type==0xe1){
        const int32_t first=id-b->subtype;int32_t minimum=21;
        for(int32_t i=0;i<25;++i){
            const int32_t stage=c->buildings[first+i].monument_stage;
            if(stage<minimum)minimum=stage;
        }
        return minimum>0;
    }
    int32_t previous=(int32_t)((uint32_t)tile-20u),found=0;
    while(previous>c->map->origin){
        found=c->building_ids[previous];
        if(found || !(c->map->terrain[previous]&0x10000000u))break;
        previous-=20;
    }
    if(found && c->buildings[found].building_type==0xb7)
        pharaoh_pyramid_queue_surface_refresh(c,found);
    return 0;
}

static void redraw_floodplain(PharaohMapSurfaceContext *c,int32_t tile)
{
    uint32_t group,variant;const uint32_t saved=*c->current_tile;
    *c->current_tile=(uint32_t)tile;
    pharaoh_map_adjacent_terrain_any(c->adjacent,c->map->terrain,tile,0x10000u);
    *c->current_tile=saved;
    if(pharaoh_map_adjacent_flags_count(c->adjacent)){
        const PharaohMapImageContext image={c->adjacent,*c->orientation,c->selection};
        (void)pharaoh_map_image_context_match(&image,c->floodplain_edges,26,-1,0);
        group=0x210;variant=c->selection->orientation_offset+0x30u;
    }else{
        const uint32_t fertility=c->fertility[tile];
        variant=(uint32_t)pharaoh_x87_floodplain_fertility(fertility);
        if(fertility>100u){
            variant=(variant&8u) ? ((variant-(uint32_t)(*c->orientation/2))&3u)+(variant&8u)
                : (variant-(uint32_t)*c->orientation)&7u;
            variant|=0x80u;
        }
        const uint32_t random=c->random[tile];
        if((int32_t)variant<11){group=0x21f;variant+=(random%3u)*12u;}
        else if((int32_t)variant>=0x80){group=0x231;variant&=0x7fu;}
        else if(!(c->map->terrain[tile]&0x10000000u) && neighbors_blocked(c,tile)){
            group=0x225;variant=random%12u;
        }else{
            group=0x226;variant=random%12u;
            for(int32_t i=0;i<8;++i){
                const int32_t at=tile+neighbors[i];
                uint32_t fertility_n=c->fertility[at];
                if(fertility_n>100u)fertility_n=100u;
                if(pharaoh_x87_integer_ratio((int32_t)fertility_n,8)!=11){group=0x229;break;}
                const uint32_t flags=c->map->terrain[at];
                if((flags&0x10801u) || (!(flags&0x10000000u) && neighbors_blocked(c,at))){group=0x229;break;}
            }
        }
    }
    if(c->map->terrain[tile]&0x40u)
        variant=(uint32_t)pharaoh_map_road_canal_crossing_image((uint32_t)tile,(int32_t)variant);
    c->image_ids[tile]=image_group(c,group)+variant;
}

/* Complete typed reuse of MenuDragTerrainGraph, native004767D0. */
void pharaoh_map_refresh_empty_region(PharaohMapSurfaceContext *c,
    int32_t x0,int32_t y0,int32_t x1,int32_t y1,
    uint8_t preserve_clear,uint8_t exact_rectangle)
{
    if(!exact_rectangle){x0=(int32_t)((uint32_t)x0-1u);y0=(int32_t)((uint32_t)y0-1u);
        x1=(int32_t)((uint32_t)x1+1u);y1=(int32_t)((uint32_t)y1+1u);}
    if(x0<0)x0=0;if(y0<0)y0=0;
    const int32_t width=c->map->width,height=c->map->height;
    if(x1>=width)x1=(int32_t)((uint32_t)width-1u);
    if(y1>=height)y1=(int32_t)((uint32_t)height-1u);
    const uint32_t step=(uint32_t)width-(uint32_t)x1-1u+(uint32_t)x0;
    uint32_t tile=(uint32_t)c->map->origin+(uint32_t)y0*228u+(uint32_t)x0;
    for(int32_t y=y0;y<=y1;++y){
        for(int32_t x=x0;x<=x1;++x,++tile){
            uint32_t flags=c->map->terrain[tile];
            if(flags&0x10000000u){flags&=0xeffffff7u;if(retain_building(c,(int32_t)tile))continue;}
            if(c->flood_tiles[tile] && !c->flood_tiles[tile]->is_floodplain){if(flags&8u)continue;}
            else if(flags&0xeeffd77fu)continue;
            if(flags&0x80u){if(!(flags&0x22310803u))redraw_floodplain(c,(int32_t)tile);}
            else if(!preserve_clear){
                const uint8_t properties=c->property_flags[tile],edge=c->edge_flags[tile];
                c->image_ids[tile]=0;c->property_flags[tile]=(uint8_t)(properties&0xf0u);
                c->edge_flags[tile]=(uint8_t)(edge|0x40u);
            }
        }
        tile+=(uint32_t)*c->border_size+step;
    }
    if(!preserve_clear)for(int32_t phase=0;phase<2;++phase){
        tile=(uint32_t)c->map->origin+(uint32_t)y0*228u+(uint32_t)x0;
        for(int32_t y=y0;y<=y1;++y){
            for(int32_t x=x0;x<=x1;++x,++tile){
                uint32_t flags=c->map->terrain[tile],object_flags=0,image;
                int32_t building=0,size=1;
                if(flags&0x10000000u){flags&=0xeffffff7u;building=c->building_ids[tile];object_flags=0x10000008u;}
                if((flags&0xeefed77fu) || c->image_ids[tile])continue;
                if(!phase){
                    const uint32_t random=c->random[tile];if(random&0xf0u)continue;
                    image=image_group(c,(c->property_flags[tile]&0x20u)?0x1f8u:0x1fbu)+(random&7u);
                }else{
                    const uint32_t group=(c->property_flags[tile]&0x20u)?0x1f8u:0x1fbu;
                    if(pharaoh_map_empty_rectangle_fits(c,x,y,4,4,0xffff,1,1)){size=4;image=0x2a;}
                    else if(pharaoh_map_empty_rectangle_fits(c,x,y,3,3,0xffff,1,1)){size=3;image=0x18u+(c->random[tile]&1u)*9u;}
                    else if(pharaoh_map_empty_rectangle_fits(c,x,y,2,2,0xffff,1,1)){size=2;image=8u+(c->random[tile]&3u)*4u;}
                    else image=c->random[tile]&7u;
                    image+=image_group(c,group);
                }
                pharaoh_map_place_region_images(c,building,x,y,size,size,image,object_flags);
            }
            tile+=(uint32_t)*c->border_size+step;
        }
    }
    pharaoh_pyramid_publish_surface_queue(c);
}
