#include "world_offset_model.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
void restore_bandstand_images(int32_t direction,int32_t *first,int32_t *second)
{
 static const uint8_t offsets[4][2][2]={{{0,1},{3,2}},{{3,2},{1,0}},{{1,0},{2,3}},{{2,3},{0,1}}};
 uint32_t view=U(0xbca2e0u),d=(uint32_t)direction;
 if(view>6u||(view&1u)||d>1u)return;
 U((uint32_t)(uintptr_t)second)=U(0xf566fcu)+offsets[view/2u][d][0];
 /* Keep the second bank read after the first output store, including aliasing. */
 U((uint32_t)(uintptr_t)first)=U(0xf566fcu)+offsets[view/2u][d][1];
}
void FUN_004405c0(int32_t direction,int32_t *first,int32_t *second)
{restore_bandstand_images(direction,first,second);}
