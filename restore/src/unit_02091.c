#include "pyramid_parts.h"
#include "road_place.h"
#include "building_graph.h"

static void worker_offset(uint32_t record,uint32_t field,uint32_t position,uint32_t shape,uint32_t style)
{
    uint32_t index,table,dx,dy;
    if(style==2u){
        index=shape*6u+((position-1u)%6u>=3u?3u:0u);table=0x5f57c8u+index*16u;
        dx=PY_W(table);dy=PY_W(table+4u);
    }else{
        index=shape*6u+(style==1u?(position*2u-2u)%6u:(position-1u)%6u);
        table=0x5f5618u+index*8u;
        dx=PY_W(0x5f5b28u+shape*8u)+PY_W(table);
        dy=PY_W(0x5f5b2cu+shape*8u)+(style==1u?(uint32_t)(PY_I(PY_U(table+4u))/2):PY_W(table+4u));
    }
    PY_W(record+field)=(uint16_t)dx;PY_W(record+field+2u)=(uint16_t)dy;
}

/* Whole4EDF50: template shape, construction sprite and both worker offsets.
   Every fixed-address table is indexed in bytes exactly once. */
void restore_pyramid_update_piece_image(int32_t building,int32_t rotation,int32_t kind)
{
    uint32_t r=restore_pyramid_record(building),mode,shape,work_shape,work_image=0,image=0,base_sprite;
    uint32_t entry,step,layer,position,style,i,tile;int32_t piece;int ground_only=0;
    PY_U(r+PY_TRANSIENT_IMAGE)=0;
    if(PY_B(r+PY_RENDER_PHASE)>1u){restore_pyramid_update_finished_piece(building,rotation,kind);return;}
    mode=PY_U(0x5e4ef4u);piece=(int32_t)(int16_t)PY_W(r+PY_PIECE_INDEX);
    entry=(uint32_t)piece+(uint32_t)kind*(mode==5u?36u:144u);
    shape=PY_U((mode==5u?0x5e6578u:0x5e4ef8u)+entry*8u);
    if(PY_I(shape)<4){shape+=(uint32_t)rotation;if(PY_I(shape)>=4)shape-=4u;}
    else if(PY_I(shape)<8){shape+=(uint32_t)rotation;if(PY_I(shape)>=8)shape-=4u;}
    else if(shape==9u)shape=9u+(uint32_t)rotation;
    base_sprite=PY_U(0xf57334u);work_shape=mode==5u?0u:shape;
    if(mode!=5u){
        if(shape==5u)work_image=base_sprite+109u;
        else if(shape==6u)work_image=base_sprite+(mode==3u?112u:110u);
        else if(mode==3u){
            if(shape==1u)work_image=base_sprite+115u;
            else if(shape==2u)work_image=base_sprite+119u;
            else if(shape==3u)work_image=base_sprite+123u;
        }
    }
    step=PY_B(r+PY_SPRITE_STEP);layer=PY_B(r+PY_LAYER_INDEX);
    if(!PY_B(r+PY_RENDER_PHASE)||mode==5u){
        if(!step){
            if(piece<4&&mode!=5u)image=PY_U(0xf57364u)+PY_U(0x56f840u+((uint32_t)piece+(uint32_t)rotation*4u)*4u);
            else ground_only=1;
        }else if(layer<PY_B(r+PY_TEMPLATE_HEIGHT)||shape==8u){
            image=base_sprite+step+95u+(PY_B(0xacf85cu+PY_U(r+PY_CELL))&1u)*6u;
        }else if(mode==1u){
            image=PY_U(PY_B(0x5e68d8u+(uint32_t)kind*6u+layer)?0xf5738cu:0xf57334u)+step*8u-8u+shape;
        }else if(PY_I(shape)<8)image=base_sprite+step*8u-8u+shape;
        else image=base_sprite+109u+(shape&1u);
    }else if(mode==1u&&PY_B(0x5e68d8u+(uint32_t)kind*6u+layer)){
        image=PY_U(0xf5738cu)+(5u-step)*8u+shape;
    }else image=base_sprite+(11u-step)*8u+shape;
    if((!PY_B(r+PY_WORKER_POSITION_A)&&!PY_B(r+PY_WORKER_POSITION_B))||!work_image)PY_U(r+PY_WORKER_IMAGE)=0;
    else{
        style=mode==1u&&PY_B(0x5e68d8u+(uint32_t)kind*6u+PY_B(r+PY_LAYER_INDEX))?1u:(mode==3u?2u:0u);
        position=PY_B(r+PY_WORKER_POSITION_A);if(position)worker_offset(r,PY_WORKER_X_A,position,work_shape,style);
        position=PY_B(r+PY_WORKER_POSITION_B);if(position)worker_offset(r,PY_WORKER_X_B,position,work_shape,style);
        PY_U(r+PY_WORKER_IMAGE)=work_image;
    }
    if(!ground_only){
        int32_t size=(int32_t)PY_B(r+PY_SIZE);
        FUN_0047bf50((uint16_t)building,(int32_t)(int16_t)PY_W(r+PY_X),(int32_t)(int16_t)PY_W(r+PY_Y),size,size,image,8);return;
    }
    for(i=0;i<4u;++i){
        tile=PY_U(r+PY_CELL)+PY_U(0x56f880u+i*4u);
        PY_W(0xb4e6fcu+tile*2u)=(uint16_t)building;PY_U(0xaf598cu+tile*4u)=(PY_U(0xaf598cu+tile*4u)&0xefffffffu)|8u;
        (void)FUN_004f3f70(building,tile,PY_U(0x5e4ef4u)==5u?5:10,1);
    }
}
void FUN_004edf50(int32_t building,int32_t rotation,int32_t kind)
{restore_pyramid_update_piece_image(building,rotation,kind);}
