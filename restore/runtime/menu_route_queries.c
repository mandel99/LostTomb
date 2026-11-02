#define RESTORE_ROUTE_SEED_INTERNAL
#include "route_seed.h"
uint32_t FUN_0047b8e0(uint32_t cell)
{
    uint32_t flags=U(0xaf598cu+cell*4u);int32_t value;
    if(flags&0x10000u)return 0;
    value=(int8_t)B(0xac2d4cu+cell);if(value>4)return 1;if(value<=0)return 0;
    return (flags>>13)&1u;
}
uint32_t FUN_0051c390(uint32_t cell)
{
    uint32_t base=U(0xf56b84u),v=U(0xb7482cu+cell*4u)-base,side=0,mode;
    if((int32_t)v>=48)v-=48u;
    switch(v){
    case 0:case 7:case 9:case 15:case 17:case 19:case 21:case 28:case 30:case 36:case 38:case 40:case 42:case 44:case 46:side=1;break;
    case 1:case 6:case 8:case 16:case 18:case 20:case 22:case 27:case 29:case 37:case 39:case 41:case 43:case 45:case 47:break;
    default:return 0;
    }
    mode=U(0xbca2e0u);if(mode==2u||mode==6u)side^=1u;
    if(side){if(B(0xaf5990u+cell*4u)&64u)return 0;return !(B(0xaf5988u+cell*4u)&64u);}
    if(B(0xaf55fcu+cell*4u)&64u)return 0;return !(B(0xaf5d1cu+cell*4u)&64u);
}
uint32_t FUN_0051c510(uint32_t cell)
{
    uint32_t v=U(0xaf598cu+cell*4u),p,side,mode,base;
    if(v&256u)return FUN_0051c390(cell);
    p=U(0xd05720u+cell*4u);if(!p||B(p+24u))return 1;
    v=U(0xb7482cu+cell*4u);base=U(0xf56c34u);v-=base;
    if(v==52u||v==48u)side=0;else if(v==54u||v==50u)side=1;else return 0;
    mode=U(0xbca2e0u);if(mode==2u||mode==6u)side^=1u;
    if(side){
        if(B(0xaf55fcu+cell*4u)&64u)return 0;
        if((int32_t)U(0xeed3d0u+cell*4u)>0)return 0;
        if(B(0xaf5d1cu+cell*4u)&64u)return 0;
        return (int32_t)U(0xeedaf0u+cell*4u)<=0;
    }
    if(B(0xaf5990u+cell*4u)&64u)return 0;
    if((int32_t)U(0xeed764u+cell*4u)>0)return 0;
    if(B(0xaf5988u+cell*4u)&64u)return 0;
    return (int32_t)U(0xeed75cu+cell*4u)<=0;
}
static uint32_t building_type(uint32_t address)
{
    uint32_t index=(uint32_t)(int32_t)(int16_t)W(address);
    return W(0x7999e8u+index*264u);
}
uint32_t FUN_0047e0d0(uint32_t cell)
{
    uint32_t off=cell*4u,n=U(0xaf55fcu+off),e=U(0xaf5990u+off),w=U(0xaf5988u+off),s=U(0xaf5d1cu+off);
    uint32_t nn=n&0x8000040u,ee=e&0x8000040u,ww=w&0x8000040u,ss=s&0x8000040u,t;
    U(0xbc97c0u)=nn;U(0xbc97c8u)=ee;U(0xbc97d0u)=ss;U(0xbc97d8u)=ww;
    if(n&8u){t=building_type(0xb4e534u+cell*2u);if(t==138u||t==136u){nn=1;U(0xbc97c0u)=1;}ww=U(0xbc97d8u);}
    if(e&8u){t=building_type(0xb4e6feu+cell*2u);if(t==138u||t==136u){ee=1;U(0xbc97c8u)=1;}}
    if(s&8u){t=building_type(0xb4e8c4u+cell*2u);if(t==138u||t==136u){ss=1;U(0xbc97d0u)=1;}}
    if(w&8u){t=building_type(0xb4e6fau+cell*2u);if(t==138u||t==136u){ww=1;U(0xbc97d8u)=1;}}
    return (nn!=0)+(ee!=0)+(ss!=0)+(ww!=0);
}
uint32_t FUN_0051c630(uint32_t cell)
{
    uint32_t index=(uint32_t)(int32_t)(int16_t)W(0xb4e6fcu+cell*2u),v,side=0,mode,query;
    /* Native 51C6CE reads F56C44 even when F56C94 replaces it later.
       Keep that latch observable after MSVC inlines the preceding query. */
    volatile uint32_t base;
    if(index&&W(0x7999e8u+index*264u)==138u)return 1;
    v=U(0xaf598cu+cell*4u);
    if(v&0x10000u){v=U(0xb7482cu+cell*4u);base=U(0xf56cd4u);}
    else if(B(0xb285ccu+cell)&128u){
        v=U(0xb7482cu+cell*4u);base=U(0xf5695cu);
        if((int32_t)(v-base)>=3)return 0;
        if((int32_t)FUN_0047e0d0(cell)>2)return 0;
        return (uint8_t)FUN_0047e090(cell)==0;
    }else{
        query=FUN_0047b8e0(cell);base=U(0xf56c44u);v=U(0xb7482cu+cell*4u);
        if(!query)base=U(0xf56c94u);
    }
    v-=base;
    switch(v){case 0:case 2:case 8:case 10:side=1;break;case 1:case 3:case 9:case 11:break;default:return 0;}
    mode=U(0xbca2e0u);if(mode==2u||mode==6u)side^=1u;
    if(side){if(U(0xaf55fcu+cell*4u)&256u)return 0;return !(U(0xaf5d1cu+cell*4u)&256u);}
    if(U(0xaf5990u+cell*4u)&256u)return 0;return !(U(0xaf5988u+cell*4u)&256u);
}
