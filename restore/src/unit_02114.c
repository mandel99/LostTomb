#include "actor_coordinates.h"

void FUN_004f0ab0(int32_t row,int32_t column,int32_t *map_y,int32_t *map_x)
{
    int32_t start_row,start_column,inner_row,column_sign,row_sign,outer_sign,denominator,value;
    switch(AC_U(0xbca2e0)){
    case 0:start_row=ac_add(ac_mul((int32_t)AC_U(0x70e418),2),1);start_column=230;inner_row=1;column_sign=-1;row_sign=1;outer_sign=1;break;
    case 2:start_row=230;start_column=ac_add(ac_mul((int32_t)AC_U(0x70e414),2),1);inner_row=-1;column_sign=1;row_sign=1;outer_sign=1;break;
    case 4:start_row=ac_sub(455,ac_mul((int32_t)AC_U(0x70e418),2));start_column=230;inner_row=-1;column_sign=1;row_sign=-1;outer_sign=-1;break;
    default:start_row=230;start_column=ac_sub(455,ac_mul((int32_t)AC_U(0x70e418),2));inner_row=1;column_sign=-1;row_sign=-1;outer_sign=-1;break;
    }
    denominator=ac_sub(ac_mul(outer_sign,row_sign),ac_mul(column_sign,inner_row));
    value=ac_sub(ac_mul(ac_sub(column,start_column),row_sign),ac_mul(ac_sub(row,start_row),column_sign))/denominator;
    ac_store(map_x,(uint32_t)value);
    value=ac_sub(ac_sub(row,ac_mul(value,inner_row)),start_row)/row_sign;
    ac_store(map_y,(uint32_t)value);
}
