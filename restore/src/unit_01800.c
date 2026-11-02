#include "restore_types.h"
#include "mission_compat.h"
void FUN_004d19e0(void)
{
    unsigned i,bit;
    for(i=0;i<200;++i) {
        uint8_t *record=(uint8_t *)(0x77f748u+i*98u);
        uint8_t converted[98]={0};
        uint32_t high=*(uint32_t *)(record+64),medium=*(uint32_t *)(record+68),low=*(uint32_t *)(record+72);
        memcpy(converted,record,76);
        for(bit=0;bit<32;++bit) {
            uint32_t mask=1u<<bit;
            converted[62+bit]=(high&mask)?3:((medium&mask)?2:((low&mask)?1:0));
        }
        memset(converted+94,2,4);
        memcpy(record,converted,sizeof(converted));
    }
}
