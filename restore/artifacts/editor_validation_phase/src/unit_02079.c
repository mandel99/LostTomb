#include "editor_validation.h"
uint32_t FUN_004ec410(uint32_t terrain,char mode)
{
    uint32_t scene=EV_U(0x784cb0)&255u,allowed,masked,offset;
    if(mode!=0){offset=scene*48u;allowed=EV_U(0x5e4c24u+offset);}
    else{offset=((uint32_t)EV_S(0x784b94)+scene*2u)*48u;allowed=EV_U(0x5e4cb4u+offset);}
    if((terrain&0x3038d022u)!=0)return terrain&0xffffff00u;
    masked=terrain&0x26351277u;
    if(allowed!=0||masked!=0){
        if(EV_U(0x605a3c)==1)return(masked&0xffffff00u)|((masked&allowed)!=0);
        if((masked&4u)!=0 && (allowed&4u)==0)return masked&0xffffff00u;
        if((masked&0x40000u)!=0 && (allowed&0x40000u)==0)return masked&0xffffff00u;
    }
    return(masked&0xffffff00u)|1u;
}
