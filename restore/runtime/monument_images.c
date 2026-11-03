#include "monument_images.h"
#include "sprite_libraries.h"
#include <stdlib.h>
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define H(a) ((uint32_t)(int32_t)(int16_t)W(a))
static void load(uint32_t filename,int32_t slot)
{(void)FUN_0053dd90(RESTORE_SPRITE_LIBRARIES,(const char *)(uintptr_t)filename,slot);}
static uint32_t ground(uint32_t output)
{uint32_t image=U(0xf56b54u);U(output)=1;return image;}
static uint32_t variant(uint32_t address,uint32_t quarter)
{return U(address)+(quarter&1u);}

struct late_image_rule {uint32_t near_base,far_base;uint8_t fixed_near,fixed_far,minimum;};
/* LibExtra/LibExtra2: a fixed entry remains ground until minimum stage;
   a variant entry adds the low orientation bit. Earlier categories are
   handled separately, so their zero table rows are never selected. */
static const struct late_image_rule late[25]={
 {0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},
 {0xf573c4,0xf5742c,1,0,21},{0xf573c4,0xf57424,1,0,21},
 {0xf573cc,0xf5740c,1,0,21},{0xf573cc,0xf57404,1,0,21},
 {0xf573dc,0xf5743c,1,0,20},{0xf573dc,0xf57434,1,0,20},
 {0xf573e4,0xf573fc,1,0,21},{0,0,0,0,0},{0,0,0,0,0},
 {0xf573e4,0xf573f4,1,0,21},
 {0xf57414,0xf5741c,0,0,0},{0xf5741c,0xf57414,0,0,0},
 {0xf573f4,0xf573e4,0,1,21},{0xf573fc,0xf573e4,0,1,21},
 {0,0,0,0,0},{0,0,0,0,0},
 {0xf57404,0xf573cc,0,1,21},{0xf5740c,0xf573cc,0,1,21},
 {0xf57424,0xf573c4,0,1,21},{0xf5742c,0xf573c4,0,1,21},
 {0xf57434,0xf573dc,0,1,20},{0xf5743c,0xf573dc,0,1,20}
};
uint32_t restore_monument_image_0040de80(uint32_t self,uint32_t building,uint32_t output)
{
    static const uint32_t main_b[5]={0x576338,0x57632c,0x576320,0x576314,0x576308};
    static const uint32_t main_a[5]={0x5762fc,0x5762f0,0x5762e4,0x5762d8,0x5762c0};
    static const uint32_t main_c[5]={0x5762b4,0x5762a8,0x57629c,0x5762cc,0x576290};
    uint32_t r=building*264u,category,quarter,image,address,fixed,which;
    int32_t stage=(int32_t)U(0x799a30u+r);const struct late_image_rule *rule;
    if(stage>=3)stage=(int32_t)U(0x799a30u+(building-H(0x7999eau+r))*264u);
    category=U(self+0x58u+H(0x7999eau+r)*52u);
    quarter=(uint32_t)((int32_t)U(0xbca2e0u)/2);U(output)=0;
    if(category>24)return 0;
    if(stage<2){
        if(category==3||category==4||category==21||category==22){image=U(0xf573bcu);U(output)=7;return image;}
        U(output)=1;return 0;
    }
    if(category==0){
        if(stage<5)return ground(output);
        which=stage<8?0u:stage<11?1u:stage<14?2u:stage<18?3u:4u;
        load(main_b[which],21);return variant(0xf573acu,quarter);
    }
    if(category==1||category==2){
        if(stage<4)return ground(output);
        if((category==1)==((int32_t)quarter<2)){
            which=stage<7?0u:stage<10?1u:stage<13?2u:stage<17?3u:4u;
            load(main_a[which],20);return variant(0xf573a4u,quarter);
        }
        if(stage<6)return ground(output);
        which=stage<9?0u:stage<12?1u:stage<15?2u:stage<19?3u:4u;
        load(main_c[which],22);return variant(0xf573b4u,quarter);
    }
    if(category==10||category==11||category==17||category==18){
        if(stage<21)return ground(output);
        return U(((category<17)==((int32_t)quarter<2))?0xf573ecu:0xf573dcu);
    }
    if(stage<16)return ground(output);
    load(stage<20?0x576284u:0x576278u,23);
    rule=late+category;
    if((int32_t)quarter<2){address=rule->near_base;fixed=rule->fixed_near;}
    else{address=rule->far_base;fixed=rule->fixed_far;}
    if(!fixed)return variant(address,quarter);
    if(stage<rule->minimum)return ground(output);
    return U(address);
}
uint32_t restore_monument_image_00401800(uint32_t self,uint32_t building,uint32_t output)
{
    static const uint32_t rotated[7][4]={
        {0xf57564,0xf57574,0xf5757c,0xf5756c},{0xf57574,0xf5757c,0xf5756c,0xf57564},
        {0xf5756c,0xf57564,0xf57574,0xf5757c},{0xf5754c,0xf57544,0xf5755c,0xf57554},
        {0xf57554,0xf5754c,0xf57544,0xf5755c},{0xf57544,0xf5755c,0xf57554,0xf5754c},
        {0xf5755c,0xf57554,0xf5754c,0xf57544}
    };
    uint32_t r=building*264u,category=U(self+0x58u+H(0x7999eau+r)*52u);
    uint32_t quarter=(uint32_t)((int32_t)U(0xbca2e0u)/2),address,image,stage;int face=quarter==0||quarter==3;
    U(output)=0;load(0x5760b0u,26); /* AbuSimbel, even for an unsupported category. */
    if(category>11)return 0;
    if(category<7)return quarter<=3?U(rotated[category][quarter]):0;
    if(category==9){
        stage=U(0x799a30u+r);image=variant(0xf5753cu,quarter);
        return image+((int32_t)stage>0?stage*2u:0u);
    }
    if(category==8){
        image=variant(0xf57534u,quarter);
        return image+(face?U(0x799a30u+r)*2u:14u);
    }
    if(face){
        address=category==7?0xf5752cu:0xf5751cu;
        image=variant(address,quarter);return image+U(0x799a30u+r)*2u;
    }
    return U(category==7?0xf57524u:0xf57514u)+((~quarter)&1u);
}
static const uint32_t nine_rotation[36]={
    0,1,2,3,4,5,6,7,8, 2,5,8,1,4,7,0,3,6,
    8,7,6,5,4,3,2,1,0, 6,3,0,7,4,1,8,5,2
};
static uint32_t nine_offset(uint32_t slot,struct restore_menu_input_frame *context)
{
    uint32_t sp,i;
    if(slot<36)return nine_rotation[slot];
    /* Native has no bounds branch: preserve reads of saved registers or
       caller words through the explicit frame instead of returning zero. */
    if(!context)abort();sp=context->r[4];
    U(sp-148u)=context->r[3];U(sp-152u)=context->r[5];
    U(sp-156u)=context->r[6];U(sp-160u)=context->r[7];
    for(i=0;i<36u;++i)U(sp-144u+i*4u)=nine_rotation[i];
    return U(sp-144u+slot*4u);
}
static uint32_t nine_image(uint32_t self,uint32_t building,uint32_t output,struct restore_menu_input_frame *context)
{
    uint32_t r=building*264u,quarter=(uint32_t)((int32_t)U(0xbca2e0u)/2),part=H(0x7999eau+r);
    uint32_t offset=nine_offset((quarter*9u+part)&0x3fffffffu,context),root,stage,filename;
    U(output)=0;root=building-H(0x7999eau+r);stage=U(0x799a30u+root*264u);
    filename=U(self+0x220u+stage*4u);load(filename,18);return U(0xf57394u)+offset;
}
uint32_t restore_monument_image_00417310(uint32_t self,uint32_t building,uint32_t output)
{return nine_image(self,building,output,0);}
uint32_t restore_monument_image_004e6a70(uint32_t self,uint32_t building,uint32_t output)
{
    uint32_t r=building*264u,category=U(self+0x58u+H(0x7999eau+r)*52u),stage;
    if(category==1)return U(0xf55bb4u);
    if(category!=0)return 0; /* These arms never write output. */
    U(output)=0;stage=U(0x799a30u+r);
    load(U(self+0xb4u+stage*4u),19);return U(0xf5739cu);
}
uint32_t restore_monument_image_query(uint32_t target,uint32_t self,uint32_t building,uint32_t output,struct restore_menu_input_frame *context)
{
    switch(target){
    case 0x401800u:return restore_monument_image_00401800(self,building,output);
    case 0x40de80u:return restore_monument_image_0040de80(self,building,output);
    case 0x417310u:return nine_image(self,building,output,context);
    case 0x4e6a70u:return restore_monument_image_004e6a70(self,building,output);
    default:abort();
    }
}
#define ENTRY(token) \
 __declspec(naked) void FUN_##token(void) { \
 __asm { push dword ptr[esp+8] } __asm { push dword ptr[esp+8] } \
 __asm { push ecx } __asm { call restore_monument_image_##token } __asm { add esp,12 } __asm { ret } }
ENTRY(00401800)
ENTRY(0040de80)
ENTRY(00417310)
ENTRY(004e6a70)
