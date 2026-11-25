#include "pharaoh/rendering/walker_draw.h"
#include <stddef.h>

_Static_assert(sizeof(PharaohFormationDrawRecord)==0x90,"native formation record");
_Static_assert(offsetof(PharaohFormationDrawRecord,experience)==0x7a &&
    offsetof(PharaohFormationDrawRecord,request_event_id)==0x82 &&
    offsetof(PharaohFormationDrawRecord,ship_walker_id)==0x84,"native formation draw fields");
_Static_assert(offsetof(PharaohWalkerQueryRecord,progress_on_tile)==0x39 &&
    offsetof(PharaohWalkerQueryRecord,is_on_road)==0x41 &&
    offsetof(PharaohWalkerQueryRecord,cross_country_x)==0x4a &&
    offsetof(PharaohWalkerQueryRecord,formation_id_60)==0x60 &&
    offsetof(PharaohWalkerQueryRecord,cart_offset_x)==0x6c &&
    offsetof(PharaohWalkerQueryRecord,current_height)==0x78 &&
    offsetof(PharaohWalkerQueryRecord,pyramid_walk_mode)==0x120,"native walker draw fields");

enum { DirectionCount=8,ProgressSteps=16,CrossTile=15,PixelX=2,BaseY=10,
    TallBaseY=12,RoadRaiseY=5,CartRaiseY=10,MissileShadowY=10,SailRaiseY=24 };

