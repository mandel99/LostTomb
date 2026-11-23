#include "pharaoh/monuments/image_updates.h"
#include <stddef.h>

_Static_assert(sizeof(PharaohMonumentImagePart)==52 &&
    sizeof(PharaohSunTemplePartDefinition)==52 &&
    offsetof(PharaohSunTemplePartDefinition,size)==0 &&
    offsetof(PharaohMonumentImagePart,size)==8 &&
    offsetof(PharaohMonumentImagePart,kind)==12 &&
    offsetof(PharaohBuildingTypeRecord,monument_stage)==0x58 &&
    offsetof(PharaohBuildingTypeRecord,royal_tomb_chamber_kind)==0x68 &&
    offsetof(PharaohBuildingTypeRecord,royal_tomb_view)==0x90,
    "native monument image records");

static uint32_t group(const PharaohMonumentImageContext *c,int32_t id)
{return c->surface->groups[id].first_image;}
static uint32_t tile_origin(const PharaohMonumentImageContext *c,
    const PharaohBuildingTypeRecord *b)
{
    return (uint32_t)c->surface->map->origin+
        (uint32_t)(int32_t)b->y*228u+(uint32_t)(int32_t)b->x;
}
static uint32_t tile_delta(const PharaohMonumentImageContext *c,uint32_t row,uint32_t column)
{return (uint32_t)c->surface->footprints[row][column].grid_offset;}
static void square(PharaohMonumentImageContext *c,int32_t id,
    const PharaohBuildingTypeRecord *b,int32_t size,uint32_t image,uint32_t terrain)
{pharaoh_map_place_surface_square(c->surface,(uint16_t)id,b->x,b->y,size,size,image,terrain);}

/* Complete native004017A0; reused PartHelpers C with an explicit query owner. */
void pharaoh_monument_update_image_parts(PharaohMonumentImageContext *c,int32_t id)
{
    uint32_t flags;
    const uint32_t image=c->query_image(c->query_user_data?c->query_user_data:c->user_data,id,&flags);
    const PharaohBuildingTypeRecord *const b=&c->surface->buildings[id];
    square(c,id,b,c->parts[b->subtype].size,image,8);
}

/* Complete native004E69C0; reused MonumentMap C. */
void pharaoh_lighthouse_update_map(PharaohMonumentImageContext *c,int32_t id)
{
    uint32_t flags;
    const uint32_t image=c->query_image(c->query_user_data?c->query_user_data:c->user_data,id,&flags);
    PharaohMapSurfaceContext *const s=c->surface;
    const PharaohBuildingTypeRecord *const b=&s->buildings[id];
    const PharaohMonumentImagePart *const part=&c->parts[b->subtype];
    if (part->kind!=1) {square(c,id,b,part->size,image,8);return;}
    const uint32_t tile=tile_origin(c,b),terrain=s->map->terrain[tile];
    s->building_ids[tile]=(int16_t)id;
    s->map->terrain[tile]=terrain|0x10000000u;
}

static int32_t library_corner(int32_t part,uint32_t row,uint32_t column)
{
    switch (part) {
    case 13:return column==0 && row>=2;
    case 14:return column>=2 && row>=2;
    case 15:return column==0 && row==0;
    case 16:return column>=2 && row==0;
    default:return 1;
    }
}

/* Complete native0040DCA0; origin is live per cell, stage per row. */
void pharaoh_library_update_map(PharaohMonumentImageContext *c,int32_t id)
{
    uint32_t flags;
    const uint32_t image=c->query_image(c->query_user_data?c->query_user_data:c->user_data,id,&flags);
    PharaohMapSurfaceContext *const s=c->surface;
    const PharaohBuildingTypeRecord *const b=&s->buildings[id];
    const int32_t index=b->subtype,root=(int32_t)((uint32_t)id-(uint32_t)index);
    const int32_t size=c->parts[index].size;
    if ((flags&1u)==0) {square(c,id,b,size,image,8);return;}
    *s->current_tile=tile_origin(c,b);
    for (int32_t row=0;row<size;++row) {
        const int32_t stage=s->buildings[root].monument_stage;
        for (int32_t column=0;column<size;++column) {
            const uint32_t tile=*s->current_tile+tile_delta(c,(uint32_t)row,(uint32_t)column);
            if (stage<3) s->map->terrain[tile]|=0x10000000u;
            else s->map->terrain[tile]=8;
            s->property_flags[tile]&=0xe0;
            s->edge_flags[tile]|=0x40;
            s->building_ids[tile]=(int16_t)id;
            if ((flags&2u)!=0 && image==group(c,769) &&
                !library_corner(index,(uint32_t)row,(uint32_t)column)) continue;
            if (image!=0) s->image_ids[tile]=image==group(c,500)?
                image+(s->random[tile]&7u):image;
        }
    }
}

