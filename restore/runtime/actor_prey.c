#include "actor_update.h"
#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define I(a) (*(int32_t *)(uintptr_t)(a))
#define H(a) (*(int16_t *)(uintptr_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(a))
extern void FUN_004a5da0(int32_t,uint32_t);
extern void FUN_004b17d0(int32_t);
extern void FUN_004b1d50(int32_t);
extern void FUN_004b3250(int32_t);
extern void FUN_004b5050(int32_t);
extern int32_t FUN_004eab10(int32_t,int32_t,int32_t,int32_t,int32_t,int32_t);
extern uint32_t FUN_004eb000(int32_t);
static uint32_t actor(uint32_t id) {return 0xbcd600u+id*388u;}
static uint32_t random_tick(void)
{
    uint32_t index=U(0xd3b1c4),a=U(0xd3b384),b=U(0xd3b380),i;
    U(0xd3b1c8+index*4u)=U(0xd3b374);index++;
    U(0xd3b1c4)=(int32_t)index<100?index:0;
    for(i=0;i<31;++i){uint32_t x=((a>>4)^a)&1u,y=((b>>4)^b)&1u;a=(a>>1)|(x<<30);b=(b>>1)|(y<<30);}
    U(0xd3b384)=a;U(0xd3b380)=b;
    U(0xd3b364)=b&0x7fff;U(0xd3b368)=b&0x7f;U(0xd3b36c)=b&7;
    U(0xd3b370)=a&0x7fff;U(0xd3b374)=a&0x7f;U(0xd3b378)=a&7;
    return a&0x7fff;
}
static void random_wait(uint32_t a)
{
    uint32_t choice=random_tick();
    B(a+0x30)=(uint8_t)((~choice&1u)|0x12u);
    H(a+0x2e)=(int16_t)(random_tick()%150u+150u);
}
static void image(uint32_t id)
{
    uint32_t a=actor(id),sprite,frame;int32_t orientation=(int32_t)((uint32_t)(int32_t)(int8_t)B(a+9)-U(0xbca2e0));
    if(orientation<0){orientation=(int32_t)((uint32_t)orientation+8u);if(orientation<0)orientation=(int32_t)((uint32_t)orientation+((7u-(uint32_t)orientation)>>3)*8u);}
    if(orientation>=8)orientation=(int32_t)((uint32_t)orientation-((uint32_t)orientation>>3)*8u);
    U(0xe94e00)=(uint32_t)orientation;
    if(I(a+0x140)>I(a+0x13c))U(a+0x140)-=U(a+0x13c);
    if(I(a+0x140)<0)U(a+0x140)=0;
    frame=U(a+0x140);
    switch(B(a+0x30)){
    case 4:sprite=U(0xf561d4)+7u;break;
    case 5:sprite=U(0xf561dc)+frame*8u+(uint32_t)orientation;break;
    case 8:case 18:sprite=U(0xf561e4)+frame*8u+(uint32_t)orientation;break;
    case 12:sprite=U(0xf561d4)+(H(a+0x2e)<8?(uint32_t)(int32_t)H(a+0x2e):7u);break;
    case 14:case 15:sprite=U(0xf561ec)+(uint32_t)orientation;break;
    case 19:sprite=U(0xf561ec)+frame*8u+(uint32_t)orientation;break;
    default:sprite=U(0xf561cc)+frame*8u+(uint32_t)orientation;break;
    }
    U(a-4)=sprite;
}
void FUN_0049bd40(void)
{
    uint32_t id=U(0xd056f8),a=actor(id),type=B(a+2),speed=U(0x7090ec+type*56u),building=U(a+0x130),b=0x7999d8u+building*264u;
    int32_t group=H(a+0x58);int due,flee=0;
    B(a+0x6a)=5;B(a+4)=0;B(a+0x5d)=0;
    H(0xea5cea+U(0xeaac28)*0x49d8u)=(int16_t)((uint16_t)H(0xea5cea+U(0xeaac28)*0x49d8u)+1u);
    U(a+0x140)++;
    if(H(b+16)!=0x73||!B(b+0xa6)||B(b)!=1||!H(b+0x40))U(a+0x130)=0;
    due=I(a+0x140)>=I(a+0x13c);
    if(due){U(a+0x140)=0;B(a-7)++;}
    switch(B(a+0x30)){
    case 4:U(a+0x13c)=8;FUN_004b1d50((int32_t)id);id=U(0xd056f8);break;
    case 5:U(a+0x13c)=7;FUN_004b17d0((int32_t)id);id=U(0xd056f8);break;
    case 8:U(a+0x140)=0;U(a+0x13c)=8;random_wait(a);return;
    case 9:case 10:
        U(a+0x13c)=12;FUN_004a5da0((int32_t)id,speed);id=U(0xd056f8);a=actor(id);
        if(B(a+9)==9){FUN_004b3250((int32_t)id);id=U(0xd056f8);a=actor(id);B(a+9)=B(a+10);}
        else if(B(a+9)==10){B(a+9)=B(a+10);B(a+0x30)=8;H(a+0x2e)=(int16_t)((uint32_t)B(a+0x5a)*5u);U(a+0x140)=0;return;}
        else if(B(a+9)==8){B(a+9)=B(a+10);random_wait(a);}
        flee=due;break;
    case 12:
        if(!H(a+0x2e)){FUN_004b5050((int32_t)id);id=U(0xd056f8);}
        a=actor(id);H(a+0x2e)=(int16_t)((uint16_t)H(a+0x2e)+1u);
        if(H(a+0x2e)>500)B(a+0x30)=4;
        break;
    case 14:case 15:U(a+0x13c)=1;FUN_004eb000((int32_t)id);id=U(0xd056f8);break;
    case 16:case 17:
        U(a+0x13c)=12;U(a+0x140)++;if(I(a+0x140)>=12)U(a+0x140)=0;
        FUN_004a5da0((int32_t)id,speed*2u);id=U(0xd056f8);a=actor(id);
        if(B(a+9)==9){FUN_004b3250((int32_t)id);id=U(0xd056f8);a=actor(id);B(a+9)=B(a+10);B(a+0x30)=14;}
        else if(B(a+9)==10){B(a+6)=2;return;}
        else if(B(a+9)==8){B(a+9)=B(a+10);B(a+0x30)=14;U(a+0x140)=0;return;}
        break;
    case 18:
        U(a+0x13c)=8;H(a+0x2e)=(int16_t)((uint16_t)H(a+0x2e)-1u);
        if(H(a+0x2e)<0)H(a+0x2e)=0;
        if(H(a+0x2e)>0){flee=due;break;}
        if(due){
            uint32_t formation=0xbcb9c8u+(uint32_t)group*144u;int32_t position;
            uint32_t random=random_tick();
            position=FUN_004eab10(H(formation+0x3a),H(formation+0x3c),(int32_t)(random&15u),H(a+12),H(a+14),1);
            id=U(0xd056f8);a=actor(id);
            if(position>=0&&position<16){
                H(a+0x1c)=(int16_t)((uint16_t)H(0x5cdb78u+(uint32_t)position*8u)+(uint16_t)H(formation+0x3a));
                H(a+0x1e)=(int16_t)((uint16_t)H(0x5cdb7cu+(uint32_t)position*8u)+(uint16_t)H(formation+0x3c));
                B(a+0x30)=10;return;
            }
            H(a+0x2e)=100;flee=1;
        }
        break;
    case 19:
        U(a+0x13c)=8;H(a+0x2e)=(int16_t)((uint16_t)H(a+0x2e)-1u);
        if(H(a+0x2e)<=0){if(due){B(a+0x30)=18;return;}}
        else flee=due;
        break;
    case 24:
        H(a+0x2e)=(int16_t)((uint16_t)H(a+0x2e)-1u);
        if(H(a+0x2e)<=0){U(a+0x140)=0;U(a+0x13c)=8;B(a+0x30)=8;}
        U(a-4)=0;return;
    default:B(a+0x30)=8;break;
    }
    if(flee){FUN_004eb000((int32_t)id);id=U(0xd056f8);}
    image(id);
}
