#include "pyramid_parts.h"
#include "road_place.h"
#include "building_graph.h"

/* Whole4F4160: special-part paving, per-cell phase mask, and final sprite. */
void restore_pyramid_update_finished_piece(int32_t building,int32_t rotation,int32_t kind)
{
    uint32_t r=restore_pyramid_record(building),descriptor=(PY_U(0x5e4ef4u)==5u?0x5f6d58u:0x5f6cd0u)+(uint32_t)kind*8u;
    uint32_t phase=PY_B(r+PY_RENDER_PHASE),i,tile,mask,table,count;int32_t piece;
    (void)rotation;
    if(phase==3u){
        table=PY_U(descriptor);count=PY_U(descriptor+4u);piece=(int32_t)(int16_t)PY_W(r+PY_PIECE_INDEX);
        for(i=0;PY_I(i)<PY_I(count);++i)if(PY_I(PY_U(table+i*8u))==piece){
            tile=PY_U(r+PY_CELL)+PY_U(table+i*8u+4u);PY_U(0xb7482cu+tile*4u)=PY_U(0xf5736cu);
            if(PY_B(0xacf85cu+tile)&1u)PY_B(0xb285ccu+tile)|=0x20u;
            return;
        }
    }else if(phase==4u){
        tile=PY_U(r+PY_CELL);mask=PY_B(r+PY_CELL_PHASE_MASK);
        for(i=0;i<4u;++i)if(mask&(1u<<i))PY_U(0xaf598cu+(tile+PY_U(0x56f880u+i*4u))*4u)&=0xffffff6eu;
    }else if(phase>=5u&&phase<=10u){
        for(i=0;i<4u;++i){
            tile=PY_U(r+PY_CELL)+PY_U(0x56f880u+i*4u);phase=PY_B(r+PY_RENDER_PHASE);
            (void)FUN_004f3f70(building,tile,PY_I(phase-((PY_B(r+PY_CELL_PHASE_MASK)&(1u<<i))?0u:1u)),1);
        }
    }else if(phase==11u){
        uint32_t orientation=(uint32_t)(PY_I(PY_U(0xbca2e0u))/2),image;
        piece=(int32_t)(int16_t)PY_W(r+PY_PIECE_INDEX);
        image=PY_U(0x56f840u+((uint32_t)piece+orientation*4u)*4u)+PY_U(0xf57364u);
        FUN_0047bf50((uint16_t)building,(int32_t)(int16_t)PY_W(r+PY_X),(int32_t)(int16_t)PY_W(r+PY_Y),
            (int32_t)PY_B(r+PY_SIZE),(int32_t)PY_B(r+PY_SIZE),image,8);
    }
}
void FUN_004f4160(int32_t building,int32_t rotation,int32_t kind)
{restore_pyramid_update_finished_piece(building,rotation,kind);}
