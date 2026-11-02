#include "tool_selector.h"
int32_t FUN_00404120(void)
{
    int32_t count=0,row;
    for(row=0;row<30;++row) {
        uint32_t entry=(TS_U(0x7056ec)*30u+(uint32_t)row)*4u;
        int32_t kind=TS_I(0x5ce3bcu+entry),category=TS_I(0x7056ec);uint8_t enabled=0;
        if(kind<=0)return count;
        switch(category) {
        case 6:
            if(kind==0x30) {
                if((uint8_t)FUN_00403ef0()) {int32_t have=FUN_004f5020();if(have<FUN_004f4ff0())enabled=1;}
            } else if(kind==0x61&&TS_W(0xea5ce8u+TS_U(0xeaac28)*0x49d8u)) {
                if(!FUN_004c52d0(TS_I(0xea5f9cu+TS_U(0xeaac28)*0x49d8u),1))enabled=1;
                else enabled=!FUN_004c52d0(TS_I(0xea5f9cu+TS_U(0xeaac28)*0x49d8u),2);
            } else enabled=TS_I(0x70a8dcu+entry)!=0;
            break;
        case 10:
            if(kind==4) {
                if((uint8_t)FUN_00403ef0()) {
                    if((uint8_t)FUN_004f5470(0xb3)||(uint8_t)FUN_004f5470(0xb2)||(uint8_t)FUN_004f5470(0xb1))enabled=1;
                }
            } else enabled=TS_I(0x70a8dcu+entry)!=0;
            break;
        case 0x16:
            if((uint8_t)FUN_00403ef0())enabled=(uint8_t)FUN_004f5470(TS_I(0x5ce3bcu+(TS_U(0x7056ec)*30u+(uint32_t)row)*4u))!=0;
            break;
        case 0x1c:
            if((uint8_t)FUN_00403ef0()) {
                uint32_t index=(uint32_t)TS_I(0x5ce3bcu+(TS_U(0x7056ec)*30u+(uint32_t)row)*4u)-1u;
                if(TS_W(0x784de0u+index*2u))enabled=TS_I(0xea5fa0u+TS_U(0xeaac28)*0x49d8u+index*4u)==0;
            }
            break;
        case 0x24:enabled=!FUN_004c52d0(TS_I(0xea5f9cu+TS_U(0xeaac28)*0x49d8u),row+1);break;
        default:enabled=TS_I(0x70a8dcu+entry)!=0;break;
        }
        if(enabled)++count;
    }
    return count;
}
