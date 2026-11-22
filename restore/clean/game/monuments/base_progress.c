#include "pharaoh/monuments/base_progress.h"
#include "pharaoh/core/x87_numeric.h"

_Static_assert(offsetof(PharaohBuildingTypeRecord,abu_simbel_stage)==0x54 &&
    offsetof(PharaohBuildingTypeRecord,royal_tomb_progress)==0x60 &&
    offsetof(PharaohBuildingTypeRecord,royal_tomb_stock)==0x98,
    "native monument stage overlays");

static uint8_t monument_worker_active(PharaohBuildingTypeRecord *buildings,
    const PharaohWalkerQueryRecord *walkers, int32_t building_id,
    uint8_t mason_action_1, uint8_t mason_action_2,
    uint8_t mason_action_3, int32_t adjust_to_main)
{
    int32_t walker_id;
    uint8_t action;
    uint8_t type;
    if (adjust_to_main && buildings[building_id].monument_stage > 2)
        building_id -= buildings[building_id].subtype;
    walker_id = buildings[building_id].monument_worker_id;
    if (walker_id == 0) return 0;
    if (walkers[walker_id].state == 0) {
        buildings[building_id].monument_worker_id = 0;
        return 0;
    }
    action = walkers[walker_id].action_state;
    if (action == 6u || action == 7u)
        action = walkers[walker_id].action_state_before_6_7;
    type = walkers[walker_id].type;
    if (type == 79u) return action == 9u || action == 10u;
    if (type == 81u) return action == 9u || action == 11u || action == 10u;
    if (type == 85u)
        return action == 15u || action == 16u ||
            action == mason_action_1 ||
            (mason_action_2 != 0u && action == mason_action_2) ||
            (mason_action_3 != 0u && action == mason_action_3);
    buildings[building_id].monument_worker_id = 0;
    return 0;
}

/* Complete logical-byte C translation of Pharaoh.exe 0x0040EC20. */
uint8_t pharaoh_library_monument_worker_active(
    PharaohBuildingTypeRecord *buildings,
    const PharaohWalkerQueryRecord *walkers, int32_t building_id)
{
    return monument_worker_active(
        buildings, walkers, building_id, 31u, 32u, 33u, 1);
}

/* Complete logical-byte C translation of Pharaoh.exe 0x00417590. */
uint8_t pharaoh_caesareum_monument_worker_active(
    PharaohBuildingTypeRecord *buildings,
    const PharaohWalkerQueryRecord *walkers, int32_t building_id)
{
    return monument_worker_active(
        buildings, walkers, building_id, 34u, 0u, 0u, 0);
}

/* Complete logical-byte zero-callee C translation of Pharaoh.exe 0x004E6B20. */
uint8_t pharaoh_lighthouse_monument_worker_active(
    PharaohBuildingTypeRecord *buildings,
    const PharaohWalkerQueryRecord *walkers, int32_t building_id)
{
    return monument_worker_active(
        buildings, walkers, building_id, 35u, 0u, 0u, 0);
}

typedef uint8_t (*WorkerPredicate)(PharaohBuildingTypeRecord *,
    const PharaohWalkerQueryRecord *,int32_t);

/* Native immutable vtable slot24h targets are established in Ghidra:
 * library40EC20, caesareum417590, lighthouse4E6B20. Direct typed calls keep
 * their real worker algorithms; no token interpreter or callback stub. */
static int count_worker(PharaohBaseMonumentProgress *c,int32_t root,
    WorkerPredicate active,uint32_t *q,uint32_t *q10,uint32_t *u,uint32_t *o)
{
    if (active(c->buildings,c->walkers,root)) {
        const int32_t worker=c->buildings[root].monument_worker_id;
        const PharaohWalkerQueryRecord *w;
        if (!worker) return 0;
        w=&c->walkers[worker];
        if (w->type==79) *o=1;
        else if (w->type==81) {*q=1;if (w->action_state==10) *q10=1;}
        else if (w->type==85) *u=1;
    }
    return 1;
}

static uint32_t positive_cost(uint32_t value)
{
    return (value&0x80000000u)?0u:value;
}

/* Complete prior MonumentBaseQueries C reuse, native0040D940. */
int32_t pharaoh_library_progress(PharaohBaseMonumentProgress *c,
    int32_t building,int32_t output[14])
{
    const int32_t root=building-c->buildings[building].subtype;
    const uint32_t stage=(uint32_t)c->buildings[root].monument_stage;
    const uint32_t percent=stage==21?100u:(uint32_t)pharaoh_x87_scaled_progress(
        (int32_t)stage,&pharaoh_library_progress_step);
    uint32_t cost=14400,q=0,q10=0,u=0,o=0;
    if (!count_worker(c,root,pharaoh_library_monument_worker_active,&q,&q10,&u,&o)) return 0;
    if ((int32_t)stage>3) cost=positive_cost((36u-(stage-3u)*c->parts)*400u);
    if (output) {
        uint32_t *out=(uint32_t *)output;
        out[11]=cost;out[8]=q10;out[9]=u;out[7]=q;
        out[0]=stage;out[10]=o;out[12]=percent;out[13]=14400;
    }
    return (int32_t)percent;
}

