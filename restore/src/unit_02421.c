#include "selection_popup.h"
extern void FUN_00536820(int x,int y,int width,int height);
extern void FUN_004d63d0(int kind,int x,int y,int width,int height,int selected,int screen_x,int screen_y);
extern void FUN_004d6250(int group,int entry,int x,int y,int width,uint32_t font,int flags,int screen_x,int screen_y);
extern void FUN_004d6380(int group,int entry,RestoreMenuButton *button,uint32_t font,uint32_t color,int x,int y,int screen_x,int screen_y);
static int add(int a,int b){return (int)((uint32_t)a+(uint32_t)b);}
static int sub(int a,int b){return (int)((uint32_t)a-(uint32_t)b);}
static int mul(int a,uint32_t b){return (int)((uint32_t)a*b);}
static int entry_for(uint32_t index,uint32_t building)
{
    uint32_t value=SP_U(0x6057f4u+index*4u);
    if(SP_U(0x605a3c)==1&&SP_U(0x605944)==0x17&&value==0x13)
        return SP_I(0x5d2a3cu+(uint32_t)SP_B(0x77f761u+building*98u)*8u);
    return (int)(value+SP_U(0x605940));
}
static uint32_t color_for(int selected,int edge)
{
    return SP_I(0x7056c8)==selected&&!(SP_B(0x6057f0)&&edge)?0x2bfu:0;
}
void FUN_00530d70(int x,int y,int rows)
{
    int count=SP_I(0x605948),height,width,i,local_x,local_y;
    uint32_t building=SP_U(0x77591c);
    if(count>rows){height=add(mul(rows,20),24)/16;width=mul(SP_I(0x6057ec),13);}
    else{height=add(mul(count,21),24)/16;width=13;}
    FUN_00536820(x,y,width,height);
    if(SP_U(0x605a58)){
        RestoreMenuButton *button=(RestoreMenuButton *)0x5c5e78u;
        int bx,by,hit;
        count=SP_I(0x605948);
        by=add(add(y,mul(count>rows?rows:count,20)),-25);
        button->top=(int16_t)by;button->on_left=0x409300;
        button->bottom=(int16_t)add(by,30);
        bx=add(add(mul(SP_I(0x6057ec),208),x),-218);
        button->left=(int16_t)bx;button->right=(int16_t)add(bx,200);
        if(!SP_U(0x605a54))button->on_left=0x4092b0;
        hit=FUN_00426450(0,0,button,1);
        hit=FUN_004d6740(0,0,0x5c5e78,1,hit);
        if(FUN_004261d0(0,0,button,1))return;
        FUN_004d63d0(1,bx,by,200,30,hit==1,SP_I(0xe38da8),SP_I(0xe38da4));
        FUN_004d6250(0x130,SP_I(0x605a54),bx,add(by,8),200,SP_U(0xf56f8c),0,SP_I(0xe38da8),SP_I(0xe38da4));
    }
    local_y=sub(y,SP_I(0xe38da4));local_x=sub(x,SP_I(0xe38da8));
    count=SP_I(0x605948);
    if(count>rows){
        for(i=0;i<rows;++i){
            uint32_t color=color_for(i+1,SP_I(0x7056c8)==1);
            int entry=entry_for((uint32_t)i,building);
            FUN_004d6380(SP_I(0x605944),entry,(RestoreMenuButton *)(uintptr_t)(0x704dc8u+(uint32_t)i*28u),
                SP_U(0xf56f8c),color,local_x,local_y,SP_I(0xe38da8),SP_I(0xe38da4));
        }
        i=0;
        while(i<sub(SP_I(0x605948),rows)){
            uint32_t index=(uint32_t)rows+(uint32_t)i;
            uint32_t color=color_for((int)(index+1u),SP_I(0x7056c8)==add(rows,1));
            int entry=entry_for(index,building);
            FUN_004d6380(SP_I(0x605944),entry,(RestoreMenuButton *)(uintptr_t)(0x704dc8u+index*28u),
                SP_U(0xf56f8c),color,local_x,local_y,SP_I(0xe38da8),SP_I(0xe38da4));
            ++i;
        }
    }else{
        for(i=0;i<SP_I(0x605948);++i){
            uint32_t color=color_for(i+1,SP_I(0x7056c8)==1||SP_I(0x7056c8)==rows);
            int entry=(int)(SP_U(0x6057f4u+(uint32_t)i*4u)+SP_U(0x605940));
            FUN_004d6380(SP_I(0x605944),entry,(RestoreMenuButton *)(uintptr_t)(0x704dc8u+(uint32_t)i*28u),
                SP_U(0xf56f8c),color,local_x,local_y,SP_I(0xe38da8),SP_I(0xe38da4));
        }
    }
}
