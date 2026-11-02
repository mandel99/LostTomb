#include "minimap.h"
#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define I(a) (*(int32_t *)(uintptr_t)(a))
static int32_t sub(int32_t a,int32_t b){return (int32_t)((uint32_t)a-(uint32_t)b);}
void FUN_0043e670(int32_t width,int32_t height)
{
    int32_t room_x=sub(sub(I(0x70e424),I(0x70e42c)),width);
    int32_t room_y=sub(sub(I(0x70e420),I(0x70e428)),height),chosen,limit;
    I(0x70e3dc)=sub(231,width)/2;
    if((int32_t)((uint32_t)room_x+1u)>0) {
        chosen=(int32_t)((uint32_t)sub(I(0x70e464)/2,width/2)+U(0x70e478));
        I(0x70e3dc)=chosen;limit=I(0x70e42c);
        if(chosen<limit)I(0x70e3dc)=limit;
        else {limit=sub(I(0x70e424),width);if(chosen>limit)I(0x70e3dc)=limit;}
    }
    chosen=sub(457,height)/2;
    if((int32_t)((uint32_t)room_y+1u)>0) {
        chosen=(int32_t)((uint32_t)sub(I(0x70e460)/2,height/2)+U(0x70e474));
        limit=I(0x70e428);
        if(chosen<limit)chosen=limit;
        else {limit=sub(I(0x70e420),height);if(chosen>limit)chosen=limit;}
    }
    U(0x70e3d8)=(uint32_t)chosen&0xfffeu;
}
