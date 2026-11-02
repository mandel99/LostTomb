#include "refresh_wrappers.h"
#include "region_validation.h"
#include "road_place.h"
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
extern uint32_t FUN_0047cae0(int32_t,int32_t,int32_t,int32_t,uint32_t,uint32_t);
void FUN_00477210(void)
{
    uint32_t height=U(0xbca2d4u),row=0,left,right,cell,count,flags,access,sprite,col,size,variation,base;
    if((int32_t)height>0)do{
        left=U(0xbc9f24u+row*4u);right=U(0xbc9b94u+row*4u);cell=U(0xbc9804u+row*4u);
        if((int32_t)left<(int32_t)right){
            count=right-left;
            do{
                flags=U(0xaf598cu+cell*4u);
                if(flags&0x2000000u){
                    access=B(0xb285ccu+cell);
                    if(!(access&128u)&&!(flags&0x600u)){
                        U(0xb7482cu+cell*4u)=0;B(0xb285ccu+cell)=(uint8_t)(access&0xf0u);
                        sprite=B(0xb67d1cu+cell);B(0xb67d1cu+cell)=(uint8_t)(sprite|64u);
                    }
                }
                ++cell;--count;
            }while(count);
        }
        height=U(0xbca2d4u);++row;
    }while((int32_t)row<(int32_t)height);
    height=U(0xbca2d4u);row=0;
    if((int32_t)height<=0)return;
    do{
        left=U(0xbc9f24u+row*4u);right=U(0xbc9b94u+row*4u);cell=U(0xbc9804u+row*4u);
        if((int32_t)left<(int32_t)right){
            col=left;
            do{
                flags=U(0xaf598cu+cell*4u);
                if((flags&0x2000000u)&&!(B(0xb285ccu+cell)&128u)&&!(flags&0x600u)&&!U(0xb7482cu+cell*4u)){
                    if(FUN_0047cae0((int32_t)col,(int32_t)row,3,3,0x2000000u,1))size=3;
                    else if(FUN_0047cae0((int32_t)col,(int32_t)row,2,2,0x2000000u,1))size=2;
                    else size=1;
                    /* Both original query-result branches perform the same reads
                       and stores. Keep the genuine query and its state effects. */
                    FUN_00482650((int32_t)col,(int32_t)row,(int32_t)size,4,0x200u);
                    variation=B(0xacf85cu+cell);base=U(0xf56ba4u);
                    if(size==3)FUN_0047bf50(0,(int32_t)col,(int32_t)row,3,3,base+(variation&1u)+12u,0x2000000u);
                    else if(size==2)FUN_0047bf50(0,(int32_t)col,(int32_t)row,2,2,base+(variation&3u)+8u,0x2000000u);
                    else U(0xb7482cu+cell*4u)=base+(variation&7u);
                }
                ++col;++cell;
            }while((int32_t)col<(int32_t)right);
        }
        height=U(0xbca2d4u);++row;
    }while((int32_t)row<(int32_t)height);
}
