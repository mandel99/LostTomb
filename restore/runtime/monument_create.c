#include "monument_create.h"
#include "building_create_model.h"
#include <stdlib.h>
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))

static uint32_t invoke(struct restore_menu_input_frame *parent,uint32_t site,uint32_t target,
                       uint32_t self,const uint32_t *args,uint32_t count,uint32_t cleanup)
{
    uint32_t arena[256],sp,i;struct restore_menu_input_frame child;
    if(parent){child=*parent;sp=parent->r[4]-64u;}
    else{
        for(i=0;i<8u;++i)child.r[i]=0;
        child.flags=2;child.boundary=restore_building_create_dispatch;
        /* Includes the nested 417310 image query's 160-byte native frame.
           Keep its virtual stack above this C function's real call stack. */
        sp=(uint32_t)(uintptr_t)(arena+224);
    }
    child.r[4]=sp;child.r[1]=self;U(sp)=0;
    for(i=0;i<count;++i)U(sp+4u+i*4u)=args[i];
    child.boundary(site,target,&child);
    if(child.r[4]!=sp+4u+cleanup)abort();
    return child.r[0];
}
static uint32_t call_one(struct restore_menu_input_frame *context,uint32_t site,uint32_t target,uint32_t self,uint32_t building,uint32_t cleanup)
{return invoke(context,site,target,self,&building,1,cleanup);}
static void draw_part(struct restore_menu_input_frame *context,uint32_t site,uint32_t self,uint32_t building)
{(void)call_one(context,site,U(U(self)+0x18u),self,building,4);}
static void remember_part(struct restore_menu_input_frame *context,uint32_t site,uint32_t building)
{(void)call_one(context,site,0x485100u,0,building,0);}
static void entrance(struct restore_menu_input_frame *context,uint32_t site,uint32_t self,uint32_t building,uint32_t *dx,uint32_t *dy)
{
    uint32_t args[3]={building,(uint32_t)(uintptr_t)dx,(uint32_t)(uintptr_t)dy},r=building*264u;
    (void)invoke(context,site,U(U(self)+0x20u),self,args,3,12);
    W(0x7999feu+r)=(uint16_t)*dx;W(0x799a00u+r)=(uint16_t)*dy;
}
static uint32_t allocate_part(struct restore_menu_input_frame *context,uint32_t site,uint32_t kind,uint32_t x,uint32_t y,uint32_t previous)
{
    uint32_t args[6]={U(0xeaac28u),kind,x,y,0,previous+1u};
    return invoke(context,site,0x46a400u,0,args,6,0);
}
static void initialize_root(uint32_t self,uint32_t x,uint32_t y,uint32_t root,uint32_t registration,uint32_t stage,uint32_t *out_dx,uint32_t *out_dy)
{
    uint32_t dx=U(self+0x4cu),dy=U(self+0x50u),r=root*264u;
    *out_dx=dx;*out_dy=dy;
    W(0x7999deu+r)=(uint16_t)(x+dx);W(0x7999e0u+r)=(uint16_t)(y+dy);
    U(0x7999e4u+r)=U(0xbca2d8u)+(y+dy)*228u+dx+x;
    W(0x7999eau+r)=0;B(0x7999dbu+r)=B(self+0x54u);W(0x799a10u+r)=0;
    U(0x799a2cu+r)=registration;U(0x799a30u+r)=stage;
    U(0x799a38u+r)=1;U(0x799a34u+r)=0;
}
static void link_part(uint32_t previous,uint32_t building)
{
    uint32_t r=building*264u;
    W(0x799a10u+r)=(uint16_t)previous;
    W(0x799a12u+previous*264u)=(uint16_t)building;
    W(0x799a12u+r)=0;
}
static void initialize_part(uint32_t building,uint16_t index,uint8_t size,uint32_t registration,uint32_t stage,uint32_t root_active)
{
    uint32_t r=building*264u;
    W(0x7999eau+r)=index;B(0x7999dbu+r)=size;
    U(0x799a2cu+r)=registration;U(0x799a30u+r)=stage;
    U(0x799a38u+r)=root_active;U(0x799a34u+r)=0;
}
static void create_parts(uint32_t target,uint32_t self,uint32_t x,uint32_t y,uint32_t root,uint8_t skip,struct restore_menu_input_frame *context)
{
    uint32_t registration=0,building,previous=root,piece,index,dx,dy,count,stage,kind;
    if(target!=0x40da90u&&target!=0x417180u&&target!=0x4e6840u)abort();
    if(!skip)registration=call_one(context,target==0x40da90u?0x40dab2u:target==0x417180u?0x41719du:0x4e6860u,0x4f4f80u,0,root,0);
    stage=target==0x40da90u?1u:0u;
    initialize_root(self,x,y,root,registration,stage,&dx,&dy);
    if(target==0x4e6840u){
        /* The two-part method snapshots undo state BEFORE writing map tiles. */
        remember_part(context,0x4e68fdu,root);draw_part(context,0x4e690au,self,root);
        building=allocate_part(context,0x4e6931u,0xe4u,x+U(self+0x80u),y+U(self+0x84u),root);
        link_part(root,building);
        initialize_part(building,W(self+0x8cu),B(self+0x88u),registration,0,0);
        remember_part(context,0x4e6995u,building);draw_part(context,0x4e69a2u,self,building);
        U(self+8u)=0;(void)invoke(context,0x4e69a8u,0x4dea50u,0,0,0,0);return;
    }
    if(target==0x40da90u)entrance(context,0x40db5du,self,root,&dx,&dy);
    draw_part(context,target==0x40da90u?0x40db7eu:0x417238u,self,root);
    remember_part(context,target==0x40da90u?0x40db82u:0x41723cu,root);U(self+8u)=0;
    count=target==0x40da90u?25u:9u;kind=target==0x40da90u?0xe1u:0xe3u;
    for(index=1;index<count;++index){
        piece=self+0x4cu+index*52u;dx=U(piece);dy=U(piece+4u);
        building=allocate_part(context,target==0x40da90u?0x40dbc6u:0x417279u,kind,x+dx,y+dy,previous);
        link_part(previous,building);previous=building;
        initialize_part(building,(uint16_t)index,B(piece+8u),registration,stage,1);
        if(target==0x40da90u)entrance(context,0x40dc47u,self,building,&dx,&dy);
        draw_part(context,target==0x40da90u?0x40dc68u:0x4172e6u,self,building);
        remember_part(context,target==0x40da90u?0x40dc6cu:0x4172eau,building);
    }
}
void restore_monument_create_frame(uint32_t target,struct restore_menu_input_frame *f)
{
    uint32_t sp=f->r[4];
    create_parts(target,f->r[1],U(sp+4u),U(sp+8u),U(sp+12u),(uint8_t)U(sp+16u),f);
    f->r[4]=sp+20u;
    /* These are void members. The two loops nevertheless leave their bounds
       in EAX; no caller may depend on ECX, EDX or arithmetic flags. */
    if(target==0x40da90u)f->r[0]=25;
    else if(target==0x417180u)f->r[0]=9;
}
#define PUBLIC_ENTRY(token) \
void restore_monument_create_##token(uint32_t self,int32_t x,int32_t y,uint32_t root,uint8_t skip) \
{create_parts(0x##token##u,self,(uint32_t)x,(uint32_t)y,root,skip,0);}
PUBLIC_ENTRY(0040da90)
PUBLIC_ENTRY(00417180)
PUBLIC_ENTRY(004e6840)
