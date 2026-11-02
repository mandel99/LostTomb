#include "family_menu.h"
#include "campaign_index.h"
#include "sound_streams.h"
#include "text_selection.h"
#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(a))
extern void FUN_004d5100(const char *);
extern void FUN_0041f870(void);
extern void FUN_00402450(void);
extern int FUN_00538cd0(const char *,const char *,uint32_t);

/* Local native procedures omitted from the exported procedure inventory. */
void FUN_004cb800(void)
{
    FUN_0055f561(RESTORE_SOUND,0);
    if(B(0xd3b18b)) {
        U(0x605a3c)=0;B(0x6de515)=0;FUN_004d5100((const char *)0x700998u);
        B(0xe38dda)=1;
    } else {
        U(0x7027ac)=0;U(0x7027b0)=0;U(0x6fdc98)=0;B(0xe38dda)=0x17;
    }
    B(0xe38dca)=1;
}
void FUN_004cb860(void)
{
    if(!B(0xd3b188))return;
    if(!FUN_0041ba80(RESTORE_CAMPAIGN_INDEX)) {
        FUN_004cc110(5,0x8d);FUN_004cc160(5,0x8e);
        if(FUN_00538cd0(*(const char **)0xe20768u,*(const char **)0xe20764u,4)==7)return;
    }
    if(B(0xd3b189)) {
        B(0xe38dda)=0x17;U(0x6fdc98)=1;
    } else {
        U(0x605a3c)=0;B(0x6de515)=0;U(0x704dac)=2;U(0x704db0)=1;FUN_0041f870();
    }
    B(0xe38dca)=1;
}
void FUN_004cb910(void)
{
    if(B(0xd3b18a)) {
        U(0x605a3c)=0;B(0x6de515)=0;FUN_00402450();U(0xd3b1bc)=0x30;
    }
}
void FUN_004cb940(void)
{
    U(0xe38ebc)=2;U(0x605a3c)=0;B(0x6de515)=0;U(0x704db0)=0;U(0x704dac)=3;
    FUN_0041f870();
}
