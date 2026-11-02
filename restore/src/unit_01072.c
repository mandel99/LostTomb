#include "terrain_redraw.h"
#include "flood_edges.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define I(v) ((int32_t)(uint32_t)(v))
extern uint8_t FUN_00477f60(void);

/* Whole478330. Cache the clipped rectangle and DWORD map origin before
   callbacks; read the DWORD row padding again at each row end. Dispatch
   the actual cell's flags and pass both coordinates on every used path. */
void FUN_00478330(int32_t xmin,int32_t ymin,int32_t xmax,int32_t ymax)
{
    uint32_t left=(uint32_t)xmin,top=(uint32_t)ymin,right=(uint32_t)xmax,bottom=(uint32_t)ymax;
    uint32_t width,height,step,cell,row,column,flags;
    if(xmin<0)left=0;if(ymin<0)top=0;
    width=U(0xbca2d0u);if(I(right)>=I(width))right=width-1u;
    height=U(0xbca2d4u);if(I(bottom)>=I(height))bottom=height-1u;
    step=width-right+left-1u;cell=U(0xbca2d8u)+top*228u+left;
    for(row=top;I(row)<=I(bottom);++row){
        for(column=left;I(column)<=I(right);++column,++cell){
            flags=U(0xaf598cu+cell*4u);
            if(flags&4u)(void)FUN_00477f60();
            else if(flags&0x10000u)(void)FUN_00477f70(I(column),I(row));
            else if(flags&0x22341a33u)(void)FUN_00478090(I(column),I(row));
            else (void)FUN_00478270(I(column),I(row));
        }
        cell+=U(0xbca2dcu)+step;
    }
}
