#include "editor_brush_validation.h"
/* The native return retains EAX's high24 bits; consumers test AL. */
uint32_t FUN_004ecec0(int32_t center,int32_t tool,uint32_t *failure_flags)
{
    uint32_t samples=EBV_U(0x5cfcdcu),half=(uint32_t)((int32_t)EBV_U(0xbca2e0u)/2);
    uint32_t row=0,xy=0x56fd10u,offset=0x56fe30u,state,eax,cell,n,delta,count;
    uint8_t valid=1;
    EBV_W((uintptr_t)failure_flags,0);
    state=EBV_U(0x5e4ef4u);
    for(;;){
        count=EBV_U((state==5?0x56f7ecu:0x56f7d8u)+(uint32_t)tool*4u);
        if((int32_t)row>=(int32_t)count)break;
        if(state==5){cell=EBV_U(xy+4u)*228u;cell+=EBV_U(xy);}else cell=EBV_U(offset);
        cell+=(uint32_t)center;
        for(n=0;(int32_t)n<(int32_t)samples;++n){
            eax=FUN_0046f9a0((int32_t)cell%228,(int32_t)cell/228,3);
            if((uint8_t)eax)return eax&0xffffff00u;
            delta=EBV_U(0x5cfe10u+(n+half*36u)*4u)+cell;
            if(EBV_U(0xaf598cu+delta*4u)&0xfeffd76eu){valid=0;EBV_W((uintptr_t)failure_flags,EBV_U((uintptr_t)failure_flags)|1u);}
        }
        if((int32_t)samples>0)state=EBV_U(0x5e4ef4u);
        ++row;xy+=8u;offset+=4u;
    }
    if(FUN_004ecea0(tool)){
        delta=EBV_U(0x5f5d08u+(uint32_t)tool*4u)+(uint32_t)center-EBV_U(0xbca2d8u);
        FUN_004ed360((int32_t)delta%228,(int32_t)delta/228,0,0);
        if(!EBV_U(0xeaae9cu)){valid=0;EBV_W((uintptr_t)failure_flags,EBV_U((uintptr_t)failure_flags)|2u);}
    }
    state=EBV_U(0x5e4ef4u);
    count=EBV_U((state==5?0x5700a8u:0x570094u)+(uint32_t)tool*4u);
    eax=EBV_U((state==5?0x5700c8u:0x5700b4u)+(uint32_t)tool*4u);
    if((int32_t)count>0){
        eax=0xaf598cu+(eax+(uint32_t)center)*4u;
        do{
            if(EBV_U(eax)&0xfeffd72eu){valid=0;EBV_W((uintptr_t)failure_flags,EBV_U((uintptr_t)failure_flags)|4u);}
            eax+=4u;
        }while(--count);
    }
    return (eax&0xffffff00u)|valid;
}