/* Complete native00541920; reused SunTemple image controller. */
void pharaoh_sun_temple_update_map(PharaohMonumentImageContext *c,int32_t id)
{
    uint32_t flags;
    const uint32_t image=c->query_image(c->query_user_data?c->query_user_data:c->user_data,id,&flags);
    PharaohMapSurfaceContext *const s=c->surface;
    const PharaohBuildingTypeRecord *const b=&s->buildings[id];
    const int32_t size=c->sun_parts[b->subtype].size;
    if ((flags&1u)==0) {
        square(c,id,b,size,image,b->subtype==0?8u:0x10000008u);return;
    }
    *s->current_tile=tile_origin(c,b);
    for (int32_t row=0;row<size;++row) {
        for (int32_t column=0;column<size;++column) {
            const uint32_t tile=*s->current_tile+tile_delta(c,(uint32_t)row,(uint32_t)column);
            s->map->terrain[tile]|=0x10000000u;
            s->building_ids[tile]=(int16_t)id;
            s->property_flags[tile]&=0xe0;
            s->edge_flags[tile]|=0x40;
            if (((flags&2u)==0 || ((column==0 || column==size-1) &&
                    (row==0 || row==size-1))) && image!=0)
                s->image_ids[tile]=image==group(c,500)?image+(s->random[tile]&7u):image;
        }
    }
}

static const uint8_t tomb_shapes[8][4][16]={
 {{2,1,7},{2,6,7},{2,0,6},{2,0,1}},
 {{6,0,1,2,6,7,8},{6,1,2,7,8,13,14},{6,6,7,8,12,13,14},{6,0,1,6,7,12,13}},
 {{3,0,1,2},{3,2,8,14},{3,12,13,14},{3,0,6,12}},
 {{3,2,8,14},{3,12,13,14},{3,0,6,12},{3,0,1,2}},
 {{10,3,4,9,10,15,16,21,22,27,28},{10,18,19,20,21,22,24,25,26,27,28},
  {10,0,6,12,18,24,1,7,13,19,25},{10,0,1,2,3,4,6,7,8,9,10}},
 {{15,0,1,2,3,4,6,7,8,9,10,12,13,14,15,16},{15,2,8,14,20,26,3,9,15,21,27,4,10,16,22,28},
  {15,12,13,14,15,16,18,19,20,21,22,24,25,26,27,28},{15,0,6,12,18,24,1,7,13,19,25,2,8,14,20,26}},
 {{2,0,14},{2,12,14},{2,2,14},{2,0,2}},
 {{10,0,1,2,3,4,6,7,8,9,10},{10,3,9,15,21,27,4,10,16,22,28},
  {10,18,19,20,21,22,24,25,26,27,28},{10,0,6,12,18,24,1,7,13,19,25}}
};

/* Complete native005468D0; reused BuildingManagerMonumentBindings C.
 * DWORD58 is the completed overlay for these building kinds. */
