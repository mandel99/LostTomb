#include "pharaoh/map/rotated_buildings.h"
#include "pharaoh/map/surface_queries.h"
#include <stddef.h>

_Static_assert(sizeof(PharaohBuildingImageProperty)==20 &&
    offsetof(PharaohBuildingImageProperty,image_group)==8 &&
    offsetof(PharaohBuildingTypeRecord,venue_grid_offset)==0xd0 &&
    offsetof(PharaohBuildingTypeRecord,venue_orientation)==0xd4 &&
    offsetof(PharaohBuildingTypeRecord,venue_has_plaza)==0xd8,
    "native rotated building inputs");

static int32_t add(int32_t a,int32_t b)
{return (int32_t)((uint32_t)a+(uint32_t)b);}
static int32_t sub(int32_t a,int32_t b)
{return (int32_t)((uint32_t)a-(uint32_t)b);}
static int32_t image(PharaohRotatedBuildingContext *c,int32_t group)
{return (int32_t)c->surface->groups[group].first_image;}
static PharaohBuildingTypeRecord *building(PharaohRotatedBuildingContext *c,int32_t id)
{return &c->surface->buildings[id];}

static void square(PharaohRotatedBuildingContext *c,int32_t id,
    PharaohBuildingTypeRecord *b,int32_t sprite,uint32_t terrain)
{
    const uint8_t size=b->size;
    pharaoh_map_place_surface_square(c->surface,(uint16_t)id,b->x,b->y,
        size,size,(uint32_t)sprite,terrain);
}

static int32_t relative_direction(PharaohRotatedBuildingContext *c,
    const PharaohBuildingTypeRecord *b)
{
    int32_t d=sub(b->reserved_aa[0],*c->surface->orientation/2);
    if (d<0) d=add(d,4);
    return d;
}

static int32_t sphinx_image(PharaohRotatedBuildingContext *c,int32_t subtype)
{
    const int32_t view=*c->surface->orientation;
    const int32_t a=view==0 || view==6?564:566;
    const int32_t b=view==0 || view==6?566:564;
    int32_t result=image(c,subtype==0 || subtype==3?a:
        subtype==1 || subtype==4?565:b);
    if (view==0 || view==4) result=add(result,1);
    return result;
}

static void statue(PharaohRotatedBuildingContext *c,int32_t id,
    PharaohBuildingTypeRecord *b,int32_t kind)
{
    const uint8_t packed=b->reserved_aa[0];
    const int32_t variant=packed>>4;
    int32_t d=sub(sub(packed&15,*c->surface->orientation/2),1),result;
    if (d<0) d=add(d,4);
    if (variant<2) result=add(image(c,c->properties[kind].image_group),variant*4);
    else result=add(image(c,b->size==1?747:b->size==2?746:745),variant*4-8);
    square(c,id,b,add(result,d),8);
}

static void water_building(PharaohRotatedBuildingContext *c,int32_t id,
    PharaohBuildingTypeRecord *b,int32_t kind)
{
    const int32_t d=relative_direction(c,b);
    uint32_t sprite;
    if (kind==192) sprite=(uint32_t)add(image(c,524),d);
    else {
        const uint8_t changed=pharaoh_map_sweep_has_passed(*c->surface->sweep_countdown,
            *c->surface->sweep_start,b->x);
        int32_t group;
        switch (kind) {
        case 74:group=changed?723:523;break;
        case 76:group=changed?727:515;break;
        case 181:group=changed?725:514;break;
        case 182:group=changed?724:525;break;
        case 75:group=changed?729:546;break;
        default:group=changed?728:520;break;
        }
        sprite=(uint32_t)add(image(c,group),d);
    }
    pharaoh_map_bind_water_footprint(c->water_footprint,c->surface);
    if (kind==76 || kind==181 || kind==136)
        pharaoh_map_building_tiles_add_water_size2(c->water_footprint,(int16_t)id,b->x,b->y,sprite);
    else pharaoh_map_building_tiles_add_water_size3(c->water_footprint,(int16_t)id,b->x,b->y,sprite);
}

