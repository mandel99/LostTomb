#include "tool_selector_input.h"
int32_t FUN_00403f10(int32_t category,int32_t previous)
{
    uint32_t row=(uint32_t)previous+1u,phase=row+1u,offset=((uint32_t)category*30u+row)*4u,other,player;
    int32_t kind,have;
    if((int32_t)row>=30)return 0;
    do{
        kind=TI_I(0x5ce3bcu+offset);
        if(kind<=0)return 0;
        switch(category){
        case 6:
            if(kind==0x30){
                if((uint8_t)FUN_00403ef0()){
                    have=FUN_004f5020();
                    if(have<FUN_004f4ff0())return(int32_t)row;
                }
            }else{
                other=(TI_U(0x7056ec)*30u+row)*4u;
                if(TI_U(0x5ce3bcu+other)!=0x61u){
                    if(TI_I(0x70a8dcu+offset)!=0)return(int32_t)row;
                }else{
                    player=TI_U(0xeaac28)*0x49d8u;
                    if(TI_W(0xea5ce8u+player)==0){
                        if(TI_I(0x70a8dcu+other)!=0)return(int32_t)row;
                    }else{
                        if(!FUN_004c52d0(TI_I(0xea5f9cu+player),1))return(int32_t)row;
                        if(!FUN_004c52d0(TI_I(0xea5f9cu+TI_U(0xeaac28)*0x49d8u),2))return(int32_t)row;
                    }
                }
            }
            break;
        case 10:
            if(kind!=4){if(TI_I(0x70a8dcu+offset)!=0)return(int32_t)row;}
            else if((uint8_t)FUN_00403ef0()){
                if((uint8_t)FUN_004f5470(0xb3))return(int32_t)row;
                if((uint8_t)FUN_004f5470(0xb2))return(int32_t)row;
                if((uint8_t)FUN_004f5470(0xb1))return(int32_t)row;
            }
            break;
        case 0x16:
            if((uint8_t)FUN_00403ef0() && (uint8_t)FUN_004f5470(TI_I(0x5ce3bcu+offset)))return(int32_t)row;
            break;
        case 0x1c:
            if((uint8_t)FUN_00403ef0() && TI_I(0xea5fa0u+TI_U(0xeaac28)*0x49d8u+row*4u)==0)return(int32_t)row;
            break;
        case 0x24:
            if(!FUN_004c52d0(TI_I(0xea5f9cu+TI_U(0xeaac28)*0x49d8u),(int32_t)phase))return(int32_t)row;
            break;
        default:if(TI_I(0x70a8dcu+offset)!=0)return(int32_t)row;break;
        }
        ++row;offset+=4u;++phase;
    }while((int32_t)phase<31);
    return 0;
}
