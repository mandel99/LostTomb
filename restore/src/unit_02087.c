#include "editor_ring.h"
/* Native 004ed0d0: four cdecl arguments; callback result is AL only. */
void FUN_004ed0d0(uint32_t center,int32_t radius,EditorRingVisitor visitor,uint32_t context)
{
    uint32_t raw_top=(uint32_t)((int32_t)center/228),raw_bottom=raw_top;
    uint32_t diagonal=(uint32_t)((int32_t)center%228)-raw_top,ring;
    for(ring=0;(int32_t)ring<radius;++ring){
        uint32_t left=diagonal+raw_top,right=diagonal+raw_bottom,top=raw_top,bottom=raw_bottom;
        uint32_t min_x,max_x,min_y,max_y,left_clipped,right_clipped,top_clipped,bottom_clipped,x,y,end;
        min_x=ERR32(0xbca2e8u);left_clipped=(int32_t)left<(int32_t)min_x;if(left_clipped)left=min_x;
        max_x=ERR32(0xbca2f0u);right_clipped=(int32_t)right>(int32_t)max_x;if(right_clipped)right=max_x;
        min_y=ERR32(0xbca2ecu);top_clipped=(int32_t)top<(int32_t)min_y;if(top_clipped)top=min_y;
        max_y=ERR32(0xbca2f4u);bottom_clipped=(int32_t)bottom>(int32_t)max_y;if(bottom_clipped)bottom=max_y;
        if(!(top_clipped&&bottom_clipped)){
            for(x=left;(int32_t)x<=(int32_t)right;++x){
                if(!top_clipped&&!visitor(x+top*228u,(int32_t)x,(int32_t)top,context))return;
                if(!bottom_clipped&&(int32_t)bottom>(int32_t)top&&!visitor(x+bottom*228u,(int32_t)x,(int32_t)bottom,context))return;
            }
        }
        if(!(left_clipped&&right_clipped)){
            end=bottom-1u;
            for(y=top+1u;(int32_t)y<=(int32_t)end;++y){
                if(!left_clipped&&!visitor(left+y*228u,(int32_t)left,(int32_t)y,context))return;
                if(!right_clipped&&(int32_t)right>(int32_t)left&&!visitor(right+y*228u,(int32_t)right,(int32_t)y,context))return;
            }
        }
        --raw_top;++raw_bottom;
    }
}
