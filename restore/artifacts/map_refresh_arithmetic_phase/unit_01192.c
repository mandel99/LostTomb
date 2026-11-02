#include "map_refresh_arithmetic.h"
#include "map_refresh.h"
#include "map_x87_conversion.h"
int32_t FUN_00481540(int32_t x,int32_t y,int32_t rings,uint32_t mask)
{
    uint16_t cw=mr_fp_control_word();uint64_t total=0,score=0,weight;uint8_t raw[10];MRFloat v,score_value;
    uint32_t base_index=(uint32_t)x+(uint32_t)y*228u,ring=(uint32_t)rings,point,offset_address,index,flags,c0;
    int32_t count,offset;
    if(ring){
        do{
            v=mr_fp_i32((int32_t)ring);MRA_FP(0x481595,v);
            offset=(int16_t)MRA_R(0x5c9b54u+(ring*2u-2u),2);
            v=mr_fp_div(mr_fp_binary64(MRA_R(0x56f520u,8)),v,cw);MRA_FP(0x48159f,v);
            count=(int16_t)MRA_R(0x5c9b00u+(ring*2u-2u),2);
            offset_address=0x70c8ecu+(uint32_t)offset*2u;
            index=base_index+(uint32_t)MRA_R(0xbca2d8u,4);MRA_WRITE(0xbc9768u,index);
            v=mr_fp_mul(v,mr_fp_binary64(MRA_R(0x56f470u,8)),cw);MRA_FP(0x4815c9,v);
            weight=mr_fp_store64(v,cw);MRA_SPILL(0x4815cf,weight);
            for(point=0;(int32_t)point<count;++point){
                if(!FUN_00481aa0(x,y,1,(int32_t)ring,(int32_t)point)){
                    index=(uint32_t)MRA_R(0xbc9768u,4);
                    v=mr_fp_binary64(weight);MRA_FP(0x4815fb,v);
                    offset=(int16_t)MRA_R(offset_address,2);
                    v=mr_fp_add(v,mr_fp_binary64(total),cw);MRA_FP(0x481602,v);
                    flags=(uint32_t)MRA_R(0xaf598cu+(index+(uint32_t)offset)*4u,4);
                    total=mr_fp_store64(v,cw);MRA_SPILL(0x481612,total);
                    if(mask&flags){v=mr_fp_binary64(weight);MRA_FP(0x48161a,v);v=mr_fp_add(v,mr_fp_binary64(score),cw);MRA_FP(0x48161e,v);}
                    else{v=mr_fp_binary64(score);MRA_FP(0x481624,v);v=mr_fp_sub(v,mr_fp_binary64(weight),cw);MRA_FP(0x481628,v);}
                    score=mr_fp_store64(v,cw);MRA_SPILL(0x48162c,score);
                }
                offset_address+=2u;
            }
            --ring;
        }while(ring);
        score_value=mr_fp_binary64(score);MRA_FP(0x481657,score_value);
        v=mr_fp_binary64(MRA_R(0x56f5a0u,8));
        /* Native consumes C0 only; score is finite in the caller domain. */
        c0=score_value.kind==2||(score_value.negative&&score_value.significand!=0);
        MRA_COMPARE(0x48165b,c0);if(c0)score=0;
    }
    v=mr_fp_binary64(score);MRA_FP(0x481670,v);
    v=mr_fp_div(v,mr_fp_binary64(total),cw);MRA_FP(0x481674,v);
    v=mr_fp_mul(v,mr_fp_binary64(MRA_R(0x56f470u,8)),cw);MRA_FP(0x481678,v);
    MRA_FP(0x56133c,v);mr_fp_raw80(v,raw);
    return (int32_t)(uint32_t)restore_map_trunc80(raw);
}
