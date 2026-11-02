#define RESTORE_ROUTE_SEED_INTERNAL
#include "route_seed.h"
void FUN_00405c20(void)
{
    uint32_t x,y,cell,v,force,mode;
    if(!U(0xbca300u)||B(0xe38e60u))return;
    y=U(0xbca308u);x=U(0xbca304u);U(0x6059d8u)=y;U(0x6059d4u)=y;
    U(0x6059e0u)=x;U(0x6059dcu)=x;B(0x605798u)=0;
    cell=U(0xbca2d8u)+y*228u+x;U(0x6059d0u)=cell;v=B(0xab623cu+cell);
    force=U(0xbca310u);U(0xbc96ccu)=v;
    if((force||U(0x6059e8u)!=0x88u)&&!FUN_00484f60())return;
    mode=U(0x6059e8u);U(0x6059e4u)=1;
    if(mode==5u){y=U(0x6059d8u);x=U(0x6059e0u);(void)FUN_0051b680(1,(int32_t)x,(int32_t)y,0);}
    if(U(0x6059e8u)==6u){y=U(0x6059d8u);x=U(0x6059e0u);(void)FUN_0051c790(1,(int32_t)x,(int32_t)y);}
    if(U(0x6059e8u)==0xa8u){y=U(0x6059d8u);x=U(0x6059e0u);(void)FUN_0051c790(1,(int32_t)x,(int32_t)y);}
    if(U(0x6059e8u)==0xa9u){y=U(0x6059d8u);x=U(0x6059e0u);(void)FUN_0051c790(1,(int32_t)x,(int32_t)y);}
    if(U(0x6059e8u)==8u){y=U(0x6059d8u);x=U(0x6059e0u);(void)FUN_0051b680(1,(int32_t)x,(int32_t)y,1);}
    if(U(0x6059e8u)==7u){y=U(0x6059d8u);x=U(0x6059e0u);(void)FUN_0051b680(1,(int32_t)x,(int32_t)y,1);}
}
uint32_t FUN_00484f60(void)
{
    uint32_t i,mode,v;
    for(i=0;i<36u;++i)U(0xbc95f4u+i*4u)=0;
    mode=U(0x6059e8u);U(0xbc968cu)=0;U(0xbc9694u)=1;U(0xbc9688u)=0;
    U(0xbc9684u)=0;U(0xbc95f0u)=0;U(0xbc9690u)=mode;
    FUN_0040f8f0((void *)(uintptr_t)0xba7888u,0x21d68,0);
    FUN_0040f8f0((void *)(uintptr_t)0xba746cu,0x41a,0);
    for(i=0x799ae0u;i<0x89b6d8u;i+=264u){
        v=B(i);if(v==2u){U(0xbc9694u)=0;return 0;}
        if(v==6u)U(0xbc9694u)=0;
    }
    FUN_0040f860((const void *)(uintptr_t)0xb7482cu,(void *)(uintptr_t)0xa1126cu,0x32c40);
    FUN_0040f860((const void *)(uintptr_t)0xaf598cu,(void *)(uintptr_t)0x9de62cu,0x32c40);
    FUN_0040f860((const void *)(uintptr_t)0xae8e7cu,(void *)(uintptr_t)0x9d1b1cu,0xcb10);
    FUN_0040f860((const void *)(uintptr_t)0xb285ccu,(void *)(uintptr_t)0x9b84fcu,0xcb10);
    FUN_0040f860((const void *)(uintptr_t)0xb67d1cu,(void *)(uintptr_t)0x9ab9ecu,0xcb10);
    FUN_0040f860((const void *)(uintptr_t)0xadc36cu,(void *)(uintptr_t)0x9c500cu,0xcb10);
    return 1;
}
