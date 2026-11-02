#include "map_refresh_postpass1.h"
#include "city_prepare.h"
#include "map_x87_conversion.h"
void FUN_00479b70(int32_t x,int32_t y,int32_t set)
{
    uint32_t gy=(uint32_t)P1_R(0xbca2ecu,4),gx=(uint32_t)P1_R(0xbca2e8u,4),test,base,index,flags;
    int32_t px=(int32_t)(gx+(uint32_t)x),py=(int32_t)(gy+(uint32_t)y),left,top,cols=3,rows=3,limit,yend;
    uint32_t cx,remaining_x,remaining_y,first,second,record,byte_value,variant;uint16_t cw;uint8_t raw[10];MRFloat value;
    test=FUN_0046f930(px,py);P1_BOUNDS(px,py,test);
    if((uint8_t)test==0)return;
    base=(uint32_t)P1_R(0xbca2d8u,4);index=base+(uint32_t)y*228u+(uint32_t)x;
    flags=(uint32_t)P1_R(0xaf598cu+index*4u,4);if(set==1)flags|=0x800u;else flags&=~0x800u;P1_W(0xaf598cu+index*4u,4,flags);
    left=(int32_t)((uint32_t)x-1u);top=(int32_t)((uint32_t)y-1u);
    if(left<0){cols=(int32_t)((uint32_t)left+3u);left=0;}
    else{limit=(int32_t)P1_R(0xbca2d0u,4);if((int32_t)((uint32_t)left+3u)>limit)cols=(int32_t)((uint32_t)limit-(uint32_t)left);}
    if(top<0){rows=(int32_t)((uint32_t)top+3u);top=0;}
    else{limit=(int32_t)P1_R(0xbca2d4u,4);if((int32_t)((uint32_t)top+3u)>limit)rows=(int32_t)((uint32_t)limit-(uint32_t)top);}
    P1_W(0xbc9760u,4,(uint32_t)top);index=base+(uint32_t)top*228u+(uint32_t)left;yend=(int32_t)((uint32_t)top+(uint32_t)rows);P1_W(0xbc9768u,4,index);
    if(top>=yend)return;
    remaining_y=(uint32_t)yend-(uint32_t)top;P1_W(0xbc9760u,4,(uint32_t)yend);
    do{
        cx=(uint32_t)left+(uint32_t)cols;P1_W(0xbc9764u,4,(uint32_t)left);
        if(left<(int32_t)cx){
            remaining_x=cx-(uint32_t)left;P1_W(0xbc9764u,4,cx);
            do{
                flags=(uint32_t)P1_R(0xaf598cu+index*4u,4);
                if((flags&0x800u)&&!(flags&0x700u)&&!(flags&0x40u)&&(!(flags&8u)||(flags&0x10000000u))&&!(flags&0x20u)&&!(flags&0x1000u)){
                    byte_value=(uint32_t)P1_R(0xa5d4ccu+index,1);cw=mr_fp_control_word();value=mr_fp_i32((int32_t)(byte_value-1u));P1_FP(0x479d1f,value);
                    value=mr_fp_mul(value,mr_fp_binary64(P1_R(0x56f588u,8)),cw);P1_FP(0x479d23,value);
                    value=mr_fp_mul(value,mr_fp_binary64(P1_R(0x56f580u,8)),cw);P1_FP(0x479d29,value);P1_FP(0x56133c,value);mr_fp_raw80(value,raw);first=(uint32_t)restore_map_trunc80(raw);
                    byte_value=(uint32_t)P1_R(0xa509bcu+index,1);if(byte_value>100u)byte_value=100u;
                    value=mr_fp_i32((int32_t)(byte_value-1u));P1_FP(0x479d55,value);
                    value=mr_fp_div(value,mr_fp_binary64(P1_R(0x56f570u,8)),cw);P1_FP(0x479d59,value);
                    value=mr_fp_mul(value,mr_fp_binary64(P1_R(0x56f578u,8)),cw);P1_FP(0x479d5f,value);P1_FP(0x56133c,value);mr_fp_raw80(value,raw);second=(uint32_t)restore_map_trunc80(raw);
                    record=0x216u;
                    /* In each fallback path native EAX is zero before MOV AL,
                       or explicitly XORed to zero. Thus the signed remainder
                       correction blocks cannot execute for these BYTE inputs. */
                    if(first==0&&second==0){record=0x233u;second=(uint32_t)P1_R(0xacf85cu+index,1)&7u;}
                    else if(first==2&&second==0){record=0x228u;first=0;second=(uint32_t)P1_R(0xacf85cu+index,1)&7u;}
                    else if(first==2&&second==11){record=0x227u;first=0;second=(uint32_t)P1_R(0xacf85cu+index,1)&7u;}
                    variant=(uint32_t)P1_R(0xf55bb4u+record*8u,4);variant+=first*12u+second;P1_W(0xb7482cu+index*4u,4,variant);
                    variant=(uint32_t)P1_R(0xb285ccu+index,1)&0xf0u;P1_W(0xb285ccu+index,1,variant);
                    variant=(uint32_t)P1_R(0xb67d1cu+index,1)|0x40u;P1_W(0xb67d1cu+index,1,variant);P1_W(0xae8e7cu+index,1,0);
                }
                ++index;--remaining_x;
            }while(remaining_x);
        }
        index+=228u-(uint32_t)cols;--remaining_y;
    }while(remaining_y);
    P1_W(0xbc9768u,4,index);
}
