#include "pharaoh/building/venue_images.h"
#include "pharaoh/map/rotated_buildings.h"
#include <stddef.h>

_Static_assert(offsetof(PharaohBuildingTypeRecord,venue_orientation)==0xd4 &&
    offsetof(PharaohBuildingTypeRecord,next_part_building_id)==0x3a,
    "native adjacent venue fields");

/* Complete native004405C0; reuse prior orientation table. The image bank
 * remains live between the second-output and first-output writes. */
void pharaoh_venue_orientation_images(int32_t view_orientation,
    const uint32_t *base_image, int32_t which,
    int32_t *main_image, int32_t *next_image)
{
    static const uint8_t offsets[4][2][2] = {
        {{1, 0}, {2, 3}},
        {{2, 3}, {0, 1}},
        {{0, 1}, {3, 2}},
        {{3, 2}, {1, 0}}
    };
    uint32_t view;
    if ((uint32_t)which > 1u || (uint32_t)view_orientation > 6u ||
        (view_orientation & 1) != 0) return;
    view = (uint32_t)view_orientation / 2u;
    *next_image = (int32_t)(*base_image + offsets[view][which][1]);
    *main_image = (int32_t)(*base_image + offsets[view][which][0]);
}

/* Reused BridgePlace C. Native00440540 caches direction and linked ID before
 * querying images. Unsupported rules leave its second local undefined; this
 * owner retains the native valid-rule precondition instead of inventing an
 * extra input for stack garbage. */
void pharaoh_map_publish_venue_adjacent_images(PharaohMapSurfaceContext *s,
    int32_t id,int32_t x,int32_t y)
{
    const int32_t direction=s->buildings[id].venue_orientation;
    const int32_t linked=s->buildings[id].next_part_building_id;
    int32_t first=id,second;
    pharaoh_venue_orientation_images(*s->orientation,&s->groups[361].first_image,
        direction,&first,&second);
    const int32_t next_x=direction==0?x:(int32_t)((uint32_t)x+1u);
    const int32_t next_y=direction==0?(int32_t)((uint32_t)y+1u):y;
    pharaoh_map_place_surface_square(s,(uint16_t)id,x,y,1,1,(uint32_t)first,8);
    pharaoh_map_place_surface_square(s,(uint16_t)linked,next_x,next_y,1,1,(uint32_t)second,8);
}

static void publish_adjacent(void *data,int32_t id,int32_t x,int32_t y)
{pharaoh_map_publish_venue_adjacent_images(data,id,x,y);}
void pharaoh_rotated_bind_venue_adjacent_images(PharaohRotatedBuildingOperations *ops,
    PharaohMapSurfaceContext *s)
{ops->bandstand_adjacent=publish_adjacent;ops->bandstand_user_data=s;}