static void venue(PharaohRotatedBuildingContext *c,int32_t id,
    PharaohBuildingTypeRecord *b,int32_t kind)
{
    if (kind==30) {
        const int32_t next=b->next_part_building_id;
        if (building(c,next)->building_type==30)
            c->operations->bandstand_adjacent(c->operations->user_data,id,b->x,b->y);
    }
    if (pharaoh_building_follow_link(c->surface->buildings,id)==id && b->venue_has_plaza!=0) {
        pharaoh_map_bind_venue_tiles(c->venue,c->surface);
        pharaoh_map_add_venue_plaza_tiles(c->venue,b->venue_grid_offset,kind,(uint16_t)id);
    }
}

/* Complete native004EE560: scans all3999 non-sentinel records, independently
 * of highest_id, and reloads the variant/style after the child. */
void pharaoh_pyramids_update_all_images(PharaohRotatedBuildingContext *c)
{
    uint32_t id=1;
    do {
        if (building(c,(int32_t)id)->state!=0 && building(c,(int32_t)id)->building_type==183) {
            c->operations->pyramid(c->operations->user_data,(int32_t)id);
            const uint8_t variant=building(c,(int32_t)id)->pyramid_size_variant;
            const int32_t count=*c->pyramid_style==5?
                c->mastaba_part_counts[variant]:c->pyramid_part_counts[variant];
            id=id-1u+(uint32_t)count;
        }
        ++id;
    } while ((int32_t)id<4000);
}

/* Reused Rotation54 C; complete native0046D840 controller. */
void pharaoh_map_refresh_rotated_building_images(PharaohRotatedBuildingContext *c)
{
    const PharaohRotatedBuildingOperations *const ops=c->operations;
    for (int32_t id=1;id<=*c->highest_id;++id) {
        PharaohBuildingTypeRecord *const b=building(c,id);
        if (b->state==0) continue;
        const int32_t kind=b->building_type;
        if (kind>=140 && kind<=149) {
            square(c,id,b,image(c,c->properties[kind].image_group),8);continue;
        }
        if (kind>=41 && kind<=43) {statue(c,id,b,kind);continue;}
        int32_t result,index=-1;
        switch (kind) {
        case 58:case 200:case 170:case 201:case 56:break;
        case 171:case 202:ops->gatehouse(ops->user_data,id);break;
        case 210:square(c,id,b,sphinx_image(c,b->subtype),8);break;
        case 74:case 76:case 181:case 182:case 75:case 192:case 136:
            water_building(c,id,b,kind);break;
        case 90:
            pharaoh_map_update_water_lift_image(c->surface,c->water_footprint,c->water_lift,id);break;
        case 30:case 31:case 33:case 209:venue(c,id,b,kind);break;
        case 65:case 66:case 67:case 68:case 69:ops->temple_complex(ops->user_data,id,0);break;
        case 213:ops->sun_temple(ops->user_data,id);break;
        case 215:square(c,id,b,image(c,497),0x48);break;
        case 218:case 219:case 220:
            result=add(image(c,kind==218?760:kind==219?761:762),
                kind==220?*c->surface->orientation/2:(*c->surface->orientation/2)&1);
            if (result!=0) square(c,id,b,result,8);
            break;
        case 222:ops->mausoleum(ops->user_data,id);break;
        case 225:index=PHARAOH_ROTATED_LIBRARY;break;
        case 227:index=PHARAOH_ROTATED_CAESAREUM;break;
        case 228:index=PHARAOH_ROTATED_LIGHTHOUSE;break;
        case 230:index=PHARAOH_ROTATED_ABU_SIMBEL;break;
        case 229:index=PHARAOH_ROTATED_SMALL_TOMB;break;
        case 234:index=PHARAOH_ROTATED_MEDIUM_TOMB;break;
        case 235:index=PHARAOH_ROTATED_LARGE_TOMB;break;
        case 236:index=PHARAOH_ROTATED_GRAND_TOMB;break;
        }
        if (index>=0) ops->monument[index].draw(ops->monument[index].user_data,id);
    }
    pharaoh_pyramids_update_all_images(c);
}

static void refresh_rotated(void *context)
{pharaoh_map_refresh_rotated_building_images(context);}
void pharaoh_saved_game_bind_rotated_buildings_step(
    PharaohSavedGameInitializeOperations *ops,PharaohRotatedBuildingContext *c)
{
    ops->step[PHARAOH_INIT_ROTATED_BUILDINGS]=
        (PharaohSavedGameStepBinding){c,refresh_rotated};
}