/* Complete prior MonumentBaseQueries C reuse, native00417050. */
int32_t pharaoh_caesareum_progress(PharaohBaseMonumentProgress *c,
    int32_t building,int32_t output[8])
{
    const int32_t root=building-c->buildings[building].subtype;
    const uint32_t stage=(uint32_t)c->buildings[root].monument_stage;
    const uint32_t percent=stage==19?100u:stage*5u+5u;
    const uint32_t cost=positive_cost((38u-c->parts*stage)*400u);
    uint32_t q=0,q10=0,u=0,o=0;
    if (!count_worker(c,root,pharaoh_caesareum_monument_worker_active,&q,&q10,&u,&o)) return 0;
    if (output) {
        uint32_t *out=(uint32_t *)output;
        out[0]=stage;out[2]=q10;out[3]=u;out[5]=cost;
        out[1]=q;out[4]=o;out[6]=percent;out[7]=15200;
    }
    return (int32_t)percent;
}

/* Complete prior MonumentBaseQueries C reuse, native004E6710. */
int32_t pharaoh_lighthouse_progress(PharaohBaseMonumentProgress *c,
    int32_t building,int32_t output[8])
{
    const int32_t root=building-c->buildings[building].subtype;
    const uint32_t stage=(uint32_t)c->buildings[root].monument_stage;
    const uint32_t percent=stage==9?100u:(stage*5u+5u)*2u;
    const uint32_t cost=positive_cost((9u-stage)*c->parts*400u);
    uint32_t q=0,q10=0,u=0,o=0;
    if (!count_worker(c,root,pharaoh_lighthouse_monument_worker_active,&q,&q10,&u,&o)) return 0;
    if (output) {
        uint32_t *out=(uint32_t *)output;
        out[0]=stage;out[1]=q;out[2]=q10;out[3]=u;
        out[4]=o;out[5]=cost;out[6]=percent;out[7]=c->parts*3600u;
    }
    return (int32_t)percent;
}

/* Complete prior CampaignScoreNumeric C reuse, native00401540. */
int32_t pharaoh_abu_simbel_progress(const PharaohBuildingTypeRecord *buildings,
    int32_t building,int32_t output[4])
{
    const int32_t root=building-buildings[building].subtype;
    const int32_t stage=buildings[root].abu_simbel_stage;
    const int32_t percent=stage==56?100:pharaoh_x87_scaled_progress(
        stage,&pharaoh_abu_simbel_progress_step);
    if (output) {output[0]=stage;output[3]=percent;}
    return percent;
}

/* Complete prior MonumentProgress C leaf, native005477E0, logical AL. */
uint8_t pharaoh_royal_tomb_worker_active(PharaohBuildingTypeRecord *buildings,
    const PharaohWalkerQueryRecord *walkers,int32_t building)
{
    PharaohBuildingTypeRecord *b=&buildings[building];
    const PharaohWalkerQueryRecord *w;
    uint32_t state;
    if (!b->monument_worker_id) return 0;
    w=&walkers[b->monument_worker_id];
    if (!w->state) {b->monument_worker_id=0;return 0;}
    state=w->action_state;
    if (state==6 || state==7) state=w->action_state_before_6_7;
    if (w->type==81) return state==9 || state==11;
    if (w->type==108) return state==9 || state==12;
    b->monument_worker_id=0;
    return 0;
}

/* Complete prior MonumentProgress C reuse, native00546790. */
int32_t pharaoh_royal_tomb_progress(PharaohBaseMonumentProgress *c,
    int32_t building,int32_t count,int32_t output[7])
{
    const int32_t root=building-c->buildings[building].subtype;
    const uint32_t end=(uint32_t)root+(uint32_t)count;
    uint32_t sum=0,q=0,u=0,l=0;
    int32_t percent;
    if (root<(int32_t)end)
        for (uint32_t id=(uint32_t)root;id!=end;++id)
            sum+=(uint32_t)c->buildings[id].royal_tomb_progress;
    percent=pharaoh_x87_tomb_percent((int32_t)sum,count);
    for (int32_t i=0;i<count;++i) {
        /* Possible native bug: the worker predicate repeatedly uses ROOT,
         * while the counter advances through individual parts. Keep it. */
        if (pharaoh_royal_tomb_worker_active(c->buildings,c->walkers,root)) {
            const int32_t worker=c->buildings[root+i].monument_worker_id;
            uint8_t type;
            if (!worker) break;
            type=c->walkers[worker].type;
            if (type==81) ++q;else if (type==85) ++u;else if (type==108) ++l;
        }
    }
    if (output) {
        output[5]=percent;output[1]=(int32_t)q;output[2]=(int32_t)u;
        output[4]=(int32_t)l;output[6]=c->buildings[root].royal_tomb_stock;
    }
    return percent;
}

#define TOMB_PROGRESS(name,count) \
int32_t name(PharaohBaseMonumentProgress *c,int32_t building,int32_t out[7]) \
{return pharaoh_royal_tomb_progress(c,building,count,out);}
TOMB_PROGRESS(pharaoh_small_royal_tomb_progress,9)
TOMB_PROGRESS(pharaoh_medium_royal_tomb_progress,10)
TOMB_PROGRESS(pharaoh_large_royal_tomb_progress,21)
TOMB_PROGRESS(pharaoh_grand_royal_tomb_progress,31)
