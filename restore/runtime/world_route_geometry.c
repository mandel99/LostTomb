#include "world_route_geometry.h"
#include "menu_full_input_routes.h"
#include "menu_full_input_numeric.h"
#include "menu_route_capacity.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
extern void *FUN_00561a1d(uint32_t bytes);
extern void FUN_00561a2b(void *allocation);

/* The controller already owns complete C bodies for these native members.
   Public legacy entries now share those providers, with explicit receivers. */
uint8_t restore_world_route_bind_cities(volatile RestoreWorldRoute *self)
{return restore_menu_route_endpoints((uint32_t)(uintptr_t)self);}
uint8_t restore_world_route_erase_point(volatile RestoreWorldRoute *self,int32_t index)
{return restore_menu_route_delete((uint32_t)(uintptr_t)self,index);}
int32_t restore_world_route_hit(const volatile RestoreWorldRoute *self,int32_t x,int32_t y,uint8_t *vertex)
{return restore_menu_route_hit((uint32_t)(uintptr_t)self,x,y,vertex);}

/* Whole4D4720, using the same existing x87 primitives, capacity planner and
   endpoint scan as the editor. The planner extends undersized diagonal
   allocations; native samples, early-stop order and healthy sizes remain.
   Retain the loader's existing rejection of malformed data/allocation failure. */
uint8_t restore_world_route_rebuild(volatile RestoreWorldRoute *self)
{
    uint32_t i,count=self->point_count,required=0,capacity,used=0,address;
    int32_t estimate=0,old=self->capacity;
    if(count>50u)return 0;
    for(i=1;i<count;++i){
        int32_t dx=(int32_t)self->points[i].x-self->points[i-1u].x;
        int32_t dy=(int32_t)self->points[i].y-self->points[i-1u].y;
        if(dx<0)dx=-dx;if(dy<0)dy=-dy;
        estimate+=1+(dx>dy?dx:dy);
    }
    capacity=old<estimate?(uint32_t)estimate+50u:(uint32_t)old;
    if(!restore_menu_route_required_samples((const uint8_t *)(uintptr_t)self+8u,
        count,self->spacing,(uint32_t)estimate,&required))return 0;
    if(required>capacity)capacity=required;
    if((int32_t)capacity>old){
        if(self->samples){FUN_00561a2b(self->samples);self->samples=0;}
        self->samples=(RestoreRoutePoint *)FUN_00561a1d(capacity*6u);
        self->capacity=(int32_t)capacity;
    }
    self->sample_count=estimate;
    for(i=1;i<self->point_count;++i){
        int32_t x=self->points[i-1u].x,y=self->points[i-1u].y;
        int32_t dx=(int32_t)self->points[i].x-x,dy=(int32_t)self->points[i].y-y;
        uint32_t length,ratio_x,ratio_y,distance=0,emitted=0,offset=used*6u;
        uint8_t raw[10];
        if((int32_t)used>self->sample_count)return 0;
        length=restore_menu_input_length32(dx,dy);
        if((length&0x7fffffffu)&&self->spacing){
            ratio_y=restore_menu_input_ratio32(dy,length);
            ratio_x=restore_menu_input_ratio32(dx,length);
            if(restore_menu_input_compare32(0,length)&0x4100u){
                do{
                    if(!self->samples||used+emitted>=(uint32_t)self->capacity)return 0;
                    restore_menu_input_coordinate80(x,ratio_x,(int32_t)distance,raw);
                    address=(uint32_t)(uintptr_t)self->samples;
                    W(address+offset)=(uint16_t)restore_menu_input_trunc80(raw);
                    restore_menu_input_coordinate80(y,ratio_y,(int32_t)distance,raw);
                    address=(uint32_t)(uintptr_t)self->samples;
                    W(address+offset+2u)=(uint16_t)restore_menu_input_trunc80(raw);
                    address=(uint32_t)(uintptr_t)self->samples;
                    B(address+offset+4u)=(uint8_t)(i-1u);
                    distance+=self->spacing;++emitted;offset+=6u;
                }while(restore_menu_input_compare32((int32_t)distance,length)&0x4100u);
            }
        }
        used+=emitted;
    }
    self->sample_count=(int32_t)used;
    (void)restore_menu_route_endpoints((uint32_t)(uintptr_t)self);
    return 1;
}
