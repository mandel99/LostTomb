#include "actor_callbacks_small.h"
static void next_frame(uint32_t a)
{
    uint8_t next=(uint8_t)(AS_B(a-7u)+1u);AS_B(a-7u)=next;if(next>=12)AS_B(a-7u)=0;
}
static int32_t orientation(uint32_t a)
{
    int32_t result=(int32_t)((uint32_t)(int32_t)(int8_t)AS_B(a+9u)-AS_U(0x00bca2e0u));
    AS_U(0x00e94e00u)=(uint32_t)result;
    if(result<0){result=(int32_t)((uint32_t)result+8u);AS_U(0x00e94e00u)=(uint32_t)result;}
    return result;
}
static void fixed_images(uint32_t a,int32_t direction,uint32_t walking,uint32_t dying)
{
    if(AS_B(a+0x30u)==4){int32_t frame=AS_S(a+0x2eu)/2;AS_U(a-4u)=AS_U(dying)+AS_B(0x005d048cu+(uint32_t)frame);}
    else AS_U(a-4u)=AS_U(walking)+(uint32_t)AS_B(a-7u)*8u+(uint32_t)direction;
}
/* Original0049b390, actor type65. */
void FUN_0049b390(void)
{
    uint32_t a=as_current();AS_B(a+0x6au)=0;AS_B(a+0x5du)=1;AS_B(a+4u)=1;
}
/* Original00496f20, actor type40. */
void FUN_00496f20(void)
{
    uint32_t a=as_current(),building;int32_t direction;
    AS_B(a+0x6au)=1;AS_B(a+4u)=0;AS_W(a+0x3au)=128;
    building=as_building(AS_S(a+0x52u));if(AS_B(building)!=1)AS_B(a+6u)=2;
    next_frame(a);restore_actor_small_state(1);a=as_current();direction=orientation(a);
    fixed_images(a,direction,0x00f56324u,0x00f5632cu);
}
/* Original00493ec0, actor type26. */
void FUN_00493ec0(void)
{
    uint32_t id=AS_U(0x00d056f8u),a=as_actor(id),b;int32_t home,first,total,direction;
    AS_B(a+0x6au)=1;AS_B(a+4u)=0;AS_W(a+0x3au)=384;
    home=AS_S(a+0x52u);b=as_building(home);
    if(AS_B(b)!=1)AS_B(a+6u)=2;
    if((uint32_t)(int32_t)AS_S(b+0x2au)!=id&&(uint32_t)(int32_t)AS_S(b+0xb4u)!=id)AS_B(a+6u)=2;
    next_frame(a);
    if(AS_B(a+0x30u)==1){
        first=FUN_004578b0(home);total=(int32_t)((uint32_t)first+(uint32_t)FUN_004579d0(home));a=as_current();
        if(AS_S(a+0x3cu)>=96&&total<=0)AS_W(a+0x3cu)=AS_W(a+0x3au);
    }
    restore_actor_small_state(1);a=as_current();direction=orientation(a);
    fixed_images(a,direction,0x00f55d7cu,0x00f55d84u);
}
/* Original00494010, actor type27. */
void FUN_00494010(void)
{
    static const uint32_t groups[5][2]={{0xec,0xed},{0xf9,0xfa},{0xe2,0xe3},{0xe8,0xe9},{0xf7,0xf8}};
    uint32_t id=AS_U(0x00d056f8u),a=as_actor(id),b,kind,walking,dying;int32_t home;
    AS_B(a+0x6au)=1;AS_B(a+4u)=0;AS_W(a+0x3au)=384;
    home=AS_S(a+0x52u);b=as_building(home);
    if(AS_B(b)!=1||AS_W(b+0x4au)!=0||(uint32_t)(int32_t)AS_S(b+0x2au)!=id){AS_B(a+6u)=2;return;}
    next_frame(a);
    if(AS_B(a+0x30u)==8)AS_B(a+0x30u)=1;else restore_actor_small_state(1);
    a=as_current();orientation(a);kind=FUN_004c5120(home);
    if(kind>4)kind=0;walking=groups[kind][0];dying=groups[kind][1];a=as_current();
    if(AS_B(a+0x30u)==4){int32_t frame=AS_S(a+0x2eu)/2;AS_U(a-4u)=AS_U(0x00f55bb4u+dying*8u)+AS_B(0x005d048cu+(uint32_t)frame);}
    else AS_U(a-4u)=AS_U(0x00f55bb4u+walking*8u)+(uint32_t)AS_B(a-7u)*8u+AS_U(0x00e94e00u);
}
