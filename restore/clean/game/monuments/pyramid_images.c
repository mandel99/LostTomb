#include "pharaoh/monuments/pyramid_images.h"
#include <stddef.h>
#include <string.h>

_Static_assert(sizeof(PharaohPyramidImageShape)==8 &&
    offsetof(PharaohPyramidImageShape,native_04)==4 &&
    sizeof(PharaohPyramidWorkerOffset)==8 &&
    offsetof(PharaohPyramidWorkerOffset,y)==4,
    "qualified template/worker table views");
_Static_assert(offsetof(PharaohBuildingTypeRecord,pyramid_worker_x_a)==0x5c &&
    offsetof(PharaohBuildingTypeRecord,pyramid_worker_x_b)==0x60 &&
    offsetof(PharaohBuildingTypeRecord,pyramid_worker_image)==0x68 &&
    offsetof(PharaohBuildingTypeRecord,pyramid_transient_image)==0x6c,
    "native pyramid rendering fields");

typedef struct PharaohStyleThreeOverride {
    uint8_t slot;
    int16_t x,y;
} PharaohStyleThreeOverride;

/* Native 005F57C8 has a second coordinate pair for main pyramid parts.
 * Most pairs equal the first; these are its fourteen differing records. */
static const PharaohStyleThreeOverride style_three_main_overrides[]={
    {6,-69,-23},{7,-49,-37},{8,-29,-52},{9,-7,-68},
    {10,13,-82},{11,32,-97},{31,-20,-22},{32,-14,-37},
    {34,12,-83},{35,18,-98},{37,23,-26},{38,21,-42},
    {40,-9,-86},{41,-11,-102}
};

void pharaoh_pyramid_coordinate_tables_init(
    PharaohPyramidCoordinateTableStorage *s,
    const PharaohPyramidImageData *d)
{
    static const int32_t heights[30]={
        0,90,0,0,0,0,0,90,135,180,0,0,0,90,180,225
    };
    memcpy(s->style_one_heights,heights,sizeof(heights));
    for(int32_t shape=0;shape<9;++shape) {
        for(int32_t step=0;step<6;++step) {
            const int32_t slot=shape*6+step;
            const PharaohPyramidWorkerOffset *const regular=&d->worker[shape][step];
            const PharaohPyramidWorkerPoint *const style3=&d->worker_style3[shape][step];
            s->regular_offsets[slot*2]=(int16_t)regular->x;
            s->regular_offsets[slot*2+1]=regular->y;
            s->style_three_offsets[slot*4]=(int16_t)style3->x;
            s->style_three_offsets[slot*4+1]=(int16_t)style3->y;
            s->style_three_offsets[slot*4+2]=(int16_t)style3->x;
            s->style_three_offsets[slot*4+3]=(int16_t)style3->y;
        }
        s->main_part_offsets[shape*2]=(int16_t)d->worker_base[shape].x;
        s->main_part_offsets[shape*2+1]=(int16_t)d->worker_base[shape].y;
    }
    for(size_t i=0;i<sizeof(style_three_main_overrides)/
            sizeof(style_three_main_overrides[0]);++i) {
        const PharaohStyleThreeOverride *const value=&style_three_main_overrides[i];
        s->style_three_offsets[value->slot*4+2]=value->x;
        s->style_three_offsets[value->slot*4+3]=value->y;
    }
    s->tables=(PharaohMonumentCoordinateTables){
        s->style_one_heights,
        (const int32_t *)(const void *)&d->regular[0][0].native_04,
        (const int32_t *)(const void *)&d->mastaba[0][0].native_04,
        s->regular_offsets,s->style_three_offsets,s->main_part_offsets};
}

