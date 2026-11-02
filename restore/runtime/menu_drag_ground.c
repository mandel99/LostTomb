#include "ground.h"
#include "map_refresh.h"
#include "city_prepare.h"
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define H(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
extern uint32_t FUN_00477690(uint32_t mask,uint32_t cell);
extern uint32_t FUN_0047a1c0(int32_t x);
int32_t FUN_00481490(int32_t x,int32_t y,int32_t ring)
{
    uint32_t level,p,cell,j;int32_t count,delta;
    if(ring<=0)return 1;
    level=(uint32_t)ring;if(ring>=11)level=11;
    delta=(int16_t)H(0x5c9b52u+level*2u);
    count=(int16_t)H(0x5c9afeu+level*2u);
    p=0x70c8ecu+(uint32_t)delta*2u;
    cell=U(0xbca2d8u)+(uint32_t)y*228u+(uint32_t)x;U(0xbc9768u)=cell;
    if(count>0)for(j=0;(int32_t)j<count;++j){
        if(!FUN_00481aa0(x,y,1,(int32_t)level,(int32_t)j)){
            cell=U(0xbc9768u);delta=(int16_t)H(p);
            if(!(B(0xaf598cu+(cell+(uint32_t)delta)*4u)&3u))return 0;
        }
        p+=2u;
    }
    return 1;
}
void FUN_00486600(uint32_t cell)
{
    uint32_t flags=U(0xaf598cu+cell*4u),fertility,band,variation,base,bound;
    int32_t relative,x,y,ring;
    if((flags&1u)&&!(flags&0x80000u)){
        fertility=B(0xa509bcu+cell);if(fertility>100u)fertility=100u;band=fertility/34u;
        if(B(0xa43eacu+cell)!=255u){variation=B(0xacf85cu+cell)&7u;base=U(0xf56b9cu);U(0xb7482cu+cell*4u)=base+variation+band*8u;return;}
        relative=(int32_t)(cell-U(0xbca2d8u));x=relative%228;y=relative/228;
        for(ring=3;;--ring)if(FUN_00481490(x,y,ring)||ring==0)break;
        if(ring<2)band<<=4;else{band=48;ring-=2;}
        variation=B(0xacf85cu+cell)&7u;base=U(0xf56b5cu);U(0xb7482cu+cell*4u)=base+variation+(uint32_t)ring*8u+band;return;
    }
    if(!(flags&0x40000u)||(flags&0x80000u)||FUN_00477690(0x40000u,cell)!=8u)return;
    relative=(int32_t)(cell-U(0xbca2d8u));x=relative%228;
    /* The native caller also pushes an unused quotient. The leaf consumes x only. */
    bound=(uint8_t)FUN_0047a1c0(x);
    if(B(0xa43eacu+cell)==255u){
        variation=B(0xacf85cu+cell)&7u;base=U(bound?0xf57244u:0xf56c3cu);
    }else{variation=B(0xacf85cu+cell)&7u;base=U(bound?0xf5723cu:0xf56b94u);}
    U(0xb7482cu+cell*4u)=variation+base;
}
void FUN_00479770(int32_t x_in,int32_t y_in,int32_t mode)
{
    uint32_t dy=U(0xbca2ecu),dx=U(0xbca2e8u),x=(uint32_t)x_in,y=(uint32_t)y_in,offset,cell,flags,access,sprite;
    uint32_t left,top,columns=3,rows=3,width,height,endrow,endcol,col,row;
    if(!(uint8_t)FUN_0046f930((int32_t)(dx+x),(int32_t)(dy+y)))return;
    offset=U(0xbca2d8u);cell=offset+y*228u+x;
    flags=U(0xaf598cu+cell*4u);U(0xaf598cu+cell*4u)=mode==1?flags|1u:flags&0xfffffffeu;
    left=x-1u;top=y-1u;
    if((int32_t)left<0){columns=left+3u;left=0;}
    else{width=U(0xbca2d0u);if((int32_t)(left+3u)>(int32_t)width)columns=width-left;}
    if((int32_t)top<0){rows=top+3u;top=0;}
    else{height=U(0xbca2d4u);if((int32_t)(top+3u)>(int32_t)height)rows=height-top;}
    endrow=top+rows;U(0xbc9760u)=top;cell=offset+top*228u+left;U(0xbc9768u)=cell;
    if((int32_t)top>=(int32_t)endrow)return;
    endcol=left+columns;
    do{
        col=left;U(0xbc9764u)=col;
        if((int32_t)col<(int32_t)endcol)do{
            flags=U(0xaf598cu+cell*4u);
            if((flags&1u)&&!(flags&0x600u)){
                FUN_00486600(cell);cell=U(0xbc9768u);access=B(0xb285ccu+cell);sprite=B(0xb67d1cu+cell);
                B(0xb285ccu+cell)=(uint8_t)(access&0xf0u);B(0xb67d1cu+cell)=(uint8_t)(sprite|64u);B(0xae8e7cu+cell)=0;
            }
            col=U(0xbc9764u)+1u;++cell;U(0xbc9764u)=col;U(0xbc9768u)=cell;
        }while((int32_t)col<(int32_t)endcol);
        row=U(0xbc9760u)+1u;U(0xbc9760u)=row;row=U(0xbc9760u);cell+=228u-columns;U(0xbc9768u)=cell;
    }while((int32_t)row<(int32_t)endrow);
}
void FUN_00475bf0(int32_t left_in,int32_t top_in,int32_t right_in,int32_t bottom_in)
{
    uint32_t left=(uint32_t)left_in,top=(uint32_t)top_in,right=(uint32_t)right_in,bottom=(uint32_t)bottom_in,width,height,cell,stride,row,col,flags;
    if(left_in<0)left=0;if(top_in<0)top=0;
    width=U(0xbca2d0u);if((int32_t)right>=(int32_t)width)right=width-1u;
    height=U(0xbca2d4u);if((int32_t)bottom>=(int32_t)height)bottom=height-1u;
    cell=U(0xbca2d8u);stride=width-right+left-1u;cell+=top*228u+left;row=top;
    if((int32_t)row>(int32_t)bottom)return;
    do{
        if((int32_t)left<=(int32_t)right){
            col=left;
            do{flags=U(0xaf598cu+cell*4u);if((flags&1u)&&!(flags&0x600u))FUN_00479770((int32_t)col,(int32_t)row,1);++cell;++col;}while((int32_t)col<=(int32_t)right);
        }
        cell+=U(0xbca2dcu)+stride;++row;
    }while((int32_t)row<=(int32_t)bottom);
}
