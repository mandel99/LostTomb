#include "camera_pan.h"
void FUN_004ddde0(int32_t direction)
{
    uint32_t step=CP_U(0x5e2a60)!=0?4u:1u,x,y;
    if(direction==8)return;
    switch(direction){
    case 0:CP_U(0xbca2c0)-=step;break;
    case 1:
        y=CP_U(0xbca2c0)-step;x=CP_U(0xbca2bc);
        CP_U(0xbca2c0)=y;CP_U(0xbca2bc)=x+1u;break;
    case 2:CP_U(0xbca2bc)+=1u;break;
    case 3:
        y=CP_U(0xbca2c0)+step;x=CP_U(0xbca2bc);
        CP_U(0xbca2c0)=y;CP_U(0xbca2bc)=x+1u;break;
    case 4:CP_U(0xbca2c0)+=step;break;
    case 5:CP_U(0xbca2c0)+=step;CP_U(0xbca2bc)-=1u;break;
    case 6:CP_U(0xbca2bc)-=1u;break;
    case 7:CP_U(0xbca2c0)-=step;CP_U(0xbca2bc)-=1u;break;
    }
    FUN_0041d6f0();
    FUN_004de080();
}
