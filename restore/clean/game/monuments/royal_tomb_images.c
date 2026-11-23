#include "pharaoh/monuments/royal_tomb_images.h"
#include "pharaoh/monuments/stages.h"
#include <stddef.h>

_Static_assert(offsetof(PharaohBuildingTypeRecord,royal_tomb_progress)==0x60 &&
    offsetof(PharaohBuildingTypeRecord,royal_tomb_completed_image)==0x94 &&
    offsetof(PharaohBuildingTypeRecord,royal_tomb_closed_image)==0x9c,
    "native royal-tomb image overlays");

/* Completed old MonumentComplexImages C, exact SG3 groups and shape rules.
 * Receiver construction remains separate; no address macros or IL boundary. */
enum TombSpriteGroup {
    TombSpriteNone = -1,
    TombSmallKind0WorkingGroup = 791,
    TombSmallKind0IdleGroup = 792,
    TombSmallKind1WorkingGroup = 793,
    TombSmallKind1IdleGroup = 794,
    TombSmallKind2WorkingGroup = 795,
    TombSmallKind2IdleGroup = 796,
    TombSmallKind3WorkingGroup = 797,
    TombSmallKind3IdleGroup = 798,
    TombSmallKind6WorkingGroup = 799,
    TombSmallKind6IdleGroup = 800,
    TombSmallKind15WorkingGroup = 801,
    TombSmallKind15IdleGroup = 802,
    TombSmallKind16WorkingGroup = 803,
    TombSmallKind16IdleGroup = 804,
    TombSmallKind18WorkingGroup = 807,
    TombSmallKind27WorkingGroup = 808,
    TombSmallKind28WorkingGroup = 809,
    TombSmallKind29WorkingGroup = 810,
    TombSmallKind29IdleGroup = 811,
    TombMediumKind0WorkingGroup = 828,
    TombMediumKind0IdleGroup = 829,
    TombMediumKind1WorkingGroup = 830,
    TombMediumKind1IdleGroup = 831,
    TombMediumKind2WorkingGroup = 832,
    TombMediumKind2IdleGroup = 833,
    TombMediumKind9WorkingGroup = 834,
    TombMediumKind9IdleGroup = 835,
    TombMediumKind10WorkingGroup = 836,
    TombMediumKind10IdleGroup = 837,
    TombMediumKind19WorkingGroup = 838,
    TombMediumKind19IdleGroup = 839,
    TombMediumKind20WorkingGroup = 840,
    TombMediumKind20IdleGroup = 841,
    TombMediumKind21WorkingGroup = 842,
    TombMediumKind21IdleGroup = 843,
    TombMediumKind17WorkingGroup = 844,
    TombMediumKind18WorkingGroup = 846,
    TombMediumKind27WorkingGroup = 847,
    TombMediumKind28WorkingGroup = 848,
    TombMediumKind29WorkingGroup = 849,
    TombMediumKind29IdleGroup = 850,
    TombLargeKind0WorkingGroup = 851,
    TombLargeKind0IdleGroup = 852,
    TombLargeKind1WorkingGroup = 853,
    TombLargeKind1IdleGroup = 854,
    TombLargeKind2WorkingGroup = 855,
    TombLargeKind2IdleGroup = 856,
    TombLargeKind4WorkingGroup = 857,
    TombLargeKind4IdleGroup = 858,
    TombLargeKind5WorkingGroup = 859,
    TombLargeKind5IdleGroup = 860,
    TombLargeKind6WorkingGroup = 861,
    TombLargeKind6IdleGroup = 862,
    TombLargeKind7WorkingGroup = 863,
    TombLargeKind7IdleGroup = 864,
    TombLargeKind8WorkingGroup = 865,
    TombLargeKind8IdleGroup = 866,
    TombLargeKind12WorkingGroup = 867,
    TombLargeKind12IdleGroup = 868,
    TombLargeKind22WorkingGroup = 869,
    TombLargeKind22IdleGroup = 870,
    TombLargeKind23WorkingGroup = 871,
    TombLargeKind23IdleGroup = 872,
    TombLargeKind17WorkingGroup = 873,
    TombLargeKind18WorkingGroup = 875,
    TombLargeKind27WorkingGroup = 876,
    TombLargeKind28WorkingGroup = 877,
    TombLargeKind29WorkingGroup = 878,
    TombLargeKind29IdleGroup = 879,
    TombGrandKind0WorkingGroup = 880,
    TombGrandKind0IdleGroup = 881,
    TombGrandKind1WorkingGroup = 882,
    TombGrandKind1IdleGroup = 883,
    TombGrandKind2WorkingGroup = 884,
    TombGrandKind2IdleGroup = 885,
    TombGrandKind4WorkingGroup = 886,
    TombGrandKind4IdleGroup = 887,
    TombGrandKind6WorkingGroup = 888,
    TombGrandKind6IdleGroup = 889,
    TombGrandKind7WorkingGroup = 890,
    TombGrandKind7IdleGroup = 891,
    TombGrandKind8WorkingGroup = 892,
    TombGrandKind8IdleGroup = 893,
    TombGrandKind11WorkingGroup = 894,
    TombGrandKind11IdleGroup = 895,
    TombGrandKind12WorkingGroup = 896,
    TombGrandKind12IdleGroup = 897,
    TombGrandKind13WorkingGroup = 898,
    TombGrandKind13IdleGroup = 899,
    TombGrandKind14WorkingGroup = 900,
    TombGrandKind14IdleGroup = 901,
    TombGrandKind24WorkingGroup = 902,
    TombGrandKind24IdleGroup = 903,
    TombGrandKind25WorkingGroup = 904,
    TombGrandKind25IdleGroup = 905,
    TombGrandKind26WorkingGroup = 906,
    TombGrandKind26IdleGroup = 907,
    TombGrandKind17WorkingGroup = 908,
    TombGrandKind18WorkingGroup = 910,
    TombGrandKind27WorkingGroup = 911,
    TombGrandKind28WorkingGroup = 912,
    TombGrandKind29WorkingGroup = 913,
    TombGrandKind29IdleGroup = 914,
};
struct ImageRule {int32_t cold,hot;uint8_t coldPhase,coldOrientation,hotPhase,hotOrientation;};
static const struct ImageRule rules[]={
 {TombSmallKind0WorkingGroup,TombSmallKind0IdleGroup,1,1,0,1},
 {TombSmallKind1WorkingGroup,TombSmallKind1IdleGroup,1,1,0,1},
 {TombSmallKind2WorkingGroup,TombSmallKind2IdleGroup,1,1,0,1},
 {TombSmallKind3WorkingGroup,TombSmallKind3IdleGroup,1,1,0,1},
 {TombSpriteNone,TombSpriteNone,0,0,0,0},
 {TombSmallKind6WorkingGroup,TombSmallKind6IdleGroup,1,1,0,1},
 {TombSmallKind15WorkingGroup,TombSmallKind15IdleGroup,1,1,0,1},
 {TombSmallKind16WorkingGroup,TombSmallKind16IdleGroup,1,1,0,1},
 {805,TombSpriteNone,1,0,0,0},
 {TombSmallKind18WorkingGroup,TombSpriteNone,0,1,0,0},
 {TombSmallKind27WorkingGroup,TombSpriteNone,0,1,0,0},
 {TombSmallKind28WorkingGroup,TombSpriteNone,0,1,0,0},
 {TombSmallKind29WorkingGroup,TombSmallKind29IdleGroup,1,1,0,1},
 {TombMediumKind0WorkingGroup,TombMediumKind0IdleGroup,1,1,0,1},
 {TombMediumKind1WorkingGroup,TombMediumKind1IdleGroup,1,1,0,1},
 {TombMediumKind2WorkingGroup,TombMediumKind2IdleGroup,1,1,0,1},
 {TombMediumKind9WorkingGroup,TombMediumKind9IdleGroup,1,1,0,1},
 {TombMediumKind10WorkingGroup,TombMediumKind10IdleGroup,1,1,0,1},
 {TombMediumKind17WorkingGroup,TombSpriteNone,0,0,0,0},
 {TombMediumKind18WorkingGroup,TombSpriteNone,0,1,0,0},
 {TombMediumKind19WorkingGroup,TombMediumKind19IdleGroup,1,1,0,1},
 {TombMediumKind20WorkingGroup,TombMediumKind20IdleGroup,1,1,0,1},
 {TombMediumKind21WorkingGroup,TombMediumKind21IdleGroup,1,1,0,1},
 {TombMediumKind27WorkingGroup,TombSpriteNone,0,1,0,0},
 {TombMediumKind28WorkingGroup,TombSpriteNone,0,1,0,0},
 {TombMediumKind29WorkingGroup,TombMediumKind29IdleGroup,1,1,0,1},
 {TombLargeKind0WorkingGroup,TombLargeKind0IdleGroup,1,1,0,1},
 {TombLargeKind1WorkingGroup,TombLargeKind1IdleGroup,1,1,0,1},
 {TombLargeKind2WorkingGroup,TombLargeKind2IdleGroup,1,1,0,1},
 {TombLargeKind4WorkingGroup,TombLargeKind4IdleGroup,1,1,0,1},
 {TombLargeKind5WorkingGroup,TombLargeKind5IdleGroup,1,1,0,1},
 {TombLargeKind6WorkingGroup,TombLargeKind6IdleGroup,1,1,0,1},
 {TombLargeKind7WorkingGroup,TombLargeKind7IdleGroup,1,1,0,1},
 {TombLargeKind8WorkingGroup,TombLargeKind8IdleGroup,1,1,0,1},
 {TombLargeKind12WorkingGroup,TombLargeKind12IdleGroup,1,1,0,1},
 {TombLargeKind17WorkingGroup,TombSpriteNone,0,0,0,0},
 {TombLargeKind18WorkingGroup,TombSpriteNone,0,1,0,0},
 {TombLargeKind22WorkingGroup,TombLargeKind22IdleGroup,1,1,0,1},
 {TombLargeKind23WorkingGroup,TombLargeKind23IdleGroup,1,1,0,1},
 {TombLargeKind27WorkingGroup,TombSpriteNone,0,1,0,0},
 {TombLargeKind28WorkingGroup,TombSpriteNone,0,1,0,0},
 {TombLargeKind29WorkingGroup,TombLargeKind29IdleGroup,1,1,0,1},
 {TombGrandKind0WorkingGroup,TombGrandKind0IdleGroup,1,1,0,1},
 {TombGrandKind1WorkingGroup,TombGrandKind1IdleGroup,1,1,0,1},
 {TombGrandKind2WorkingGroup,TombGrandKind2IdleGroup,1,1,0,1},
 {TombGrandKind4WorkingGroup,TombGrandKind4IdleGroup,1,1,0,1},
 {TombGrandKind6WorkingGroup,TombGrandKind6IdleGroup,1,1,0,1},
 {TombGrandKind7WorkingGroup,TombGrandKind7IdleGroup,1,1,0,1},
 {TombGrandKind8WorkingGroup,TombGrandKind8IdleGroup,1,1,0,1},
 {TombGrandKind11WorkingGroup,TombGrandKind11IdleGroup,1,1,0,1},
 {TombGrandKind12WorkingGroup,TombGrandKind12IdleGroup,1,1,0,1},
 {TombGrandKind13WorkingGroup,TombGrandKind13IdleGroup,1,1,0,1},
 {TombGrandKind14WorkingGroup,TombGrandKind14IdleGroup,1,1,0,1},
 {TombGrandKind17WorkingGroup,TombSpriteNone,0,0,0,0},
 {TombGrandKind18WorkingGroup,TombSpriteNone,0,1,0,0},
 {TombGrandKind24WorkingGroup,TombGrandKind24IdleGroup,1,1,0,1},
 {TombGrandKind25WorkingGroup,TombGrandKind25IdleGroup,1,1,0,1},
 {TombGrandKind26WorkingGroup,TombGrandKind26IdleGroup,1,1,0,1},
 {TombGrandKind27WorkingGroup,TombSpriteNone,0,1,0,0},
 {TombGrandKind28WorkingGroup,TombSpriteNone,0,1,0,0},
 {TombGrandKind29WorkingGroup,TombGrandKind29IdleGroup,1,1,0,1},
};
static const uint8_t smallShapes[]={0,1,2,3,4,4,5,4,4,4,4,4,4,4,4,6,7,8,9,4,4,4,4,4,4,4,4,10,11,12};
static const uint8_t mediumShapes[]={13,14,15,4,4,4,4,4,4,16,17,4,4,4,4,4,4,18,19,20,21,22,4,4,4,4,4,23,24,25};
static const uint8_t largeShapes[]={26,27,28,4,29,30,31,32,33,4,4,4,34,4,4,4,4,35,36,4,4,4,37,38,4,4,4,39,40,41};
static const uint8_t grandShapes[]={42,43,44,4,45,4,46,47,48,4,4,49,50,51,52,4,4,53,54,4,4,4,4,4,55,56,57,58,59,60};



