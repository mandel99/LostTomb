#include "pharaoh/ui/building_info_init_support.h"
#include <stddef.h>
#include <string.h>

_Static_assert(offsetof(PharaohBuildingTypeRecord,resource_id)==0x43,"native resource BYTE");
_Static_assert(offsetof(PharaohBuildingModelRecord,laborers)==0x14,"native labor DWORD");
_Static_assert(offsetof(PharaohMonumentDefinition,unknown_0c)+2==0x0e,"native advisor BYTE");
_Static_assert(offsetof(PharaohScenarioPlayerRuntime,monument_definition_ids)==0x4730,"native goal DWORDs");
_Static_assert(offsetof(PharaohScenarioEvent,item_runtime_value)==4,"native request runtime WORD");
_Static_assert(sizeof(PharaohInfoPortraitScript)==48,"native portrait script stride");
_Static_assert(offsetof(PharaohWalkerQueryRecord,resource_id)==0xb,"native alternate kind BYTE");
_Static_assert(offsetof(PharaohWalkerQueryRecord,building_id)==0x5a,"native home building WORD");

static int32_t signed_bits(uint32_t v)
{ return v<=INT32_MAX?(int32_t)v:(int32_t)(v&INT32_MAX)-INT32_MAX-1; }

/* Whole prior MenuMonumentSearch C; Native70 instructions, RET0. */
int32_t pharaoh_info_pyramid_from_complex(
    const PharaohInfoPyramidSelectionContext *c,int32_t building)
{
    PharaohMapSurfaceContext *s=c->surface;
    uint32_t id=1;
    s->pyramid_placement->target_building=(uint32_t)building;
    do {
        const PharaohBuildingTypeRecord *b=&s->buildings[id];
        if(b->state!=0&&b->building_type==183) {
            uint32_t kind=b->pyramid_size_variant;
            if(pharaoh_pyramid_has_complex_temple((int32_t)*s->pyramid_style,
                s->pyramid_complex_offsets,(int32_t)kind)) {
                const int32_t delta=signed_bits((uint32_t)s->pyramid_complex_offsets[kind]+
                    (uint32_t)b->grid_offset-(uint32_t)s->map->origin);
                pharaoh_pyramid_map_segment_update(s,delta%228,delta/228,6,0);
                if(s->pyramid_placement->found!=0)return signed_bits(id);
            }
            kind=s->buildings[id].pyramid_size_variant;
            const uint32_t step=*s->pyramid_style==5?
                c->mastaba_part_counts[kind]:c->part_counts[kind];
            id+=step-1u;
        }
        ++id;
    } while(signed_bits(id)<4000);
    return building;
}

static const PharaohBuildingTypeRecord *selected(const PharaohInfoAdvisorButtonsContext *c)
{ return &c->ui->buildings[c->ui->window->building_id]; }
static void workers(const PharaohInfoAdvisorButtonsContext *c,volatile uint32_t *out)
{
    const PharaohBuildingTypeRecord *b=selected(c);
    if(b->num_workers<c->models[b->building_type].laborers)out[1]=1;
}
static int material(uint32_t resource)
{
    switch(resource) {
    case 1:case 8:case 10:case 13:case 15:case 17:case 18:case 19:
    case 20:case 23:case 24:case 25:case 26:case 28:case 30:return 1;
    default:return 0;
    }
}
static void supply(const PharaohInfoAdvisorButtonsContext *c,volatile uint32_t *out)
{
    const uint32_t resource=selected(c)->resource_id;
    if(material(resource)&&c->resource_advisors[resource]!=0)out[14]=1;
}

