#include "map_refresh_arithmetic.h"
#include <windows.h>
#include <stdio.h>
#include <string.h>
#define CHECK(c) do{if(!(c)){fprintf(stderr,"map FP numeric line%d\n",__LINE__);ExitProcess(1);}}while(0)
int main(int argc,char **argv)
{
    uint32_t operation,lo,hi,exponent,blo,bhi,control,i;uint16_t cw,old_cw,after;uint8_t raw[10];MRFloat a,b,out;uint64_t bits,spill;
    SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOGPFAULTERRORBOX);
    if(argc==2&&!strcmp(argv[1],"--oracle-batch")){
        puts("{\"fields\":7}");fflush(stdout);
        while(scanf("%u %u %u %u %u %u %u",&operation,&lo,&hi,&exponent,&blo,&bhi,&control)==7){
            memset(&a,0,sizeof a);a.significand=(uint64_t)lo|((uint64_t)hi<<32);a.negative=exponent>>15;a.exponent=(int32_t)(exponent&0x7fff)-16383;if(!a.significand)a.exponent=0;
            bits=(uint64_t)blo|((uint64_t)bhi<<32);b=mr_fp_binary64(bits);cw=(uint16_t)control;
            __asm {fnstcw old_cw}
            __asm {fldcw cw}
            switch(operation){case 0:out=mr_fp_add(a,b,cw);break;case 1:out=mr_fp_sub(a,b,cw);break;case 2:out=mr_fp_mul(a,b,cw);break;case 3:out=mr_fp_div(a,b,cw);break;case 4:out=mr_fp_div(b,a,cw);break;default:CHECK(0);return 1;}
            mr_fp_raw80(out,raw);spill=mr_fp_store64(out,cw);
            __asm {fnstcw after}
            __asm {fldcw old_cw}
            CHECK(after==cw);printf("{\"raw80\":\"");for(i=0;i<10;++i)printf("%02x",raw[i]);printf("\",\"spill\":[%u,%u],\"cw\":%u}\n",(uint32_t)spill,(uint32_t)(spill>>32),(uint32_t)after);fflush(stdout);
        }
    }else puts("exact map FP arithmetic component ready");
    return 0;
}
