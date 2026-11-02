#include "editor_brush_validation.h"
uint32_t FUN_00485100(int32_t record)
{
    uint32_t found=0,count=0,address,index;
    if(record<=0)return 0;
    EBV_W(0xbc95f0u,0);
    for(address=0xba746cu;address<0xba7886u;address+=2u){
        int32_t value=EBV_S(address);
        if(value)++count;
        if(value==record)found=1;
    }
    EBV_W(0xbc95f0u,count);
    if(found)return 0;
    for(index=0,address=0xba746cu;address<0xba7886u;address+=2u,++index){
        if(!EBV_S(address)){
            EBV_W(0xbc95f0u,count+1u);
            FUN_0040f860((void *)(uintptr_t)(0x7999d8u+(uint32_t)record*264u),(void *)(uintptr_t)(0xba7888u+index*264u),264);
            EBV_WS(0xba746cu+index*2u,record);
            return 1;
        }
    }
    EBV_W(0xbc9694u,0);
    return 0;
}
