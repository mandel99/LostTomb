#include "map_refresh_postpass4.h"
void FUN_0047e5c0(void)
{
    static const int32_t offsets[4]={-228,1,228,-1};
    uint32_t tile=P4_R(0xbc9768u,4),i,flags,record,type;
    for(i=0;i<4;++i){
        flags=P4_R(p4_terrain(tile,offsets[i]),4);
        if(flags&0x8000u){
            record=p4_record(tile,offsets[i]);type=P4_R(record,2);
            if((type==0x3au||type==0xaau||type==0xabu||type==0xc8u||type==0xc9u||type==0xcau)&&P4_R(record+2u,2)==(i&1u?2u:1u))P4_W(0xbc97c0u+i*8u,4,1);
        }
    }
}
