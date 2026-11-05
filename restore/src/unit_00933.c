#include "city_capacity.h"
#include "menu_prepare_integer.h"
#include "menu_resource_neighbors.h"
#include "actor_teardown_model.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define H(a) ((int32_t)(int16_t)W(a))
#define I(v) ((int32_t)(uint32_t)(v))

/* The two 36-dword service arrays have matching offsets. These entries are
   the literal kind tests at4678CE..467DD3, including their native holes. */
static uint32_t service_counter_offset(uint16_t kind)
{
    switch(kind){
    case 0x66:return 4;case 0xc2:return 8;case 0x67:return 12;
    case 0x69:return 16;case 0x68:return 20;case 0xc4:return 24;
    case 0x73:return 32;case 0x70:return 40;case 0x6d:return 44;
    case 0xcc:return 48;case 0x72:return 52;case 0x64:return 56;
    case 0x6e:return 60;case 0x65:return 64;case 0xe0:return 128;
    case 0x6f:return 68;case 0xa2:return 72;case 0x71:return 76;
    case 0x6c:return 80;case 0xa1:return 84;case 0xc3:return 88;
    case 0xcb:return 92;case 0x6a:return 96;case 0x6b:return 100;
    case 0xd8:return 104;case 0xcd:return 112;case 0xd9:return 116;
    case 0xdd:return 120;case 0xe8:return 136;case 0xe9:return 132;
    default:return 0;
    }
}

void restore_city_rebuild_building_counts(void)
{
    uint32_t owner=U(0xeaac28u),off=owner*0x49d8u,id=1,p,at,kind,slot,linked,root;
    uint32_t count,force_services,available,value,a,b,c;
    for(at=0;at<36u;++at)U(0x78d64cu+at*4u)=0;
    for(at=0;at<36u;++at)U(0x78d6dcu+at*4u)=0;
    W(0xea5cf8u+off)=0;U(0xea5ea0u+off)=0;
    for(at=0;at<5u;++at)U(0xea5cd0u+off+at*4u)=0;
    U(0xea188cu+off)=0;
    for(at=0;at<237u;++at)U(0x705af0u+at*4u)=0;
    for(at=0;at<237u;++at)U(0x70573cu+at*4u)=0;
    if(I(U(0x78d790u))<1)return;
    do{
        p=id*264u;
        value=B(0x7999d8u+p);
        if(value!=1u&&value!=3u)goto next_building;
        if(B(0x7999ddu+p)){
            kind=(uint32_t)H(0x7999e8u+p);at=kind*4u;
            value=U(0x705af0u+at)+1u;available=W(0x7999f4u+p);
            U(0x705af0u+at)=value;
            if(available)++U(0x70573cu+at);
            goto next_building;
        }
        kind=W(0x7999e8u+p);force_services=0;
        if(kind==0x5au||kind==0x5cu||kind==0xb4u){
            at=(uint32_t)(int32_t)(int16_t)kind*4u;++U(0x705af0u+at);
            if(B(0x799a0du+p)&&(kind==0x5cu||H(0x799a18u+p)>0))++U(0x70573cu+at);
        }else if((at=service_counter_offset((uint16_t)kind))!=0){
            value=U(0x78d6dcu+at)+1u;available=(uint32_t)H(0x799a18u+p);
            U(0x78d6dcu+at)=value;if(I(available)>0)++U(0x78d64cu+at);
        }else if(kind==0x4cu){
            value=U(0x78d6f8u)+1u;available=(uint32_t)H(0x799a18u+p);U(0x78d6f8u)=value;
            if(I(available)>0){
                off=owner*0x49d8u;++W(0xea5cf8u+off);
                if(!W(0x799a8cu+p))++U(0xea5ea0u+off);
                ++U(0x78d668u);
            }
        }else if(kind==0x4bu||kind==0xc0u){
            slot=U(0x705c1cu);
            if(I(slot)<10){
                if(H(0x799a18u+p)>0&&B(0x799a0du+p))W(0xea5cd0u+(owner*0x24ecu+slot)*2u)=(uint16_t)id;
                U(0x705c1cu)=slot+1u;
            }
        }else if((kind>=0x41u&&kind<=0x45u)||(kind>=0x9cu&&kind<=0xa0u)){
            if(restore_building_root(id)==id){
                at=(uint32_t)H(0x7999e8u+p)*4u;
                value=U(0x705af0u+at)+1u;available=(uint32_t)H(0x799a18u+p);
                U(0x705af0u+at)=value;if(I(available)>0)++U(0x70573cu+at);
            }
        }else{
            kind=(uint32_t)(int32_t)(int16_t)kind;at=kind*4u;
            if(I(kind)>=0x8c&&I(kind)<=0x95){
                value=U(0x705af0u+at)+1u;a=(uint32_t)H(0x7999deu+p);
                U(0x705af0u+at)=value;b=(uint32_t)H(0x7999e0u+p);
                if(FUN_004802c0(I(a),I(b)))++U(0x70573cu+(uint32_t)H(0x7999e8u+p)*4u);
            }else{
                ++U(0x705af0u+at);
                if(kind==0x1fu){
                    root=restore_building_root(id);if(root!=id)goto next_building;
                    if(H(0x799a18u+root*264u)>0)++U(0x7057b8u);
                    force_services=1;
                }else if(kind==0x1eu){
                    root=restore_building_root(id);if(root!=id)goto next_building;
                    root=restore_building_root(id);
                    if(H(0x799a18u+root*264u)>0){
                        a=U(0x7057b8u);b=U(0x7057b4u);U(0x7057b8u)=a+1u;U(0x7057b4u)=b+1u;
                    }
                    force_services=1;
                }else if(kind==0x21u){
                    if(H(0x799a18u+p)>0){
                        a=U(0x7057b8u);b=U(0x7057b4u);c=U(0x7057c0u);
                        U(0x7057b8u)=a+1u;U(0x7057b4u)=b+1u;U(0x7057c0u)=c+1u;
                    }
                    force_services=1;
                }else if(kind==0x20u){
                    if(H(0x799a18u+p)>0)++U(0x7057bcu);
                }else if(kind==0xe2u){
                    if(H(0x799a18u+p)>0)++U(0x705ac4u);
                }else{
                    if(kind==0x5fu)U(0xea5d1cu+owner*0x49d8u)=id;
                    if(H(0x799a18u+p)>0)++U(0x70573cu+at);
                }
            }
        }
        linked=(uint32_t)H(0x799a06u+p);
        if(linked&&(B(0xbcd606u+linked*388u)!=1u||H(0xbcd656u+linked*388u)!=I(id)))W(0x799a06u+p)=0;
        if(force_services||(uint8_t)restore_actor_building_gate(I(id))){
            count=0;value=B(0x799a7fu+p);
            if(value){B(0x799a7fu+p)=(uint8_t)(value-1u);count=1;}
            value=B(0x799a80u+p);if(value){++count;B(0x799a80u+p)=(uint8_t)(value-1u);}
            value=B(0x799a81u+p);if(value){++count;B(0x799a81u+p)=(uint8_t)(value-1u);}
            B(0x799a7eu+p)=(uint8_t)count;
        }
next_building:
        value=U(0x78d790u);++id;if(I(id)>I(value))break;
        owner=U(0xeaac28u);
    }while(1);
}
