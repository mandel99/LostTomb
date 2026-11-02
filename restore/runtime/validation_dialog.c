#include "validation_dialog.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
extern void FUN_00536820(int32_t,int32_t,int32_t,int32_t);
extern void FUN_004d6250(uint32_t,uint32_t,int32_t,int32_t,uint32_t,
                       uint32_t,uint32_t,int32_t,int32_t);

/* Whole 5344C0..53453E, 46 authenticated instructions. Native cdecl1/RET0.
   The actual existing panel, layout and text providers remain composed. */
void FUN_005344c0(int32_t message)
{
 uint32_t y=U(0xe38da4u),x=U(0xe38da8u),top=y+32u,left=x+16u;
 uint32_t anchor_y,anchor_x,font;
 FUN_00536820((int32_t)left,(int32_t)top,35,4);
 /* Native keeps original panel coordinates in ESI/EDI across calls, but
    reloads the live screen anchors and font for each text-layout call. */
 anchor_y=U(0xe38da4u);anchor_x=U(0xe38da8u);font=U(0xf56f84u);
 FUN_004d6250(19,(uint32_t)message-2u,(int32_t)(left+3u),
              (int32_t)(top+16u),560,font,0,(int32_t)anchor_x,(int32_t)anchor_y);
 anchor_y=U(0xe38da4u);anchor_x=U(0xe38da8u);font=U(0xf56f8cu);
 FUN_004d6250(13,1,(int32_t)left,(int32_t)(top+36u),560,font,0,
              (int32_t)anchor_x,(int32_t)anchor_y);
}
