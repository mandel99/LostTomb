#include "minimap.h"
#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define I(a) (*(int32_t *)(uintptr_t)(a))
#define H(a) (*(int16_t *)(uintptr_t)(a))
#define W(a) (*(uint16_t *)(uintptr_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(a))
extern int32_t FUN_004339e0(int32_t),FUN_00433a60(int32_t),FUN_00433ae0(int32_t,int32_t,int32_t);
extern int32_t FUN_00433c80(int32_t),FUN_00433c90(int32_t),FUN_00433d30(int32_t),FUN_00433d50(int32_t);
extern int32_t FUN_00433e00(int32_t),FUN_00433e40(int32_t),FUN_00433e80(int32_t),FUN_00433ed0(int32_t);
extern int32_t FUN_00434060(int32_t),FUN_004340e0(int32_t),FUN_00434150(int32_t),FUN_004341d0(int32_t);
extern int32_t FUN_00434250(int32_t),FUN_004342c0(int32_t),FUN_004343a0(int32_t),FUN_00434400(int32_t);
extern int32_t FUN_00434510(int32_t),FUN_00434570(int32_t),FUN_004345d0(int32_t),FUN_00434630(int32_t);
extern int32_t FUN_00434690(int32_t),FUN_004346f0(int32_t),FUN_00434750(int32_t),FUN_004347b0(int32_t);
extern uint8_t FUN_00434820(int32_t);
extern int32_t FUN_00434a10(int32_t),FUN_00434a60(int32_t),FUN_0047d350(int32_t);
static int32_t add(int32_t a,int32_t b){return (int32_t)((uint32_t)a+(uint32_t)b);}
int32_t FUN_00439980(int32_t building,int32_t x,int32_t y)
{
    uint32_t tile=U(0x70e488),a=0x7999d8u+(uint32_t)building*264u;
    int32_t image,kind;uint32_t footprint;
    if(!(B(0xb67d1cu+tile)&0x40u)||!(U(0xaf598cu+tile*4u)&8u))return 0;
    switch(I(0xe94e0c)) {
    case 4:image=FUN_00434400(building);break;
    case 8:image=FUN_00433c90(building);break;
    case 9:image=U(0xaf598cu+tile*4u)&0x4140u?FUN_00433d30((int32_t)tile):FUN_00433d50(building);break;
    case 10:image=FUN_00433e80(building);break;
    case 11:image=FUN_00433ed0(building);break;
    case 12:image=FUN_004340e0(building);break;
    case 13:image=FUN_00434150(building);break;
    case 14:image=FUN_004341d0(building);break;
    case 15:case 16:image=FUN_00434250(building);break;
    case 17:image=FUN_00434060(building);break;
    case 18:image=FUN_00434510(building);break;
    case 19:image=FUN_00434570(building);break;
    case 20:image=FUN_0047d350(building);break;
    case 21:image=FUN_004345d0(building);break;
    case 22:image=FUN_00434630(building);break;
    case 23:image=FUN_00434690(building);break;
    case 24:image=FUN_004346f0(building);break;
    case 25:image=FUN_004347b0(building);break;
    case 26:image=FUN_004342c0(building);break;
    case 27:image=FUN_00433a60((int32_t)tile);break;
    case 28:image=FUN_004339e0((int32_t)tile);break;
    case 29:image=FUN_00434a10(building);break;
    case 30:image=FUN_00433c80(building);break;
    case 31:image=FUN_00434820(building);break;
    case 33:image=FUN_00433ae0(building,1,0);break;
    case 34:image=FUN_00433ae0(building,3,2);break;
    case 35:image=FUN_00433ae0(building,6,5);break;
    case 36:image=FUN_00433ae0(building,2,1);break;
    case 37:image=FUN_00433ae0(building,4,3);break;
    case 38:image=FUN_00433ae0(building,5,4);break;
    case 39:image=FUN_00433ae0(building,13,9);break;
    case 40:image=FUN_00433ae0(building,19,11);break;
    case 41:image=FUN_00433ae0(building,17,10);break;
    case 42:image=FUN_00433ae0(building,15,8);break;
    case 43:image=FUN_00433e00(building);break;
    case 45:image=FUN_004343a0(building);break;
    case 48:image=FUN_00433e40(building);break;
    case 49:image=FUN_00434a60(building);break;
    case 50:image=FUN_00434750(building);break;
    default:
        if(B(a+5))image=I(0xf568bc);
        else {
            kind=H(a+16);
            if(kind==90)image=add(I(0xf568d4),-1);
            else {
                kind=FUN_0043e770(kind);if(kind<0)return 0;
                image=add(I(0xf56b14),(int32_t)((uint32_t)kind*5u));
            }
        }
        W(0x70e120)=H(a+74)?0xf863u:0;
    }
    if(I(0xe94e0c)&&I(0xe94e0c)!=51) {
        if(image>10)image=10;
        image=add(I(0xf568c4),(int32_t)((uint32_t)image*5u));
    }
    kind=H(a+16);
    if((kind==31||kind==30||kind==33||kind==209)&&(B(0xaf598cu+U(0x70e488)*4u)&0x40u))image=I(0xf568b4);
    footprint=B(0xb285ccu+U(0x70e488))&15u;
    if(!footprint)FUN_0053f4a0(image,x,y);
    else if(footprint<=4)FUN_0053e590(add(image,(int32_t)footprint),x,add(y,-(int32_t)footprint));
    else if(footprint==5) {
        image=add(image,2);
        FUN_0053e590(image,add(x,3),add(y,-5));
        FUN_0053e590(image,add(x,3),add(y,1));
        FUN_0053e590(image,x,add(y,-2));
        FUN_0053e590(image,add(x,6),add(y,-2));
    }
    W(0x70e120)=0;return 1;
}
