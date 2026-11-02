#include "viewport_controller.h"
#include "standalone_data.h"
#include <windows.h>
#include <wincrypt.h>
#include <stdio.h>
#include <string.h>
#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(a))
#define CHECK(x) do{if(!(x)){fprintf(stderr,"Viewport controller failure at %d\n",__LINE__);ExitProcess(1);}}while(0)
/* op,seed,x,y,cameraMode,clipLeft,clipRight,clipTop,clipBottom,clock,last,
   overlay,special,counter,screenW,screenH,facadeX,facadeY,facadeMode,
   mutateAtCallMask,mutationBits,nextOverlay,nextSpecial,nextCounter,
   nextScreenW,nextScreenH,nextLeft,nextRight,nextTop,nextBottom,
   nextX,nextY,nextCameraMode,nextClock,nextLast,nextTick,guardSpecial,repeats. */
static uint32_t p[38],ops[256][4],op_count,events[16][25],event_count;
static const uint32_t regions[][2]={{0x6de510,12},{0x70e3b8,24},{0x70e428,0x68},{0x775608,16},{0xbca2b4,0x2c},{0xd3b548,0x20},{0xe38da4,0x18},{0xe92e04,16},{0xe94e04,16}};
static const uint32_t watched[]={0x70e478,0x70e474,0x70e484,0xd3b55c,0xd3b558,0xd3b554,0xd3b550,0x70e3c0,0x70e3c4,0x775610,0xe92e0c,0xe94e0c,0x6de515,0xe38db0,0xe38dac,0x70e43c,0x70e438,0x70e434,0x70e430,0xbca2bc,0xbca2c0,0xbca2d8};
static HCRYPTPROV provider;
void viewport_controller_actual_empty(void);
uint32_t restore_viewport_controller_read(uint32_t address,uint32_t width)
{
    uint32_t value=width==1?B(address):U(address);CHECK(op_count<256);
    ops[op_count][0]=0;ops[op_count][1]=address;ops[op_count][2]=width;ops[op_count++][3]=value;return value;
}
void restore_viewport_controller_write(uint32_t address,uint32_t value)
{
    CHECK(op_count<256);ops[op_count][0]=1;ops[op_count][1]=address;ops[op_count][2]=4;ops[op_count++][3]=value;U(address)=value;
}
static void endpoint(uint32_t address,uint32_t count,uint32_t arg)
{
    uint32_t *e,i;CHECK(event_count<16);e=events[event_count++];e[0]=address;e[1]=count;e[2]=arg;
    for(i=0;i<22;++i)e[3+i]=i==12?(p[36]?0xffffffffu:B(watched[i])):U(watched[i]);
    if(p[19]&(1u<<(event_count-1))){
        if(p[20]&1){U(0xe94e0c)=p[21];B(0x6de515)=(uint8_t)p[22];}
        if(p[20]&2)U(0x775610)=p[23];
        if(p[20]&4){U(0xe38db0)=p[24];U(0xe38dac)=p[25];}
        if(p[20]&8){U(0xd3b55c)=p[26];U(0xd3b558)=p[27];U(0xd3b554)=p[28];U(0xd3b550)=p[29];}
        if(p[20]&16){U(0x70e478)=p[30];U(0x70e474)=p[31];U(0x70e484)=p[32];}
        if(p[20]&32){U(0xe92e0c)=p[33];U(0x70e3c0)=p[34];U(0x70e3c4)=p[35];}
        if(p[20]&64){U(0x70e43c)=p[26];U(0x70e438)=p[27];U(0x70e434)=p[28];U(0x70e430)=p[29];}
        if(p[20]&128){U(0xbca2bc)=p[30];U(0xbca2c0)=p[31];U(0xbca2d8)=p[32];}
    }
}
/* Seven controlled drawing endpoints. Their bodies are outside this proof. */
void FUN_00430ef0(void){endpoint(0x430ef0,0,0);}
void FUN_004316f0(void){endpoint(0x4316f0,0,0);}
void FUN_00436b60(uint32_t a){endpoint(0x436b60,1,a);}
void FUN_00439f10(void){endpoint(0x439f10,0,0);}
void FUN_0043b630(void){endpoint(0x43b630,0,0);}
void FUN_00439740(uint32_t a){endpoint(0x439740,1,a);}
void FUN_0043e300(void){endpoint(0x43e300,0,0);}
/* Instrumentation observes the real no-op and can inject the same declared
   fixture mutations as other endpoints. The production body stays untouched. */
