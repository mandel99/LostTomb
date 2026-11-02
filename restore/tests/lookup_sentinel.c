#include "lookup_sentinel.h"
#include "standalone_data.h"
#include <windows.h>
#include <wincrypt.h>
#include <stdio.h>
#include <string.h>
#define U(a) (*(uint32_t *)(uintptr_t)(uint32_t)(a))
#define CHECK(x) do{if(!(x)){fprintf(stderr,"Lookup sentinel failure at%d\n",__LINE__);ExitProcess(1);}}while(0)
#define FIELDS 11
#define IMAGE_COUNT (228u*228u)
/* op,orientation,asset,pattern,seed,mutationBits,beforeAsset,afterAsset,
   mutationImageIndex,mutationImageValue,repeats */
static uint32_t p[FIELDS],events[16][13],event_count;
static HCRYPTPROV provider;
static const uint32_t regions[][2]={{0x70e3fcu,0x54u},{LOOKUP_SENTINEL_GRID-16u,LOOKUP_SENTINEL_CELLS*4u+32u},{LOOKUP_SENTINEL_IMAGES-16u,IMAGE_COUNT*4u+32u},{0xf56b34u,36u}};
void lookup_sentinel_actual_clear(void);
static void observe(uint32_t phase)
{
    uint32_t i,*e;DWORD old,ignored;
    CHECK(event_count<16);e=events[event_count++];e[0]=phase;e[1]=U(0xf56b44);
    for(i=0;i<9;++i)e[2+i]=U(0x70e40c+i*4u);
    e[11]=U(LOOKUP_SENTINEL_GRID);e[12]=U(LOOKUP_SENTINEL_GRID+(LOOKUP_SENTINEL_CELLS-1u)*4u);
    if(phase==0&&(p[5]&1u))U(0xf56b44)=p[6];
    if(phase==1){
        if(p[5]&2u)U(0xf56b44)=p[7];
        if(p[5]&4u){
            CHECK(VirtualProtect((void *)(uintptr_t)(LOOKUP_SENTINEL_IMAGES+p[8]%IMAGE_COUNT*4u),4,PAGE_READWRITE,&old));
            U(LOOKUP_SENTINEL_IMAGES+p[8]%IMAGE_COUNT*4u)=p[9];
            CHECK(VirtualProtect((void *)(uintptr_t)(LOOKUP_SENTINEL_IMAGES+p[8]%IMAGE_COUNT*4u),4,old,&ignored));
        }
        if(p[5]&8u)for(i=0;i<9;++i)U(0x70e40c+i*4u)=0x80010000u+i;
    }
}
void FUN_00430480(void){observe(0);lookup_sentinel_actual_clear();observe(1);}
#pragma warning(push)
#pragma warning(disable:4100)
__declspec(naked) static void checked_call(uint32_t op,uint32_t orientation,uint32_t out[6])
{
    __asm{
        push ebp
        push ebx
        push esi
        push edi
        mov ecx,[esp+28]
        mov [ecx+20],esp
        mov ebx,011111111h
        mov ebp,022222222h
        mov esi,033333333h
        mov edi,044444444h
        cmp dword ptr[esp+20],0
        jne build_grid
        call FUN_00430480
        jmp complete
build_grid:
        push dword ptr[esp+24]
        call FUN_00430530
        add esp,4
complete:
        mov ecx,[esp+28]
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
static uint32_t image_value(uint32_t index,uint32_t *random)
{
    static const uint32_t cycle[]={0,1,5,6,7,255,256,65535,0x80000000u,0xfffffffeu,0x12345678u,0x80000001u};
    uint32_t row=index/228u,col=index%228u;
    *random^=*random<<13;*random^=*random>>17;*random^=*random<<5;
    switch(p[3]){
    case 0:return 0;case 1:return 5;case 2:return 6;case 3:return 0x7fffffffu;
    case 4:return 0x80000000u;case 5:return 0xffffffffu;
    case 6:return cycle[index%12u];case 7:return *random;
    case 8:return index==p[4]%IMAGE_COUNT?6u:0u;
    case 9:return index==p[4]%IMAGE_COUNT?0u:6u;
    case 10:return row>p[4]%80u&&row<148u&&col>p[4]%60u&&col<168u?0x12345678u:1u;
    case 11:return row<col?6u:5u;
    case 12:return row%3u==0?0x80000000u:col%5u==0?5u:65536u;
    case 13:return index;
    default:return p[4];
    }
}
static void run(const uint32_t *q)
{
    uint32_t i,j,random,out[6];DWORD old;
    memcpy(p,q,sizeof(p));event_count=0;random=p[4]?p[4]:1;
    for(i=0;i<sizeof(regions)/sizeof(regions[0]);++i){
        CHECK(VirtualProtect((void *)(uintptr_t)regions[i][0],regions[i][1],PAGE_READWRITE,&old));
        memset((void *)(uintptr_t)regions[i][0],(int)((p[4]+i*29u)&255u),regions[i][1]);
    }
    U(0xf56b44)=p[2];
    for(i=0;i<9;++i)U(0x70e40c+i*4u)=p[4]*0x01010101u+0x80000100u+i;
    for(i=0;i<IMAGE_COUNT;++i)U(LOOKUP_SENTINEL_IMAGES+i*4u)=image_value(i,&random);
    CHECK(VirtualProtect((void *)(uintptr_t)regions[2][0],regions[2][1],PAGE_READONLY,&old));
    for(j=0;j<p[10];++j){
        checked_call(p[0],p[1],out);
        CHECK(out[0]==0x11111111&&out[1]==0x22222222&&out[2]==0x33333333&&out[3]==0x44444444&&out[4]==out[5]);
    }
}
static void digest(char hex[65])
{
    HCRYPTHASH h;DWORD length=32;uint8_t b[32];uint32_t i;static const char digits[]="0123456789abcdef";
    CHECK(CryptCreateHash(provider,CALG_SHA_256,0,0,&h));
    for(i=0;i<sizeof(regions)/sizeof(regions[0]);++i){CHECK(CryptHashData(h,(const BYTE *)regions[i],8,0));CHECK(CryptHashData(h,(const BYTE *)(uintptr_t)regions[i][0],regions[i][1],0));}
    CHECK(CryptGetHashParam(h,HP_HASHVAL,b,&length,0));CHECK(CryptDestroyHash(h));
    for(i=0;i<32;++i){hex[i*2]=digits[b[i]>>4];hex[i*2+1]=digits[b[i]&15];}hex[64]=0;
}
int main(int argc,char **argv)
{
    uint32_t q[FIELDS]={0},i,j;char hex[65];
    SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOGPFAULTERRORBOX);CHECK(restore_data_open());
    CHECK(CryptAcquireContextW(&provider,NULL,NULL,PROV_RSA_AES,CRYPT_VERIFYCONTEXT));
    if(argc==2&&!strcmp(argv[1],"--oracle-batch")){
        puts("{\"fields\":11}");fflush(stdout);
        for(;;){
            for(i=0;i<FIELDS;++i)if(scanf("%u",q+i)!=1)goto done;
            run(q);digest(hex);printf("{\"memory\":\"%s\",\"events\":[",hex);
            for(i=0;i<event_count;++i){printf("%s[",i?",":"");for(j=0;j<13;++j)printf("%s%u",j?",":"",events[i][j]);putchar(']');}
            puts("]}");fflush(stdout);
        }
    }else{
        q[0]=1;q[2]=0x1234;q[3]=6;q[4]=51983;q[10]=1;run(q);
        CHECK(U(LOOKUP_SENTINEL_GRID)==0x0fff1234u);CHECK(U(0x70e420)>255&&U(0x70e41c)==0);
        CHECK(U(0x70e40c)==228&&U(0x70e410)==228&&event_count==2);
        puts("Lookup sentinel: complete art IDs, signed eligibility, rotations, full grid/bounds and real clear helper.");
    }
done:CryptReleaseContext(provider,0);restore_data_close();return 0;
}
