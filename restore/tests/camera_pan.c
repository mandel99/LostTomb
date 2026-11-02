#include "camera_pan.h"
#include "standalone_data.h"
#include <windows.h>
#include <wincrypt.h>
#include <stdio.h>
#include <string.h>
#define U(a) (*(uint32_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(uint32_t)(a))
#define FIELDS 43
#define CLOCK_ID 0x35000000u
#define CHECK(x) do{if(!(x)){fprintf(stderr,"Camera pan failure at %d\n",__LINE__);ExitProcess(1);}}while(0)
/* op,seed,now,last,speed,screenW,screenH,cursorX,cursorY,right,left,down,up,
 dirty,step,cameraX,cameraY,columns,rows,leftBound,topBound,rightBound,
 bottomBound,editor,mode,dirtyByte,direction,clockDelta,repeats,
 mutateFunction,phase,occurrence,bits,newNow,newLast,newSpeed,newCameraX,
 newCameraY,newWidth,newHeight,newMode,newFlags,newStep. */
static uint32_t p[FIELDS],events[256][31],used,seen,clock_now;
static HCRYPTPROV provider;
static const uint32_t regions[][2]={{0xe38da0,0x100},{0xe94da0,128},{0xe92e10,64},{0x5e2a50,32},{0xbca2b0,96},{0x70e450,64},{0x6fdc40,32},{0x605a30,32},{0x702970,0x22b0}};
static const uint32_t watched[]={0xe94db4,0xe38e70,0xe38db8,0xe38db4,0xe38dc4,0xe38dc0,0xe92e2c,0xe92e30,0xe92e34,0xe92e38,0xe38dcc,0x5e2a60,0xbca2bc,0xbca2c0,0x70e464,0x70e460,0xe94dd0,0xe94dc8,0xe94dcc,0xe94dc4,0x605a3c,0x6fdc50,0x702984,0x704b84,0x70298c,0x704b8c};
int32_t camera_pan_actual_sample(void);
uint32_t camera_pan_actual_cadence(void),camera_pan_actual_width_facade(void),camera_pan_actual_height_facade(void),camera_pan_actual_width(void),camera_pan_actual_height(void);
void camera_pan_actual_move(int32_t),camera_pan_actual_decay(void),camera_pan_actual_clamp(void);
static void observe(uint32_t fn,uint32_t phase,uint32_t arg,uint32_t result)
{
    uint32_t *e,j;CHECK(used<256);e=events[used++];e[0]=fn;e[1]=phase;e[2]=arg;e[3]=result;for(j=0;j<26;++j)e[4+j]=U(watched[j]);e[30]=B(0xe38dca);
    if(fn==p[29]&&phase==p[30]&&++seen==p[31]){
        if(p[32]&1u)clock_now=p[33];
        if(p[32]&2u){U(0xe94db4)=p[34];U(0xe38e70)=p[35];}
        if(p[32]&4u){U(0xbca2bc)=p[36];U(0xbca2c0)=p[37];}
        if(p[32]&8u){U(0x70e464)=p[38];U(0x70e460)=p[39];}
        if(p[32]&16u){U(0x605a3c)=p[40];U(0x6fdc50)=1;}
        if(p[32]&32u){U(0xe92e2c)=p[41]&1u?256:0;U(0xe92e30)=p[41]&2u?256:0;U(0xe92e34)=p[41]&4u?256:0;U(0xe92e38)=p[41]&8u?256:0;}
        if(p[32]&64u){U(0xe38dc4)=p[36];U(0xe38dc0)=p[37];U(0xe38db8)=p[38];U(0xe38db4)=p[39];}
        if(p[32]&128u)U(0x5e2a60)=p[42];
        if(p[32]&256u){U(0xe38dcc)=0x87654321;B(0xe38dca)=0x80;}
        if(p[32]&512u){U(0x702984)=0xfffffffd;U(0x70298c)=0x11110000;U(0x704b84)=0x80000001;U(0x704b8c)=0x22220000;}
    }
}
#define CP_WRAP_RET(fn,actual,id,type) type fn(void){type r;observe(id,0,0,0);r=actual();observe(id,1,0,(uint32_t)r);return r;}
#define CP_WRAP_VOID(fn,actual,id) void fn(void){observe(id,0,0,0);actual();observe(id,1,0,0);}
CP_WRAP_RET(FUN_004cf660,camera_pan_actual_sample,0x4cf660,int32_t)
CP_WRAP_RET(FUN_004ddd60,camera_pan_actual_cadence,0x4ddd60,uint32_t)
CP_WRAP_RET(FUN_004ddea0,camera_pan_actual_width_facade,0x4ddea0,uint32_t)
CP_WRAP_RET(FUN_004ddeb0,camera_pan_actual_height_facade,0x4ddeb0,uint32_t)
CP_WRAP_RET(FUN_00430ca0,camera_pan_actual_width,0x430ca0,uint32_t)
CP_WRAP_RET(FUN_00430cb0,camera_pan_actual_height,0x430cb0,uint32_t)
CP_WRAP_VOID(FUN_0041d6f0,camera_pan_actual_decay,0x41d6f0)
CP_WRAP_VOID(FUN_004de080,camera_pan_actual_clamp,0x4de080)
void FUN_004ddde0(int32_t direction){observe(0x4ddde0,0,(uint32_t)direction,0);camera_pan_actual_move(direction);observe(0x4ddde0,1,(uint32_t)direction,0);}
DWORD WINAPI camera_pan_verify_time(void){uint32_t r;observe(CLOCK_ID,0,0,0);r=clock_now;clock_now+=p[27];observe(CLOCK_ID,1,0,r);return r;}
#pragma warning(push)
#pragma warning(disable:4100)
__declspec(naked) static void checked_call(uint32_t op,int32_t direction,uint32_t out[7])
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
        cmp eax,1
        je cadence_call
        cmp eax,2
        je move_call
        cmp eax,4
        je decay_call
        cmp eax,5
        je clamp_call
        call FUN_004cf660
        cmp dword ptr[esp+20],3
        jne complete
        push eax
        call FUN_004ddde0
        pop eax
        jmp complete
