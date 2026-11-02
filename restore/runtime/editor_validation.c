#include "editor_validation.h"
uint8_t EV_PUBLIC(FUN_00409620)(uint32_t terrain)
{
    uint32_t masked;
    if((terrain&0x3038d022u)!=0)return 0;
    masked=terrain&0x26351277u;
    if(masked==0)return 1;
    if((masked&4u)!=0)return 0;
    return(uint8_t)(~(masked>>18)&1u);
}
uint8_t EV_PUBLIC(FUN_004ec360)(void)
{
    uint32_t cursor,index;int32_t x,y;
    for(cursor=0x784b64;cursor<0x784b6c;cursor+=2u){
        x=EV_S(cursor);if(x<0)continue;y=EV_S(cursor+8u);if(y<0)continue;
        index=EV_U(0xbca2d8)+(uint32_t)x+(uint32_t)y*228u;
        if((uint8_t)FUN_004ec410(EV_U(0xaf598cu+index*4u),0)==0)return 0;
    }
    for(cursor=0x784cc0;cursor<0x784cd0;cursor+=4u){
        x=EV_I(cursor);if(x<0)continue;y=EV_I(cursor+16u);if(y<0)continue;
        index=(uint32_t)x+(uint32_t)y*228u+EV_U(0xbca2d8);
        if((uint8_t)FUN_004ec410(EV_U(0xaf598cu+index*4u),1)==0)return 0;
    }
    return 1;
}
uint8_t EV_PUBLIC(FUN_004095c0)(void)
{
    uint32_t base=EV_U(0xbca2d8),cursor,index;int32_t x,y;
    for(cursor=0x784b74;cursor<0x784b84;cursor+=2u){
        x=EV_S(cursor);if(x<0)continue;y=EV_S(cursor+16u);if(y<0)continue;
        index=base+(uint32_t)x+(uint32_t)y*228u;
        if((EV_B(0xaf598cu+index*4u)&4u)==0)return 0;
    }
    return 1;
}
uint8_t EV_PUBLIC(FUN_00409650)(void)
{
    uint32_t index;int32_t x=EV_S(0x784c64),y=EV_S(0x784c66);
    if(x>0||y>0){index=EV_U(0xbca2d8)+(uint32_t)x+(uint32_t)y*228u;if(FUN_00409620(EV_U(0xaf598cu+index*4u))==0)return 0;}
    x=EV_S(0x784c68);y=EV_S(0x784c6a);
    if(x>0||y>0){index=EV_U(0xbca2d8)+(uint32_t)x+(uint32_t)y*228u;if(FUN_00409620(EV_U(0xaf598cu+index*4u))==0)return 0;}
    return 1;
}
uint8_t EV_PUBLIC(FUN_004096e0)(void)
{
    int32_t x=EV_S(0x784c8c);uint32_t base=EV_U(0xbca2d8),index;int32_t y=EV_S(0x784c8e);
    if(x>0||y>0){index=base+(uint32_t)x+(uint32_t)y*228u;if((EV_B(0xaf598cu+index*4u)&4u)==0)return 0;}
    x=EV_S(0x784c90);y=EV_S(0x784c92);
    if(x>0||y>0){index=base+(uint32_t)x+(uint32_t)y*228u;if((EV_B(0xaf598cu+index*4u)&4u)==0)return 0;}
    return 1;
}
uint8_t EV_PUBLIC(FUN_00409760)(void)
{
    uint32_t cursor,index;int32_t x,y;
    for(cursor=0x784bc0;cursor<0x784bd0;cursor+=2u){
        x=EV_S(cursor);if(x<0)continue;y=EV_S(cursor+32u);if(y<0)continue;
        index=EV_U(0xbca2d8)+(uint32_t)x+(uint32_t)y*228u;
        if(FUN_00409620(EV_U(0xaf598cu+index*4u))==0)return 0;
    }
    return 1;
}
uint8_t EV_PUBLIC(FUN_004097c0)(void)
{
    uint32_t base=EV_U(0xbca2d8),cursor,index;int32_t x,y;
    for(cursor=0x784bd0;cursor<0x784be0;cursor+=2u){
        x=EV_S(cursor);if(x<0)continue;y=EV_S(cursor+32u);if(y<0)continue;
        index=base+(uint32_t)x+(uint32_t)y*228u;
        if((EV_B(0xaf598cu+index*4u)&4u)==0)return 0;
    }
    return 1;
}
static void validation_message(int first,int second)
{
    uint32_t body,title;
    FUN_004cc110(5,first);FUN_004cc160(5,second);
    body=EV_U(0xe20764);title=EV_U(0xe20768);(void)FUN_00538cd0(title,body,0);
}
void EV_PUBLIC(FUN_00409830)(void)
{
    if(FUN_004ec360()==0){validation_message(0x6c,0x6d);return;}
    if(FUN_004095c0()==0){validation_message(0x7f,0x80);return;}
    if(FUN_00409650()==0){validation_message(0x81,0x82);return;}
    if(FUN_004096e0()==0){validation_message(0x83,0x84);return;}
    if(FUN_00409760()==0){validation_message(0x85,0x86);return;}
    if(FUN_004097c0()==0){validation_message(0x87,0x88);return;}
    if(!FUN_00409820()){validation_message(0x79,0x7a);return;}
    FUN_004cdff0(0x576260u,1);
    EV_PUT32(0xe20998,0);EV_PUT8(0xe38dda,16);EV_PUT8(0xd3b1c0,0);EV_PUT32(0xf53f2c,0);EV_PUT8(0xe38dca,1);
}