int32_t pharaoh_small_royal_tomb_chamber_count(void)
{
    return 9;
}
int32_t pharaoh_medium_royal_tomb_chamber_count(void)
{
    return 10;
}
int32_t pharaoh_large_royal_tomb_chamber_count(void)
{
    return 21;
}
int32_t pharaoh_grand_royal_tomb_chamber_count(void)
{
    return 31;
}

static int32_t chambers(enum PharaohRoyalTombImageClass kind)
{
    static int32_t (*const counts[4])(void)={
        pharaoh_small_royal_tomb_chamber_count,pharaoh_medium_royal_tomb_chamber_count,
        pharaoh_large_royal_tomb_chamber_count,pharaoh_grand_royal_tomb_chamber_count};
    return counts[kind]();
}
static uint32_t query(PharaohRoyalTombImageContext *c,
    enum PharaohRoyalTombImageClass kind,int32_t building_id,uint32_t *flags)
{
    static const uint8_t *const shapes[4]={smallShapes,mediumShapes,largeShapes,grandShapes};
    volatile PharaohBuildingTypeRecord *const b=&c->surface->buildings[building_id];
    uint32_t stage,shape,relative,phase_delta;
    int32_t group;
    const struct ImageRule *rule;
    uint8_t phase,orientation;
    *flags=0;
    stage=(uint32_t)b->royal_tomb_progress;
    if (!stage) {
        shape=(uint32_t)b->royal_tomb_chamber_kind;
        if (shape!=18 && shape!=27 && shape!=28) {
            *flags=1;
            return (uint32_t)b->royal_tomb_closed_image+c->surface->groups[805].first_image;
        }
    }
    relative=b->royal_tomb_view-(uint32_t)(*c->surface->orientation/2);
    if ((int32_t)relative<0) relative+=4u;
    phase_delta=stage==3?4u:stage==4?8u:stage==5?12u:0u;
    if (kind!=PHARAOH_TOMB_SMALL && b->subtype==*c->last_part) phase_delta=0;
    if (pharaoh_monument_resources_complete_thunk(c->stored_resources,c->required_resources) &&
            b->royal_tomb_completed_image) {
        PharaohBaseMonumentProgress progress={c->surface->buildings,c->walkers,0};
        if (pharaoh_royal_tomb_progress(&progress,building_id,chambers(kind),NULL)==100) {
            phase_delta+=4u;goto select_image;
        }
    }
    if ((uint16_t)b->runtime_value_016==UINT16_MAX && b->royal_tomb_completed_image) phase_delta+=4u;
select_image:
    shape=(uint32_t)b->royal_tomb_chamber_kind;
    if (shape>29) return 0;
    rule=&rules[shapes[kind][shape]];
    group=rule->cold;phase=rule->coldPhase;orientation=rule->coldOrientation;
    if (rule->hot!=TombSpriteNone && *c->idle) {
        group=rule->hot;phase=rule->hotPhase;orientation=rule->hotOrientation;
    }
    if (group==TombSpriteNone) return 0;
    return c->surface->groups[group].first_image+phase_delta*phase+relative*orientation;
}
#define IMAGE(name,kind) \
uint32_t pharaoh_##name##_royal_tomb_query_image(PharaohRoyalTombImageContext *c, \
    int32_t id,uint32_t *flags){return query(c,kind,id,flags);} \
