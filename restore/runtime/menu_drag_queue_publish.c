#include "queue_publish.h"
#include "editor_brush_validation.h"
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
void FUN_004f6130(void)
{
    uint32_t count=U(0xeaaf64u),i=0,index,record,category,delta,origin,base,cell;
    if((int32_t)count<=0){U(0xeaaf64u)=0;return;}
    do{
        index=U(0xeaac30u+i*4u);record=0x7999e8u+index*264u;category=B(record+68u);
        if(FUN_004ecea0((int32_t)category)){
            delta=U(0x5f5d08u+category*4u);origin=U(record-4u);base=U(0xbca2d8u);cell=origin+delta-base;
            FUN_004ed360((int32_t)cell%228,(int32_t)cell/228,4,0);
        }
        count=U(0xeaaf64u);++i;
    }while((int32_t)i<(int32_t)count);
    U(0xeaaf64u)=0;
}
