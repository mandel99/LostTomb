#include "neighbors.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
static const uint32_t neighbor[8]={0xaf55fc,0xaf5600,0xaf5990,0xaf5d20,0xaf5d1c,0xaf5d18,0xaf5988,0xaf55f8};
uint32_t FUN_00477690(uint32_t mask,uint32_t cell)
{
    uint32_t i,count=0;
    for(i=0;i<8u;++i)if(U(neighbor[i]+cell*4u)&mask)++count;
    return count;
}
uint32_t FUN_00477eb0(uint32_t cell,uint32_t mask,uint8_t diagonal)
{
    uint32_t nw,se,ne,sw,w,n,e,s,v;
    if(diagonal){
        nw=U(0xaf55f8u+cell*4u);se=U(0xaf5d20u+cell*4u);
        if(nw&se&mask)return cell&0xffffff00u;
        ne=U(0xaf5600u+cell*4u);sw=U(0xaf5d18u+cell*4u);
        if(ne&sw&mask)return cell&0xffffff00u;
    }
    w=U(0xaf5988u+cell*4u);nw=U(0xaf55f8u+cell*4u);n=U(0xaf55fcu+cell*4u);
    if(!((nw|w|n)&mask))return (cell&0xffffff00u)|1u;
    e=U(0xaf5990u+cell*4u);ne=U(0xaf5600u+cell*4u);
    if(!((ne|e|n)&mask))return (cell&0xffffff00u)|1u;
    s=U(0xaf5d1cu+cell*4u);sw=U(0xaf5d18u+cell*4u);
    if(!((sw|s|w)&mask))return (cell&0xffffff00u)|1u;
    se=U(0xaf5d20u+cell*4u);v=se|s|e;
    return (v&0xffffff00u)|((v&mask)?0u:1u);
}
int32_t FUN_004781a0(uint32_t cell,uint32_t mask)
{
    static const uint32_t delta[8]={0xffffff1c,1,228,0xffffffff,0xffffff1d,229,227,0xffffff1b};
    uint32_t i,count,flags,score,best=0,result=0,n;
    if((uint8_t)FUN_00477eb0(cell,mask,1))return 0;
    for(i=0;i<8u;++i){
        n=cell+delta[i];count=FUN_00477690(mask,n);flags=U(0xaf598cu+n*4u);
        if(flags&mask){score=8u-count;if((int32_t)score>(int32_t)best){best=score;result=delta[i];}}
    }
    return (int32_t)result;
}
uint32_t FUN_0047cae0(int32_t x_in,int32_t y_in,int32_t width_in,int32_t height_in,uint32_t flags,uint32_t image)
{
    uint32_t x=(uint32_t)x_in,y=(uint32_t)y_in,width=(uint32_t)width_in,height=(uint32_t)height_in,base,i,j,n;
    if(x_in<0||y_in<0)return 0;
    if((int32_t)(x+width-1u)>=(int32_t)U(0xbca2d0u))return 0;
    if((int32_t)(y+height-1u)>=(int32_t)U(0xbca2d4u))return 0;
    base=U(0xbca2d8u)+y*228u+x;U(0xbc9768u)=base;
    if(height_in>0)for(i=0;i<height;++i)if(width_in>0)for(j=0;j<width;++j){
        n=base+U(0x5cc8c0u+i*48u+j*8u);
        if((U(0xaf598cu+n*4u)&0xeefed77fu)!=flags)return 0;
        if(image&&U(0xb7482cu+n*4u)!=image-1u)return 0;
    }
    return 1;
}
