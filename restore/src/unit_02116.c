#include "actor_coordinates.h"

void FUN_004f0d30(int32_t actor,int32_t *x,int32_t *y)
{
    uint32_t record=(uint32_t)actor*388u,source_record,target_record,parent_record,mode;
    int32_t source=AC_S(0xbcd642u+record),target=AC_S(0xbcd646u+record),world_x,world_y;
    int32_t start_x,start_y,offset_x,offset_y,end_x,end_y,map_x,map_y,fraction,total,result_x,result_y;
    target_record=(uint32_t)target*264u;
    if(source==0||source==target){
        parent_record=(uint32_t)ac_sub(target,AC_S(0x7999eau+target_record))*264u;
        map_y=AC_S(0x799a00u+parent_record);map_x=AC_S(0x7999feu+parent_record);
        FUN_004f0ba0(map_x,map_y,&start_x,&start_y);
    }else{
        FUN_004f08c0(source,AC_S(0xbcd644u+record),&start_x,&start_y);
        source_record=(uint32_t)source*264u;
        map_x=AC_S(0x7999deu+source_record);map_y=AC_S(0x7999e0u+source_record);
        mode=AC_U(0xe94e04);
        if(mode==0)map_y=ac_add(map_y,1);
        else if(mode==4)map_x=ac_add(map_x,1);
        else if(mode==6){map_x=ac_add(map_x,1);map_y=ac_add(map_y,1);}
        FUN_004f0ba0(map_x,map_y,&world_x,&world_y);
        start_x=ac_add(start_x,world_x);start_y=ac_add(start_y,world_y);
    }
    map_x=AC_S(0x7999deu+target_record);map_y=AC_S(0x7999e0u+target_record);
    mode=AC_U(0xe94e04);
    if(mode==0)map_y=ac_add(map_y,1);
    else if(mode==4)map_x=ac_add(map_x,1);
    else if(mode==6){map_x=ac_add(map_x,1);map_y=ac_add(map_y,1);}
    FUN_004f08c0(target,AC_S(0xbcd648u+record),&offset_x,&offset_y);
    FUN_004f0ba0(map_x,map_y,&world_x,&world_y);
    end_x=ac_add(offset_x,world_x);end_y=ac_add(offset_y,world_y);
    fraction=AC_S(0xbcd64au+record);total=AC_S(0xbcd64cu+record);
    if(fraction<total){
        result_x=ac_add(ac_mul(ac_sub(end_x,start_x),fraction)/total,start_x);
        result_y=ac_add(ac_mul(ac_sub(end_y,start_y),fraction)/total,start_y);
        ac_store(x,(uint32_t)result_x);ac_store(y,(uint32_t)result_y);return;
    }
    ac_store(x,(uint32_t)end_x);ac_store(y,(uint32_t)end_y);
}
