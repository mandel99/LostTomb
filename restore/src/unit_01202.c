#include "region_validation.h"
uint32_t FUN_00482650(int32_t x,int32_t y,int32_t size,int32_t radius,uint32_t bits)
{
    RestoreRegionBounds b=rv_bounds(x,y,size,radius);uint32_t index=b.index;int32_t row=b.top,column;
    if(row>b.bottom)return 0;
    do {
        column=b.left;RV_U(RV_COL)=(uint32_t)column;
        if(column<=b.right)do {
            if(RV_U(0x00af598cu+index*4u)&bits)return 1;
            column=rv_add(column,1);++index;RV_U(RV_COL)=(uint32_t)column;
        }while(column<=b.right);
        index+=b.stride;row=rv_add(RV_I(RV_ROW),1);RV_U(RV_ROW)=(uint32_t)row;
    }while(row<=b.bottom);
    return 0;
}
