#include "load_save_menu.h"
#include "file_list.h"
#include "menu_input.h"
#include <string.h>
#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define I(a) (*(int32_t *)(uintptr_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(a))
extern char *FUN_005169c0(void);
extern void FUN_004cdff0(const char *,char);
extern void FUN_0040f8f0(void *,int,int);
extern void FUN_004cc500(int),FUN_004cc3c0(void),FUN_004cc3e0(void);
extern uint32_t FUN_004d6740(int,int,const void *,int,uint32_t);
extern void FUN_00425bc0(int,int,const void *,int,int);
extern int FUN_00425990(int,int,const void *,int);

/* Local entries omitted from the REA procedure export. */
static void enter(uint8_t mode)
{
    char pattern[260];
    strcpy(pattern,FUN_005169c0());strcat(pattern,(const char *)0x5761c4u);
    FUN_004cdff0(pattern,0);
    U(0xe20998)=0;B(0xe38dda)=13;U(0xd3b1bc)=1;B(0xd3b1c0)=mode;
    FUN_0040f8f0((void *)0xe37100u,64,0);
    strcpy((char *)0xe37100u,(const char *)0xe370bcu);
    FUN_004cc500(2);FUN_004cc3c0();FUN_004cc3e0();B(0xe38dca)=1;
}
void FUN_00402540(void){enter(0);}
void FUN_00402630(void){enter(2);}

void FUN_00402a80(void)
{
    int32_t selected=I(0xd3b198),index;
    char *destination;const char *name;
    if(selected>=I(0x6059f8))return;
    U(0x6059f0)=(uint32_t)selected;
    destination=(char *)(B(0xe38dda)==13?0xe37100u:0xe37078u);
    FUN_0040f8f0(destination,64,0);
    index=(int32_t)(U(0x6059f0)+U(0x6059f4));
    name=FUN_0041e010(RESTORE_SAVE_FILES,index);
    /* Invalid source selection has no defined native behavior. */
    if(!name)return;
    strcpy(destination,name);
    FUN_004cc500(B(0xe38dda)==13?2:4);
    FUN_004cc3c0();FUN_004cc3e0();B(0xd3b1b3)=0;
}

/* Native mode-13 branch in 004d8310. Re-entering another mode during confirm
   still executes the remaining original branch in the current iteration. */
void restore_load_save_input(void)
{
    int x,y;uint32_t hit;
    FUN_004cc500(2);
    if(U(0xe20998)){U(0xe20998)=0;U(0xd3b198)=1;FUN_00402720();}
    if(B(0xe38df8)){B(0xe38df8)=0;B(0xe38dda)=B(0xd3b1bc);B(0xe38dca)=1;return;}
    x=I(0xe38da8);y=I(0xe38da4);
    hit=FUN_00426450(x,y,(const RestoreMenuButton *)0x5c3118u,12);
    U(0x7056c8)=FUN_004d6740(x,y,(const void *)0x5c3118u,12,hit);
    if(FUN_004261d0(x,y,(RestoreMenuButton *)0x5c3118u,12))return;
    FUN_00425bc0(x,y,(const void *)0x5c79f0u,4,10);
    if(FUN_00425990(x,y,(const void *)0x5c79f0u,4))return;
    FUN_004029d0();
}
