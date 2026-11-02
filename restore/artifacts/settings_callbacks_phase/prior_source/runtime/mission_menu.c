#include "mission_menu.h"
#include "menu_input.h"
#include "file_list.h"
#include "sound_streams.h"
#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define I(a) (*(int32_t *)(uintptr_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(a))
extern void *FUN_0041e100(void);
extern void *FUN_0041e120(void);
extern void *FUN_0041e130(void);
extern uint32_t FUN_0041e140(void);
extern unsigned char FUN_0041e170(void);
extern void *FUN_0041e190(void);
extern int FUN_0041f210(void);
extern int FUN_00425990(int,int,void *,int);
extern int FUN_004d6740(int,int,const void *,int,int);
extern void FUN_00425730(int,int,void *,int);
extern void FUN_005405b0(char *,char);
extern void FUN_004181e0(void);
extern void FUN_00540c90(void);
extern void FUN_00540ed0(void);
extern void FUN_00541160(void);
extern void FUN_004bc720(void);
extern int FUN_004cde40(int,int);

void FUN_00402e50(void)
{
    int32_t difficulty=(int32_t)(U(0xe39080)+(U(0xd3b198)?0xffffffffu:1u));
    if(difficulty<0)difficulty=0;else if(difficulty>=5)difficulty=4;
    U(0xe39080)=difficulty;
    FUN_00540c90();FUN_00540ed0();FUN_00541160();FUN_004bc720();
    if(B(0xe38dda)==53)
        U(0xea187c+U(0xeaac28)*0x49d8u)=FUN_004cde40(I(0x7848f8),I(0x5d1960+difficulty*4));
    B(0xe38dca)=1;
}

void FUN_0041e0e0(void)
{
    B(0xe38dca)=1;U(0x704dac)=(U(0x704dac)!=3)+2;
}

void FUN_0041f5c0(void)
{
    uint32_t kind;
    FUN_0055f561(RESTORE_SOUND,0);
    kind=FUN_0041dfe0(RESTORE_MISSION_FILES,I(0x704dbc));
    if(kind>1)return;
    B(0x7027b4)=kind?3:0;
    FUN_005405b0((char *)0xe37078u,(char)kind);
    if(B(0x7027b4)==3)FUN_004181e0();
    else {B(0xe38dda)=1;B(0xe38dd6)=2;}
    B(0xe38dca)=1;
}

/* Original 004dcfca..004dd169 branch. Getter calls have no arguments;
   parameters left on the native stack belong to the following UI call. */
void restore_mission_menu_input(void)
{
    int x=I(0xe38da8),y=I(0xe38da4),hit;
    if(FUN_0041f4d0())return;
    if(FUN_00425990(x,y,FUN_0041e100(),FUN_0041e170()))return;
    if(FUN_0041e140()&&FUN_00425990(x,y,FUN_0041e130(),2))return;
    if(U(0x704db0)==1) {
        hit=FUN_00426450(x+360,y+410,FUN_0041e120(),1);
        U(0x7056c4)=FUN_004d6740(x+360,y+410,FUN_0041e120(),1,hit);
        if(FUN_004261d0(x+350,y+410,FUN_0041e120(),1))return;
    }
    hit=FUN_00426450(x,y,FUN_0041e190(),FUN_0041f210());
    U(0x7056c8)=FUN_004d6740(x,y,FUN_0041e190(),FUN_0041f210(),hit);
    if(FUN_004261d0(x,y,FUN_0041e190(),FUN_0041f210()))return;
    if(B(0xe38df8))FUN_0041f650();
}
/* Original 004d801c..004d806f foreground branch. */
void restore_mission_menu_buttons(void)
{
    B(0xe38dc9)=1;
    FUN_00425730(I(0xe38da8),I(0xe38da4),FUN_0041e100(),FUN_0041e170());
    if(FUN_0041e140())FUN_00425730(I(0xe38da8),I(0xe38da4),FUN_0041e130(),2);
    FUN_0041f220();
}
