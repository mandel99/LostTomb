#include "campaign_parser.h"
int FUN_0041ad50(int32_t *value)
{
    const char *text=FUN_00561db1(NULL,",="),*p;uint32_t number=0;int negative;
    if(!text)return 0;
    negative=*text=='-';p=text+negative;
    for(;*p;++p) {
        if((uint8_t)*p<'0'||(uint8_t)*p>'9')return 2;
        number=number*10u+(uint8_t)*p-'0';
    }
    if(negative)number=0u-number;
    *value=(int32_t)number;return 1;
}
