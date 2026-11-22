#include "pharaoh/monuments/progress_reports.h"
#include "pharaoh/core/static_objects.h"

int32_t pharaoh_native_reference_count_004204a0(
    int32_t id, const int32_t record_ids[13],
    const int16_t *const targets[13])
{
    int32_t index, count = 0;
    for (index = 0; index < 13; ++index)
        if (targets[index][9] != 0 && record_ids[index] == id) ++count;
    return count;
}

_Static_assert(sizeof(PharaohComplexPartDefinition) == 52 &&
    offsetof(PharaohBuildingTypeRecord, complex_progress_flags) == 0x64,
    "native complex definition/flags layout");

/* Reuse completed MonumentComplexQueries.c, with actual record state.
 * Port correction: rejected actions of known worker types must clear the
 * DWORD claim too. The earlier generic clean helper returned without clearing. */
static uint8_t complex_worker(PharaohBuildingTypeRecord *buildings,
    const PharaohWalkerQueryRecord *walkers, int32_t building, int mausoleum)
{
    PharaohBuildingTypeRecord *b = &buildings[building];
    const PharaohWalkerQueryRecord *w;
    uint32_t state;
    if (mausoleum && b->building_type != 0xde) return 0;
    if (!b->monument_worker_id) return 0;
    w = &walkers[b->monument_worker_id];
    if (!w->state) goto clear;
    state = w->action_state;
    if (state == 6 || state == 7) state = w->action_state_before_6_7;
    if (w->type == 79 && (state == 9 || state == 10)) return 1;
    if (w->type == 81 && (state == 9 || state == 10 || state == 11)) return 1;
    if (w->type == 85 && (state == 15 || state == 16 ||
        (mausoleum ? (state >= 26 && state <= 28) :
                     (state >= 23 && state <= 25)))) return 1;
clear:
    b->monument_worker_id = 0;
    return 0;
}

uint8_t pharaoh_mausoleum_monument_worker_active(
    PharaohBuildingTypeRecord *buildings, const PharaohWalkerQueryRecord *walkers,
    int32_t building)
{
    return complex_worker(buildings, walkers, building, 1);
}

uint8_t pharaoh_sun_temple_monument_worker_active(
    PharaohBuildingTypeRecord *buildings, const PharaohWalkerQueryRecord *walkers,
    int32_t building)
{
    return complex_worker(buildings, walkers, building, 0);
}

static int count_worker(PharaohComplexProgressContext *c, int32_t id,
    uint32_t *q, uint32_t *q10, uint32_t *u, uint32_t *o)
{
    const int32_t worker = c->buildings[id].monument_worker_id;
    const PharaohWalkerQueryRecord *w;
    if (!worker) return 0;
    w = &c->walkers[worker];
    if (w->type == 79) ++*o;
    else if (w->type == 81) { if (w->action_state == 10) ++*q10; ++*q; }
    else if (w->type == 85) ++*u;
    return 1;
}

