#include "standalone_data.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void FUN_004d1ad0(void);
static void require(int condition) {if(!condition) abort();}
int main(void)
{
    unsigned char *records=(unsigned char *)0x00ea1870u;
    unsigned char before[2][0x49d8];
    size_t city,i;
    if(!restore_data_open()) return 1;
    records[-1]=0x73; records[2*0x49d8]=0xb9;
    for(city=0;city<2;++city) for(i=0;i<0x49d8;++i)
        before[city][i]=records[city*0x49d8+i]=(unsigned char)(1+(i*13+city*89)%255);
    FUN_004d1ad0();
    require(records[-1]==0x73&&records[2*0x49d8]==0xb9);
    for(city=0;city<2;++city) {
        unsigned char *record=records+city*0x49d8;
        require(!memcmp(record,before[city],0x27d2));
        for(i=0x27d2;i<0x27de;++i) require(record[i]==0);
        require(!memcmp(record+0x27de,before[city]+0x27d2,60));
        require(!memcmp(record+0x48f0,before[city]+0x48d4,120));
        for(i=0x2b00;i<0x2b18;++i) require(record[i]==0);
        for(i=0x48d8;i<0x48f0;++i) require(record[i]==0);
        for(i=0x4968;i<0x4980;++i) require(record[i]==0);
        require(!memcmp(record+0x4980,before[city]+0x494c,10));
        for(i=0x498a;i<0x49d8;++i) require(record[i]==0);
    }
    restore_data_close();
    puts("City migration: two distinct records, preserved prefix, shifted fields, new zeros and boundary guards verified.");
    return 0;
}