static uint32_t name##_query(void *c,int32_t id,uint32_t *flags) \
{return pharaoh_##name##_royal_tomb_query_image(c,id,flags);}
IMAGE(small,PHARAOH_TOMB_SMALL)
IMAGE(medium,PHARAOH_TOMB_MEDIUM)
IMAGE(large,PHARAOH_TOMB_LARGE)
IMAGE(grand,PHARAOH_TOMB_GRAND)
#undef IMAGE
static uint8_t has_worker(void *context,int32_t building)
{
    PharaohRoyalTombImageContext *const c=context;
    return pharaoh_royal_tomb_worker_active(c->surface->buildings,c->walkers,building);
}
#define IDLE(name) \
static int32_t name##_chambers(void *c){(void)c;return pharaoh_##name##_royal_tomb_chamber_count();} \
static uint8_t name##_idle(void *context,int32_t id) \
{PharaohRoyalTombImageContext *const c=context;return (uint8_t)pharaoh_royal_tomb_is_idle( \
    c,c->surface->buildings,id,name##_chambers,has_worker);}
IDLE(small)
IDLE(medium)
IDLE(large)
IDLE(grand)
#undef IDLE
void pharaoh_royal_tomb_bind_image_query(PharaohMonumentImageContext *images,
    enum PharaohRoyalTombImageClass kind,PharaohRoyalTombImageContext *context)
{
    static uint32_t (*const queries[4])(void *,int32_t,uint32_t *)={
        small_query,medium_query,large_query,grand_query};
    static uint8_t (*const idles[4])(void *,int32_t)={small_idle,medium_idle,large_idle,grand_idle};
    images->query_image=queries[kind];images->query_user_data=context;
    images->workers_idle=idles[kind];images->idle_user_data=context;
}
void pharaoh_rotated_bind_royal_tomb_images(PharaohRotatedBuildingOperations *ops,
    PharaohMonumentImageContext *const images[4],
    PharaohRoyalTombImageContext *const queries[4])
{
    for (int32_t kind=0;kind<4;++kind) {
        pharaoh_royal_tomb_bind_image_query(images[kind],
            (enum PharaohRoyalTombImageClass)kind,queries[kind]);
        ops->monument[PHARAOH_ROTATED_SMALL_TOMB+kind]=
            pharaoh_monument_image_binding(PHARAOH_IMAGE_ROYAL_TOMB,images[kind]);
    }
}
