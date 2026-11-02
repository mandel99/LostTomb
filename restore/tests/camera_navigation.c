#include "camera_navigation.h"
#include "standalone_data.h"
#include <windows.h>
#include <wincrypt.h>
#include <stdio.h>
#include <string.h>
#define U(a) (*(uint32_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(uint32_t)(a))
#define FIELDS 38
#define CELLS (231u*457u)
#define CHECK(x) do{if(!(x)){fprintf(stderr,"Camera navigation failure at %d\n",__LINE__);ExitProcess(1);}}while(0)
/* op,seed,pattern,target,matchIndex,duplicate,width,height,rows,initialX,
 initialY,left,top,right,bottom,editor,mode,dirtyByte,dirtyWord,
 mutateFunction,mutatePhase,occurrence,bits,newWidth,newHeight,newX,newY,
 newLeft,newTop,newRight,newBottom,newEditor,newMode,newRows,
 lookupRow,lookupColumn,repeats,resetBefore. */
static uint32_t p[FIELDS],events[128][28],event_count,mutation_seen;
static HCRYPTPROV provider;
static const uint32_t regions[][2]={{0x70e440,80},{0x70e484,CELLS*4+32},{0xbca2b0,96},{0xd3b4d0,48},{0xe94db0,128},{0x6fdc40,32},{0x605a30,32},{0xe38dc0,32}};
static const uint32_t observed[]={0x70e458,0x70e460,0x70e464,0xd3b4e0,0xd3b4e4,0xbca2bc,0xbca2c0,0xe94dd0,0xe94dc8,0xe94dcc,0xe94dc4,0x6fdc50,0x605a3c,0xe38dcc,0xe94ddc,0xe94de0,0xe94df4,0xe94df8,0xe94dfc,0x70e494,0x77560c,0xe94e00,0xbca2c4};
uint32_t camera_navigation_actual_area(void),camera_navigation_actual_width(void),camera_navigation_actual_height(void),camera_navigation_actual_width_facade(void),camera_navigation_actual_height_facade(void);
void camera_navigation_actual_navigate(uint32_t),camera_navigation_actual_lookup(uint32_t),camera_navigation_actual_reset(void),camera_navigation_actual_clamp(void);
static void observe(uint32_t fn,uint32_t phase,uint32_t arg,uint32_t result)
{
    uint32_t *e,i;CHECK(event_count<128);e=events[event_count++];e[0]=fn;e[1]=phase;e[2]=arg;e[3]=result;
    for(i=0;i<23;++i)e[4+i]=U(observed[i]);e[27]=B(0xe38dca);
    if(fn==p[19]&&phase==p[20]&&++mutation_seen==p[21]){
        if(p[22]&1u){U(0x70e464)=p[23];U(0x70e460)=p[24];U(0x70e458)=p[33];}
        if(p[22]&2u){U(0xd3b4e0)=p[34];U(0xd3b4e4)=p[35];}
        if(p[22]&4u){U(0xbca2bc)=p[25];U(0xbca2c0)=p[26];}
        if(p[22]&8u){U(0xe94dd0)=p[27];U(0xe94dc8)=p[28];U(0xe94dcc)=p[29];U(0xe94dc4)=p[30];}
        if(p[22]&16u){U(0x6fdc50)=p[31];U(0x605a3c)=p[32];}
        if(p[22]&32u){B(0xe38dca)=0x81;U(0xe38dcc)=0x10203040;}
        if(p[22]&64u){U(0x70e494)=p[3];U(0x77560c)=p[3];}
        if(p[22]&128u){U(0xe94ddc)=0x12345678;U(0xe94df8)=0x87654321;}
    }
}
#define WRAP_RETURN(fn,actual,address) uint32_t fn(void){uint32_t r;observe(address,0,0,0);r=actual();observe(address,1,0,r);return r;}
#define WRAP_VOID(fn,actual,address) void fn(void){observe(address,0,0,0);actual();observe(address,1,0,0);}
#define WRAP_ARG(fn,actual,address) void fn(uint32_t arg){observe(address,0,arg,0);actual(arg);observe(address,1,arg,0);}
WRAP_RETURN(FUN_00430cc0,camera_navigation_actual_area,0x430cc0)
WRAP_RETURN(FUN_00430ca0,camera_navigation_actual_width,0x430ca0)
WRAP_RETURN(FUN_00430cb0,camera_navigation_actual_height,0x430cb0)
WRAP_RETURN(FUN_004ddea0,camera_navigation_actual_width_facade,0x4ddea0)
WRAP_RETURN(FUN_004ddeb0,camera_navigation_actual_height_facade,0x4ddeb0)
WRAP_VOID(FUN_004de140,camera_navigation_actual_reset,0x4de140)
WRAP_VOID(FUN_004de080,camera_navigation_actual_clamp,0x4de080)
WRAP_ARG(FUN_00430cd0,camera_navigation_actual_navigate,0x430cd0)
WRAP_ARG(FUN_004304c0,camera_navigation_actual_lookup,0x4304c0)
#pragma warning(push)
#pragma warning(disable:4100)
__declspec(naked) static void checked_call(uint32_t op,uint32_t arg,uint32_t out[7])
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
        mov eax,[esp+20]
        cmp eax,0
        je area_call
        cmp eax,2
        je lookup_call
        cmp eax,3
        je reset_call
        cmp eax,4
        je clamp_call
        push dword ptr[esp+24]
        call FUN_00430cd0
        add esp,4
        jmp complete_void