static void worker_offset(const PharaohPyramidImageData *d,
    int16_t *x,int16_t *y,uint32_t position,uint32_t shape,uint32_t style)
{
    if(style==2){
        const uint32_t step=(position-1u)%6u>=3u?3u:0u;
        const PharaohPyramidWorkerPoint *const p=&d->worker_style3[shape][step];
        *x=(int16_t)p->x;*y=(int16_t)p->y;
    }else{
        const uint32_t step=style==1?(position*2u-2u)%6u:(position-1u)%6u;
        const PharaohPyramidWorkerOffset *const p=&d->worker[shape][step];
        *x=(int16_t)(d->worker_base[shape].x+p->x);
        *y=(int16_t)((uint32_t)d->worker_base[shape].y+
            (style==1?(uint32_t)(p->y/2):(uint16_t)p->y));
    }
}

/* Reused whole Unit02091 C, native004EDF50. */
void pharaoh_pyramid_update_piece_image(PharaohPyramidImageContext *c,
    int32_t id,int32_t rotation,int32_t kind)
{
    PharaohMapSurfaceContext *const s=c->surface;
    PharaohBuildingTypeRecord *const b=&s->buildings[id];
    b->pyramid_transient_image=0;
    if(b->pyramid_work_phase>1){
        pharaoh_pyramid_update_finished_piece(c,id,rotation,kind);return;
    }
    const uint32_t mode=*s->pyramid_style;
    const int32_t piece=b->subtype;
    uint32_t shape=(uint32_t)(mode==5?c->data->mastaba[kind][piece].shape:
        c->data->regular[kind][piece].shape);
    if((int32_t)shape<4){shape+=(uint32_t)rotation;if((int32_t)shape>=4)shape-=4;}
    else if((int32_t)shape<8){shape+=(uint32_t)rotation;if((int32_t)shape>=8)shape-=4;}
    else if(shape==9)shape=9u+(uint32_t)rotation;
    const uint32_t base=s->groups[752].first_image;
    const uint32_t work_shape=mode==5?0:shape;
    uint32_t work_image=0,image=0;
    int ground_only=0;
    if(mode!=5){
        if(shape==5)work_image=base+109;
        else if(shape==6)work_image=base+(mode==3?112u:110u);
        else if(mode==3){
            if(shape==1)work_image=base+115;
            else if(shape==2)work_image=base+119;
            else if(shape==3)work_image=base+123;
        }
    }
    const uint32_t step=b->pyramid_work_column,layer=b->pyramid_work_row;
    if(b->pyramid_work_phase==0 || mode==5){
        if(step==0){
            if(piece<4 && mode!=5)
                image=s->groups[758].first_image+c->data->corners[rotation][piece];
            else ground_only=1;
        }else if(layer<b->pyramid_target_layers || shape==8)
            image=base+step+95u+(s->random[b->grid_offset]&1u)*6u;
        else if(mode==1)
            image=s->groups[c->data->roof_layers[(uint32_t)kind*6u+layer]?763:752].first_image+
                step*8u-8u+shape;
        else if((int32_t)shape<8)image=base+step*8u-8u+shape;
        else image=base+109u+(shape&1u);
    }else if(mode==1 && c->data->roof_layers[(uint32_t)kind*6u+layer])
        image=s->groups[763].first_image+(5u-step)*8u+shape;
    else image=base+(11u-step)*8u+shape;
    if((b->pyramid_ramp1_height==0 && b->pyramid_ramp2_height==0) || work_image==0)
        b->pyramid_worker_image=0;
    else{
        const uint32_t style=mode==1 && c->data->roof_layers[(uint32_t)kind*6u+b->pyramid_work_row]?
            1u:(mode==3?2u:0u);
        if(b->pyramid_ramp1_height)worker_offset(c->data,&b->pyramid_worker_x_a,
            &b->pyramid_worker_y_a,b->pyramid_ramp1_height,work_shape,style);
        if(b->pyramid_ramp2_height)worker_offset(c->data,&b->pyramid_worker_x_b,
            &b->pyramid_worker_y_b,b->pyramid_ramp2_height,work_shape,style);
        b->pyramid_worker_image=work_image;
    }
    if(!ground_only){
        pharaoh_map_place_surface_square(s,(uint16_t)id,b->x,b->y,b->size,b->size,image,8);
        return;
    }
    for(uint32_t i=0;i<4;++i){
        const uint32_t tile=(uint32_t)b->grid_offset+c->data->quad[i];
        s->building_ids[tile]=(int16_t)id;
        s->map->terrain[tile]=(s->map->terrain[tile]&0xefffffffu)|8u;
        (void)pharaoh_pyramid_set_construction_tile_image(s,id,tile,
            *s->pyramid_style==5?5:10,1);
    }
}

