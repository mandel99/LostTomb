#include "region_validation.h"
uint32_t FUN_004829b0(int32_t x,int32_t y,int32_t size,int32_t radius,uint32_t bits,int32_t required)
{
    RestoreRegionBounds b=rv_bounds(x,y,size,radius);uint32_t index=b.index,remaining=(uint32_t)required,rows,columns,address;
    if(b.top<=b.bottom){
        rows=(uint32_t)b.bottom-(uint32_t)b.top+1u;
        /* This native counting variant publishes the end counters before
           scanning, rather than the coordinates of each inspected cell. */
        RV_U(RV_ROW)=rows+(uint32_t)b.top;
        do {
            RV_U(RV_COL)=(uint32_t)b.left;
            if(b.left<=b.right){
                columns=(uint32_t)b.right-(uint32_t)b.left+1u;address=0x00af598cu+index*4u;
                index+=columns;RV_U(RV_COL)=columns+(uint32_t)b.left;
                do {if(RV_U(address)&bits)--remaining;address+=4u;--columns;}while(columns);
            }
            index+=b.stride;--rows;
        }while(rows);
    }
    return (uint32_t)((int32_t)remaining<=0);
}
