#include "menu_full_input_routes.h"
#include "menu_full_input_numeric.h"
#include "menu_route_capacity.h"
#include <stddef.h>
#define MR_U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define MR_B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define MR_W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define MR_S(a) ((int32_t)*(volatile int16_t *)(uintptr_t)(uint32_t)(a))
extern void *FUN_00561a1d(uint32_t bytes);
extern void FUN_00561a2b(uint32_t allocation);

int32_t MIR_PUBLIC(restore_menu_route_last)(uint32_t self){return(int32_t)MR_B(self+0x141u)-1;}
uint32_t MIR_PUBLIC(restore_menu_route_clear)(uint32_t self){MR_B(self+0x141u)=0;MR_U(self+0x134u)=0;return 1;}
void MIR_PUBLIC(restore_menu_route_mark)(uint32_t self,uint32_t value){MR_B(self+0x142u)=(uint8_t)value;}
int32_t MIR_PUBLIC(restore_menu_route_start_city)(uint32_t self){return MR_S(self+0x13cu);}
int32_t MIR_PUBLIC(restore_menu_route_end_city)(uint32_t self){return MR_S(self+0x13eu);}

uint8_t MIR_PUBLIC(restore_menu_route_endpoints)(uint32_t self)
{
    uint32_t count,row=0,city=0x77f748u,type,asset0,asset1,asset_limit;
    int32_t left,top,right,bottom,x,y,width,height;
    MR_W(self+0x13eu)=0xffffu;
    MR_W(self+0x13cu)=0xffffu;
    count=MR_B(self+0x141u);
    if(count==0)return 0;
    do{
        if(MR_B(city+1u)!=0&&MR_B(city)==1){
            asset_limit=MR_U(0x126517cu);type=MR_W(city+12u);
            asset0=(int32_t)type<(int32_t)asset_limit?type:0;
            width=MR_S(0x10d6a0cu+asset0*64u);
            asset1=(int32_t)type<(int32_t)asset_limit?type:0;
            top=MR_S(city+6u);left=MR_S(city+4u);
            right=(int32_t)((uint32_t)left+(uint32_t)width);
            height=MR_S(0x10d6a0eu+asset1*64u);
            x=MR_S(self+8u);
            bottom=(int32_t)((uint32_t)top+(uint32_t)height);
            if(x>=left&&x<=right){y=MR_S(self+10u);if(y>=top&&y<=bottom)MR_W(self+0x13cu)=(uint16_t)row;}
            x=MR_S(self+count*6u+2u);
            if(x>=left&&x<=right){y=MR_S(self+count*6u+4u);if(y>=top&&y<=bottom)MR_W(self+0x13eu)=(uint16_t)row;}
        }
        city+=98u;++row;
    }while((int32_t)city<0x7843d8);
    return 1;
}

int32_t MIR_PUBLIC(restore_menu_route_hit)(uint32_t self,int32_t x,int32_t y,uint8_t *vertex)
{
    uint32_t count=MR_B(self+0x141u),index=0,cursor=self+8u,allocation,samples;
    int32_t left=(int32_t)((uint32_t)x-8u),right=(int32_t)((uint32_t)x+8u);
    int32_t top=(int32_t)((uint32_t)y-8u),bottom=(int32_t)((uint32_t)y+8u),px,py;
    while(index<count){
        px=MR_S(cursor);
        if(px>=left&&px<=right){py=MR_S(cursor+2u);if(py>=top&&py<=bottom){*(volatile uint8_t *)vertex=1;return(int32_t)index;}}
        ++index;cursor+=6u;
    }
    *(volatile uint8_t *)vertex=0;
    samples=MR_U(self+0x134u);
    if((int32_t)samples<=0)return -1;
    allocation=MR_U(self+4u);cursor=allocation;index=0;
    do{
        px=MR_S(cursor);
        if(px>=left&&px<=right){py=MR_S(cursor+2u);if(py>=top&&py<=bottom)return MR_B(allocation+index*6u+4u);}
        ++index;cursor+=6u;
    }while((int32_t)index<(int32_t)samples);
    return -1;
}

