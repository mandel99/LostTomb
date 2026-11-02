#include "city_prepare.h"
#include <stdlib.h>
#include <string.h>
#define I(a) (*(int32_t *)(uintptr_t)(a))
#define U(a) (*(uint32_t *)(uintptr_t)(a))
extern void FUN_004746d0(int,int,int);
extern int FUN_004775f0(uint32_t,uint32_t,int);
extern char FUN_00477690(uint32_t,int);
extern void FUN_004dea50(void);
extern void FUN_0040f8f0(void *,int,int);
void FUN_004be160(void *world,int32_t x0,int32_t y0,int32_t x1,int32_t y1)
{
    static const int32_t offsets[8]={-228,-227,1,229,228,227,-1,-229};
    uint8_t visited[51984];
    int32_t left,right,top,bottom,x,y,index,pass=30;
    int changed;
    FUN_004be0f0(world);FUN_0040f8f0(visited,sizeof(visited),0);
    left=x0<x1?x0:x1;right=x0<x1?x1:x0;
    top=y0<y1?y0:y1;bottom=y0<y1?y1:y0;
    left=(int32_t)((uint32_t)left-1u);right=(int32_t)((uint32_t)right+1u);
    top=(int32_t)((uint32_t)top-1u);bottom=(int32_t)((uint32_t)bottom+1u);
    if(left<0)left=0;if(right>=I(0xbca2d0))right=I(0xbca2d0)-1;
    if(top<0)top=0;if(bottom>=I(0xbca2d4))bottom=I(0xbca2d4)-1;
    do {
        changed=0;
        for(y=top;y<=bottom;++y)for(x=left;x<=right;++x) {
            uint32_t flags;int neighbors,adjacent,group=30,k;
            uint8_t water;
            index=x+y*228+I(0xbca2d8);
            if(index<0||index>=51984)abort();
            if(visited[index])continue;
            flags=U(0xaf598c+(uint32_t)index*4u);water=(uint8_t)((flags>>16)&1);
            neighbors=FUN_00477690(0x10000,index);
            if(!water&&!neighbors)continue;
            adjacent=FUN_004775f0(4,0x90000,index);
            if(adjacent+neighbors==8&&(U(0xaf598c+(uint32_t)index*4u)&0x10004)==4)continue;
            if(!adjacent) {
                group=0;
                for(k=0;k<8;++k) {
                    int32_t neighbor=index+offsets[k],label;
                    if(!(U(0xaf598c+(uint32_t)neighbor*4u)&0x10000))continue;
                    if(neighbor<0||neighbor>=51984)abort();
                    label=visited[neighbor];
                    if(label>group&&(label==1||label!=pass)) {
                        group=label-1;if(group<1)group=1;
                    }
                }
                if(!group)continue;
            }
            U(0xaf598c+(uint32_t)index*4u)=(U(0xaf598c+(uint32_t)index*4u)&0xfdfbf7ffu)|0x80u;
            FUN_004746d0(x,y,1);visited[index]=(uint8_t)group;
            U(0xd05720+(uint32_t)index*4u)=(uint32_t)(uintptr_t)FUN_004bd740(
                (RestoreWorldRegion *)(uintptr_t)(0xd38384u+(uint32_t)(group-1)*28u),
                index,x,y,group,water);
            changed=1;((uint8_t *)world)[0x32fbc]=1;
        }
        --pass;
    } while(changed);
    U((uintptr_t)world+0x32fb8)=0;
    for(index=29;index>=0;--index) {
        if(!U((uintptr_t)world+0x32c64u+(uint32_t)index*28u))break;
        ++U((uintptr_t)world+0x32fb8);
    }
    FUN_004dea50();
}
