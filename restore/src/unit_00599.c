#include "placement_preview.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define I(v) ((int32_t)(uint32_t)(v))

static int sphinx_blocked(uint32_t tile)
{
    uint32_t flags=U(0xaf598cu+tile*4u)&0xeefed77fu;
    uint8_t outside=FUN_0046f9a0(I(tile)%228,I(tile)/228,3);
    return outside!=0||flags!=0;
}
static void sphinx_offset(uint32_t part,uint32_t rotation,uint32_t sx,uint32_t sy,
                          uint32_t height,int valid,uint32_t adjust,uint32_t *x,uint32_t *y)
{
    uint32_t half=part==1u?0x7756acu:0x7756ccu;
    uint32_t coeff=part==1u?0x5d28b4u:0x5d28d4u,shift=part==1u?0x5d28f4u:0x5d2904u;
    *x=U(half+rotation*8u)*U(0x70e45cu)+U(coeff+rotation*8u)*U(0x70e458u)+U(shift+rotation*4u)+sx+adjust;
    *y=U(coeff+rotation*8u+4u)*U(0x70e448u)+sy+(U(half+rotation*8u+4u)-(valid?1u:0u))*height;
}
static void sphinx_pair(uint32_t base,uint32_t offset,uint32_t x,uint32_t y)
{
    (void)FUN_0053e590(I(U(base)+offset),I(x),I(y));
    (void)FUN_0053f250(I(U(base)+offset),I(x),I(y));
}

/* Whole441490: three Sphinx footprint groups. Preserve DWORD geometry and
   count fields, signed map divisions, cached pre-query terrain flags and the
   native painter order. Tint alone is a WORD; selection/profile are DWORDs. */
void FUN_00441490(void)
{
    int32_t screen_x=0,screen_y=0,count;
    uint32_t sx,sy,height,rotation,base,table,group,i,x,y,px,py,player;
    int all_invalid,blocked;
    FUN_004f0ba0(I(U(0xbca304u)),I(U(0xbca308u)),&screen_x,&screen_y);
    player=U(0xeaac28u)*0x49d8u;height=U(0x70e44cu);
    sx=(uint32_t)screen_x;sy=(uint32_t)screen_y+height;
    all_invalid=W(0xea5baau+player)!=0||I(U(0xea187cu+player))<=-5000;blocked=all_invalid;
    for(group=0;group<3u;++group){
        rotation=(uint32_t)(I(U(0xbca2e0u))/2);base=U(0xbca300u);
        if(group)base+=U((group==1u?0x5cdfa8u:0x5cdfb8u)+rotation*4u);
        table=0x5cfe10u+rotation*144u;count=I(U(0x5cfcecu));
        for(i=0;I(i)<count;++i)if(sphinx_blocked(base+U(table+i*4u)))blocked=1;
    }
    rotation=(uint32_t)(I(U(0xbca2e0u))/2);height=U(0x70e44cu);
    if(blocked){
        for(group=0;group<3u;++group){
            rotation=(uint32_t)(I(U(0xbca2e0u))/2);base=U(0xbca300u);
            if(group){
                base+=U((group==1u?0x5cdfa8u:0x5cdfb8u)+rotation*4u);
                sphinx_offset(group,rotation,sx,sy,height,0,0,&x,&y);
            }else{x=sx;y=sy;}
            table=0x5cfe10u+rotation*144u;count=I(U(0x5cfcecu));
            for(i=0;I(i)<count;++i){
                px=U(0x5cfcf0u+i*8u)+x;py=U(0x5cfcf4u+i*8u)-height+y;
                /* The query executes even when a scenario/debt rule forces
                   every tile red. Its low-byte result is consumed only after
                   the coordinates and terrain flags have been captured. */
                int cell_invalid=sphinx_blocked(base+U(table+i*4u));
                (void)FUN_0053f4f0(I(U(0xf56be4u)),I(px),I(py),(cell_invalid||all_invalid)?0xfffff863u:0x1fe3u);
                height=U(0x70e44cu);
            }
        }
        return;
    }
    W(0x70e120u)=0x1fe3u;
    if(rotation==0u||rotation==2u||rotation==3u){
        uint32_t first=rotation==2u?0xf56d64u:0xf56d54u;
        uint32_t last=rotation==2u?0xf56d54u:0xf56d64u,extra=rotation==3u?0u:1u;
        sphinx_pair(first,extra,sx,sy-height);
        sphinx_offset(1,rotation,sx,sy,U(0x70e44cu),1,rotation==3u?UINT32_MAX:0u,&x,&y);
        sphinx_pair(0xf56d5cu,extra,x,y);
        sphinx_offset(2,rotation,sx,sy,U(0x70e44cu),1,rotation==3u?UINT32_MAX-1u:0u,&x,&y);
        sphinx_pair(last,extra,x,y);
    }else{
        sphinx_offset(2,rotation,sx,sy,height,1,0,&x,&y);sphinx_pair(0xf56d54u,0,x,y);
        sphinx_offset(1,rotation,sx,sy,U(0x70e44cu),1,0,&x,&y);sphinx_pair(0xf56d5cu,0,x,y);
        sphinx_pair(0xf56d64u,0,sx,sy-U(0x70e44cu));
    }
    W(0x70e120u)=0;
}
