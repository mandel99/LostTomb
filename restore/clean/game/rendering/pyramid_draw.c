#include "pharaoh/rendering/pyramid_draw.h"
#include "pharaoh/rendering/upper_sprite_draw.h"
#include <stddef.h>

typedef struct PharaohPyramidDrawBuilding {
    uint8_t state,reserved_01[5];int16_t x,y;uint8_t reserved_0a[2];
    int32_t grid_offset;int16_t type,subtype;uint8_t reserved_14[0x40];
    uint8_t size_variant,stage,blocks,layer,target_layers,ramp1_height,
        ramp2_height,prep_mask;
    int16_t ramp_x1,ramp_y1,ramp_x2,ramp_y2,worker_id;
    uint8_t reserved_66[2];uint32_t ramp_image_id,ramp_link_building_id;
    int32_t ramp_link_draw_x,ramp_link_draw_y,ramp_link_x,ramp_link_y;
    uint8_t completion_shown,reserved_81[0x87];
} PharaohPyramidDrawBuilding;
typedef struct PyramidPoint {int32_t x,y;} PyramidPoint;

_Static_assert(sizeof(PharaohPyramidDrawBuilding)==0x108 &&
    offsetof(PharaohPyramidDrawBuilding,grid_offset)==0x0c &&
    offsetof(PharaohPyramidDrawBuilding,size_variant)==0x54 &&
    offsetof(PharaohPyramidDrawBuilding,ramp_image_id)==0x68 &&
    offsetof(PharaohPyramidDrawBuilding,ramp_link_building_id)==0x6c &&
    offsetof(PharaohPyramidDrawBuilding,ramp_link_y)==0x7c,
    "native pyramid draw record");

static const int32_t figure_tiles[4]={0,1,228,229};
static const int32_t neighbours[4][3]={{458,2,456},{454,456,-2},
    {-458,-2,-456},{-454,-456,2}};
static const int32_t bent_heights[30]={0,90,0,0,0,0,0,90,135,180,0,0,0,90,180,225};
static const uint8_t mastaba_decor[40]={0,0,1,0,1,1,0,1,0,1,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,1,1,1,1,1,1,1};
static const PyramidPoint ramp_shift[18]={{-16,36},{-20,18},{-16,0},{3,20},{-1,2},{-5,-15},{-8,10},{2,6},{-8,10},{-8,-10},{-47,3},{-43,-15},{-15,20},{-11,2},{-24,11},{-26,7},{6,11},{0,-10}};

static PharaohPyramidDrawBuilding *building(PharaohPyramidDrawContext *c,int32_t id)
{return (PharaohPyramidDrawBuilding *)(void *)&c->buildings[id];}
static void draw(PharaohPyramidDrawContext *c,int32_t image,int32_t x,int32_t y)
{(void)pharaoh_image_draw(c->draw,image,x,y);}
static void upper(PharaohPyramidDrawContext *c,int32_t image,int32_t x,int32_t y)
{PharaohUpperSpriteDrawContext top={c->draw->images,c->draw->image_pixels,*c->draw->image_count,c->draw->clip,c->draw->surface_pixels,c->draw->pitch_bytes,*c->draw->tint,&c->draw->state->source.pixels};(void)pharaoh_image_draw_isometric_top(&top,image,x,y);}
static int32_t shape(const PharaohPyramidDrawContext *c,
    const PharaohPyramidDrawBuilding *b,uint8_t size)
{return c->layout->regular[size][b->subtype].shape;}
static int32_t ramp_variant(uint8_t height)
{switch(height%6u){case 2:return 1;case 4:return 2;case 5:return 3;default:return 0;}}
static void ramps(PharaohPyramidDrawContext *c,
    const PharaohPyramidDrawBuilding *b,uint8_t size,int32_t x,int32_t y)
{
    if(!b->ramp_image_id)return;int32_t first=0,second=0;
    if(*c->style==3){if(shape(c,b,size)<4){first=ramp_variant(b->ramp1_height);second=ramp_variant(b->ramp2_height);}else{first=((int32_t)b->ramp1_height-1)%3;second=((int32_t)b->ramp2_height-1)%3;}}
    if(b->ramp1_height){int32_t image=(int32_t)b->ramp_image_id+first,dx=0,dy=0;if(*c->style==3){int32_t i=image-(int32_t)c->groups[752].first_image-109;dx=ramp_shift[i].x;dy=ramp_shift[i].y;}draw(c,image,b->ramp_x1+dx+x,b->ramp_y1+dy+y);}
    if(b->ramp2_height){int32_t image=(int32_t)b->ramp_image_id+second,dx=0,dy=0;if(*c->style==3){int32_t i=image-(int32_t)c->groups[752].first_image-109;dx=ramp_shift[i].x;dy=ramp_shift[i].y;}draw(c,image,b->ramp_x2+dx+x,b->ramp_y2+dy+y);}
}
static void walkers(PharaohPyramidDrawContext *c,
    const PharaohPyramidDrawBuilding *b,int32_t x,int32_t y,int32_t mode,
    int32_t *last_tile)
{
    for(int32_t i=0;i<4;++i){int32_t tile=b->grid_offset+figure_tiles[i];if(last_tile)*last_tile=tile;for(int32_t id=c->walker_ids[tile];id>0;id=c->walker->walkers[id].next_walker_id_on_same_tile){int32_t work=c->walker->walkers[id].pyramid_walk_mode;if((mode==0&&work==3)||(mode==1&&work!=0)||(mode==2&&work>0&&work!=3))pharaoh_city_draw_walker(c->walker,id,x,y,0);}}
}