lookup_call:
        push dword ptr[esp+24]
        call FUN_004304c0
        add esp,4
        jmp complete_void
reset_call:
        call FUN_004de140
        jmp complete_void
clamp_call:
        call FUN_004de080
        jmp complete_void
area_call:
        call FUN_00430cc0
        jmp complete
complete_void:
        xor eax,eax
complete:
        mov ecx,[esp+28]
        mov [ecx],ebx
        mov [ecx+4],ebp
        mov [ecx+8],esi
        mov [ecx+12],edi
        mov [ecx+16],esp
        mov [ecx+24],eax
        pop edi
        pop esi
        pop ebx
        pop ebp
        ret
    }
}
#pragma warning(pop)
static uint32_t run(const uint32_t *q)
{
    uint32_t i,v,out[7];DWORD old;memcpy(p,q,sizeof(p));event_count=mutation_seen=0;
    for(i=0;i<8;++i){CHECK(VirtualProtect((void *)(uintptr_t)regions[i][0],regions[i][1],PAGE_READWRITE,&old));memset((void *)(uintptr_t)regions[i][0],(p[1]+i*29u)&255u,regions[i][1]);}
    for(i=0;i<CELLS;++i){v=p[2]==1?i:p[2]==2?p[3]:0x0fff0000u;U(0x70e494+i*4)=v;}
    if(p[4]<CELLS)U(0x70e494+p[4]*4)=p[3];if(p[5]<CELLS)U(0x70e494+p[5]*4)=p[3];
    U(0x70e464)=p[6];U(0x70e460)=p[7];U(0x70e458)=p[8];U(0xbca2bc)=p[9];U(0xbca2c0)=p[10];
    U(0xe94dd0)=p[11];U(0xe94dc8)=p[12];U(0xe94dcc)=p[13];U(0xe94dc4)=p[14];U(0x6fdc50)=p[15];U(0x605a3c)=p[16];B(0xe38dca)=(uint8_t)p[17];U(0xe38dcc)=p[18];
    if(p[37])FUN_004de140();
    for(i=0;i<p[36];++i){checked_call(p[0],p[3],out);CHECK(out[0]==0x11111111&&out[1]==0x22222222&&out[2]==0x33333333&&out[3]==0x44444444&&out[4]==out[5]);}
    return out[6];
}
static void digest(char hex[65])
{
    HCRYPTHASH h;DWORD length=32;uint8_t b[32];uint32_t i;static const char d[]="0123456789abcdef";CHECK(CryptCreateHash(provider,CALG_SHA_256,0,0,&h));
    for(i=0;i<8;++i){CHECK(CryptHashData(h,(const BYTE *)regions[i],8,0));CHECK(CryptHashData(h,(const BYTE *)(uintptr_t)regions[i][0],regions[i][1],0));}
    CHECK(CryptGetHashParam(h,HP_HASHVAL,b,&length,0));CHECK(CryptDestroyHash(h));for(i=0;i<32;++i){hex[i*2]=d[b[i]>>4];hex[i*2+1]=d[b[i]&15];}hex[64]=0;
}
int main(int argc,char **argv)
{
    uint32_t q[FIELDS]={0},i,j,r;char hex[65];SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOGPFAULTERRORBOX);CHECK(restore_data_open());CHECK(CryptAcquireContextW(&provider,NULL,NULL,PROV_RSA_AES,CRYPT_VERIFYCONTEXT));
    if(argc==2&&!strcmp(argv[1],"--oracle-batch")){
        puts("{\"fields\":38}");fflush(stdout);
        for(;;){for(i=0;i<FIELDS;++i)if(scanf("%u",q+i)!=1)goto done;r=run(q);digest(hex);printf("{\"memory\":\"%s\",\"return\":%u,\"events\":[",hex,r);for(i=0;i<event_count;++i){if(i)putchar(',');putchar('[');for(j=0;j<28;++j){if(j)putchar(',');printf("%u",events[i][j]);}putchar(']');}puts("]}");fflush(stdout);}
    }else{
        q[0]=1;q[3]=12345;q[4]=1000;q[5]=0xffffffff;q[6]=14;q[7]=30;q[8]=20;q[11]=0;q[12]=0;q[13]=500;q[14]=500;q[16]=1;q[36]=1;q[37]=1;run(q);CHECK(U(0xd3b4e0)==1000/231&&U(0xd3b4e4)==1000%231);CHECK(U(0xe38dcc)==1&&U(0xe94ddc)==0xffffffff&&U(0xe94df8)==0xffffffff);q[0]=0;q[6]=0x10002;q[8]=0x7fffffff;r=run(q);CHECK(r==(0x7fffffffu+2u)*0x10002u);puts("Camera navigation: real lookup, signed centering, reset, clamp and dimension chain.");
    }
done:CHECK(CryptReleaseContext(provider,0));return 0;
}