/* Native004DF040: output may be null; claim cleanup still occurs. */
int32_t pharaoh_mausoleum_progress(PharaohComplexProgressContext *c,
    int32_t building, int32_t output[12])
{
    const int32_t root = building - c->buildings[building].subtype;
    const uint32_t flags = c->buildings[root].complex_progress_flags;
    uint32_t remaining = 0, phase = 1, missing = 0, empty = 0, tiles = 0;
    uint32_t cost = 0, q = 0, q10 = 0, u = 0, o = 0;
    int32_t percent;
    if (flags & 1u) {
        remaining = (flags & 0x10u) ? 3u : 4u;
        if (flags & 0x20u) --remaining;
        if (flags & 0x40u) --remaining;
        if (flags & 0x80u) --remaining;
    }
    for (int32_t i = 0; i < 116; ++i) {
        const int32_t id = root + i;
        PharaohBuildingTypeRecord *b = &c->buildings[id];
        uint32_t kind, stage;
        if (pharaoh_mausoleum_monument_worker_active(c->buildings,c->walkers,id) &&
            !count_worker(c,id,&q,&q10,&u,&o)) return 0;
        kind = (uint8_t)c->mausoleum_parts[b->subtype].kind;
        stage = (uint32_t)b->monument_stage;
        if (kind <= 3) {
            uint32_t set = 0;
            for (uint32_t j = 0; j < 16; ++j) if (stage & (1u << j)) ++set;
            tiles += 16u - set; cost += (16u - set) * 400u;
            if (set) phase = 2;
        } else if ((kind >= 4 && kind <= 10) || kind == 12) {
            if (stage == 0) { ++missing; ++empty; }
            else if (stage == 1) ++empty;
        }
    }
    if (!missing && phase == 1) phase = 2;
    if (flags & 3u) phase = 3;
    percent = (int32_t)((288u - empty - tiles - missing) * 100u) / 288;
    if (output) {
        uint32_t *out = (uint32_t *)output;
        out[0]=phase;out[1]=missing;out[2]=tiles;out[3]=empty;
        out[4]=q;out[5]=q10;out[6]=u;out[7]=cost;
        out[8]=(uint32_t)percent;out[9]=0x6400;out[10]=o;out[11]=remaining;
    }
    return percent;
}

/* Native00542AD0: preserve native stage arithmetic and output order. */
int32_t pharaoh_sun_temple_progress(PharaohComplexProgressContext *c,
    int32_t building, int32_t output[14])
{
    const int32_t root = building - c->buildings[building].subtype;
    const uint32_t flag = (c->buildings[root].complex_progress_flags >> 1) & 1u;
    uint32_t phase=1,missing=0,stages=0,kind1=0,kind47=0,kind8=0,special=0;
    uint32_t cost=0,q=0,q10=0,u=0,o=0;
    int32_t global_o,global_q,percent;
    for (int32_t i=0;i<100;++i) {
        const int32_t id=root+i;
        PharaohBuildingTypeRecord *b=&c->buildings[id];
        uint32_t stage;
        if (pharaoh_sun_temple_monument_worker_active(c->buildings,c->walkers,id) &&
            !count_worker(c,id,&q,&q10,&u,&o)) return 0;
        stage=(uint32_t)b->monument_stage;
        switch (c->sun_temple_parts[b->subtype].kind) {
        case 0:
            stages+=13u-stage;
            if (!stage) phase=flag+1u;
            else if (stage<=3) phase=2;
            else if (stage<=12) phase=3;
            else if (stage==13) phase=4;
            break;
        case 1:
            if (!stage) {++missing;++kind1;cost+=400u;}
            else if (stage==1) {++kind1;cost+=400u;} break;
        case 2:case 3:case 4:case 5:case 6:case 7:
            if (!stage) {++missing;++kind47;cost+=400u;}
            else if (stage==1) {++kind47;cost+=400u;} break;
        case 8:case 9:case 10:case 11:
            if (!stage) {++missing;++kind8;} else if (stage==1) ++kind8; break;
        case 12:
            if (stage<=3) {
                const uint32_t amount=stage<=1?3u:4u-stage;
                if (!stage) ++missing;
                special+=amount;cost+=amount*400u;
            } break;
        }
    }
    global_o=pharaoh_native_reference_count_004204a0(79,c->reference_ids,c->reference_targets);
    global_q=pharaoh_native_reference_count_004204a0(81,c->reference_ids,c->reference_targets);
    percent=(int32_t)((213u-special-kind8-kind47-stages-missing)*100u)/213;
    if (output) {
        uint32_t *out=(uint32_t *)output;
        out[0]=phase;out[1]=missing;out[2]=stages;out[3]=kind1;out[4]=kind47;
        out[5]=kind8;out[6]=special;out[7]=(uint32_t)global_q+q;out[8]=q10;
        out[9]=u;out[10]=(uint32_t)global_o+o;out[11]=cost;
        out[12]=(uint32_t)percent;out[13]=38000;
    }
    return percent;
}
