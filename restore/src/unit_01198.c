#include "map_refresh.h"
/* Native table families deliberately have different selector conventions in
   their last branch. The fallback is meaningful for every non1..5 family. */
int32_t FUN_00481aa0(int32_t x,int32_t y,int32_t family,int32_t ring,int32_t point)
{
    static const uint32_t tables[6][6]={
        {0x70bf7c,0x70bf5c,0x70bf2c,0x70beec,0x70be9c,0x70be3c},
        {0x5c9ba8,0x5c9bc0,0x5c9be8,0x5c9c20,0x5c9c68,0x5c9cc0},
        {0x5c9d28,0x5c9d48,0x5c9d78,0x5c9db8,0x5c9e08,0x5c9e68},
        {0x5c9ed8,0x5c9f00,0x5c9f38,0x5c9f80,0x5c9fd8,0x5ca040},
        {0x5ca0b8,0x5ca0e8,0x5ca128,0x5ca178,0x5ca1d8,0x5ca248},
        {0x5ca2c8,0x5ca300,0x5ca348,0x5ca3a0,0x5ca408,0x5ca480}
    };
    uint32_t row=(uint32_t)family-1u,column,address;
    int32_t dx,dy,px,py;
    if(row<5u){column=(uint32_t)ring-1u;if(column>=5u)column=5u;}
    else{row=5u;column=(uint32_t)ring;if(column>=5u)column=5u;}
    address=tables[row][column]+(uint32_t)point*2u;
    dx=(int32_t)(int8_t)MR_B(address);
    dy=(int32_t)(int8_t)MR_B(address+1u);
    px=(int32_t)((uint32_t)x+(uint32_t)dx);
    if(px < -1)return 1;
    py=(int32_t)((uint32_t)y+(uint32_t)dy);
    if(py < -1)return 1;
    if(px > (int32_t)MR_U(0xbca2d0u))return 1;
    return py > (int32_t)MR_U(0xbca2d4u);
}
