#include "menu_tail_ui.h"
#include "message_panel_input.h"
#include "menu_layout.h"
#define T_B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define T_W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define T_U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
extern void FUN_004d5d90(void);
extern void FUN_004d6090(int32_t kind,int32_t x,int32_t y,int32_t width,int32_t height,
                       int32_t relative_x,int32_t relative_y,int32_t origin_x,int32_t origin_y);
extern void FUN_00536c30(int32_t style,int32_t x,int32_t y,uint32_t width,uint32_t height,int32_t flags);

void FUN_00515c60(void)
{
    uint32_t row=T_U(0x00eadd84u)*16u-40u,advance=0,slot,buttons,y,x;
    T_B(0x00e38dc9u)=1;
    for(slot=0;slot<15;++slot){
        if(!T_U(0x00eadce8u+slot*4u))continue;
        if(!T_B(0x006fdc88u+slot))continue;
        buttons=T_U(0x005f78f0u+slot*4u);y=T_U(0x00eadd8cu);x=T_U(0x00eadd90u);
        FUN_00425bc0((int32_t)(x+advance+40u),(int32_t)(y+row),(const void *)(uintptr_t)buttons,1,(int32_t)(slot+40u));
        /* Native reloads all three values after the first callback. */
        buttons=T_U(0x005f78f0u+slot*4u);y=T_U(0x00eadd8cu);x=T_U(0x00eadd90u);
        if(FUN_00425990((int32_t)(x+advance+40u),(int32_t)(y+row),(const void *)(uintptr_t)buttons,1))return;
        advance+=27u;
    }
    T_B(0x00e38dc8u)=0;
}

void FUN_00516020(void)
{
    uint32_t offset=T_U(0x00eaac28u)*0x49d8u,flags=0,x,y;
    if(T_U(0x00ea5d68u+offset))return;
    if(T_W(0x00ea6084u+offset))flags=T_U(0x007056c8u)==1u;
    y=T_U(0x00eadd8cu);x=T_U(0x00eadd90u);
    FUN_004d63d0(0,x+82u,y+153u,300,20,flags,x,y);
}

/* Actual layout/drawing providers remain the production boundary. The linked
   layout initializer and raster implementation are not revalidated by this
   phase; finite substitutes exist only in test_boundaries.c and the oracle. */
__declspec(noinline) void FUN_004d63d0(uint32_t style,uint32_t x,uint32_t y,uint32_t width,uint32_t height,
                 uint32_t flags,uint32_t origin_x,uint32_t origin_y)
{
    uint32_t relative_x,relative_y,mode,address;
    if(!T_U(0x00e94d9cu))FUN_004d5d90();
    relative_y=y-origin_y;relative_x=x-origin_x;
    mode=(uint32_t)FUN_004d5d70(T_B(0x00e38ddau));
    address=(uint32_t)(uintptr_t)FUN_004d5fe0(3,(int32_t)mode,(int32_t)relative_x,(int32_t)relative_y);
    if(address){width=T_U(address+16u);height=T_U(address+20u);relative_x=T_U(address+8u);relative_y=T_U(address+12u);}
    FUN_004d6090(3,(int32_t)x,(int32_t)y,(int32_t)width,(int32_t)height,
                 (int32_t)relative_x,(int32_t)relative_y,(int32_t)origin_x,(int32_t)origin_y);
    FUN_00536c30((int32_t)style,(int32_t)(relative_x+origin_x),(int32_t)(relative_y+origin_y),width,height,(int32_t)flags);
}
