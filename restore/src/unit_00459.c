#include "lookup_sentinel.h"

void FUN_00430530(uint32_t orientation)
{
    uint32_t row_start,column_start,row_step,outer_row_step,column_step,outer_column_step;
    uint32_t row_base,row,column,offset,outer,inner,index,half_column,image;

    FUN_00430480();
    LOOKUP_SENTINEL_U32(0x0070e410u)=228;
    LOOKUP_SENTINEL_U32(0x0070e40cu)=228;
    LOOKUP_SENTINEL_U32(0x0070e418u)=0;
    LOOKUP_SENTINEL_U32(0x0070e414u)=0;
    LOOKUP_SENTINEL_U32(0x0070e41cu)=orientation;

    if(orientation==0){
        row_start=1;column_start=230;row_step=1;outer_row_step=1;
        column_step=1;outer_column_step=0xffffffffu;
    }else if(orientation==2){
        row_start=230;column_start=1;row_step=0xffffffffu;outer_row_step=1;
        column_step=1;outer_column_step=1;
    }else if(orientation==4){
        row_start=455;column_start=230;row_step=0xffffffffu;outer_row_step=0xffffffffu;
        column_step=0xffffffffu;outer_column_step=1;
    }else{
        row_start=230;column_start=455;row_step=1;outer_row_step=0xffffffffu;
        column_step=0xffffffffu;outer_column_step=0xffffffffu;
    }
    LOOKUP_SENTINEL_U32(0x0070e42cu)=230;
    LOOKUP_SENTINEL_U32(0x0070e428u)=456;
    LOOKUP_SENTINEL_U32(0x0070e424u)=0;
    LOOKUP_SENTINEL_U32(0x0070e420u)=0;
    row_base=row_start*231u;
    index=0;
    for(outer=0;outer<228;++outer){
        row=row_start;column=column_start;offset=row_base;
        for(inner=0;inner<228;++inner,++index){
            image=LOOKUP_SENTINEL_U32(LOOKUP_SENTINEL_IMAGES+index*4u);
            /* The native CDQ/SUB/SAR sequence divides a signed value by two
               with truncation toward zero. Image eligibility is signed too. */
            half_column=(uint32_t)((int32_t)column/2);
            if((int32_t)image>=6){
                if((int32_t)half_column<=(int32_t)LOOKUP_SENTINEL_U32(0x0070e42cu))
                    LOOKUP_SENTINEL_U32(0x0070e42cu)=half_column;
                if((int32_t)row<=(int32_t)LOOKUP_SENTINEL_U32(0x0070e428u))
                    LOOKUP_SENTINEL_U32(0x0070e428u)=row;
                if((int32_t)LOOKUP_SENTINEL_U32(0x0070e424u)<=(int32_t)half_column)
                    LOOKUP_SENTINEL_U32(0x0070e424u)=half_column;
                if((int32_t)LOOKUP_SENTINEL_U32(0x0070e420u)<=(int32_t)row)
                    LOOKUP_SENTINEL_U32(0x0070e420u)=row;
                if((int32_t)row>=0&&(int32_t)half_column>=0&&(int32_t)row<457&&(int32_t)half_column<231)
                    LOOKUP_SENTINEL_U32(LOOKUP_SENTINEL_GRID+(offset+half_column)*4u)=index;
            }else if((int32_t)row>=0&&(int32_t)half_column>=0&&(int32_t)row<457&&(int32_t)half_column<231){
                LOOKUP_SENTINEL_U32(LOOKUP_SENTINEL_GRID+(offset+half_column)*4u)=
                    LOOKUP_SENTINEL_U32(0x00f56b44u)+0x0fff0000u;
            }
            offset+=row_step*231u;row+=row_step;column+=column_step;
        }
        row_start+=outer_row_step;row_base+=outer_row_step*231u;column_start+=outer_column_step;
    }
}