void FUN_0051f780(void){endpoint(0x51f780,0,0);viewport_controller_actual_empty();}
#pragma warning(push)
#pragma warning(disable:4100)
__declspec(naked) static void call_abi(uint32_t op,uint32_t x,uint32_t y,uint32_t mode,uint32_t out[6])
{
    __asm{
        push ebp
        push ebx
        push esi
        push edi
        mov ecx,[esp+36]
        mov [ecx+20],esp
        mov ebp,022222222h
        mov ebx,011111111h
        mov esi,033333333h
        mov edi,044444444h
        cmp dword ptr[esp+20],1
        je facade
        mov eax,[esp+24]
        mov edx,[esp+28]
        mov ecx,[esp+32]
        push ecx
        push edx
        push eax
        mov ecx,0badc0deh
        call FUN_00430d30
        add esp,12
        jmp complete
facade:
        mov ecx,0badc0deh
        call FUN_004dddc0
complete:
        mov ecx,[esp+36]
        mov [ecx],ebx
        mov [ecx+4],ebp
        mov [ecx+8],esi
        mov [ecx+12],edi
        mov [ecx+16],esp
        pop edi
        pop esi
        pop ebx
        pop ebp
        ret
    }
}
#pragma warning(pop)
static void run(const uint32_t *params)
{
    uint32_t i,out[6];DWORD previous=0,unused;memcpy(p,params,sizeof(p));CHECK(p[0]<=1&&p[37]>=1&&p[37]<=2&&(!p[36]||!(p[20]&1)));op_count=event_count=0;
    for(i=0;i<sizeof(regions)/sizeof(regions[0]);++i)memset((void *)(uintptr_t)regions[i][0],(int)((p[1]+i)&255),regions[i][1]);
    U(0x70e43c)=p[5];U(0x70e438)=p[6];U(0x70e434)=p[7];U(0x70e430)=p[8];U(0xe92e0c)=p[9];U(0x70e3c0)=p[10];U(0xe94e0c)=p[11];B(0x6de515)=(uint8_t)p[12];
    U(0x775610)=p[13];U(0xe38db0)=p[14];U(0xe38dac)=p[15];U(0xbca2bc)=p[16];U(0xbca2c0)=p[17];U(0xbca2d8)=p[18];
    if(p[36])CHECK(VirtualProtect((void *)0x6de000,4096,PAGE_NOACCESS,&previous));
    for(i=0;i<p[37];++i){call_abi(p[0],p[2],p[3],p[4],out);CHECK(out[0]==0x11111111&&out[1]==0x22222222&&out[2]==0x33333333&&out[3]==0x44444444&&out[4]==out[5]);}
    if(p[36])CHECK(VirtualProtect((void *)0x6de000,4096,previous,&unused));
}
static void digest(char hex[65])
{
    HCRYPTHASH h;DWORD length=32;uint8_t b[32];uint32_t i;static const char digits[]="0123456789abcdef";
    CHECK(CryptCreateHash(provider,CALG_SHA_256,0,0,&h));for(i=0;i<sizeof(regions)/sizeof(regions[0]);++i){CHECK(CryptHashData(h,(const BYTE *)regions[i],8,0));CHECK(CryptHashData(h,(const BYTE *)(uintptr_t)regions[i][0],regions[i][1],0));}
    CHECK(CryptGetHashParam(h,HP_HASHVAL,b,&length,0));CHECK(CryptDestroyHash(h));for(i=0;i<32;++i){hex[i*2]=digits[b[i]>>4];hex[i*2+1]=digits[b[i]&15];}hex[64]=0;
}
int main(int argc,char **argv)
{
    uint32_t params[38]={0,1,0x123,0x234,0x345,1,1023,2,767,61,0,0,0,1,1024,768,0x12345,0x23456,0x34567,0,0,51,1,0x80000000,1920,1080,10,1100,20,900,0x87654321,0x76543210,0x65432109,0xffffffff,0x80000000,0x12345678,0,1},i,j;DWORD previous;char hex[65];
    SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOGPFAULTERRORBOX);CHECK(restore_data_open());for(i=0;i<sizeof(regions)/sizeof(regions[0]);++i)CHECK(VirtualProtect((void *)(uintptr_t)regions[i][0],regions[i][1],PAGE_READWRITE,&previous));CHECK(CryptAcquireContextW(&provider,NULL,NULL,PROV_RSA_AES,CRYPT_VERIFYCONTEXT));
    if(argc==2&&!strcmp(argv[1],"--oracle-batch")){
        puts("{\"fields\":38}");fflush(stdout);for(;;){for(i=0;i<38;++i)if(scanf("%u",params+i)!=1)goto done;run(params);digest(hex);printf("{\"memory\":\"%s\",\"ops\":[",hex);
            for(i=0;i<op_count;++i)printf("%s[%u,%u,%u,%u]",i?",":"",ops[i][0],ops[i][1],ops[i][2],ops[i][3]);printf("],\"events\":[");
            for(i=0;i<event_count;++i){printf("%s[",i?",":"");for(j=0;j<25;++j)printf("%s%u",j?",":"",events[i][j]);putchar(']');}puts("]}");fflush(stdout);
        }
    }else{
        run(params);CHECK(event_count==6&&events[0][0]==0x430ef0&&events[1][0]==0x436b60&&events[4][2]==0&&U(0x70e478)==0x123&&U(0x70e3c0)==61&&U(0x775610)==0&&U(0xd3b558)==1024);
        params[0]=1;params[11]=9;params[19]=32;params[20]=2|4;run(params);CHECK(events[0][0]==0x4316f0&&events[2][0]==0x439f10&&events[4][2]==9999&&U(0x70e478)==0x12345&&U(0x775610)==0x7fffffff&&U(0xd3b558)==1920);
        params[11]=51;params[36]=1;run(params);CHECK(events[1][0]==0x436b60&&U(0x775610)==0x7fffffff);
        puts("Viewport controller: full DWORD camera/clip/clock/counter, draw order, actual facade and tail reloads.");
    }
done:CryptReleaseContext(provider,0);restore_data_close();return 0;
}