/* Whole prior MenuBuildingOptions C; Native536 instructions. */
void pharaoh_info_advisor_buttons(const PharaohInfoAdvisorButtonsContext *c,
    uint32_t output[15])
{
    volatile uint32_t *out=output;
    uint32_t i,kind;
    int32_t type;
    for(i=0;i<15;++i)out[i]=0;
    type=selected(c)->building_type;
    switch(type) {
    case 0x1e:case 0x1f:case 0x20:case 0x21:case 0x22:case 0x23:case 0x24:
    case 0x25:case 0xe2:workers(c,out);out[10]=1;break;
    case 0x26:case 0x27:case 0x29:case 0x2a:case 0x2b:case 0x52:case 0x53:
    case 0x88:case 0x8a:out[13]=1;break;
    case 0x28:case 0x2c:case 0x2d:case 0xa9:out[2]=1;break;
    case 0x2e:case 0x2f:case 0x31:case 0xb4:case 0xce:out[8]=1;workers(c,out);break;
    case 0x33:case 0x35:out[9]=1;workers(c,out);break;
    case 0x37:case 0x51:case 0xa7:case 0xb8:out[13]=1;workers(c,out);break;
    case 0x46:case 0x4c:case 0xbe:case 0xc2:out[7]=1;workers(c,out);break;
    case 0x47:case 0x48: {
        workers(c,out);i=0;out[type==0x47?7:6]=1;
        PharaohScenarioEvent *event=pharaoh_scenario_request_next_active(c->events,*c->event_count,NULL);
        while(event!=NULL) {
            if(pharaoh_event_record_is_enabled_0044d4c0((const uint8_t *)(const void *)event)) {
                if(i>=5)break;
                const int32_t resource=event->item_runtime_value;
                if(type==0x47?(resource>0&&resource<=8):resource<36){out[4]=1;break;}
                ++i;
            }
            event=pharaoh_scenario_request_next_active(c->events,*c->event_count,event);
        }
        break;
    }
    case 0x4a:case 0xb5:case 0xb6:workers(c,out);out[3]=1;break;
    case 0x4b:case 0x6d:case 0x6e:case 0x6f:case 0x71:case 0x72:case 0xa2:
    case 0xc0:case 0xc3:out[6]=1;workers(c,out);supply(c,out);break;
    case 0x4d:case 0x4e:case 0x4f:out[4]=1;workers(c,out);break;
    case 0x56:case 0x57:case 0xa1:case 0xbb:case 0xbc:case 0xbd:
        out[12]=1;out[5]=1;workers(c,out);break;
    case 0x5a:workers(c,out);break;
    case 0x5c:out[8]=1;break;
    case 0x5e:case 0x5f:case 0xab:case 0xad:case 0xca:
        out[2]=1;workers(c,out);supply(c,out);break;
    case 0x6a:case 0x6b:case 0x6c:case 0xd8:case 0xdd: {
        workers(c,out);
        const int32_t resource=type==0x6a?24:type==0x6b?25:type==0x6c?20:type==0xd8?26:30;
        if(pharaoh_monument_resource_is_required(c->requirements,resource,1))out[14]=1;
        supply(c,out);break;
    }
    case 0x70:case 0xcd:out[2]=1;out[6]=1;workers(c,out);supply(c,out);break;
    case 0x73:out[7]=1;workers(c,out);supply(c,out);break;
    case 0xb1:case 0xb2:case 0xb3:workers(c,out);out[14]=1;break;
    case 0xb7:case 0xcf:case 0xd0:case 0xd2:case 0xd5:case 0xde:out[14]=1;break;
    case 0xc7: {
        workers(c,out);
        const PharaohMonumentResourceDemand *d=c->requirements->demand;
        const PharaohScenarioPlayerRuntime *player=&d->players[*d->current_player];
        for(i=0;i<3;++i) {
            int32_t definition=player->monument_definition_ids[i];
            if(definition==0)definition=d->planned_kinds[i];
            if(c->requirements->definitions[definition].unknown_0c[2]!=0){out[14]=1;break;}
        }
        break;
    }
    case 0xcb:out[6]=1;out[9]=1;workers(c,out);supply(c,out);break;
    case 0xcc:
        workers(c,out);
        if(pharaoh_monument_resource_is_required(c->requirements,12,1))out[14]=1;
        break;
    default:break;
    }
    type=selected(c)->building_type;
    if(type>=10&&type<=29) {
        supply(c,out);
        /* The high byte of fishing_boat_id is the house overlay at+85. */
        kind=((const uint8_t *)(const void *)selected(c))[0x85];
        if(kind<30)kind+=30;
        kind=kind>=68?6u:pharaoh_building_advisor_option_category(kind);
        if(kind!=0)out[kind]=1;
        out[7]=1;
    }
    if((uint8_t)pharaoh_building_is_farm(c->ui->buildings,c->ui->window->building_id)) {
        out[7]=1;workers(c,out);
    }
    type=selected(c)->building_type;if(type>=140&&type<=149)out[11]=1;
    type=selected(c)->building_type;
    if((type>=60&&type<=69)||type==209){out[11]=1;workers(c,out);}
}

/* Whole65 instruction prior RestoreActorWalkerCode C. */
int32_t pharaoh_info_walker_portrait_code(
    const PharaohInfoWalkerPortraitCodeContext *c,int32_t walker)
{
    const PharaohWalkerQueryRecord *w=&c->walkers[walker];
    const int32_t building=w->building_id;
    int32_t code=c->portrait_codes[w->type];
    if(code>0)return code;
    if(code==-4) {
        code=c->portrait_codes[w->resource_id];
        if((uint32_t)code+4u>3u)return code;
        if(code!=-3)return -1;
    } else if(code==-2) {
        const int32_t script=c->scripts[(uint8_t)*c->selected_scenario].walker_type;
        return script==68?210:script==69?209:script==70?208:-1;
    } else if(code!=-3)return -1;
    const uint32_t group=pharaoh_building_group_index(c->buildings,building);
    return group<=4u?(int32_t)(230u+group):-1;
}

