#include "footprint.h"
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
static void rotate_point(uint32_t *x,uint32_t *y,uint32_t rotate,uint32_t distance)
{
    uint32_t mode;
    if(rotate==1u){mode=U(0xbca2e0u);if(mode==6u)*y-=distance;else if(mode==4u){*x-=distance;*y-=distance;}else if(mode==2u)*x-=distance;}
}
int32_t FUN_0047cc70(int32_t x_in,int32_t y_in,uint32_t rotate,uint8_t allow)
{
    static const uint32_t border[3]={0x5dbde8,0x5dbe08,0x5dbe28};
    uint32_t x=(uint32_t)x_in,y=(uint32_t)y_in,base,orient=0,state=0,cost,matched,i,j,n,required,flags,p,retain,latched,mode,result;
    rotate_point(&x,&y,rotate,1);
    if((int32_t)x<0||(int32_t)y<0)return 999;
    if((int32_t)(x+1u)>=(int32_t)U(0xbca2d0u))return 999;
    if((int32_t)(y+1u)>=(int32_t)U(0xbca2d4u))return 999;
    U(0xbc96dcu)=0;base=U(0xbca2d8u)+y*228u+x;
    do{
        cost=0;matched=0;
        for(i=0;i<4u;++i){
            n=U(0x5cccc8u+i*4u);required=U(0x5dbda8u+orient*16u+i*4u);n+=base;flags=U(0xaf598cu+n*4u);
            if(required){
                p=U(0xd05720u+n*4u);retain=0;
                if(p&&B(p+24u)==0u&&state!=2u){state=1;if(allow)retain=1;}
                if((flags&4u)&&state!=1u)state=2;else if(!retain)break;
                ++matched;if(flags&64u)++cost;if(flags&1u)++cost;
            }else{
                if(flags&0x10004u){if(!allow)break;p=U(0xd05720u+n*4u);if(!p||B(p+24u))break;}
                ++matched;if((flags&64u)||(flags&0x26341237u)||(flags&8u))++cost;
            }
        }
        latched=U(0xbc96dcu);
        for(i=0;i<2u;++i)for(j=0;j<3u;++j){
            n=U(border[j]+latched*8u+i*4u)+base;flags=U(0xaf598cu+n*4u);
            if(!(flags&(allow?0x10004u:4u))){
                if(!allow)matched=0;
                else{p=U(0xd05720u+n*4u);if(!p||B(p+24u))matched=0;}
            }
        }
        if(matched==4u){
            mode=U(0xbca2e0u);result=U(0xbc96dcu);U(0xbc96d8u)=latched;
            result-=(uint32_t)((int32_t)mode/2);U(0xbc96dcu)=result;
            if((int32_t)result<0)U(0xbc96dcu)=result+4u;
            return (int32_t)cost;
        }
        orient=latched+1u;U(0xbc96dcu)=orient;
    }while((int32_t)orient<4);
    return 999;
}
int32_t FUN_0047d060(int32_t x_in,int32_t y_in,uint32_t rotate)
{
    uint32_t x=(uint32_t)x_in,y=(uint32_t)y_in,base,orient=0,matched,cost,i,n,required,flags,p,delta,mode,result;
    rotate_point(&x,&y,rotate,2);
    if((int32_t)x<0||(int32_t)y<0)return 999;
    if((int32_t)(x+2u)>=(int32_t)U(0xbca2d0u))return 999;
    if((int32_t)(y+2u)>=(int32_t)U(0xbca2d4u))return 999;
    base=U(0xbca2d8u)+y*228u+x;U(0xbc96dcu)=0;
    do{
        matched=0;cost=0;
        for(i=0;i<9u;++i){
            required=U(0x5dbe48u+orient*36u+i*4u);delta=U(0x5cccecu+i*4u);n=base+delta;flags=U(0xaf598cu+n*4u);
            if(required){if(!(flags&4u))break;++matched;if(flags&64u)++cost;if(flags&1u)++cost;}
            else{
                if(flags&0x10004u)break;p=U(0xd05720u+n*4u);if(p&&B(p+24u)==0u)break;
                ++matched;if((flags&64u)||(flags&0x26341237u)||(flags&8u))++cost;
            }
        }
        for(i=0;i<2u;++i){n=U(0x5dbed8u+orient*8u+i*4u)+base;if(!(B(0xaf598cu+n*4u)&4u))matched=0;}
        if(matched==9u){mode=U(0xbca2e0u);U(0xbc96d8u)=orient;result=orient-(uint32_t)((int32_t)mode/2);U(0xbc96dcu)=result;if((int32_t)result<0)U(0xbc96dcu)=result+4u;return (int32_t)cost;}
        ++orient;U(0xbc96dcu)=orient;
    }while((int32_t)orient<4);
    return 999;
}
void FUN_0047d360(int32_t size_in,int32_t x_in,int32_t y_in,uint32_t mask,const void *placement,uint8_t allow)
{
    uint32_t size=(uint32_t)size_in,x=(uint32_t)x_in,y=(uint32_t)y_in,base,i,j,n,flags,seen_empty=0,seen_existing=0;
    U(0xbc9738u)=size;U(0xbc973cu)=size;
    if(!placement)rotate_point(&x,&y,1,size-1u);
    if((int32_t)x<3||(int32_t)y<3)return;
    if((int32_t)(size+x-1u)>=(int32_t)(U(0xbca2d0u)-3u))return;
    if((int32_t)(size+y-1u)>=(int32_t)(U(0xbca2d4u)-3u))return;
    base=U(0xbca2d8u)+y*228u+x;U(0xbc9768u)=base;
    if(size_in>0)for(i=0;i<size;++i)for(j=0;j<size;++j){
        n=U(0x5cc8c0u+i*48u+j*8u)+base;flags=U(0xaf598cu+n*4u);
        if(flags&0x10000u){if(!allow||seen_empty)return;seen_existing=1;}
        else{if(seen_existing)return;seen_empty=1;}
        if(flags&mask&0xeefed77fu)return;
        if(W(0xb350dcu+n*2u))return;
    }
    FUN_0047d4d0();
}
void FUN_0047d4d0(void)
{
    uint32_t rows=U(0xbc9738u),columns,base,i,j,n;
    U(0x605994u)=1;if((int32_t)rows<=0)return;
    columns=U(0xbc973cu);base=U(0xbc9768u);
    for(i=0;i<rows;++i)if((int32_t)columns>0)for(j=0;j<columns;++j){n=base+U(0x5cc8c0u+i*48u+j*8u);B(0xb285ccu+n)|=16u;}
}
