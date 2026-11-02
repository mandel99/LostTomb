#include "campaign_parser.h"
#include <string.h>
static void put32(uint8_t *p,int32_t value) {memcpy(p,&value,4);}
void FUN_0041ac00(RestoreCampaignRecord *record)
{
    uint8_t *p=record->bytes;int i;
    p[0]=255;p[1]=0;p[7]=0;
    for(i=8;i<=0x1c;i+=4)put32(p+i,-1);
    for(i=0;i<2;++i) {
        put32(p+0x20+i*12,(i+1)*50);put32(p+0x24+i*12,(i+1)*50);
        p[0x28+i*12]=255;p[0x29+i*12]=255;p[0x2a+i*12]=0;
    }
    put32(p+0x38,0);p[0x3c]=0;
}
