#include "canal_refresh.h"
#include "map_refresh_postpass4.h"
#include "map_refresh_postpass2.h"
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define H(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
extern uint32_t FUN_0047b920(void);
extern int32_t FUN_0051c620(int32_t image);
void FUN_0047be30(int32_t alternate)
{
    uint32_t cell=U(0xbc9768u),image,base,variant,frame,access,sprite,flags,offset;
    volatile uint32_t initial_difference;
    if(H(0xb4e6fcu+cell*2u))return;
    image=U(0xb7482cu+cell*4u);base=U(0xf56b84u);initial_difference=image-base;
    /* 47BE54 JNS, then negative-result CMP48/JL: the initial48 branch
       is unreachable for every DWORD difference. Both original reads occur. */
    (void)initial_difference;offset=alternate?48u:0u;
    FUN_0047e870();FUN_00484be0((void *)(uintptr_t)0x5cc678u,16,-1,0);
    cell=U(0xbc9768u);
    if(B(0xaf598cu+cell*4u)&64u){
        access=B(0xb285ccu+cell);variant=U(0xbc9744u);B(0xb285ccu+cell)=(uint8_t)(access&127u);
        if(!variant){flags=U(0xaf55fcu+cell*4u);U(0xbc9748u)=(flags&0x8000040u)==0;}
        flags=FUN_0047b920();variant=U(0xbc9748u);variant+=flags?42u:15u;cell=U(0xbc9768u);U(0xbc9748u)=variant;
    }
    base=U(0xf56b84u);frame=U(0xbc9740u);variant=U(0xbc9748u);flags=U(0xaf598cu+cell*4u);image=base+frame+variant+offset;
    if(flags&0x10000u){image=(uint32_t)FUN_0051c620((int32_t)image);cell=U(0xbc9768u);}
    access=B(0xb285ccu+cell);U(0xb7482cu+cell*4u)=image;sprite=B(0xb67d1cu+cell);
    B(0xb285ccu+cell)=(uint8_t)(access&0xf0u);variant=B(0xbc9744u);B(0xae8e7cu+cell)=(uint8_t)variant;B(0xb67d1cu+cell)=(uint8_t)(sprite|64u);
}
void FUN_0047bd50(int32_t left_in,int32_t top_in,int32_t right_in,int32_t bottom_in)
{
    uint32_t left=(uint32_t)left_in,top=(uint32_t)top_in,right=(uint32_t)right_in,bottom=(uint32_t)bottom_in,width,height,cell,stride,row,col,flags;
    if(left_in<0)left=0;if(top_in<0)top=0;
    width=U(0xbca2d0u);if((int32_t)right>=(int32_t)width)right=width-1u;
    height=U(0xbca2d4u);if((int32_t)bottom>=(int32_t)height)bottom=height-1u;
    U(0xbc9760u)=top;stride=width-right+left-1u;cell=U(0xbca2d8u)+top*228u+left;U(0xbc9768u)=cell;
    if((int32_t)top>(int32_t)bottom)return;
    do{
        col=left;U(0xbc9764u)=col;
        if((int32_t)col<=(int32_t)right)do{
            flags=U(0xaf598cu+cell*4u);
            if((flags&0x100u)&&!(flags&12u)&&!(B(0xb285ccu+cell)&128u)){
                FUN_0047e870();FUN_0047be30(0);cell=U(0xbc9768u);
            }
            col=U(0xbc9764u)+1u;++cell;U(0xbc9764u)=col;U(0xbc9768u)=cell;
        }while((int32_t)col<=(int32_t)right);
        width=U(0xbca2dcu);row=U(0xbc9760u)+1u;cell+=width+stride;U(0xbc9760u)=row;U(0xbc9768u)=cell;
    }while((int32_t)row<=(int32_t)bottom);
}
