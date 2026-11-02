#include <windows.h>
#include <wincrypt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "map_validation.h"
#include "standalone_data.h"
#define CHECK(x) do{if(!(x)){fprintf(stderr,"check:%u\n",__LINE__);exit(2);}}while(0)
static uint32_t p[16],abi[8],used;
static uint32_t words[500000];
static const uint32_t regions[][2]={{0x77f738,19632},{0x784cec,0x11a},{0x5d19ae,0x450},{0xe38dba,48}};
static HCRYPTPROV provider;static uint32_t messages[128],message_count;
static void put(uint32_t a,uint32_t v,uint32_t n){memcpy((void *)(uintptr_t)a,&v,n);}
static void event(uint32_t k,uint32_t a,uint32_t n,uint32_t v){CHECK(used+4<=500000);words[used++]=k;words[used++]=a;words[used++]=n;words[used++]=v;}
#ifdef MAP_VALIDATION_TRACE
uint32_t map_validation_read(uint32_t a,uint32_t n){uint32_t v=0;memcpy(&v,(void *)(uintptr_t)a,n);event(0,a,n,v);return v;}
void map_validation_write(uint32_t a,uint32_t n,uint32_t v){v&=n==1?255u:65535u;event(1,a,n,v);put(a,v,n);}
void map_validation_enter(uint32_t a,uint32_t x,uint32_t y){event(2,a,x,y);}
#endif
/* Explicit local-proof renderer boundary only. Normal product calls real 5344C0. */
void map_validation_message_boundary(int32_t message){CHECK(message_count<128);messages[message_count++]=(uint32_t)message;}
static const uint8_t patterns[][14]={
 {0},{1,2,3,4,5,6,7,8,9,10,11,12,13,14},{14,13,12,11,10,9,8,7,6,5,4,3,2,1},
 {0,2,2,1,9,0,9,255,128,0,2,8,8,1},{2,2,2,2,2,2,2,2,2,2,2,2,2,2},{0},{0},
 {1,9,8,11,13,15,17,20,23,28,29,31,33,34},{14,16,18,22,20,29,32,11,1,9,8,31,13,34},{0},
 {255,255,255,255,255,255,255,255,255,255,255,255,255,255},{255,1,128,2,0,200,128,1,0,2,200,255,3,3},{31,13,1,11,9,0,0,0,0,0,0,0,0,0}};