uint8_t MIR_PUBLIC(restore_menu_route_delete)(uint32_t self,int32_t index)
{
    uint32_t count,vertex,destination,source,need=0,new_capacity,old,generated=0,segment,point;
    uint32_t length,xratio,yratio,distance,local_count,offset,required;uint8_t raw[10];uint64_t converted;
    int32_t x0,y0,x1,y1,dx,dy,adx,ady,amount;
    if(index<0)return 0;
    count=MR_B(self+0x141u);
    if(index>=(int32_t)count)return 0;
    MR_B(self+0x141u)=(uint8_t)(count-1u);
    vertex=(uint32_t)index;destination=self+vertex*6u+8u;
    while((int32_t)vertex<(int32_t)MR_B(self+0x141u)){
        source=destination+6u;++vertex;
        MR_U(destination)=MR_U(source);
        MR_W(destination+4u)=MR_W(source+4u);
        destination+=6u;
    }
    count=MR_B(self+0x141u);point=self+8u;
    for(vertex=1;(int32_t)vertex<(int32_t)count;++vertex){
        x0=MR_S(point);x1=MR_S(point+6u);dx=x0-x1;adx=dx>-dx?dx:-dx;
        y0=MR_S(point+2u);y1=MR_S(point+8u);dy=y0-y1;ady=dy>-dy?dy:-dy;
        amount=adx>ady?adx:ady;need+=(uint32_t)amount+1u;point+=6u;
        /* The native direction byte is private and never read in this helper. */
    }
    if(!restore_menu_route_required_samples((const uint8_t *)(uintptr_t)(self+8u),count,
                                          MR_B(self),need,&required))return 0;
    old=MR_U(self+0x138u);
    if(((int32_t)need>(int32_t)old&&(int32_t)old<(int32_t)(need+50u))||required>old){
        new_capacity=((int32_t)need>(int32_t)old&&(int32_t)old<(int32_t)(need+50u))?need+50u:old;
        if(required>new_capacity)new_capacity=required;
        old=MR_U(self+4u);
        if(old!=0){FUN_00561a2b(old);MR_U(self+4u)=0;}
        MR_U(self+4u)=(uint32_t)(uintptr_t)FUN_00561a1d(new_capacity*6u);
        if(MR_U(self+4u)==0){MR_U(self+0x138u)=0;MR_U(self+0x134u)=0;return 0;}
        MR_U(self+0x138u)=new_capacity;
    }
    count=MR_B(self+0x141u);MR_U(self+0x134u)=need;
    if(count>1){
        point=self+8u;segment=1;
        do{
            if((int32_t)generated>(int32_t)MR_U(self+0x134u))return 0;
            x0=MR_S(point);x1=MR_S(point+6u);y0=MR_S(point+2u);y1=MR_S(point+8u);
            dx=x1-x0;dy=y1-y0;local_count=0;
            length=restore_menu_input_length32(dx,dy);
            if((length&0x7fffffffu)!=0&&MR_B(self)!=0){
                yratio=restore_menu_input_ratio32(dy,length);
                xratio=restore_menu_input_ratio32(dx,length);
                distance=0;
                if(restore_menu_input_compare32(0,length)&0x4100u){
                    offset=generated*6u;
                    do{
                        restore_menu_input_coordinate80(x0,xratio,(int32_t)distance,raw);
                        converted=restore_menu_input_trunc80(raw);
                        MR_W(MR_U(self+4u)+offset)=(uint16_t)converted;
                        restore_menu_input_coordinate80(y0,yratio,(int32_t)distance,raw);
                        converted=restore_menu_input_trunc80(raw);
                        MR_W(MR_U(self+4u)+offset+2u)=(uint16_t)converted;
                        MR_B(MR_U(self+4u)+offset+4u)=(uint8_t)(segment-1u);
                        distance+=MR_B(self);++local_count;offset+=6u;
                    }while(restore_menu_input_compare32((int32_t)distance,length)&0x4100u);
                }
            }
            generated+=local_count;++segment;point+=6u;
        }while((int32_t)segment<(int32_t)MR_B(self+0x141u));
    }
    MR_U(self+0x134u)=generated;
    restore_menu_route_endpoints(self);
    return 1;
}
