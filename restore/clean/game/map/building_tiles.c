#include "pharaoh/map/building_tiles.h"
#include <stddef.h>

_Static_assert(offsetof(PharaohBuildingTypeRecord,building_type)==0x10 &&
    offsetof(PharaohBuildingTypeRecord,house_is_merged)==4 &&
    offsetof(PharaohBuildingTypeRecord,grid_offset)==0x0c,
    "native building-tile views");

/* Completed prior MenuDragClearHelpers C; top-left search has no bounds guard. */
uint32_t pharaoh_map_building_tiles_top_left(PharaohMapBuildingTilesContext *c,
    int32_t x,int32_t y)
{
    PharaohMapSurfaceContext *const s=c->surface;
    uint32_t tile=(uint32_t)s->map->origin+(uint32_t)y*228u+(uint32_t)x;
    const uint32_t access=s->property_flags[tile]&15u;
    const uint32_t size=access>=1u && access<=5u?access+1u:1u;
    *c->columns=size;*c->rows=size;
    for (uint32_t i=0;i<size;++i) {if (!(s->edge_flags[tile]&7u)) break;--tile;}
    for (uint32_t i=0;i<size;++i) {if (!(s->edge_flags[tile]&56u)) break;tile-=228u;}
    return tile;
}

/* Completed prior MenuDragClearFootprint C with actual map descendants. */
void pharaoh_map_clear_building_tiles(PharaohMapBuildingTilesContext *c,
    int32_t building,int32_t x,int32_t y)
{
    PharaohMapSurfaceContext *const s=c->surface;
    const PharaohBuildingTypeRecord *const b=&s->buildings[building];
    uint32_t origin,mask=0x03052e84u,rows,columns,row=0;
    if (x<0 || y<0 || x>=s->map->width || y>=s->map->height) {
        *s->placement_failed=1;return;
    }
    if (b->building_type==183) {
        origin=(uint32_t)b->grid_offset;*c->columns=2;*c->rows=2;
    } else origin=pharaoh_map_building_tiles_top_left(c,x,y);
    if (s->map->terrain[origin]==2u) return;
    if ((uint8_t)pharaoh_building_is_farm(s->buildings,building)) {
        *c->columns=3;*c->rows=3;
    } else if (b->building_type==213) {
        rows=(uint32_t)pharaoh_sun_temple_part_size(s->buildings,building,c->sun_parts);
        *c->rows=rows;*c->columns=rows;
    }
    if (b->building_type==138) mask|=s->map->terrain[origin]&0x140u;
    rows=*c->rows;
    if ((int32_t)rows>0) {
        columns=*c->columns;
        do {
            if ((int32_t)columns>0) {
                uint32_t column=0;
                do {
                    const uint32_t tile=origin+(uint32_t)s->footprints[row][column].grid_offset;
                    if (!building || s->building_ids[tile]==building) {
                        if (building && b->building_type!=99)
                            c->native_a9010c[tile]=(uint8_t)b->building_type;
                        const uint8_t access=s->property_flags[tile];
                        s->building_ids[tile]=0;s->property_flags[tile]=access&0xe0u;
                        const uint8_t edge=s->edge_flags[tile];
                        c->native_a9cc1c[tile]=0;s->canal_water[tile]=0;
                        s->edge_flags[tile]=(edge&0xc0u)|64u;
                        const uint32_t terrain=s->map->terrain[tile];
                        c->native_adc36c[tile]=0;
                        if ((terrain&4u) && !(terrain&0x10000u)) {
                            s->map->terrain[tile]=terrain&4u;
                            pharaoh_map_set_water_image(s,(int32_t)((uint32_t)x+column),
                                (int32_t)((uint32_t)y+row),1);
                        } else {
                            const uint32_t variation=s->random[tile]&7u;
                            const uint32_t base=s->groups[500].first_image;
                            s->image_ids[tile]=base+variation;
                            s->map->terrain[tile]=terrain&mask;
                        }
                    }
                    columns=*c->columns;++column;
                } while ((int32_t)column<(int32_t)columns);
                rows=*c->rows;
            }
            ++row;
        } while ((int32_t)row<(int32_t)rows);
    } else columns=*c->columns;
    pharaoh_map_refresh_empty_region(s,x,y,(int32_t)((uint32_t)x+columns),
        (int32_t)((uint32_t)y+rows),0,0);
    columns=*c->columns;pharaoh_map_refresh_surrounding_empty(s,x,y,(int32_t)columns);
    rows=*c->rows;columns=*c->columns;
    pharaoh_map_refresh_meadow(s,x,y,(int32_t)((uint32_t)x+columns),(int32_t)((uint32_t)y+rows));
    rows=*c->rows;columns=*c->columns;
    pharaoh_map_refresh_floodplain(s,x,y,(int32_t)((uint32_t)x+columns),(int32_t)((uint32_t)y+rows));
    rows=*c->rows;columns=*c->columns;
    pharaoh_map_refresh_water(s,x,y,(int32_t)((uint32_t)x+columns),(int32_t)((uint32_t)y+rows),0);
    rows=*c->rows;columns=*c->columns;
    pharaoh_map_refresh_marshland(s,x,y,(int32_t)((uint32_t)x+columns),(int32_t)((uint32_t)y+rows));
    rows=*c->rows;columns=*c->columns;
    pharaoh_map_refresh_rubble(s,x,y,(int32_t)((uint32_t)x+columns),(int32_t)((uint32_t)y+rows));
}

/* Completed prior BuildingSupport C; type/subtype writes precede selection. */
void pharaoh_house_change_type_image(PharaohMapSurfaceContext *s,
    const PharaohHouseImageData *data,int32_t building,int16_t kind)
{
    PharaohBuildingTypeRecord *const b=&s->buildings[building];
    b->building_type=kind;b->subtype=(int16_t)((uint16_t)kind-10u);
    const uint32_t merged=b->house_is_merged;
    const int32_t index=b->subtype;
    const uint32_t group=data->image_groups[index];
    uint32_t variants=data->normal_variants[index],image=s->groups[group].first_image;
    if (merged) {
        variants=data->merged_variants[index];image+=4u;
        if (variants) ++image;
    } else {
        image+=data->merged_variants[index];
        if (variants) image+=s->random[b->grid_offset]&(variants-1u);
    }
    pharaoh_map_place_surface_square(s,(uint16_t)building,b->x,b->y,
        b->size,b->size,image,8);
}
