#include "pharaoh/building/housing_construction.h"
#include "pharaoh/building/construction_commit_support.h"
#include "pharaoh/map/surface_queries.h"

static int housing_cell_allowed(const PharaohHousingConstructionContext *c,
    uint32_t cell)
{
    const PharaohMapSurfaceContext *s=c->allocation->surface;
    const uint32_t flags=s->map->terrain[cell];
    if ((flags&0x10000u)!=0) return 0;
    if (s->flood_tiles[cell]!=NULL && s->flood_tiles[cell]->is_floodplain==0)
        return 0;
    if ((flags&0xeefed77fu)!=0) return 0;
    if (s->blocked_grid[cell]!=0) return 0;
    return 1;
}

static int nearby_road(PharaohMapSurfaceContext *s, int32_t x, int32_t y)
{
    PharaohMapAreaCursor cursor={*s->area_x,*s->area_y,0};
    const int32_t result=pharaoh_map_terrain_exists_in_area_with(
        s->map,x,y,1,2,0x40,&cursor);
    *s->area_x=cursor.x; *s->area_y=cursor.y;
    return result!=0;
}

PharaohPercentageScaleStatus pharaoh_construction_build_housing_rectangle(
    const PharaohHousingConstructionContext *c, uint32_t preview,
    int32_t x1, int32_t y1, int32_t x2, int32_t y2)
{
    PharaohMapSurfaceContext *s=c->allocation->surface;
    int32_t left=x1,right=x2,top=y1,bottom=y2,swap;
    uint32_t failed=0;
    if (left>right) {swap=left;left=right;right=swap;}
    if (top>bottom) {swap=top;top=bottom;bottom=swap;}
    if (left<0) left=0;
    if (right>=s->map->width) right=(int32_t)((uint32_t)s->map->width-1u);
    if (top<0) top=0;
    if (bottom>=s->map->height) bottom=(int32_t)((uint32_t)s->map->height-1u);
    *c->placed_count=0;
    const uint32_t span=(uint32_t)right-(uint32_t)left+1u;
    pharaoh_map_undo_release_reservations(s->undo,s->buildings);
    uint32_t cell=(uint32_t)s->map->origin+(uint32_t)top*228u+(uint32_t)left;
    *s->scan_y=top; *s->current_tile=cell;
    if (top<=bottom) {
        uint32_t row=(uint32_t)top;
        do {
            *s->scan_x=left;
            if (left<=right) {
                uint32_t column=(uint32_t)left;
                do {
                    if (housing_cell_allowed(c,cell)) {
                        if (preview!=0) {
                            const uint32_t next=*c->placed_count+1u;
                            s->property_flags[cell]|=0x10u;
                            *c->placed_count=next;
                        } else {
                            const int32_t building=pharaoh_building_allocate(
                                c->allocation,*c->allocation->current_player,
                                10,*s->scan_x,*s->scan_y,0,0);
                            (void)pharaoh_map_building_undo_capture(s->undo,building);
                            if (building>0) {
                                const uint32_t next=*c->placed_count+1u;
                                const uint32_t image=*c->house_image;
                                const int32_t x=*s->scan_x;
                                *c->placed_count=next;
                                pharaoh_map_place_surface_square(s,(uint16_t)building,
                                    x,*s->scan_y,1,1,image,8);
                                if (!nearby_road(s,*s->scan_x,*s->scan_y))
                                    failed=1;
                            }
                            cell=*s->current_tile;
                        }
                    }
                    column=(uint32_t)*s->scan_x+1u;
                    ++cell;
                    *s->scan_x=(int32_t)column; *s->current_tile=cell;
                } while ((int32_t)column<=right);
            }
            row=(uint32_t)*s->scan_y+1u;
            cell+=228u-span;
            *s->scan_y=(int32_t)row; *s->current_tile=cell;
        } while ((int32_t)row<=bottom);
    }
    if (preview==0) {
        const PharaohPercentageScaleStatus status=
            pharaoh_construction_warning_food_supply(c->food_warnings,10);
        if (status!=PHARAOH_PERCENTAGE_SCALE_VALUE) return status;
        pharaoh_housing_warning_unemployment(c->housing_warnings);
        pharaoh_housing_warning_labor_need(c->housing_warnings);
        pharaoh_housing_migration_post_message(c->housing_warnings,10);
        if (failed==1)
            pharaoh_city_warning_post(c->housing_warnings->warnings,62);
        pharaoh_map_routing_update_citizen_rect(c->routing,x1,y1,x2,y2,1);
        pharaoh_map_routing_update_land_noncitizen_rect(c->routing->terrain,
            c->routing->building_ids,c->routing->buildings,c->routing->noncitizen,
            c->routing->geometry->grid_start,c->routing->geometry->width,
            c->routing->geometry->height,x1,y1,x2,y2);
        *c->map_changed=1;
    }
    return PHARAOH_PERCENTAGE_SCALE_VALUE;
}