static const uint8_t second[][8]={{0},{8,7,6,5,4,3,2,1},{0,2,2,255,128,0,128,2},{0},{0,255,1,128,1,0,2,2}};
static const uint32_t flags[]={0x784d0c,0x784cfc,0x784d3a,0x784d00,0x784d02,0x784d32,0x784d34,0x784d2a,0x784d2c,0x784d2e,0x784d30,0x784d28};
static void inventory(uint32_t row,uint32_t pattern){uint32_t i;for(i=0;i<14;++i){uint32_t v=patterns[pattern][i];if(pattern==5)v=p[2]&255;if(pattern==6)v=i%2?p[2]&255:0;if(pattern==9)v=i==13?p[2]&255:0;put(0x77f766+row*98+i,v,1);}}
static void prepare(void)
{
 uint32_t i,j,r,other;int32_t prod;
 for(i=0;i<sizeof(regions)/sizeof(regions[0]);++i)memset((void *)(uintptr_t)regions[i][0],(p[13]+37*i)&255,regions[i][1]);
 for(i=0;i<200;++i){r=0x77f748+i*98;put(r,1,1);put(r+1,p[14]==1||p[14]==2||p[14]==6,1);put(r+24,p[14]==2?1+i%3*2:p[14]==6?i%7:0,1);inventory(i,p[14]==6?7:0);memset((void *)(uintptr_t)(r+52),0,8);}
 r=0x77f748+p[3]*98;put(r,p[6],1);put(r+1,p[5],1);put(r+24,p[4],1);inventory(p[3],p[7]);
 for(i=0;i<8;++i)put(r+52+i,p[8]==3?p[2]&255:second[p[8]][i],1);
 if(p[14]>=3&&p[14]<=5){other=p[3]==199?0:199;r=0x77f748+other*98;put(r+1,1,1);put(r+24,p[14]-3,1);inventory(other,8);}
 if(p[14]==2)inventory(199,9);
 for(i=0;i<12;++i)put(flags[i],p[9]>>i&1?p[10]:0,2);
 for(prod=-1;prod<64;++prod)for(j=0;j<3;++j)put(0x5d19ce+(uint32_t)prod*16+j*2,0,2);
 put(0x5d19ce+16,p[15],2);put(0x5d19ce+32+2,p[15],2);put(0x5d19ce+48+4,p[15],2);put(0x5d19ce+64,p[15],2);put(0x5d19ce+64+2,p[2],2);put(0x5d19ce+33*16,0xffff,2);
 put(0x784de0,p[11]==1||p[11]==4?1:p[11]==5?33:p[11]==6?0xffff:0,2);put(0x784de2,p[11]==2||p[11]==4?2:0,2);put(0x784de4,p[11]==3||p[11]==4?3:0,2);
 put(0xe38dda,p[12],1);put(0xe38dca,p[13],1);used=0;message_count=0;
}
__declspec(naked) static void invoke(void)
{
    __asm{
        push ebp
        push ebx
        push esi
        push edi
        mov dword ptr [abi+20],esp
        mov ebx,011111111h
        mov ebp,022222222h
        mov esi,033333333h
        mov edi,044444444h
        mov ecx,0badc0deh
        mov edx,0fefefefeh
        push 0baadf00dh
        cmp dword ptr [p],0
        jne other
        push dword ptr [p+4]
        call FUN_00442a40
        add esp,4
        xor eax,eax
        jmp result
other:
        push dword ptr [p+4]
        cmp dword ptr [p],1
        je capability
        cmp dword ptr [p],2
        je product
        cmp dword ptr [p],3
        je contains
        cmp dword ptr [p],4
        je one_three_five
        cmp dword ptr [p],5
        je zero_state
        call FUN_00443a80
        jmp one_arg
capability:
        call FUN_00442870
        movzx eax,al
        jmp one_arg
product:
        call FUN_0040c8b0
        movzx eax,al
        jmp one_arg
contains:
        pop eax
        push dword ptr [p+8]
        push eax
        call FUN_00443000
        add esp,8
        jmp result
one_three_five:
        call FUN_004437b0
        jmp one_arg
zero_state:
        call FUN_00443820
one_arg:
        add esp,4
result:
        mov dword ptr [abi+28],eax
finished:
        pop eax
        mov dword ptr [abi+24],eax
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
static void digest(char out[65],int trace)
{
    HCRYPTHASH h;DWORD n=32;uint8_t b[32];uint32_t i;const char *d="0123456789abcdef";
    CHECK(CryptCreateHash(provider,CALG_SHA_256,0,0,&h));
    if(trace){if(used)CHECK(CryptHashData(h,(BYTE *)words,used*4,0));}
    else for(i=0;i<sizeof(regions)/sizeof(regions[0]);++i){CHECK(CryptHashData(h,(BYTE *)regions[i],8,0));CHECK(CryptHashData(h,(BYTE *)(uintptr_t)regions[i][0],regions[i][1],0));}
    CHECK(CryptGetHashParam(h,HP_HASHVAL,b,&n,0));CryptDestroyHash(h);
    for(i=0;i<32;++i){out[2*i]=d[b[i]>>4];out[2*i+1]=d[b[i]&15];}out[64]=0;
}
int main(int argc,char **argv)
{
    uint32_t i;char memory[65],trace[65];CHECK(restore_data_open());CHECK(CryptAcquireContext(&provider,0,0,PROV_RSA_AES,CRYPT_VERIFYCONTEXT));
    if(argc==2&&!strcmp(argv[1],"--oracle-batch")){
        puts("{\"fields\":16}");fflush(stdout);
        for(;;){
            for(i=0;i<16;++i)if(scanf("%u",p+i)!=1)goto done;
            CHECK(p[0]<=6&&p[3]<200&&p[7]<=12&&p[8]<=4&&p[14]<=6);
            prepare();invoke();CHECK(abi[0]==0x11111111&&abi[1]==0x22222222&&abi[2]==0x33333333&&abi[3]==0x44444444&&abi[4]==abi[5]&&abi[6]==0xbaadf00d);
            digest(memory,0);digest(trace,1);printf("{\"result\":%u,\"memory\":\"%s\",\"trace\":\"%s\",\"events\":%u,\"messages\":[",abi[7],memory,trace,used/4);for(i=0;i<message_count;++i)printf("%s%u",i?",":"",messages[i]);puts("]}");fflush(stdout);
        }
    }else puts("map validation C ready");
done:CryptReleaseContext(provider,0);restore_data_close();return 0;
}
