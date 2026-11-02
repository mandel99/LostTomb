#define RESTORE_ROUTE_SEED_INTERNAL
#include "route_seed.h"
static const uint32_t delta[4]={0xffffff1cu,1,228,0xffffffffu};
static void enqueue(uint32_t cell)
{
    uint32_t tail=U(0xeae008u);U(0xf203a0u+tail*4u)=cell;++tail;U(0xeae008u)=tail;
    if((int32_t)tail>51983)U(0xeae008u)=0;
}
uint32_t FUN_0051b680(uint32_t ignored,int32_t x,int32_t y,int32_t mode)
{
    uint32_t cell,flags,bit,origin,head=0,distance,n,d,blocked,v,base;
    (void)ignored;
    FUN_0040f8f0((void *)(uintptr_t)0xeed760u,0x32c40,0);
    cell=U(0xbca2d8u)+(uint32_t)y*228u+(uint32_t)x;U(0xeae004u)=cell;
    flags=U(0xaf598cu+cell*4u);
    v=(flags&0x12040011u)!=0;
    if(!v)v=B(0xee0c50u+cell)==255u;
    if(!v&&(flags&256u)){v=FUN_0051c390(cell);cell=U(0xeae004u);v=v==0;}
    if(v){
        if(!mode)return 0;
        bit=flags&256u;
        if(!bit){
            if(!(flags&8u))return 0;
            v=(uint32_t)(int32_t)(int16_t)W(0xb4e6fcu+cell*2u);v=W(0x7999e8u+v*264u);
            if(v!=90u&&v!=138u)return 0;
        }
    }else bit=flags&256u;
    if(B(0xee0c50u+cell)==2u)return 0;
    if(bit&&!mode){if(!FUN_0051c390(cell))return 0;cell=U(0xeae004u);}
    if(B(0xee0c50u+cell)==251u){if(mode||!FUN_0051c510(cell))return 0;cell=U(0xeae004u);}
    if((flags&64u)&&mode&&!FUN_0051c630(cell))return 0;
    v=U(0xeadfd4u);U(0xeadfd4u)=v+1u;
    FUN_0040f8f0((void *)(uintptr_t)0xeae010u,0xcb10,0);
    cell=U(0xeae004u);U(0xeed760u+cell*4u)=1;
    cell=U(0xeae004u);U(0xf203a0u)=cell;U(0xeae00cu)=0;U(0xeae008u)=1;
    do{
        origin=U(0xf203a0u+head*4u);distance=U(0xeed760u+origin*4u)+1u;
        for(d=0;d<4u;++d){
            n=origin+delta[d];base=0xeed760u+n*4u;
            if(U(base))continue;
            flags=U(0xaf598cu+n*4u);blocked=0;
            if((flags&256u)&&FUN_0051c390(n)){
                if(!mode&&!FUN_0051c390(n)){U(base)=0xffffffffu;blocked=1;}
            }else{
                v=B(0xee0c50u+n);
                if(v==251u){
                    if(mode)blocked=1;
                    else if(!FUN_0051c510(n)){U(base)=0xffffffffu;blocked=1;}
                }else if(v==2u||v==255u)blocked=1;
                else{
                    flags=U(0xaf598cu+n*4u);
                    if(flags&0x12040011u)blocked=1;
                    else if((flags&8u)&&(v!=(d==0u?250u:252u)||mode!=1))blocked=1;
                }
            }
            if((B(0xaf598cu+n*4u)&64u)&&mode&&!FUN_0051c630(n)){U(base)=0xffffffffu;continue;}
            if(blocked)continue;
            U(base)=distance;v=B(0xee0c50u+n);if(v==254u)U(base)=distance+4u;
            enqueue(n);
        }
        head=U(0xeae00cu)+1u;U(0xeae00cu)=head;
        if((int32_t)head>51983){head=0;U(0xeae00cu)=head;}
    }while(head!=U(0xeae008u));
    return 1;
}
uint32_t FUN_0051c790(uint32_t ignored,int32_t x,int32_t y)
{
    uint32_t cell,head=0,tail=1,distance,n,d;
    (void)ignored;
    FUN_0040f8f0((void *)(uintptr_t)0xeed760u,0x32c40,0);
    cell=U(0xbca2d8u)+(uint32_t)y*228u+(uint32_t)x;U(0xeae004u)=cell;U(0xeed760u+cell*4u)=1;
    cell=U(0xeae004u);U(0xf203a0u)=cell;U(0xeae00cu)=0;U(0xeae008u)=1;
    do{
        cell=U(0xf203a0u+head*4u);distance=U(0xeed760u+cell*4u)+1u;
        for(d=0;d<4u;++d){
            n=cell+delta[d];
            if(!U(0xeed760u+n*4u)&&B(0xee0c50u+n)==4u){
                U(0xeed760u+n*4u)=distance;tail=U(0xeae008u);U(0xf203a0u+tail*4u)=n;++tail;U(0xeae008u)=tail;
                if((int32_t)tail>51983){tail=0;U(0xeae008u)=tail;}
            }
        }
        head=U(0xeae00cu)+1u;U(0xeae00cu)=head;
        if((int32_t)head>51983){head=0;U(0xeae00cu)=head;}
    }while(head!=tail);
    return 1;
}
