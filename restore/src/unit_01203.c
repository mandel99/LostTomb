#include "region_validation.h"
uint32_t FUN_00482720(int32_t x,int32_t y,int32_t size,int32_t radius,code *predicate)
{
    RestoreRegionBounds b=rv_bounds(x,y,size,radius);uint32_t index=b.index,address;int32_t row=b.top,column;
    if(row>b.bottom)return 0;
    do {
        RV_U(RV_COL)=(uint32_t)b.left;
        if(b.left<=b.right){
            address=0x00af598cu+index*4u;
            do {
                /* Native consumes AL and reloads the shared column after the
                   callback. The scan pointer advances independently of it. */
                if((uint8_t)(*predicate)(RV_U(address)))return 1;
                column=rv_add(RV_I(RV_COL),1);address+=4u;++index;RV_U(RV_COL)=(uint32_t)column;
            }while(column<=b.right);
        }
        index+=b.stride;row=rv_add(RV_I(RV_ROW),1);RV_U(RV_ROW)=(uint32_t)row;
    }while(row<=b.bottom);
    return 0;
}
