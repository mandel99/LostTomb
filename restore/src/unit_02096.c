#include "pyramid_parts.h"

/* Whole4EE6C0: stamp the BYTE-sized footprint using WORD ownership and
   DWORD terrain flags. Valid size zero still updates the DWORD scratch cell. */
void restore_pyramid_stamp_footprint(int32_t building)
{
    uint32_t record=restore_pyramid_record(building),size=PY_B(record+PY_SIZE),base,tile,row,column;
    int32_t x=(int32_t)(int16_t)PY_W(record+PY_X),y=(int32_t)(int16_t)PY_W(record+PY_Y);
    if(x<0||y<0||PY_I((uint32_t)x+size-1u)>=PY_I(PY_U(0xbca2d0u))||
       PY_I((uint32_t)y+size-1u)>=PY_I(PY_U(0xbca2d4u)))return;
    base=PY_U(0xbca2d8u)+(uint32_t)y*228u+(uint32_t)x;PY_U(0xbc9768u)=base;
    for(row=0;row<size;++row)for(column=0;column<size;++column){
        tile=base+PY_U(0x5cc8c0u+row*48u+column*8u);
        PY_U(0xaf598cu+tile*4u)|=0x10000008u;
        PY_W(0xb4e6fcu+tile*2u)=(uint16_t)building;
    }
}
void FUN_004ee6c0(int32_t building){restore_pyramid_stamp_footprint(building);}
