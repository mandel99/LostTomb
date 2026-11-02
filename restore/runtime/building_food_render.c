#include "building_food_render.h"
#include "resource_availability.h"
#include "menu_prepare_integer.h"
#include "sprite_render.h"
#include "menu_building_kind.h"
#include "graphics_53f250.h"
#include "world_building_draw.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define I(v) ((int32_t)(uint32_t)(v))

uint32_t restore_building_food_00442280(RestoreBuildingFoodFrame *f)
{
    static const uint32_t offsets[16]={110,0xffffffb7u,126,0xffffffc0u,
        145,0xffffffc9u,161,0xffffffd1u,94,0xffffffbeu,111,0xffffffc7u,
        130,0xffffffd1u,147,0xffffffdau};
    uint32_t s=f->sp,base=s-0x7cu,tile=U(s+8u),eax,ecx,index;
    uint32_t building_offset,cursor,resource,threshold,icons,position,x,y,image;
    uint8_t flags;uint16_t quantity;
    U(s-0x70u)=f->ebx;U(s-0x74u)=f->ebp;
    U(s-0x78u)=f->esi;U(s-0x7cu)=f->edi;
    flags=B(0xb67d1cu+tile);
    U(base-4u)=0x442298u;
    FUN_005596d0();
    eax=0xffffffd1u;
    for(index=0;index<16u;++index)U(s-0x40u+index*4u)=offsets[index];
    for(index=0;index<10u;++index)U(s-0x68u+index*4u)=0;
    U(s-0x6cu)=0;
    if(flags&0x40u){
        ecx=U(0xf53dd8u);
        if(I(ecx)>0){
            eax=U(s+4u);
            U(s+8u)=0xf53ddcu;
            building_offset=eax*132u;
            U(s+4u)=ecx;
            do{
                cursor=U(s+8u);resource=U(cursor);
                quantity=W(0x799a2eu+(building_offset+resource)*2u);
                threshold=600;icons=0;
                do{
                    if(quantity!=0u){
                        uint32_t limit=I(threshold)>3200?3200u:threshold;
                        if((int32_t)(int16_t)quantity<=I(limit)){
                            if(I(icons)<8)U(s-0x68u+resource*4u)=icons+1u;
                            break;
                        }
                    }
                    threshold+=400u;++icons;
                }while(I(threshold)<3800);
                cursor=U(s+8u);eax=U(s+4u)-1u;
                U(s+8u)=cursor+4u;U(s+4u)=eax;
            }while(eax!=0u);
            ecx=U(0xf53dd8u);
        }
        U(s+4u)=0;
        if(I(ecx)>0){
            y=U(s+16u);
            U(s+8u)=0xf53ddcu;
            do{
                cursor=U(s+8u);resource=U(cursor);
                icons=U(s-0x68u+resource*4u);
                if(I(icons)>0){
                    eax=U(s-0x6cu);
                    position=s-0x40u+eax*8u;
                    U(s-0x6cu)=eax+icons;
                    do{
                        uint32_t dy=U(position+4u),dx=U(position);
                        image=U(0xf5648cu);
                        dy+=y;U(base-4u)=dy;
                        x=U(s+12u);dx+=x;
                        image+=resource-1u;
                        U(base-8u)=dx;U(base-12u)=image;
                        U(base-16u)=0x44245eu;
                        (void)restore_sprite_draw(RESTORE_SPRITE_LIBRARIES,I(image),I(dx),I(dy));
                        position+=8u;--icons;
                    }while(icons!=0u);
                }
                eax=U(s+4u)+1u;cursor=U(s+8u);ecx=U(0xf53dd8u);
                U(s+4u)=eax;U(s+8u)=cursor+4u;
            }while(I(eax)<I(ecx));
        }
    }
    f->edi=U(s-0x7cu);f->esi=U(s-0x78u);
    f->ebp=U(s-0x74u);f->ebx=U(s-0x70u);f->sp=s+4u;
    return (eax&0xffffff00u)|1u;
}