cadence_call:
        call FUN_004ddd60
        jmp complete
move_call:
        push dword ptr[esp+24]
        call FUN_004ddde0
        add esp,4
        jmp complete_void
decay_call:
        call FUN_0041d6f0
        jmp complete_void
clamp_call:
        call FUN_004de080
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
    uint32_t j,k,out[7];DWORD old;memcpy(p,q,sizeof(p));used=seen=0;clock_now=p[2];
    for(j=0;j<9;++j){CHECK(VirtualProtect((void *)(uintptr_t)regions[j][0],regions[j][1],PAGE_READWRITE,&old));memset((void *)(uintptr_t)regions[j][0],(p[1]+j*29u)&255u,regions[j][1]);}
    for(j=0;j<69;++j)for(k=0;k<32;++k)U(0x702984+j*128u+k*4u)=(j*0x1020304u+k*0x1234567u+p[1])^(j&1u?0x80000001u:0xffffffffu);
    U(0xe94db4)=p[3];U(0xe38e70)=p[4];U(0xe38db8)=p[5];U(0xe38db4)=p[6];U(0xe38dc4)=p[7];U(0xe38dc0)=p[8];
    U(0xe92e2c)=p[9];U(0xe92e30)=p[10];U(0xe92e34)=p[11];U(0xe92e38)=p[12];U(0xe38dcc)=p[13];U(0x5e2a60)=p[14];U(0xbca2bc)=p[15];U(0xbca2c0)=p[16];
    U(0x70e464)=p[17];U(0x70e460)=p[18];U(0xe94dd0)=p[19];U(0xe94dc8)=p[20];U(0xe94dcc)=p[21];U(0xe94dc4)=p[22];U(0x6fdc50)=p[23];U(0x605a3c)=p[24];B(0xe38dca)=(uint8_t)p[25];
    for(j=0;j<p[28];++j){checked_call(p[0],(int32_t)p[26],out);CHECK(out[0]==0x11111111&&out[1]==0x22222222&&out[2]==0x33333333&&out[3]==0x44444444&&out[4]==out[5]);}return out[6];
}
static void digest(char hex[65])
{HCRYPTHASH h;DWORD n=32;uint8_t b[32];uint32_t j;static const char d[]="0123456789abcdef";CHECK(CryptCreateHash(provider,CALG_SHA_256,0,0,&h));for(j=0;j<9;++j){CHECK(CryptHashData(h,(const BYTE *)regions[j],8,0));CHECK(CryptHashData(h,(const BYTE *)(uintptr_t)regions[j][0],regions[j][1],0));}CHECK(CryptGetHashParam(h,HP_HASHVAL,b,&n,0));CHECK(CryptDestroyHash(h));for(j=0;j<32;++j){hex[j*2]=d[b[j]>>4];hex[j*2+1]=d[b[j]&15];}hex[64]=0;}
int main(int argc,char **argv)
{
    uint32_t q[FIELDS]={0},j,k,r;char hex[65];SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOGPFAULTERRORBOX);CHECK(restore_data_open());CHECK(CryptAcquireContextW(&provider,NULL,NULL,PROV_RSA_AES,CRYPT_VERIFYCONTEXT));
    if(argc==2&&!strcmp(argv[1],"--oracle-batch")){puts("{\"fields\":43}");fflush(stdout);for(;;){for(j=0;j<FIELDS;++j)if(scanf("%u",q+j)!=1)goto done;r=run(q);digest(hex);printf("{\"return\":%u,\"memory\":\"%s\",\"events\":[",r,hex);for(j=0;j<used;++j){if(j)putchar(',');putchar('[');for(k=0;k<31;++k){if(k)putchar(',');printf("%u",events[j][k]);}putchar(']');}puts("]}");fflush(stdout);}}
    else{q[0]=3;q[2]=1000;q[4]=100;q[5]=1024;q[6]=768;q[7]=512;q[8]=384;q[10]=q[12]=256;q[14]=1;q[15]=100;q[16]=100;q[17]=14;q[18]=30;q[21]=500;q[22]=500;q[24]=1;q[28]=1;r=run(q);CHECK(r==7&&U(0xbca2bc)==99&&U(0xbca2c0)==96&&U(0xe92e30)==0&&U(0xe94db4)==1000);puts("Camera pan: full DWORD cadence/keys, native direction precedence and actual decay/clamp.");}
done:CHECK(CryptReleaseContext(provider,0));restore_data_close();return 0;
}
