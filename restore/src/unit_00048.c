#include "editor_brush_validation.h"
#include "map_hover_geometry.h"
extern void FUN_00405f60(void);
void FUN_00405f80(void)
{
    uint32_t cell,delta;int32_t x,y;uint8_t blocked;
    EBV_W(0xbca2ccu,0);EBV_W(0xbca2c8u,0);
    cell=(uint32_t)FUN_00430790(0);EBV_W(0xbca2c4u,cell);
    if(!cell)return;
    delta=cell-EBV_U(0xbca2d8u);x=(int32_t)delta%228;y=(int32_t)delta/228;
    EBV_W(0xbca2c8u,x);blocked=*(volatile uint8_t *)(uintptr_t)0x605a5cu;EBV_W(0xbca2ccu,y);
    if(blocked)return;
    EBV_W(0xbca300u,cell);EBV_W(0xbca304u,x);EBV_W(0xbca308u,y);
    FUN_00405f60();
}
