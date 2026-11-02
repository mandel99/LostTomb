#include "city_numeric_confirm.h"
#include "menu_full_input_routes.h"
#include "menu_full_input_numeric.h"
#include "menu_route_capacity.h"
#include "mission_compat.h"
#include "mission_load_closure.h"
#include "map_record_refresh.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define H(a) ((int32_t)(int16_t)W(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
extern void FUN_0040f860(const void *source,void *destination,int32_t bytes);
extern int32_t FUN_004ce480(char *path,uint32_t output,uint32_t bytes,uint32_t offset);
extern uint32_t FUN_004ce920(uint32_t path,uint32_t source,uint32_t bytes,uint32_t offset);
extern void *FUN_00561a1d(uint32_t bytes);
extern void FUN_00561a2b(uint32_t allocation);

/* Complete native445D10,108 instructions. Genuine hardware primitives retain
   FSQRT and binary32 length/ratio spills, then extended coordinate arithmetic
   and the actual raw80 truncation. No double approximation or old in_ECX. */
int32_t restore_city_numeric_00445d10(uint32_t self,int32_t from,int32_t to,uint32_t first_sample)
{
    uint32_t a=self+(uint32_t)from*6u,b=self+(uint32_t)to*6u;
    uint32_t length,yratio,xratio,distance=0,count=0,offset=first_sample*6u;
    int32_t x0=H(a+8u),y0=H(a+10u),x1=H(b+8u),y1=H(b+10u),dx,dy;
    uint8_t raw80[10];uint64_t converted;
    dx=(int32_t)((uint32_t)x1-(uint32_t)x0);dy=(int32_t)((uint32_t)y1-(uint32_t)y0);
    length=restore_menu_input_length32(dx,dy);
    if((length&0x7fffffffu)==0)return 0;
    if(B(self)==0)return 0;
    yratio=restore_menu_input_ratio32(dy,length);
    xratio=restore_menu_input_ratio32(dx,length);
    if((restore_menu_input_compare32(0,length)&0x4100u)==0)return 0;
    do {
        restore_menu_input_coordinate80(x0,xratio,(int32_t)distance,raw80);
        converted=restore_menu_input_trunc80(raw80);
        W(U(self+4u)+offset)=(uint16_t)converted;
        restore_menu_input_coordinate80(y0,yratio,(int32_t)distance,raw80);
        converted=restore_menu_input_trunc80(raw80);
        W(U(self+4u)+offset+2u)=(uint16_t)converted;
        B(U(self+4u)+offset+4u)=(uint8_t)from;
        distance+=B(self);++count;offset+=6u;
    }while(restore_menu_input_compare32((int32_t)distance,length)&0x4100u);
    return (int32_t)count;
}

void restore_city_numeric_free_samples(void)
{
    uint32_t slot,allocation;
    for(slot=0x776fb4u;slot<0x77aefcu;slot+=324u){
        allocation=U(slot);
        if(allocation){FUN_00561a2b(allocation);U(slot)=0;U(slot+308u)=0;}
    }
}
/* Identical native rebuild sequences in444270 and4444E0, factored as C.
   Direction BYTEs written to the native private stack array have no readers
   and are not passed to any callee; their dead stores are omitted here. */
