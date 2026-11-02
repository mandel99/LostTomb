#include "placement_preview.h"
#include <stdlib.h>
static void member_preview(void *self,const void *pieces,int32_t count,uint8_t special)
{
    uint32_t object=(uint32_t)(uintptr_t)self,first=(uint32_t)(uintptr_t)pieces;
    int32_t orientation=PV_I(0xbca2e0)/2,dx=pv_add(PV_I(0x70e45c),1),dy=PV_I(0x70e44c),screen_x,screen_y,cursor_x,cursor_y,i;
    uint32_t order=special?0x603258u:0x5e44d0u;
    FUN_004f0ba0(PV_I(0xbca304),PV_I(0xbca308),&screen_x,&screen_y);
    cursor_x=pv_sub(PV_I(0xbca300),PV_I(0xbca2d8))%228;cursor_y=pv_sub(PV_I(0xbca300),PV_I(0xbca2d8))/228;
    for(i=0;i<count;i=pv_add(i,1)) {
        uint32_t piece=first+(uint32_t)i*52u;int32_t base_x,base_y,row,column;uint8_t neighbor=0;
        if(special&&i==PV_I(object+0x50)) {
            static const uint32_t grids[]={0xaf55fc,0xaf5d1c,0xaf5988,0xaf5990};uint32_t k;
            for(k=0;k<4;++k) {
                int32_t tile=pv_add(pv_add(PV_I(piece),pv_mul(pv_add(PV_I(piece+4),cursor_y),228)),pv_add(PV_I(0xbca2d8),cursor_x));
                if(restore_placement_predicate(self,PV_U(grids[k]+(uint32_t)tile*4u),i))neighbor=1;
            }
        }
        base_x=pv_add(PV_I(piece),cursor_x);base_y=pv_add(PV_I(piece+4),cursor_y);
        PV_I(0xbc9768)=pv_add(pv_add(PV_I(0xbca2d8),pv_mul(base_y,228)),base_x);
        for(row=0;row<PV_I(piece+8);row=pv_add(row,1))for(column=0;column<PV_I(piece+8);column=pv_add(column,1)) {
            int32_t draw_x=pv_add(screen_x,PV_I(piece+20u+(uint32_t)orientation*4u));
            int32_t draw_y=pv_add(pv_add(screen_y,PV_I(0x70e44c)),PV_I(piece+36u+(uint32_t)orientation*4u));
            int32_t size,index=0,offset,tile;uint32_t mask;
            if(FUN_0046f9a0(pv_add(pv_add(PV_I(0xbca2e8),base_x),column),pv_add(pv_add(PV_I(0xbca2ec),base_y),row),3))continue;
            tile=pv_add(PV_I(0xbc9768),PV_I(0x5cc8c0u+(uint32_t)pv_add(pv_mul(row,6),column)*8u));
            mask=restore_placement_predicate(self,PV_U(0xaf598cu+(uint32_t)tile*4u),i)?0x1fe3u:0xf863u;
            if(special&&i==PV_I(object+0x50)&&!neighbor)mask=0xf863;
            size=PV_I(piece+8);offset=pv_sub(size,1);
            switch(PV_U(0xbca2e0)) {
            case 0:index=PV_I(order+(uint32_t)pv_add(row,pv_mul(column,6))*4u);break;
            case 2:index=PV_I(order-4u+(uint32_t)pv_add(pv_sub(pv_mul(row,6),column),size)*4u);draw_x=pv_add(draw_x,pv_mul(offset,dx));draw_y=pv_sub(draw_y,pv_mul(offset,dy));break;
            case 4:index=PV_I(order-28u+(uint32_t)pv_sub(pv_sub(pv_mul(size,7),pv_mul(column,6)),row)*4u);draw_y=pv_sub(draw_y,pv_mul(pv_mul(offset,dy),2));break;
            case 6:index=PV_I(order+(uint32_t)pv_add(column,pv_mul(pv_sub(pv_sub(size,row),1),6))*4u);draw_x=pv_sub(draw_x,pv_mul(offset,dx));draw_y=pv_sub(draw_y,pv_mul(offset,dy));break;
            }
            FUN_0053f4f0(PV_I(0xf56be4),pv_add(draw_x,PV_I(0x5cfcf0u+(uint32_t)index*8u)),pv_add(draw_y,pv_sub(PV_I(0x5cfcf4u+(uint32_t)index*8u),PV_I(0x70e44c))),mask);
        }
    }
}
void restore_placement_member_004e5490(void *self,const void *pieces,int32_t count){member_preview(self,pieces,count,0);}
void restore_placement_member_00545fc0(void *self,const void *pieces,int32_t count){member_preview(self,pieces,count,1);}
void restore_placement_member_adapter(uint32_t address,void *self)
{
    uint32_t object=(uint32_t)(uintptr_t)self;
    switch(address) {
    case 0x40d870:restore_placement_member_004e5490(self,(void *)(uintptr_t)(object+0x4c),0x19);break;
    case 0x401420:restore_placement_member_004e5490(self,(void *)(uintptr_t)(object+0x4c),0x10);break;
    case 0x416f90:restore_placement_member_004e5490(self,(void *)(uintptr_t)(object+0x4c),9);break;
    case 0x4e6610:restore_placement_member_004e5490(self,(void *)(uintptr_t)(object+0x4c),2);break;
    case 0x5456e0:restore_placement_member_00545fc0(self,(void *)(uintptr_t)(object+0x1e5c),0xa7);break;
    case 0x558b80:restore_placement_member_00545fc0(self,(void *)(uintptr_t)(object+0x1cbc),0x9e);break;
    case 0x555e20:restore_placement_member_00545fc0(self,(void *)(uintptr_t)(object+0x4ee0),0x1b5);break;
    case 0x54f410:restore_placement_member_00545fc0(self,(void *)(uintptr_t)(object+0x5c88),0x201);break;
    default:fprintf(stderr,"Unreconstructed placement member adapter %08lx.\n",(unsigned long)address);abort();
    }
}
