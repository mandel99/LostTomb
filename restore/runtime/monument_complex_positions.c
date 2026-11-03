#include "monument_complex_positions.h"
#include <stdlib.h>
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define H(a) ((uint32_t)(int32_t)*(volatile int16_t *)(uintptr_t)(uint32_t)(a))
struct position_rule {int8_t dx,dy;uint8_t pointer_return;};
static const struct position_rule offsets[]={
 {2,1,0},
 {3,2,1},
 {3,1,0},
 {2,-1,1},
 {2,-1,0},
 {1,-1,1},
 {1,-1,0},
 {-1,0,1},
 {-1,0,0},
 {0,0,1},
 {3,0,0},
 {1,0,1},
 {3,2,0},
 {1,3,1},
 {0,1,0},
 {3,5,1},
 {-1,1,1},
 {0,-1,1},
 {3,-1,0},
 {2,0,0},
 {-1,1,0},
 {0,1,1},
 {1,3,0},
 {0,2,1},
 {5,4,0},
 {3,6,0},
};
static const uint8_t pieces_00545a30[]={0,1,2,3,4,5,6,7,8};
static const uint8_t pieces_00558f00[]={0,1,10,11,12,13,14,15,8,16};
static const uint8_t pieces_00556230[]={6,3,6,17,6,11,2,3,6,3,18,11,19,7,8,11,0,7,20,17,4};
static const uint8_t pieces_0054f8a0[]={0,1,0,11,0,11,2,21,22,23,12,1,24,21,25,11,0,7,8,7,20,11,19,21,22,21,22,7,20,21,22};
uint32_t restore_complex_position_query(uint32_t target,uint32_t building,uint32_t out_x,uint32_t out_y)
{
 const uint8_t *pieces=0;const struct position_rule *rule;uint32_t count=0,fallback=255;
 uint32_t r=building*264u,piece=H(0x7999eau+r),which,y;
 switch(target){
 case 0x545a30u:pieces=pieces_00545a30;count=9u;fallback=9u;break;
 case 0x558f00u:pieces=pieces_00558f00;count=10u;fallback=255u;break;
 case 0x556230u:pieces=pieces_00556230;count=21u;fallback=255u;break;
 case 0x54f8a0u:pieces=pieces_0054f8a0;count=31u;fallback=255u;break;
 default:abort();
 }
 which=piece<count?pieces[piece]:fallback;
 if(which==255u)return(r&0xffffff00u)|1u;
 rule=offsets+which;
 U(out_x)=H(0x7999deu+r)+(uint32_t)(int32_t)rule->dx;
 /* Output X can change the record Y; keep this second load after the store. */
 y=H(0x7999e0u+r)+(uint32_t)(int32_t)rule->dy;U(out_y)=y;
 return((rule->pointer_return?out_y:y)&0xffffff00u)|1u;
}
#define ENTRY(token) \
uint32_t restore_complex_position_##token(uint32_t building,uint32_t out_x,uint32_t out_y) \
{return restore_complex_position_query(0x##token##u,building,out_x,out_y);}
ENTRY(00545a30)
ENTRY(00558f00)
ENTRY(00556230)
ENTRY(0054f8a0)
