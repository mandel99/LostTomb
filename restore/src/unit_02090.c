#include "editor_brush_validation.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define I(v) ((int32_t)(uint32_t)(v))

/* Whole4EDBB0: pyramid construction preview. Both layout families contain
   sixteen-byte records, with36 or144 records per orientation respectively.
   Geometry, selection, table indices and map cells remain full DWORDs. */
void FUN_004edbb0(void)
{
    int32_t sx=0,sy=0,tx=0,ty=0;
    uint32_t failure_flags,allowed,color,rotation,origin,part,mode,tool,record,index;
    uint32_t delta,cell,px,py,table,i,flags,mask,span,start;
    int32_t count,wx,wy;
    int debt=I(U(0xea187cu+U(0xeaac28u)*0x49d8u))<=-5000;
    allowed=FUN_004ecec0(I(U(0xbca300u)),I(U(0x5e6970u)),&failure_flags);
    color=(uint8_t)allowed?0x1fe3u:0xf863u;
    rotation=(uint32_t)(I(U(0xbca2e0u))/2);
    FUN_004f0ba0(I(U(0xbca304u)),I(U(0xbca308u)),&sx,&sy);
    origin=U(0xbca300u);
    for(part=0;;++part){
        mode=U(0x5e4ef4u);tool=U(0x5e6970u);
        count=I(U((mode==5u?0x56f7ecu:0x56f7d8u)+tool*4u));
        if(I(part)>=count)break;
        if(mode==5u){
            record=((tool*4u+rotation)*36u+part)*16u;
            index=U(0x5f1d7cu+record);
            delta=U(0x56fd10u+index*8u)+U(0x56fd14u+index*8u)*228u;
            px=U(0x5f1d74u+record)*(U(0x70e458u)+2u)+(uint32_t)sx;
            py=U(0x5f1d78u+record)*U(0x70e448u)+(uint32_t)sy;
        }else{
            record=((tool*4u+rotation)*144u+part)*16u;
            index=U(0x5e697cu+record);delta=U(0x56fe30u+index*4u);
            px=U(0x5e6974u+record)*(U(0x70e458u)+2u)+(uint32_t)sx;
            py=U(0x5e6978u+record)*U(0x70e448u)+(uint32_t)sy;
        }
        cell=origin+delta;count=I(U(0x5cfcdcu));
        if(count>0){
            wx=I(cell)%228;wy=I(cell)/228;table=0x5cfe10u+rotation*144u;
            for(i=0;I(i)<count;++i){
                uint32_t x=U(0x5cfcf0u+i*8u)+px,y=U(0x5cfcf4u+i*8u)+py;
                flags=U(0xaf598cu+(cell+U(table+i*4u))*4u)&0xfeffd76eu;
                /* Native queries use the piece origin for every subtile,
                   while terrain tests use each individual cell. */
                allowed=FUN_0046f9a0(wx,wy,3);
                mask=((uint8_t)allowed||flags||debt)?0xfffff863u:0x1fe3u;
                (void)FUN_0053f4f0(I(U(0xf56be4u)),I(x),I(y),mask);
            }
            origin=U(0xbca300u);
        }
    }
    if(FUN_004ecea0(I(U(0x5e6970u)))){
        delta=U(0x5f5d08u+U(0x5e6970u)*4u)+U(0xbca300u)-U(0xbca2d8u);
        FUN_004ed360(I(delta)%228,I(delta)/228,1,color&0xffffu);
    }
    mode=U(0x5e4ef4u);tool=U(0x5e6970u);
    span=U((mode==5u?0x5700a8u:0x570094u)+tool*4u);
    start=U((mode==5u?0x5700c8u:0x5700b4u)+tool*4u);
    for(i=0;I(i)<I(span);++i){
        cell=U(0xbca300u)+start+i;
        if(U(0xaf598cu+cell*4u)&0xfeffd72eu){
            FUN_004f0ba0(I((uint32_t)(I(cell)%228)-U(0xbca2e8u)),
                I((uint32_t)(I(cell)/228)-U(0xbca2ecu)),&tx,&ty);
            (void)FUN_0053f4f0(I(U(0xf56be4u)),tx,ty,0xfffff863u);
        }
    }
}
