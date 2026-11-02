#include "map_validation.h"
static __inline void validation_error(uint32_t field,uint32_t message,uint32_t dirty)
{
    MVW(field,2,0);MVW(0xe38ddau,1,0x47);FUN_005344c0((int32_t)message);
    if(dirty)MVW(0xe38dcau,1,1);
}
static void sort_unique(uint32_t base,uint32_t count)
{
    uint32_t pass,i,right,left;
    for(pass=count;pass;--pass){
        for(i=count-1u;i;i--){
            right=MVR(base+i,1);
            if(right){
                left=MVR(base+i-1u,1);
                if(left==right)MVW(base+i,1,0);
                else if(left==0u||right<left){MVW(base+i-1u,1,right);MVW(base+i,1,left);}
            }
        }
    }
}
static __inline void check_flag(uint32_t field,uint32_t kind,uint32_t message)
{
    if(MVR(field,2)&&MVR(0xe38ddau,1)!=0x47u&&!FUN_00442870(kind))validation_error(field,message,1);
}
void FUN_00442a40(uint32_t record_index)
{
    uint32_t base=record_index*98u,i,sticky=0,product;
    MVE(0x442a40,record_index,0);
    if(MVR(0x77f760u+base,1)==0u&&FUN_00443000(record_index,2)&&!FUN_00442870(9)){
        for(i=0;i<14u;++i){
            if(MVR(0x77f766u+base+i,1)==2u){
                MVW(0x77f766u+base+i,1,0);MVW(0xe38ddau,1,0x47);FUN_005344c0(0x47);break;
            }
        }
    }
    sort_unique(0x77f766u+base,14);sort_unique(0x77f77cu+base,8);
    for(i=0;i<3u;++i){
        product=MVR(0x784de0u+i*2u,2);
        if(!FUN_0040c8b0((int32_t)(int16_t)product)){MVW(0x784de0u+i*2u,2,0);sticky=1;}
        if(sticky){MVW(0xe38ddau,1,0x47);FUN_005344c0(0x57);}
    }
    check_flag(0x784d0c,17,0xb2);check_flag(0x784cfc,15,0xb3);
    if(MVR(0x784d3au,2)&&MVR(0xe38ddau,1)!=0x47u){
        if(!FUN_00442870(8))validation_error(0x784d3a,0xec,1);
        if(!FUN_00442870(9))validation_error(0x784d3a,0xec,1);
    }
    check_flag(0x784d00,23,0xb4);check_flag(0x784d02,23,0xb5);
    check_flag(0x784d32,29,0xb6);check_flag(0x784d34,20,0xb7);
    if(MVR(0x784d2au,2)&&MVR(0xe38ddau,1)!=0x47u){
        if(MVR(0x784d28u,2)==0u)validation_error(0x784d2a,0xb8,1);
        if(MVR(0x784d32u,2)==0u&&!FUN_00442870(10))validation_error(0x784d2a,0xbc,1);
    }
    if(MVR(0x784d2cu,2)&&MVR(0xe38ddau,1)!=0x47u&&MVR(0x784d28u,2)==0u)validation_error(0x784d2c,0xb9,1);
    if(MVR(0x784d2eu,2)&&MVR(0xe38ddau,1)!=0x47u){
        if(MVR(0x784d28u,2)==0u)validation_error(0x784d2e,0xba,1);
        if(MVR(0x784d34u,2)==0u&&!FUN_00442870(28))validation_error(0x784d2e,0xbd,0);
    }
    if(MVR(0x784d30u,2)&&MVR(0xe38ddau,1)!=0x47u&&MVR(0x784d28u,2)==0u)validation_error(0x784d30,0xbb,1);
}
