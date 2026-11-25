#include "pharaoh/rendering/pyramid_walker_draw.h"

static void project(const PharaohPyramidWalkerDrawContext *c,
    int32_t x,int32_t y,int32_t *px,int32_t *py)
{pharaoh_city_view_tile_to_pixel(c->view,c->projection,c->bounds,x,y,px,py);}
static void anchor(const PharaohPyramidWalkerDrawContext *c,int32_t id,
    int32_t *x,int32_t *y)
{
    const PharaohBuildingTypeRecord *b=&c->buildings[id];*x=b->x;*y=b->y;
    switch(*c->anchor_orientation){case 0:++*y;break;case 4:++*x;break;case 6:++*x;++*y;break;default:break;}
}
static void part(const PharaohPyramidWalkerDrawContext *c,int32_t id,
    int32_t height,int32_t *x,int32_t *y)
{pharaoh_monument_part_coordinates(&c->buildings[id],height,(int32_t)*c->pyramid_style,c->view->viewport.tile_half_width,c->view->viewport.tile_half_height,c->projection->orientation,c->tables,x,y);}

/* Complete native 004F0D30 interpolation between pyramid work positions. */
void pharaoh_pyramid_walker_pixel_offset(PharaohPyramidWalkerDrawContext *c,
    int32_t id,int32_t *out_x,int32_t *out_y)
{
    const PharaohWalkerQueryRecord *f=&c->walkers[id];
    int32_t previous=f->cross_country_x,previous_height=f->cross_country_y;
    int32_t target=f->monument_to_building,height=f->monument_to_position;
    int32_t ax,ay;
    if(previous==0||previous==target){const PharaohBuildingTypeRecord *main=&c->buildings[target-c->buildings[target].subtype];project(c,main->road_access_x,main->road_access_y,&ax,&ay);}
    else{int32_t tx,ty,px,py;part(c,previous,previous_height,&ax,&ay);anchor(c,previous,&tx,&ty);project(c,tx,ty,&px,&py);ax+=px;ay+=py;}
    int32_t tx,ty,ox,oy,px,py;anchor(c,target,&tx,&ty);part(c,target,height,&ox,&oy);project(c,tx,ty,&px,&py);
    int32_t bx=ox+px,by=oy+py,progress=f->monument_travel_progress,total=f->monument_travel_length;
    if(progress<total){*out_x=((bx-ax)*progress)/total+ax;*out_y=((by-ay)*progress)/total+ay;}else{*out_x=bx;*out_y=by;}
}
