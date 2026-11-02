#include "order_branch.h"
void FUN_0040b830(void)
{
    int32_t selected=(int32_t)OB_U(0xd05704u),target,x,y,ax,ay;uint32_t actor,tile,ty;uint8_t state;
    if(selected<=0)goto cancel;
    actor=0xbcd5f8u+(uint32_t)selected*388u;
    if(OB_S(actor+306u)>=0||OB_B(actor+56u)==8||!OB_U(0xbca2c4u))goto cancel;
    ty=OB_U(0xbca2ccu);tile=OB_U(0xbca2c8u)+ty*228u;tile+=OB_U(0xbca2d8u);
    target=FUN_004b5640(tile);
    if(target){
        state=OB_B(actor+56u);OB_WB(actor+16u,state);OB_WB(actor+56u,24);
        OB_W(actor+348u,16);OB_WS(actor+130u,target);OB_WB(0xe38ddau,1);OB_W(0xd0570cu,0);return;
    }
    state=OB_B(actor+56u);
    switch(state){
    case 10:case 11:case 12:case 13:case 20:case 23:
        ty=OB_U(0xbca2ccu);tile=OB_U(0xbca2c8u);
        ay=OB_S(actor+22u);ax=OB_S(actor+20u);
        (void)FUN_0040c180(ax,ay,(int32_t)tile,(int32_t)ty,&x,&y);
        OB_WS(actor+36u,x);OB_WS(actor+38u,y);
        OB_WS(actor+332u,OB_S(actor+36u));OB_WS(actor+334u,OB_S(actor+38u));
        FUN_004b3250(selected);state=OB_B(actor+56u);OB_WB(0xe38dcau,1);
        if(state==23){OB_W(actor+348u,22);return;}
        if(state==10){OB_W(actor+348u,16);OB_WB(0xe38ddau,1);OB_W(0xd0570cu,0);}return;
    case 9:
        ty=OB_U(0xbca2ccu);tile=OB_U(0xbca2c8u);
        ay=OB_S(actor+22u);ax=OB_S(actor+20u);
        (void)FUN_0040c180(ax,ay,(int32_t)tile,(int32_t)ty,&x,&y);
        OB_WS(actor+36u,x);OB_WS(actor+38u,y);FUN_004b3250(selected);OB_WB(0xe38dcau,1);return;
    default:return;
    }
cancel:OB_WB(0xe38ddau,1);OB_WB(0xe38dcau,1);
}
