#include "campaign_menu.h"
#include "campaign_index.h"
#include "video_player.h"
#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define I(a) (*(int32_t *)(uintptr_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(a))
extern void FUN_004181e0(void);
extern void FUN_0041f870(void);
void FUN_0041bb10(void)
{B(0xe38dca)=1;U(0x7027ac)=U(0xd3b198);}
void FUN_0041bb70(void)
{
    static const uint32_t movies[5][2]={{0x57c3a0,0x57c380},{0x57c364,0x57c348},{0x57c328,0x57c308},{0x57c2e4,0x57c2c0},{0x57c2a0,0x57c284}};
    uint32_t category=U(0x7027ac),mode=U(0x6fdc98);
    if(!mode&&category!=U(0x7027b0))return;
    B(0xe38ea8)=0;U(0x605a3c)=0;B(0x6de515)=0;U(0xe38ebc)=0;
    U(0x5d2a2c)=1;B(0xe38dca)=1;B(0x7027b4)=(mode!=0)+1;
    if(category<5) {
        const char *movie=(const char *)(uintptr_t)movies[category][I(0xe38edc)>=233];
        FUN_00413580(RESTORE_CURRENT_VIDEO,movie,(I(0xe38db0)-480)/2,(I(0xe38dac)-286)/2);
    }
    FUN_00417dd0((int8_t)category);
    if(B(0xe38ea8)==0x7f)B(0xe38dda)=0x34;else FUN_004181e0();
}
void FUN_0041c310(void)
{
    if(B(0xe38dda)==23&&U(0x6fdc98)==1) {
        U(0x704db0)=1;U(0x605a3c)=0;B(0x6de515)=0;U(0x704dac)=2;
        FUN_0041f870();B(0xe38dca)=1;
    }
}
void FUN_0041c350(void)
{
    if(B(0xe38dda)!=23) {B(0xe38dda)=23;U(0x6fdc98)=1;B(0xe38dca)=1;}
}
