#include "event_packet.h"
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
int32_t restore_event_message_kind_0044f660(uint32_t self)
{
    static const int32_t town[7]={0x35,0x11,0x23,0x47,0x59,0x6b,0x7d};
    static const int32_t requests[4]={0xbe,0xb4,0xaa,0x107};
    uint32_t subtype;
    switch(B(self+2u)){
    case 1:subtype=B(self+0x60u);if(subtype<7u)return town[subtype]-(B(self+0x56u)!=0);break;
    case 2:subtype=W(self+4u);if(subtype>=1u&&subtype<=4u)return requests[subtype-1u];break;
    case 3:return 0xda;case 6:return 0xde;case 7:return 0xe2;case 8:return 0xe6;
    case 9:return 0xea;case 10:return 0xee;case 11:return 0xf2;
    case 13:return 0xd2;case 14:return 0xd6;case 15:return 0xca;case 16:return 0xce;
    case 17:return 0xc2;case 18:return 0xc6;
    case 19:subtype=B(self+0x60u);if(subtype<5u)return (int32_t)(0x81u+subtype*4u);break;
    case 20:subtype=B(self+0x60u);if(subtype<4u)return (int32_t)(0x95u+subtype*4u);break;
    case 21:return 0xfa;case 22:return 0xfe;
    case 23:return 0x125-(B(self+0x56u)!=0);
    }
    return -1;
}