void restore_city_numeric_rebuild_routes(void)
{
    uint32_t row,count,point,i,need,capacity,new_capacity,allocation,generated,required;
    int32_t x0,x1,y0,y1,dx,dy;int planned,native_resize;
    for(row=0x776fb0u;row<0x77aef8u;row+=324u){
        need=0;
        if(U(row+4u)!=0||U(row+312u)!=0)goto invalid;
        count=B(row+321u);
        if(count>50u||B(row+320u)>2u)goto invalid;
        i=B(row);if(i<1u||i>50u)goto invalid;
        if(count>1u){
            point=row+8u;i=1;
            do {
                x0=H(point);x1=H(point+6u);dx=x0-x1;if(dx<=-dx)dx=-dx;
                y0=H(point+2u);y1=H(point+8u);dy=y0-y1;if(dy<=-dy)dy=-dy;
                need+=(uint32_t)(dx>dy?dx:dy)+1u;
                ++i;point+=6u;
            }while(i<count);
        }
        /* ROUTECAPACITYREPAIR: keep the native logical need/early stop, but
           reserve all samples that the unchanged generator will emit. On
           accepted count<=50/signed-WORD routes the real planner succeeds.
           Outside its accepted numeric domain retain the original policy. */
        planned=restore_menu_route_required_samples((const uint8_t *)(uintptr_t)(row+8u),
                                                   count,B(row),need,&required);
        capacity=U(row+312u);
        native_resize=(int32_t)need>(int32_t)capacity&&(int32_t)capacity<(int32_t)(need+50u);
        new_capacity=native_resize?need+50u:capacity;
        if(planned&&required>new_capacity)new_capacity=required;
        if(native_resize||(planned&&required>capacity)){
            allocation=U(row+4u);
            if(allocation){FUN_00561a2b(allocation);U(row+4u)=0;}
            U(row+4u)=(uint32_t)(uintptr_t)FUN_00561a1d(new_capacity*6u);
            U(row+312u)=new_capacity;
        }
        count=B(row+321u);U(row+308u)=need;generated=0;i=1;
        if(count>1u){
            do {
                if((int32_t)generated>(int32_t)U(row+308u))goto next_route;
                generated+=(uint32_t)restore_city_numeric_00445d10(row,(int32_t)(i-1u),(int32_t)i,generated);
                ++i;
            }while(i<(uint32_t)B(row+321u));
        }
        U(row+308u)=generated;
        (void)restore_menu_route_endpoints(row);
        goto next_route;
invalid:
        U(row+308u)=0;
        U(row+4u)=0;
        B(row+321u)=0;
        U(row+312u)=0;
        B(row)=5;
        B(row+322u)=0;
        W(row+316u)=0xffffu;
        W(row+318u)=0xffffu;
        B(row+320u)=1;
next_route:;
    }
}

/* Complete4444E0,170 instructions, with genuine file write/allocator calls. */
void restore_city_numeric_004444e0(void)
{
    uint32_t city,path,offset;
    path=U(0x5d2a28u);
    (void)FUN_004ce920(path,0x7843d8u,0x500u,0);
    city=U(0x784f10u);path=U(0x5d2a28u);offset=city*19600u+0x500u;
    (void)FUN_004ce920(path,0x77f748u,0x4c90u,offset);
    B(0x7770f1u)=0;U(0x7770e4u)=0;
    restore_city_numeric_free_samples();
    city=U(0x784f10u);path=U(0x5d2a28u);offset=city*16200u+0xbfb80u;
    (void)FUN_004ce920(path,0x776fb0u,0x4c90u,offset);
    restore_city_numeric_rebuild_routes();
}

/* Complete444270,181 instructions. Numeric confirmation chooses a city row,
   reloads all200 converted city records and50 routes, then saves/rebuilds.
   File-return failures are not invented as new early-return conditions. */
void restore_city_numeric_00444270(int32_t city_index)
{
    uint32_t selected,row,file_offset,path;
    if(city_index>=40||city_index<0)return;
    selected=U(0x784f10u);
    FUN_0040f860((const void *)(uintptr_t)(0x7843d8u+(uint32_t)city_index*32u),
                (void *)(uintptr_t)(0x7843d8u+selected*32u),32);
    row=0x77f748u;file_offset=0x500u;
    do {
        path=U(0x5d2a28u);
        (void)FUN_004ce480((char *)(uintptr_t)path,row,76,file_offset);
        file_offset+=76u;row+=98u;
    }while(file_offset<0x4060u);
    FUN_004d19e0();
    restore_city_numeric_free_samples();
    path=U(0x5d2a28u);
    (void)FUN_004ce480((char *)(uintptr_t)path,0x776fb0u,0x4c90u,
                     (uint32_t)city_index*16200u+0xbfb80u);
    B(0x7770f1u)=0;U(0x7770e4u)=0;
    restore_city_numeric_rebuild_routes();
    FUN_00442660();
    FUN_004426c0();
    restore_city_numeric_004444e0();
}
