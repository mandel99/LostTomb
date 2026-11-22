#include "pharaoh/monuments/build_requirements.h"

_Static_assert(offsetof(PharaohScenarioPlayerRuntime,native_value_433a)==0x433a,
    "native signed progress WORD");

static uint8_t uses_resource(const PharaohMonumentDefinition *d,int32_t resource)
{
    return d->resources[0]==resource || d->resources[1]==resource ||
        d->resources[2]==resource;
}

/* Complete prior MenuBuildingOptions C reuse, native004F5050. Progress
 * getters must still run when include_completed is nonzero: they can clear
 * worker claims. The logical native result is AL, not incidental upper EAX. */
uint8_t pharaoh_monument_resource_is_required(PharaohMonumentBuildRequirements *c,
    int32_t resource,uint8_t include_completed)
{
    for (int32_t slot=0;slot<3;++slot) {
        const int32_t player=*c->demand->current_player;
        const int32_t kind=c->demand->players[player].monument_definition_ids[slot];
        if (!kind) {
            if (uses_resource(&c->definitions[c->demand->planned_kinds[slot]],resource))
                return 1;
        } else {
            const int32_t id=c->demand->players[player].monument_building_ids[slot];
            const PharaohBuildingTypeRecord *b;
            int32_t percent;
            uint8_t incomplete;
            if (!id) continue;
            b=&c->demand->complex->buildings[id];
            switch (b->building_type) {
            case 0xb7: {
                PharaohPyramidProgressReport out;
                pharaoh_pyramid_build_progress_report(c->demand->pyramid,id,&out);
                percent=out.percent;break;
            }
            case 0xcf:case 0xd0:
                incomplete=b->pyramid_work_phase!=100;goto known_state;
            case 0xd2:
                incomplete=c->demand->players[*c->demand->current_player].native_value_433a<16;
                goto known_state;
            case 0xd5:percent=pharaoh_sun_temple_progress(c->demand->complex,id,NULL);break;
            case 0xe1:percent=pharaoh_library_progress(c->library,id,NULL);break;
            case 0xe3:percent=pharaoh_caesareum_progress(c->caesareum,id,NULL);break;
            case 0xe4:percent=pharaoh_lighthouse_progress(c->lighthouse,id,NULL);break;
            case 0xe5:percent=pharaoh_small_royal_tomb_progress(c->royal_tombs,id,NULL);break;
            case 0xe6:percent=pharaoh_abu_simbel_progress(c->demand->complex->buildings,id,NULL);break;
            case 0xea:percent=pharaoh_medium_royal_tomb_progress(c->royal_tombs,id,NULL);break;
            case 0xeb:percent=pharaoh_large_royal_tomb_progress(c->royal_tombs,id,NULL);break;
            case 0xec:percent=pharaoh_grand_royal_tomb_progress(c->royal_tombs,id,NULL);break;
            default:percent=pharaoh_mausoleum_progress(c->demand->complex,id,NULL);break;
            }
            incomplete=percent<100;
known_state:
            if ((incomplete || include_completed) &&
                uses_resource(&c->definitions[kind],resource)) return 1;
        }
    }
    return 0;
}

/* Complete prior Unit02166 C reuse, native004F5470. Preserve short-circuit
 * order, including repeated real progress getters across resource probes. */
uint8_t pharaoh_monument_guild_is_required(PharaohMonumentBuildRequirements *c,
    int32_t building_type)
{
    static const int32_t stone[6]={24,25,26,30,34,35};
    switch (building_type) {
    case 0xb1:return pharaoh_monument_resource_is_required(c,20,0);
    case 0xb2:return pharaoh_monument_resource_is_required(c,12,0);
    case 0xb3:
        for (int32_t i=0;i<6;++i)
            if (pharaoh_monument_resource_is_required(c,stone[i],0)) return 1;
        return 0;
    case 0xe7:
        return pharaoh_monument_resource_is_required(c,33,0) ||
            pharaoh_monument_resource_is_required(c,11,0);
    default:return 0;
    }
}

void pharaoh_monument_requirements_bind_records(PharaohMonumentBuildRequirements *c)
{
    PharaohBaseMonumentProgress *objects[4]={c->library,c->caesareum,c->lighthouse,c->royal_tombs};
    for (int32_t i=0;i<4;++i) {
        objects[i]->buildings=c->demand->complex->buildings;
        objects[i]->walkers=c->demand->complex->walkers;
    }
}