static uint32_t terminated_bytes(const char *p)
{ uint32_t size=0;do{++size;}while(p[size-1u]!=0);return size; }
static void copy_words(char *target,const char *source,uint32_t bytes)
{
    uint32_t i;
    for(i=0;i<(bytes&~3u);i+=4u)memcpy(target+i,source+i,4);
    for(;i<bytes;++i)target[i]=source[i];
}

/* Whole61-instruction native539A70. The first live volume read precedes
 * the name's BYTE tail, just as539AD4 precedes539ADF. */
int32_t pharaoh_info_voice_stream_play(const PharaohInfoVoiceStreamContext *c,
    const char *name)
{
    if(*c->enabled==0||*c->voice_enabled==0)return 0;
    uint32_t size=terminated_bytes(c->prefix);
    copy_words(c->filename_scratch,c->prefix,size);
    size=terminated_bytes(name);
    char *target=c->filename_scratch+terminated_bytes(c->filename_scratch)-1u;
    copy_words(target,name,size&~3u);
    const int32_t first_volume=*c->volume;
    for(uint32_t i=size&~3u;i<size;++i)target[i]=name[i];
    pharaoh_digital_sound_stream_set_volume(c->device,1,first_volume,c->operations);
    pharaoh_digital_sound_set_speech_volume(c->device,*c->volume,c->operations);
    return pharaoh_digital_sound_stream_play(c->device,c->filename_scratch,1,1,c->operations);
}

/* Whole44-instruction native53BFE0. Recovered direct callers discard EAX. */
void pharaoh_info_walker_voice_variant(const PharaohInfoWalkerVoiceContext *c,
    int32_t kind,int32_t variant)
{
    if(kind<=0||variant<0)return;
    char name[32]={0};
    const char *source=kind==108?c->special_voice_names[(uint32_t)variant]:
        c->voice_names[(uint32_t)variant-20u+(uint32_t)kind*20u];
    copy_words(name,source,terminated_bytes(source));
    (void)pharaoh_info_voice_stream_play(c->stream,name);
}

/* Whole130-instruction native53D0B0; its direct UI callers discard EAX.
 * reserved_e0 is the actual EADCE0 image/speech-kind DWORD overlay. */
void pharaoh_info_walker_selection_voice(
    const PharaohInfoWalkerSelectionVoiceContext *c)
{
    PharaohBuildingInfoLiveState *state=c->ui->window;
    const int32_t actor=state->walker_ids[state->selected_walker_index];
    if(actor<=0)return;
    const PharaohInfoWalkerPortraitCodeContext *p=c->portrait;
    uint32_t kind=p->walkers[actor].type,image;
    int32_t cue=-1;
    switch(kind) {
    case 83:cue=0;break;case 103:cue=1;break;case 104:cue=2;break;
    case 102:cue=3;break;case 69:cue=4;break;case 70:cue=5;break;
    case 84:cue=6;break;case 82:cue=7;break;case 68:cue=8;break;
    default:break;
    }
    if(cue>=0)pharaoh_walker_sound_play(c->sound,actor,cue,0);
    kind=p->walkers[actor].type;image=(uint32_t)p->portrait_codes[kind];
    state->reserved_e0=signed_bits(image);
    if(image==UINT32_C(0xfffffffe)) {
        const int32_t script=p->scripts[(uint8_t)*p->selected_scenario].walker_type;
        if(script==68)image=210;else if(script==69)image=209;else if(script==70)image=208;
    } else {
        if(image==UINT32_C(0xfffffffd)) {
            kind=pharaoh_building_group_index(p->buildings,p->walkers[actor].building_id);
            image=kind<=4u?230u+kind:(uint32_t)state->reserved_e0;
        }
        if(image==UINT32_C(0xfffffffc)) {
            kind=p->walkers[actor].resource_id;image=(uint32_t)p->portrait_codes[kind];
            state->reserved_e0=signed_bits(image);
            if(image==UINT32_C(0xfffffffd)) {
                kind=pharaoh_building_group_index(p->buildings,p->walkers[actor].building_id);
                image=kind<=4u?230u+kind:(uint32_t)state->reserved_e0;
            }
        }
        if(image==UINT32_MAX)return;
    }
    image-=201u;state->reserved_e0=signed_bits(image);
    if(signed_bits(image)<0)return;
    const int32_t voice=((const int8_t *)(const void *)&p->walkers[actor])[0x7d];
    state->walker_phrase=voice;
    pharaoh_info_walker_voice_variant(c->voice,signed_bits(image),voice);
}
