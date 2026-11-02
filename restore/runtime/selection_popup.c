#include "selection_popup.h"
extern int FUN_00443710(void);
extern uint8_t FUN_00442870(int value);
extern void FUN_005344c0(int message);
extern void FUN_004de430(int player);
extern void FUN_00444b10(void);
extern void FUN_004bb090(void);
static uint32_t mapped(uint32_t index){return SP_U(0x6057f4u+index*4u);}
static void byte_store(uint32_t index){SP_B(SP_U(0x605930))=(uint8_t)mapped(index);}
static void error(int message){SP_B(0xe38dda)=0x47;FUN_005344c0(message);}
void FUN_00403950(void)
{
    uint32_t building=SP_U(0x77591c),index=SP_U(0xd3b198),kind;
    uint8_t old=0;
    if(SP_B(0x6057f0)&&(index==0||index==SP_U(0x605950)))return;
    if(SP_U(0x605a58))SP_U(0x605a58)=0;
    kind=SP_U(0x60592c);
    if(kind==1){
        if(SP_B(0xe38dda)==0x48){
            uint32_t base=building*98u,value=mapped(index);
            uint8_t special=SP_B(0x77f760u+base);
            if((int32_t)value>0&&(int32_t)value<=8){
                uint32_t target=SP_U(0x605930),start=0x77f766u+base,i;
                int count;
                for(i=0;i<(special?8u:14u);++i)if(target==start+i)old=SP_B(start+i);
                SP_B(target)=0;
                count=FUN_00443710();
                if(special){
                    if(count>4){SP_B(SP_U(0x605930))=old;error(0x45);return;}
                    index=SP_U(0xd3b198);
                    if(count==4){
                        if(!FUN_00442870((int)mapped(index))){SP_B(SP_U(0x605930))=old;error(0x45);return;}
                        index=SP_U(0xd3b198);
                    }
                }else{
                    index=SP_U(0xd3b198);value=mapped(index);
                    if(count==4){
                        if(!FUN_00442870((int)value)){SP_B(SP_U(0x605930))=old;error(0x45);return;}
                        index=SP_U(0xd3b198);value=mapped(index);
                    }
                    if(value==2){
                        if(!FUN_00442870(9)){error(0x46);return;}
                        index=SP_U(0xd3b198);
                    }
                }
            }
        }
        byte_store(index);
    }else if(kind==2)SP_W(SP_U(0x605934))=(uint16_t)mapped(index);
    else if(kind==4)SP_U(SP_U(0x605938))=mapped(index)+SP_U(0x605940);
    if(SP_U(0x60593c)==2&&(int32_t)SP_U(0x605a18)<=5){
        FUN_004de430(SP_I(0x605a18));
        if(SP_U(0x605a3c)==1){FUN_00444b10();FUN_004bb090();}
    }
    SP_U(0xe38ddc)=20;SP_B(0xe38dda)=SP_B(0x605958);
    {uint32_t completion=SP_U(0x605a50);SP_B(0xe38dca)=1;
     if(completion){restore_menu_callback(completion);SP_U(0x605a50)=0;}}
}
void FUN_00409540(void)
{
    FUN_00403c00(50,50,7,0x21,0,0,0,0x605a18,2,4,0,0,0,-1,0);
    SP_B(0xe38dca)=1;
}
void FUN_004092b0(void)
{
    uint8_t previous=SP_B(0x605958);
    SP_U(0x605a54)=1;SP_B(0xe38dda)=previous;
    FUN_00403c00(0,0,62,0x97,63,0,0,0x784f08,0,4,0,0,0,22,0);
    SP_B(0xe38dca)=1;
}
void FUN_00409300(void)
{
    uint8_t previous=SP_B(0x605958);
    SP_U(0x605a54)=0;SP_B(0xe38dda)=previous;
    FUN_00403c00(0,0,63,0x97,0,0,0,0x784f08,0,4,0,0,0,22,0);
    SP_B(0xe38dca)=1;
}
int restore_selection_popup_action(uint32_t token)
{
    switch(token){case 0x403950:FUN_00403950();break;
    case 0x409540:FUN_00409540();break;case 0x4092b0:FUN_004092b0();break;
    case 0x409300:FUN_00409300();break;default:return 0;}return 1;
}
void restore_selection_popup_input(void)
{
    if(!SP_B(0xe38df7)){
        RestoreMenuButton *buttons=(RestoreMenuButton *)0x704dc8u;
        int hit=FUN_00426450(SP_I(0x605954),SP_I(0x60594c),buttons,SP_I(0x605948));
        SP_I(0x7056c8)=FUN_004d6740(SP_I(0x605954),SP_I(0x60594c),0x704dc8,SP_U(0x605948),hit);
        FUN_004261d0(SP_I(0x605954),SP_I(0x60594c),buttons,SP_I(0x605948));
    }else{
        SP_U(0x605a58)=0;SP_B(0xe38dda)=SP_B(0x605958);SP_B(0xe38dca)=1;
    }
}
