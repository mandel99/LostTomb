#include "chunk_io.h"
#include <stdlib.h>
#include <string.h>
#define H(p,o) (*(int16_t *)((uint8_t *)(p)+(o)))
#define D(p,o) (*(uint32_t *)((uint8_t *)(p)+(o)))
void FUN_004d0d40(void *record,int old)
{
    uint8_t expanded[48]={0};
    if(old) {
        memcpy(expanded,record,44);D(expanded,0)=D(record,0);D(expanded,4)=(uint16_t)H(record,6);
        H(expanded,8)=H(record,4);H(expanded,10)=H(record,8);H(expanded,12)=H(record,10);
        expanded[14]=((uint8_t *)record)[12];expanded[15]=((uint8_t *)record)[13];
        memcpy(expanded+16,(uint8_t *)record+14,30);
    }
    memcpy(record,expanded,sizeof(expanded));
}
void FUN_004d0c60(void *record,int old)
{
    uint8_t expanded[18628]={0};unsigned i,j;
    static const int16_t values[][2]={{0, -1},{6,1},{8,-1},{10,-1},{14,-1},{16,5},{18,10},{22,-1},{24,1},{26,2},{30,-1},{32,1},{34,8},{36,-1},{38,-1},{44,2},{46,0},{48,0},{52,-1},{56,0},{82,-1},{84,-1},{90,-1},{92,0},{94,49}};
    if(!old){memset(record,0,sizeof(expanded));return;}
    for(i=0;i<150;++i) {
        uint8_t *row=expanded+4+i*124;
        for(j=4;j<=34;j+=2)H(row,j)=-1;
        for(j=88;j<=94;j+=2)H(row,j)=-1;
        for(j=0;j<sizeof(values)/sizeof(values[0]);++j)H(row,values[j][0])=values[j][1];
        D(row,40)=2;H(row,0)=(int16_t)i;
    }
    D(expanded,18604)=0xffffffffu;expanded[4+140*124+2]=2;
    memcpy(expanded,record,15628);
    for(i=0;i<150;++i) {
        uint8_t *row=expanded+4+i*124;memcpy(row,(uint8_t *)record+4+i*104,104);
        H(row,102)=H(row,104)=0;
    }
    memcpy(expanded+18604,(uint8_t *)record+15604,10);
    memcpy(record,expanded,sizeof(expanded));
}
void restore_chunk_migrate(uint32_t address,void *record,int old)
{
    if(address==0x4d0c60u){FUN_004d0c60(record,old);return;}
    if(address==0x4d0d40u){FUN_004d0d40(record,old);return;}
    abort();
}