void pharaoh_royal_tomb_update_map(PharaohMonumentImageContext *c,int32_t id)
{
    PharaohMapSurfaceContext *const s=c->surface;
    const PharaohBuildingTypeRecord *const b=&s->buildings[id];
    const int32_t root=(int32_t)((uint32_t)id-(uint32_t)(int32_t)b->subtype);
    if (b->subtype==0) *c->idle=c->workers_idle(c->user_data,id);
    const uint32_t size=b->size;
    if (s->buildings[root].monument_stage==1 && *c->city_overlay!=51) {
        const uint32_t base=tile_origin(c,b);
        if (b->subtype==*c->last_part) {
            const uint32_t tile=base+tile_delta(c,0,0);
            s->property_flags[tile]&=0xe0;
            const uint32_t image=(s->random[tile]&7u)+group(c,500);
            s->edge_flags[tile]|=0x40;s->image_ids[tile]=image;return;
        }
        for (uint32_t row=0;row<size;++row) for (uint32_t column=0;column<size;++column) {
            const uint32_t tile=base+tile_delta(c,row,column);
            c->rock->orientation=*s->orientation;
            const uint32_t image=(uint32_t)pharaoh_map_royal_tomb_rock_variant(c->rock,tile);
            s->property_flags[tile]&=0xe0;
            const uint32_t result=image+group(c,743);
            s->edge_flags[tile]|=0x40;s->image_ids[tile]=result;
        }
        return;
    }
    uint32_t flags;
    const uint32_t image=c->query_image(c->query_user_data?c->query_user_data:c->user_data,id,&flags);
    if ((flags&1u)!=0) {
        const uint32_t base=tile_origin(c,b);
        for (uint32_t row=0;row<size;++row) for (uint32_t column=0;column<size;++column) {
            const uint32_t tile=base+tile_delta(c,row,column);
            s->map->terrain[tile]=8;s->building_ids[tile]=(int16_t)id;
            s->property_flags[tile]&=0xe0;s->edge_flags[tile]|=0x40;
            if (image!=0) s->image_ids[tile]=image==group(c,500)?image+(s->random[tile]&7u):image;
        }
        return;
    }
    square(c,id,b,(int32_t)size,image,0x40000000u);
    const uint32_t base=tile_origin(c,b);
    const int32_t shape=b->royal_tomb_chamber_kind;
    if (shape==27 || shape==28) {s->map->terrain[base+tile_delta(c,0,0)]=8;return;}
    int32_t pattern;
    switch (shape) {
    case 1:case 29:pattern=0;break;
    case 2:pattern=1;break;
    case 9:case 10:case 13:case 14:pattern=2;break;
    case 11:pattern=3;break;
    case 19:pattern=4;break;
    case 22:pattern=5;break;
    case 24:pattern=6;break;
    case 26:pattern=7;break;
    default:return;
    }
    const uint32_t view=b->royal_tomb_view;
    if (view>3) return;
    const uint8_t *const row=tomb_shapes[pattern][view];
    for (uint32_t i=1;i<=row[0];++i) {
        const uint32_t index=row[i];
        s->map->terrain[base+tile_delta(c,index/6,index%6)]=8;
    }
}

static void draw_parts(void *c,int32_t id){pharaoh_monument_update_image_parts(c,id);}
static void draw_library(void *c,int32_t id){pharaoh_library_update_map(c,id);}
static void draw_lighthouse(void *c,int32_t id){pharaoh_lighthouse_update_map(c,id);}
static void draw_sun(void *c,int32_t id){pharaoh_sun_temple_update_map(c,id);}
static void draw_tomb(void *c,int32_t id){pharaoh_royal_tomb_update_map(c,id);}
PharaohBuildingImageBinding pharaoh_monument_image_binding(
    enum PharaohMonumentImageMethod method,PharaohMonumentImageContext *context)
{
    static void (*const draw[5])(void *,int32_t)={draw_parts,draw_library,draw_lighthouse,draw_sun,draw_tomb};
    return (PharaohBuildingImageBinding){context,draw[method]};
}

void pharaoh_rotated_bind_monument_images(PharaohRotatedBuildingOperations *ops,
    PharaohMonumentImageContext *const contexts[PHARAOH_ROTATED_MONUMENT_COUNT])
{
    static const enum PharaohMonumentImageMethod methods[PHARAOH_ROTATED_MONUMENT_COUNT]={
        PHARAOH_IMAGE_LIBRARY,PHARAOH_IMAGE_PARTS,PHARAOH_IMAGE_LIGHTHOUSE,
        PHARAOH_IMAGE_PARTS,PHARAOH_IMAGE_ROYAL_TOMB,PHARAOH_IMAGE_ROYAL_TOMB,
        PHARAOH_IMAGE_ROYAL_TOMB,PHARAOH_IMAGE_ROYAL_TOMB
    };
    for (uint32_t i=0;i<PHARAOH_ROTATED_MONUMENT_COUNT;++i)
        ops->monument[i]=pharaoh_monument_image_binding(methods[i],contexts[i]);
}

void pharaoh_rotated_bind_sun_temple_image(PharaohRotatedBuildingOperations *ops,
    PharaohMonumentImageContext *context)
{
    ops->sun_temple=draw_sun;
    ops->sun_temple_user_data=context;
}
