#include "map_x87_conversion.h"
#include <windows.h>
#include <stdio.h>
#include <string.h>
#define CHECK(c) do{if(!(c)){fprintf(stderr,"raw80 fixture line%d\n",__LINE__);ExitProcess(1);}}while(0)
static uint8_t guarded[42];
static uint32_t abi[6];
#pragma warning(push)
#pragma warning(disable:4100)
__declspec(naked) static uint64_t invoke(const uint8_t *raw)
{
    __asm {
        push ebp
        push ebx
        push esi
        push edi
        mov dword ptr [abi+20],esp
        mov ebx,011111111h
        mov ebp,022222222h
        mov esi,033333333h
        mov edi,044444444h
        push dword ptr [esp+20]
        mov ecx,0badc0deh
        call restore_map_trunc80
        add esp,4
        mov dword ptr [abi],ebx
        mov dword ptr [abi+4],ebp
        mov dword ptr [abi+8],esi
        mov dword ptr [abi+12],edi
        mov dword ptr [abi+16],esp
        pop edi
        pop esi
        pop ebx
        pop ebp
        ret
    }
}
#pragma warning(pop)
int main(int argc,char **argv)
{
    uint32_t low,high,exponent,control,i;uint64_t result;uint16_t old_cw,wanted,before,after;
    SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOGPFAULTERRORBOX);
    if(argc==2&&!strcmp(argv[1],"--oracle-batch")){
        puts("{\"fields\":4}");fflush(stdout);
        while(scanf("%u %u %u %u",&low,&high,&exponent,&control)==4){
            memset(guarded,0xa5,sizeof guarded);memcpy(guarded+16,&low,4);memcpy(guarded+20,&high,4);guarded[24]=(uint8_t)exponent;guarded[25]=(uint8_t)(exponent>>8);
            wanted=(uint16_t)control;
            __asm {fnstcw old_cw}
            __asm {fldcw wanted}
            __asm {fnstcw before}
            result=invoke(guarded+16);
            __asm {fnstcw after}
            __asm {fldcw old_cw}
            CHECK(before==wanted&&after==before);CHECK(abi[0]==0x11111111&&abi[1]==0x22222222&&abi[2]==0x33333333&&abi[3]==0x44444444&&abi[4]==abi[5]);
            for(i=0;i<16;++i)CHECK(guarded[i]==0xa5&&guarded[i+26]==0xa5);
            CHECK(!memcmp(guarded+16,&low,4)&&!memcmp(guarded+20,&high,4)&&guarded[24]==(uint8_t)exponent&&guarded[25]==(uint8_t)(exponent>>8));
            printf("{\"low\":%u,\"high\":%u,\"cw\":%u}\n",(uint32_t)result,(uint32_t)(result>>32),(uint32_t)after);fflush(stdout);
        }
    }else puts("raw80 explicit-input conversion component ready");
    return 0;
}