static const int8_t progress_x[128]={
 2,4,6,8,10,12,14,16,-12,-10,-8,-6,-4,-2,0,0,4,8,12,16,20,24,28,32,-24,-20,-16,-12,-8,-4,0,0,
 2,4,6,8,10,12,14,16,-12,-10,-8,-6,-4,-2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 -2,-4,-6,-8,-10,-12,-14,-16,12,10,8,6,4,2,0,0,-4,-8,-12,-16,-20,-24,-28,-32,24,20,16,12,8,4,0,0,
 -2,-4,-6,-8,-10,-12,-14,-16,12,10,8,6,4,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
static const int8_t progress_y[128]={
 -1,-2,-3,-4,-5,-6,-7,-8,6,5,4,3,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 1,2,3,4,5,6,7,8,-6,-5,-4,-3,-2,-1,0,0,2,4,6,8,10,12,14,16,-12,-10,-8,-6,-4,-2,0,0,
 1,2,3,4,5,6,7,8,-6,-5,-4,-3,-2,-1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 -1,-2,-3,-4,-5,-6,-7,-8,6,5,4,3,2,1,0,0,-2,-4,-6,-8,-10,-12,-14,-16,12,10,8,6,4,2,0,0};

static int tall(uint8_t t)
{switch(t){case 1:case 11:case 12:case 13:case 43:case 44:case 45:case 54:case 55:case 56:case 82:case 83:case 84:case 99:case 102:case 103:case 104:case 106:case 109:return 1;default:return 0;}}
static int flag_number(uint8_t f)
{if(f>=4&&f<=12)return f-3;if(f>=14&&f<=21)return f-13;if(f>=22&&f<=25)return f-21;if(f>=26&&f<=29)return f-25;if(f>=30&&f<=32)return f-29;if(f>=50&&f<=65)return f-49;return 0;}
static const PharaohSpriteImageRecord *image(const PharaohWalkerDrawContext *c,int32_t id)
{return pharaoh_sprite_image_record(c->draw->images,*c->draw->image_count,id);}
static void draw(PharaohWalkerDrawContext *c,int32_t id,int32_t x,int32_t y)
{(void)pharaoh_image_draw(c->draw,id,x,y);}

void pharaoh_city_draw_walker(PharaohWalkerDrawContext *c,int32_t id,int32_t x,int32_t y,uint32_t selection)
{
 const PharaohWalkerQueryRecord *f=&c->walkers[id];int32_t cart=f->cart_image_id,px=PixelX;
 int32_t py=tall(f->type)?TallBaseY+(f->is_on_road?RoadRaiseY:0):BaseY;
 int32_t direction=(int8_t)f->direction;if(direction>=DirectionCount){direction=(int8_t)f->previous_direction;if(direction>=DirectionCount)direction=0;}
 direction-=(int32_t)*c->orientation;if(direction<0)direction+=DirectionCount;
 if(*c->skip_draw==1)return;
 if(!f->use_cross_country){
  if(f->pyramid_walk_mode<1){int32_t step=direction*ProgressSteps+f->progress_on_tile;px+=progress_x[step];py+=progress_y[step]-f->current_height;}
  else c->operations->pyramid_walker_offset(c->operations->user_data,id,&x,&y);
 }else{
  int32_t cx=f->cross_country_x%CrossTile,cy=f->cross_country_y%CrossTile,orientation=(int32_t)*c->orientation;
  if(orientation==0){px+=2*(cx-cy);py+=cx+cy;}else if(orientation==2){px+=2*(cx+cy);py-=cx-cy;}
  else if(orientation==4){px-=2*(cx-cy);py-=cx+cy;}else if(orientation==6){px-=2*(cx+cy);py+=cx-cy;}
  py-=f->missile_damage;
  if(f->type==60 && c->walkers[f->building_id].type==62)py-=18+c->walkers[f->building_id].current_height;
 }
 py+=*c->tile_half_height;px+=*c->tile_half_width;
 int32_t cart_x=px-PixelX-image(c,cart)->sprite_offset_x;
 int32_t cart_y=py-CartRaiseY-image(c,cart)->sprite_offset_y;
 px-=image(c,f->image_id)->sprite_offset_x;py-=image(c,f->image_id)->sprite_offset_y;
 if(selection){if(selection==9999){if(!pharaoh_walker_matches_overlay(c->overlay,id))return;}else{if((uint32_t)id!=selection)return;*c->selected_x=px+x;*c->selected_y=py+y;}}
 int32_t fx=px+x,fy=py+y;if(!cart){draw(c,f->image_id,fx,fy);return;}
 switch(f->type){
 case 4:case 38:case 9:{int32_t facing=(int8_t)f->direction+(int32_t)*c->orientation;if(facing>=8)facing-=8;int32_t cx=f->cart_offset_x+cart_x+x,cy=f->cart_offset_y+cart_y+y;if(facing==3){draw(c,f->image_id,fx,fy);draw(c,cart,cx,cy);}else{draw(c,cart,cx,cy);draw(c,f->image_id,fx,fy);}return;}
 case 1:case 2:case 106:case 58:case 13:case 56:case 45:{int32_t cx=f->cart_offset_x+fx,cy=f->cart_offset_y+fy;if(f->cart_offset_y>=0){draw(c,f->image_id,fx,fy);draw(c,cart,cx,cy);}else{draw(c,cart,cx,cy);draw(c,f->image_id,fx,fy);}return;}
 case 14:{const PharaohFormationDrawRecord *formation=&c->formations[f->formation_id_60];if(formation->request_event_id>=0)return;int32_t icon=formation->legion_id+(int32_t)c->groups[396].first_image;if(formation->ship_walker_id){draw(c,icon,fx,fy-image(c,cart)->height);return;}draw(c,f->image_id,fx,fy);draw(c,(int32_t)c->groups[493].first_image-(formation->experience+3)/5+20,fx,fy);draw(c,cart,fx,fy-image(c,cart)->height);draw(c,icon,fx,fy-image(c,icon)->height-image(c,cart)->height);return;}
 case 36:{if(f->x<0||f->y<0)return;if(f->animation_frame&1)--fy;draw(c,f->image_id,fx,fy);draw(c,cart,fx,fy-image(c,cart)->height);int32_t n=flag_number(f->resource_id);if(n)c->operations->draw_number(c->operations->user_data,n,32," ",fx+9,fy+14,0x6b,0);return;}
 case 74:case 60:case 71:draw(c,cart,fx,fy+MissileShadowY);draw(c,f->image_id,fx,fy);return;
 case 77:case 78:draw(c,f->image_id,fx,fy);draw(c,cart,f->cart_offset_x+cart_x+x,f->cart_offset_y+cart_y-SailRaiseY+y);return;
 default:draw(c,f->image_id,fx,fy);return;
 }
}
