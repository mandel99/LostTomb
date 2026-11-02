#include "placement_preview.h"
void FUN_004f0ba0(int32_t x,int32_t y,int32_t *screen_x,int32_t *screen_y)
{
    int32_t tile=PV_I(0x70e494u+(PV_U(0x70e474)*231u+PV_U(0x70e478))*4u),map_x,map_y,a,b,out;
    if(tile<0xfff0000){map_x=tile%228;map_y=tile/228;}
    else FUN_004f0ab0(PV_I(0x70e474),pv_add(pv_mul(PV_I(0x70e478),2),1),&map_y,&map_x);
    a=pv_add(pv_sub(PV_I(0xbca2e8),map_x),x);b=pv_add(pv_sub(PV_I(0xbca2ec),map_y),y);
    switch(PV_U(0xbca2e0)) {
    case 0:*screen_x=pv_add(pv_mul(pv_sub(a,b),pv_add(PV_I(0x70e45c),1)),PV_I(0x70e490));out=pv_sub(pv_add(a,b),1);break;
    case 2:*screen_x=pv_add(pv_mul(pv_add(a,b),pv_add(PV_I(0x70e45c),1)),PV_I(0x70e490));out=pv_sub(pv_sub(b,a),1);break;
    case 4:*screen_x=pv_add(pv_mul(pv_sub(b,a),pv_add(PV_I(0x70e45c),1)),PV_I(0x70e490));*screen_y=pv_sub(PV_I(0x70e48c),pv_mul(pv_add(pv_add(a,b),1),PV_I(0x70e44c)));return;
    case 6:*screen_x=pv_sub(PV_I(0x70e490),pv_mul(pv_add(a,b),pv_add(PV_I(0x70e45c),1)));*screen_y=pv_add(pv_mul(pv_sub(pv_sub(a,b),1),PV_I(0x70e44c)),PV_I(0x70e48c));return;
    default:return;
    }
    *screen_y=pv_add(pv_mul(out,PV_I(0x70e44c)),PV_I(0x70e48c));
}
