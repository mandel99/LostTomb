#include "settings_callbacks.h"
void FUN_00403320(void)
{
    uint32_t selected,value;
    if(SSET_U(0x605a3c)==1)return;
    selected=SSET_U(0xd3b198);
    if(selected==1)SSET_PUT32(0xe38e6c,SSET_U(0xe38e6c)-10u);else if(selected==0)SSET_PUT32(0xe38e6c,SSET_U(0xe38e6c)+10u);
    value=SSET_U(0xe38e6c);if((int32_t)value<10){SSET_PUT32(0xe38e6c,10);SSET_PUT8(0xe38dca,1);return;}
    if((int32_t)value>100)SSET_PUT32(0xe38e6c,100);SSET_PUT8(0xe38dca,1);
}
