#include "order_branch.h"
int32_t FUN_004b5640(uint32_t tile)
{
    int32_t id=OB_S(0xb350dcu+tile*2u);
    while(id>0){
        uint32_t actor=0xbcd5f8u+(uint32_t)id*388u;uint8_t state=OB_B(actor+56u);
        if(state!=4){
            uint8_t kind=OB_B(actor+10u);
            if((kind>=0x2b&&kind<=0x39)||(kind==0x29&&state==11))return id;
            switch(kind){case 0x52:case 0x53:case 0x68:case 0x66:case 0x67:case 0x54:case 0x6d:case 0x63:case 0x64:case 0x65:case 0x5d:case 0x5c:return id;}
        }
        id=OB_S(actor+8u);
    }
    return 0;
}
