#include "pharaoh/rendering/building_animations.h"
#include <stddef.h>

_Static_assert(offsetof(PharaohSpriteImageRecord,sprite_offset_x)==0x22 && offsetof(PharaohSpriteImageRecord,sprite_offset_y)==0x24,"native sprite placement words");
_Static_assert(offsetof(PharaohWalkerQueryRecord,state)==0x0e && offsetof(PharaohWalkerQueryRecord,action_state)==0x38 && offsetof(PharaohWalkerQueryRecord,type)==0x0a,"native walker fields relative to BCD5F8");

/* Group numbers are indices in the native image collection, not image IDs. */
enum WorldAnimationGroup {
    WorldGroupFishingWharf = 515,
    WorldGroupDanceSchoolAnimation = 583,
    WorldGroupBoothAnimation = 584,
    WorldGroupBandstandGateAnimation = 586,
    WorldGroupBandstandStageAnimation = 587,
    WorldGroupShipwrightAnimation = 622,
    WorldGroupFishingWharfBoatAnimation = 623,
    WorldGroupMineAnimation = 625,
    WorldGroupQuarryFirstAnimation = 626,
    WorldGroupQuarrySecondAnimation = 627,
    WorldGroupShipwrightTransportAnimation = 629,
    WorldGroupShipwrightFishingAnimation = 631,
    WorldGroupFishingWharfIdleAnimation = 632
};
enum WorldAnimationHolder {
    WorldHolderIdMask = 4095,
    WorldHolderKindMask = 61440,
    WorldHolderWalkerType = 4096,
    WorldHolderWalkerId = 8192
};
typedef struct { int32_t x,y; } WorldAnimationOffset;

static int32_t animation_image(const PharaohBuildingAnimationContext *c,int32_t group,int32_t frame)
{
    return (int32_t)((uint32_t)pharaoh_sprite_group_first_image(c->groups,group)+(uint32_t)frame-1u);
}
int32_t pharaoh_building_draw_sprite_at_offset(const PharaohBuildingAnimationContext *c,int32_t image,int32_t x,int32_t y,int32_t dx,int32_t dy)
{
    const PharaohSpriteImageRecord *header=pharaoh_sprite_image_record(c->draw->images,*c->draw->image_count,image);
    int32_t drawX=(int32_t)((uint32_t)x+(uint32_t)dx-(uint32_t)(int32_t)header->sprite_offset_x);
    int32_t drawY=(int32_t)((uint32_t)y+(uint32_t)dy-(uint32_t)(int32_t)header->sprite_offset_y);
    return pharaoh_image_draw(c->draw,image,drawX,drawY);
}
void pharaoh_building_draw_shipwright_animation(const PharaohBuildingAnimationContext *c,int32_t image,int32_t frame,int32_t x,int32_t y,uint32_t holder)
{
    static const WorldAnimationOffset places[4]={{110,20},{85,20},{85,20},{100,10}};
    int32_t group=WorldGroupShipwrightAnimation;
    uint32_t kind=holder&WorldHolderKindMask,id=holder&WorldHolderIdMask;
    const WorldAnimationOffset *place;
    if(!holder)return;
    if(kind==WorldHolderWalkerType){
        if(id==25)group=WorldGroupShipwrightFishingAnimation;
        else if(id==77)group=WorldGroupShipwrightTransportAnimation;
    }else if(kind==WorldHolderWalkerId){
        const PharaohWalkerQueryRecord *walker=&c->walkers[id];
        if(walker->state!=1||walker->action_state==4)return;
        if(walker->type==77)group=WorldGroupShipwrightTransportAnimation;
    }
    place=&places[(int32_t)((uint32_t)image-(uint32_t)pharaoh_sprite_group_first_image(c->groups,523))];
    (void)pharaoh_building_draw_sprite_at_offset(c,animation_image(c,group,frame),x,y,place->x,place->y);
}
int32_t pharaoh_building_draw_fishing_wharf_animation(const PharaohBuildingAnimationContext *c,int32_t image,int32_t frame,int32_t x,int32_t y,int32_t boat)
{
    static const WorldAnimationOffset places[8]={{75,10},{78,7},{62,18},{48,8},{75,10},{78,7},{62,18},{48,8}};
    uint32_t view=(uint32_t)image-(uint32_t)pharaoh_sprite_group_first_image(c->groups,WorldGroupFishingWharf);
    uint32_t imageView=(view-1u)&3u;
    int32_t group=WorldGroupFishingWharfIdleAnimation,drawImage;
    if(boat&&c->walkers[boat].action_state==12){
        group=WorldGroupFishingWharfBoatAnimation;
        view+=4u;
    }
    drawImage=(int32_t)((uint32_t)pharaoh_sprite_group_first_image(c->groups,group)+(uint32_t)frame*4u-4u+imageView);
    return pharaoh_building_draw_sprite_at_offset(c,drawImage,x,y,places[view].x,places[view].y);
}
int32_t pharaoh_building_draw_mine_animation(const PharaohBuildingAnimationContext *c,int32_t image,int32_t frame,int32_t x,int32_t y)
{
    (void)image;
    return pharaoh_building_draw_sprite_at_offset(c,animation_image(c,WorldGroupMineAnimation,frame),x,y,54,15);
}
int32_t pharaoh_building_draw_quarry_animation(const PharaohBuildingAnimationContext *c,int32_t image,int32_t frame,int32_t x,int32_t y)
{
    (void)image;
    (void)pharaoh_building_draw_sprite_at_offset(c,animation_image(c,WorldGroupQuarryFirstAnimation,frame),x,y,90,17);
    return pharaoh_building_draw_sprite_at_offset(c,animation_image(c,WorldGroupQuarrySecondAnimation,frame),x,y,40,20);
}
int32_t pharaoh_building_draw_bandstand_animation(const PharaohBuildingAnimationContext *c,int32_t image,int32_t frame,int32_t x,int32_t y)
{
    if((uint32_t)image==pharaoh_sprite_group_first_image(c->groups,361))
        return pharaoh_building_draw_sprite_at_offset(c,animation_image(c,WorldGroupBandstandGateAnimation,frame),x,y,52,-3);
    return pharaoh_building_draw_sprite_at_offset(c,animation_image(c,WorldGroupBandstandStageAnimation,frame),x,y,20,0);
}
int32_t pharaoh_building_draw_conservatory_animation(const PharaohBuildingAnimationContext *c,int32_t image,int32_t frame,int32_t x,int32_t y)
{
    (void)image;
    return pharaoh_building_draw_sprite_at_offset(c,animation_image(c,WorldGroupBandstandStageAnimation,frame),x,y,82,14);
}
int32_t pharaoh_building_draw_booth_animation(const PharaohBuildingAnimationContext *c,int32_t image,int32_t frame,int32_t x,int32_t y)
{
    (void)image;
    return pharaoh_building_draw_sprite_at_offset(c,animation_image(c,WorldGroupBoothAnimation,frame),x,y,29,14);
}
int32_t pharaoh_building_draw_pavilion_animation(const PharaohBuildingAnimationContext *c,int32_t image,int32_t frame,int32_t x,int32_t y)
{
    (void)image;
    return pharaoh_building_draw_sprite_at_offset(c,animation_image(c,WorldGroupDanceSchoolAnimation,frame),x,y,64,0);
}
int32_t pharaoh_building_draw_dance_school_animation(const PharaohBuildingAnimationContext *c,int32_t image,int32_t frame,int32_t x,int32_t y)
{
    (void)image;
    return pharaoh_building_draw_sprite_at_offset(c,animation_image(c,WorldGroupDanceSchoolAnimation,frame),x,y,108,1);
}