/* Reused whole Unit02146 C, native004F4160. */
void pharaoh_pyramid_update_finished_piece(PharaohPyramidImageContext *c,
    int32_t id,int32_t rotation,int32_t kind)
{
    PharaohMapSurfaceContext *const s=c->surface;
    PharaohBuildingTypeRecord *const b=&s->buildings[id];
    const PharaohPyramidSpecialStageTable *const table=
        &(*s->pyramid_style==5?s->pyramid_style_five:s->pyramid_regular)[kind];
    uint32_t phase=b->pyramid_work_phase;
    (void)rotation;
    if(phase==3){
        for(int32_t i=0;i<table->count;++i)if(table->entries[i].part==b->subtype){
            const uint32_t tile=(uint32_t)b->grid_offset+(uint32_t)table->entries[i].tile_offset;
            s->image_ids[tile]=s->groups[759].first_image;
            if(s->random[tile]&1u)s->property_flags[tile]|=0x20u;
            return;
        }
    }else if(phase==4){
        const uint32_t tile=(uint32_t)b->grid_offset,mask=b->pyramid_prep_mask;
        for(uint32_t i=0;i<4;++i)if(mask&(1u<<i))
            s->map->terrain[tile+c->data->quad[i]]&=0xffffff6eu;
    }else if(phase>=5 && phase<=10){
        for(uint32_t i=0;i<4;++i){
            const uint32_t tile=(uint32_t)b->grid_offset+c->data->quad[i];
            phase=b->pyramid_work_phase;
            (void)pharaoh_pyramid_set_construction_tile_image(s,id,tile,
                (int32_t)(phase-((b->pyramid_prep_mask&(1u<<i))?0u:1u)),1);
        }
    }else if(phase==11){
        const int32_t view=*s->orientation/2;
        const uint32_t image=s->groups[758].first_image+c->data->corners[view][b->subtype];
        pharaoh_map_place_surface_square(s,(uint16_t)id,b->x,b->y,b->size,b->size,image,8);
    }
}

/* Reused whole Unit02092 C, native004EE4A0. */
void pharaoh_pyramid_refresh_part_images(PharaohPyramidImageContext *c,int32_t root)
{
    PharaohMapSurfaceContext *const s=c->surface;
    PharaohBuildingTypeRecord *const b=&s->buildings[root];
    const int32_t kind=b->pyramid_size_variant,rotation=*s->orientation/2;
    uint32_t current=(uint32_t)root;
    for(;;){
        const uint32_t limit=(uint32_t)root+
            (*s->pyramid_style==5?c->mastaba_counts:c->regular_counts)[kind];
        if((int32_t)current>=(int32_t)limit)break;
        pharaoh_pyramid_update_piece_image(c,(int32_t)current,rotation,kind);++current;
    }
    if(pharaoh_pyramid_has_complex_temple((int32_t)*s->pyramid_style,
            s->pyramid_complex_offsets,kind)){
        const int32_t relative=(int32_t)((uint32_t)s->pyramid_complex_offsets[kind]+
            (uint32_t)b->grid_offset-(uint32_t)s->map->origin);
        pharaoh_pyramid_map_segment_update(s,relative%228,relative/228,4,0);
    }
}

static void refresh(void *data,int32_t root)
{pharaoh_pyramid_refresh_part_images(data,root);}
void pharaoh_rotated_bind_pyramid_image(PharaohRotatedBuildingOperations *ops,
    PharaohPyramidImageContext *c)
{ops->pyramid=refresh;ops->pyramid_user_data=c;}
