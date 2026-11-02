#include "startup_geometry.h"
void FUN_004dee10(void)
{
    int index;int record[13];uint32_t *out,a=*(uint32_t *)0x70e44cu,b=*(uint32_t *)0x70e45cu+1u,x,y;
    for(index=0;index<116;++index) {
        out=(uint32_t *)(uintptr_t)(0x5e2aecu+(uint32_t)index*52u);
        FUN_004ded50(record,index,0,0);x=(uint32_t)record[0];y=(uint32_t)record[1];
        out[-1]=x+y*228u;out[0]=(x-y)*b;out[4]=(x+y)*a;
        FUN_004ded50(record,index,2,2);x=(uint32_t)record[0];y=(uint32_t)record[1];out[1]=(x+y)*b;out[5]=(y-x)*a;
        FUN_004ded50(record,index,4,4);x=(uint32_t)record[0];y=(uint32_t)record[1];out[2]=(y-x)*b;out[6]=0u-(x+y)*a;
        FUN_004ded50(record,index,6,6);x=(uint32_t)record[0];y=(uint32_t)record[1];out[3]=0u-(x+y)*b;out[7]=(x-y)*a;
    }
}