void pharaoh_pyramid_draw_part(PharaohPyramidDrawContext *c,int32_t image,
    int32_t x,int32_t y,int32_t id)
{
    PharaohPyramidDrawBuilding *b=building(c,id);uint8_t size=b->size_variant;
    int32_t height=*c->style==1?bent_heights[b->layer+size*6]:b->layer*90;
    int32_t px=x,py=y,last_tile=0,deferred=0,walkers_drawn=0;
    if(b->stage<2&&(b->stage||b->layer||b->blocks||*c->style==5||b->subtype<4)){
        px=*c->tile_half_width+1+x;py=y-*c->tile_half_height-height;
    }
    if(b->ramp1_height>=7 && ((*c->style!=3&&b->ramp1_height%6u==1)||
        (*c->style==3&&((int32_t)b->ramp1_height-1)%6<3))){
        int32_t kind=shape(c,b,size),turn=(int32_t)*c->orientation/2,view=kind;
        if(kind<4){view=kind+turn;if(view>3)view-=4;}else if(kind<8){view=kind+turn;if(view>7)view-=4;}
        if((*c->style==3&&view!=0&&view!=4&&view!=7)||(*c->style!=3&&(view==5||view==6))){
            int32_t neighbour=c->building_ids[b->grid_offset+neighbours[turn][view==2?0:1]];
            if(neighbour && building(c,neighbour)->type==183){PharaohPyramidDrawBuilding *n=building(c,neighbour);n->ramp_link_building_id=(uint32_t)id;n->ramp_link_draw_x=px;n->ramp_link_draw_y=py;n->ramp_link_x=x;n->ramp_link_y=y;deferred=1;}
        }
    }
    int32_t linked=(int32_t)b->ramp_link_building_id;
    if(!deferred&&!b->ramp_image_id&&!linked){walkers(c,b,x,y,0,&last_tile);walkers_drawn=1;}
    if(height){if(*c->style==5&&size>1&&mastaba_decor[b->subtype]&&b->layer){int32_t decor=(int32_t)c->groups[752].first_image+101+(c->random[last_tile]&1u)*6;draw(c,decor,px,py+height);upper(c,decor,px,py+height);}draw(c,image,px,py);}
    upper(c,image,px,py);
    if(linked){PharaohPyramidDrawBuilding *other=building(c,linked);ramps(c,other,size,b->ramp_link_draw_x,b->ramp_link_draw_y);walkers(c,other,b->ramp_link_x,b->ramp_link_y,1,0);}
    if(deferred)return;ramps(c,b,size,px,py);if(!walkers_drawn)walkers(c,b,x,y,0,0);walkers(c,b,x,y,2,0);
}
