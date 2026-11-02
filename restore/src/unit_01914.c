#include "event_publisher.h"
void FUN_004e14b0(uint32_t message)
{
    uint32_t action;
    switch(message) {
    case 0x71:case 0x76:case 0x77:case 0x7a:case 0x7b:case 0x7c:case 0x7d:case 0x7e:case 0x7f:case 0x80:case 0x81:action=5;break;
    case 0x82:case 0x135:case 0x13a:action=4;break;
    case 0x84:action=8;break;
    case 0x87:case 0x88:action=1;break;
    case 0x12d:case 0x12e:case 0x12f:action=6;break;
    default:action=0;break;
    }
    *(uint32_t *)0xd3a6e8u=action;
}
