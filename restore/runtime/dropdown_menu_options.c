#include "dropdown_menu.h"
void FUN_00403410(void)
{
    uint32_t value=DM_U(0xe38eacu);uint16_t entry;
    if(value==2){DM_U(0xe38eacu)=0;entry=2;}
    else if(!value){DM_U(0xe38eacu)=1;entry=3;}
    else{DM_U(0xe38eacu)=2;entry=4;}
    DM_W(0x5c9022u)=entry;DM_W(0x5c9062u)=entry;DM_U(0x70571cu)=1;
}
void FUN_00403490(void)
{
    if(DM_B(0xe38ec1u)){DM_B(0xe38ec1u)=0;DM_W(0x5c9032u)=5;}
    else{DM_W(0x5c9032u)=6;DM_B(0xe38ec1u)=1;}
    DM_U(0x70571cu)=1;
}
void FUN_00403500(void)
{
    if(DM_U(0xe38eecu)){DM_U(0xe38eecu)=0;DM_W(0x5c8ff2u)=9;}
    else{DM_W(0x5c8ff2u)=10;DM_U(0xe38eecu)=1;}
    DM_U(0x70571cu)=1;
}
void FUN_00403540(void)
{
    if(DM_B(0xe38ec3u)){DM_B(0xe38ec3u)=0;DM_W(0x5c9002u)=4;}
    else{DM_W(0x5c9002u)=5;DM_B(0xe38ec3u)=1;}
    DM_U(0x70571cu)=1;
}
void FUN_0040c7d0(void)
{
    if(DM_B(0xe39088u)){DM_B(0xe39088u)=0;DM_W(0x5c8fd2u)=7;}
    else{DM_W(0x5c8fd2u)=8;DM_B(0xe39088u)=1;}
    DM_U(0x70571cu)=1;
}