static uint32_t food_call_436(RestoreBuildingFoodFrame *f,uint32_t eax,uint32_t ecx,uint32_t token)
{
    uint32_t sp=f->sp,result;
    U(sp-4u)=f->esi;U(sp-8u)=f->ebp;U(sp-12u)=ecx;U(sp-16u)=eax;
    U(sp-20u)=token;f->sp=sp-20u;
    result=restore_building_food_00442280(f);
    f->sp+=16u;
    return result;
}
uint32_t restore_building_food_call_004376eb(RestoreBuildingFoodFrame *f,uint32_t eax,uint32_t ecx)
{return food_call_436(f,eax,ecx,0x4376f0u);}
uint32_t restore_building_food_call_00437de5(RestoreBuildingFoodFrame *f,uint32_t eax,uint32_t ecx)
{return food_call_436(f,eax,ecx,0x437deau);}
uint32_t restore_building_food_call_0043b468(RestoreBuildingFoodFrame *f,uint32_t eax)
{
    uint32_t sp=f->sp,building,result;
    U(sp-4u)=f->edi;U(sp-8u)=f->ebx;U(sp-12u)=eax;
    /* Native MOV EAX,[ESP+2C] follows those three pushes. */
    building=U(sp+32u);U(sp-16u)=building;
    U(sp-20u)=0x43b46du;f->sp=sp-20u;
    result=restore_building_food_00442280(f);
    f->sp+=16u;
    return result;
}

void restore_building_status_00435e70_frame(RestoreBuildingFoodFrame *f)
{
    uint32_t p=f->sp,building,root,offset,footprint,tile,image,selected,bit;
    uint32_t x,y,px,py,sx,sy,flags;
    int32_t kind;
    U(p-12u)=f->ebx;
    building=U(p+4u);
    U(p-16u)=f->ebp;U(p-20u)=f->esi;U(p-24u)=f->edi;
    root=FUN_00475b50(building);
    U(p-8u)=root;offset=root*264u;U(p+4u)=offset;
    if(!U(0x799ad8u+offset))goto done;
    tile=U(0x70e488u);y=U(p+12u);x=U(p+8u);
    flags=B(0xb285ccu+tile);image=U(0xb7482cu+tile*4u);
    footprint=flags&15u;U(0xd3b5c0u)=image;U(p-4u)=footprint;
    px=x;py=y;
    switch(footprint){
    case 0:break;
    case 1:py-=U(0x70e44cu);px+=U(0x70e45cu)+1u;break;
    case 2:py-=U(0x70e448u);px+=U(0x70e458u)+2u;break;
    case 3:py-=U(0x70e444u);px+=U(0x70e454u)+3u;break;
    case 4:py-=U(0x70e440u);px+=U(0x70e450u)+4u;break;
    case 5:
        py-=U(0x70e440u);py-=U(0x70e44cu);sx=U(0x70e45cu);
        px+=U(0x70e450u)+sx+5u;break;
    default:goto after_upper;
    }
    (void)restore_sprite_draw_upper(RESTORE_SPRITE_LIBRARIES,I(image),I(px),I(py));
after_upper:
    selected=building;
    if(!B(0x7999ddu+building*264u)){
        if(W(0x7999e8u+building*264u)==71u){
            RestoreBuildingFoodFrame child;
            uint32_t s=p-44u;
            tile=U(0x70e488u);
            U(s+16u)=y;U(s+12u)=x;U(s+8u)=tile;U(s+4u)=building;
            U(s)=0x436026u;
            child.sp=s;child.ebx=building;child.ebp=footprint;child.esi=y;child.edi=x;
            (void)restore_building_food_00442280(&child);
        }else if((uint8_t)FUN_00488030(building)){
            tile=U(0x70e488u);
            if(B(0xb67d1cu+tile)&64u){
                py=y-U(0x70e448u);px=U(0x70e458u)+x+2u;
                restore_world_building_draw_00488c20(0xbca350u,I(building),I(px),I(py),1);
            }
        }else{
            kind=(int16_t)W(0x7999e8u+U(p+4u));
            if(kind>=65&&kind<=69){
                if(!footprint)goto done;
                tile=U(0x70e488u);if(!(B(0xb67d1cu+tile)&64u))goto done;
                selected=U(p-8u);
            }
        }
    }
    sx=U(0x70e45cu);sy=U(0x70e44cu);
    px=sx*footprint+x+18u;py=sy*U(p-4u)+y-2u;
    for(bit=0;bit<14u;++bit){
        if(U(0x799ad8u+selected*264u)&(1u<<bit)){
            image=U(0xf5644cu)+(bit==12u?9u:bit);
            (void)restore_sprite_draw(RESTORE_SPRITE_LIBRARIES,I(image),I(px),I(py));
            py-=14u;
        }
    }
done:
    f->edi=U(p-24u);f->esi=U(p-20u);f->ebp=U(p-16u);f->ebx=U(p-12u);f->sp=p+4u;
}
