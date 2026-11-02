#include "map_refresh_postpass3.h"
void FUN_00479e70(int32_t x,int32_t y,int32_t set)
{
    uint32_t gy=(uint32_t)P3_R(0xbca2ecu,4),gx=(uint32_t)P3_R(0xbca2e8u,4),test,index,flags,cols=3,rows=3,base;
    int32_t left,top,limit,endx,endy;uint8_t water,raw[10];uint16_t cw;MRFloat value;
    uint32_t instance,anchor,definition,sprite,height,parity,variant,byte_mark,byte_secondary,current_column,current_row;
    int32_t worldx=(int32_t)(gx+(uint32_t)x),worldy=(int32_t)(gy+(uint32_t)y);
    test=FUN_0046f930(worldx,worldy);P3_BOUNDS(worldx,worldy,test);if((uint8_t)test==0)return;
    index=(uint32_t)P3_R(0xbca2d8u,4)+(uint32_t)y*228u+(uint32_t)x;P3_W(0xbc9768u,4,index);
    flags=(uint32_t)P3_R(0xaf598cu+index*4u,4);if(set==1)flags|=0x10000u;else flags&=~0x10000u;
    left=(int32_t)((uint32_t)x-1u);top=(int32_t)((uint32_t)y-1u);P3_W(0xaf598cu+index*4u,4,flags);
    if(left<0){cols=(uint32_t)left+3u;left=0;}else{limit=(int32_t)P3_R(0xbca2d0u,4);if((int32_t)((uint32_t)left+3u)>limit)cols=(uint32_t)limit-(uint32_t)left;}
    if(top<0){rows=(uint32_t)top+3u;top=0;}else{limit=(int32_t)P3_R(0xbca2d4u,4);if((int32_t)((uint32_t)top+3u)>limit)rows=(uint32_t)limit-(uint32_t)top;}
    water=p3_water(left,top);base=(uint32_t)P3_R(0xbca2d8u,4);endy=(int32_t)((uint32_t)top+rows);
    P3_W(0xbc9760u,4,(uint32_t)top);index=base+(uint32_t)top*228u+(uint32_t)left;P3_W(0xbc9768u,4,index);if(top>=endy)return;
    endx=(int32_t)((uint32_t)left+cols);
    do{
        P3_W(0xbc9764u,4,(uint32_t)left);
        if(left<endx){
            do{
                flags=(uint32_t)P3_R(0xaf598cu+index*4u,4);
                if((flags&0x10000u)&&!(flags&0x148u)){
                    p3_gather(0x10000u);p3_invert();
                    index=(uint32_t)P3_R(0xbc9768u,4);instance=(uint32_t)P3_R(0xd05720u+index*4u,4);
                    if(instance&&(uint8_t)P3_R(instance+24u,1)==0){FUN_00484be0((void *)0x5cac58,24,0,1);}
                    else{
                        FUN_0047ddd0(4,1);
                        P3_W(0xbc97dcu,4,0);P3_W(0xbc97d4u,4,0);P3_W(0xbc97ccu,4,0);P3_W(0xbc97c4u,4,0);
                        /* Complete484D70 consumes only the first two of the
                           native caller's three pushed arguments(base,24,2). */
                        FUN_00484d70((void *)0x5cac58,24);
                    }
                    sprite=(uint32_t)P3_R(0xf56c34u,4);variant=(uint32_t)P3_R(0xbc9740u,4);P3_W(0xbc9748u,4,0);index=(uint32_t)P3_R(0xbc9768u,4);sprite+=variant;
                    instance=(uint32_t)P3_R(0xd05720u+index*4u,4);
                    if(instance){
                        anchor=(uint32_t)P3_R(instance+8u,4);height=(uint32_t)P3_R(0xa5d4ccu+anchor,1);cw=mr_fp_control_word();value=mr_fp_i32((int32_t)height);P3_FP(0x47a049u,value);
                        value=mr_fp_mul(value,mr_fp_binary64(P3_R(0x56f598u,8)),cw);P3_FP(0x47a04du,value);
                        value=mr_fp_mul(value,mr_fp_binary64(P3_R(0x56f590u,8)),cw);P3_FP(0x47a053u,value);P3_FP(0x56133cu,value);mr_fp_raw80(value,raw);variant=(uint32_t)restore_map_trunc80(raw);
                        parity=(uint32_t)P3_R(0xacf85cu+anchor,1)&1u;variant=(variant&~1u)|parity;
                        definition=(uint32_t)P3_R(instance,4);sprite+=(uint32_t)P3_R(definition+4u,4)+variant*6u;
                    }
                    if(water){base=(uint32_t)P3_R(0xf56c34u,4);variant=sprite-base;if((int32_t)variant>=60&&(int32_t)variant<=83)sprite+=(uint32_t)P3_R(0xf57234u,4)-base-60u;}
                    byte_secondary=(uint32_t)P3_R(0xb285ccu+index,1);byte_mark=(uint32_t)P3_R(0xb67d1cu+index,1);
                    P3_W(0xb285ccu+index,1,byte_secondary&0xf0u);P3_W(0xb7482cu+index*4u,4,sprite);P3_W(0xb67d1cu+index,1,byte_mark|0x40u);P3_W(0xae8e7cu+index,1,0);
                }
                current_column=(uint32_t)P3_R(0xbc9764u,4)+1u;++index;P3_W(0xbc9764u,4,current_column);P3_W(0xbc9768u,4,index);
            }while((int32_t)current_column<endx);
        }
        current_row=(uint32_t)P3_R(0xbc9760u,4)+1u;P3_W(0xbc9760u,4,current_row);index+=228u-cols;P3_W(0xbc9768u,4,index);
    }while((int32_t)current_row<endy);
}
