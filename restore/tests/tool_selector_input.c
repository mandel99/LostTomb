#include "tool_selector_input.h"
#include "standalone_data.h"
#include "sprite_libraries.h"
#include <wincrypt.h>
#include <stdio.h>
#include <string.h>
#define U TS_U
#define I TS_I
#define W TS_W
#define B TS_B
#define FIELDS 56
#define CHECK(x) do{if(!(x)){fprintf(stderr,"Tool selector input failure at %d\n",__LINE__);ExitProcess(1);}}while(0)
/* op,seed,category,count,screen64,screen60,sidebarMode,mode,offset,menuY,
 mouseX,mouseY,click,rectLayout,rectIndex,rows,rowKind,allowed,queryBits,
 queryHigh,have,need,player,built,temple,selectorState,actor,previous,
 mutateFunction,mutatePhase,occurrence,bits,newCategory,newCount,newPlayer,
 newMode,deltaMouseX,deltaMouseY,left,top,right,bottom,newScreen64,
 newOffset,newMenuY,newClick,recordKind,subtype,payload,recordIndex,
 savedVariant,priorMode,reservedKind,newRowKind,repeats,globalCategory. */
static uint32_t p[FIELDS],events[8192][30],used,seen,regions[32][2],region_count;
static HCRYPTPROV provider;
static const uint32_t fixed[][2]={{0x5c2800,0x1d00},{0x5ce300,0x10000},{0x70a800,0x10000},{0x6059b0,192},{0x7056b0,96},{0xbc9700,0x3c0},{0xbca2b0,112},{0xd3b180,48},{0xe38dc0,0x100},{0x784b80,0x300},{0x5e6960,48},{0x576130,48},{0xeaac20,16},{0xe92e00,24},{0xf53f50,128},{0xe94da0,32}};
static const uint32_t watched[]={0x7056ec,0x7056f0,0x7056c8,0xd3b198,0x6059e8,0xeaac28,0xbca310,0x605a3c,0xf53f64,0xf53f60,0xf53f78,0x7056e8,0xe94dac,0xe38e34,0xe38e30,0x605a30,0x605a48,0xbca300,0x6059cc,0xbc9714};
int32_t tool_selector_input_actual_input(void),tool_selector_input_actual_row(int32_t,int32_t),tool_selector_input_actual_count(void),tool_selector_input_actual_hover(int,int,const RestoreMenuButton *,int),tool_selector_input_actual_offset(void);
void tool_selector_input_actual_selector(void),tool_selector_input_actual_release(int32_t),tool_selector_input_actual_media(void);
static void rows(uint32_t category,uint32_t kind,uint32_t allowed)
{uint32_t j;for(j=0;j<31;++j){U(0x5ce3b8u+(category*30u+j)*4u)=j&&j<=p[15]?kind:0;U(0x70a8dcu+(category*30u+j)*4u)=allowed;}}
static void observe(uint32_t fn,uint32_t phase,uint32_t a,uint32_t b,uint32_t c,uint32_t d,uint32_t result)
{
    uint32_t *e,j;CHECK(used<8192);e=events[used++];e[0]=fn;e[1]=phase;e[2]=a;e[3]=b;e[4]=c;e[5]=d;e[6]=result;
    for(j=0;j<20;++j)e[7+j]=U(watched[j]);e[27]=B(0xe38e03);e[28]=B(0xe38dda);e[29]=B(0xe38dca);
    if(fn==p[28]&&phase==p[29]&&++seen==p[30]){
        if(p[31]&1u)U(0x7056ec)=p[32];if(p[31]&2u)U(0x7056f0)=p[33];if(p[31]&4u)U(0xeaac28)=p[34];if(p[31]&8u)U(0x6059e8)=p[35];
        if(p[31]&16u){U(0xe38e34)+=p[36];U(0xe38e30)+=p[37];B(0xe38e03)=(uint8_t)p[45];}
        if(p[31]&32u){U(0xf53f64)=p[42];U(0xe94dac)=p[43];U(0x7056e8)=p[44];}
        if(p[31]&64u)rows(p[2],p[53],p[17]);if(p[31]&128u)U(0xd3b198)=(fn==0x486060u||(fn==0x405200u&&phase==0))?2u:0xfedcba98u;
        if(p[31]&256u){U(0xbca310)=2;U(0x7056ec)=p[32];}
        if(p[31]&512u)W(0x5c28a8u+p[14]*28u)=(uint16_t)(p[38]+1u);
    }
}
#define OBS(fn,phase,a,b,c,d,r) observe(fn,phase,(uint32_t)(a),(uint32_t)(b),(uint32_t)(c),(uint32_t)(d),(uint32_t)(r))
int32_t FUN_00403e00(void){int32_t r;OBS(0x403e00,0,0,0,0,0,0);r=tool_selector_input_actual_input();OBS(0x403e00,1,0,0,0,0,r);return r;}
int32_t FUN_00403f10(int32_t a,int32_t b){int32_t r;OBS(0x403f10,0,a,b,0,0,0);r=tool_selector_input_actual_row(a,b);OBS(0x403f10,1,a,b,0,0,r);return r;}
int32_t FUN_00404120(void){int32_t r;OBS(0x404120,0,0,0,0,0,0);r=tool_selector_input_actual_count();OBS(0x404120,1,0,0,0,0,r);return r;}
void FUN_00405200(void){OBS(0x405200,0,0,0,0,0,0);tool_selector_input_actual_selector();OBS(0x405200,1,0,0,0,0,0);}
void FUN_00486060(int32_t a){OBS(0x486060,0,a,0,0,0,0);tool_selector_input_actual_release(a);OBS(0x486060,1,a,0,0,0,0);}
void FUN_0053d850(void){OBS(0x53d850,0,0,0,0,0,0);tool_selector_input_actual_media();OBS(0x53d850,1,0,0,0,0,0);}
int FUN_00426450(int a,int b,const RestoreMenuButton *c,int d){int r;OBS(0x426450,0,a,b,(uintptr_t)c,d,0);r=tool_selector_input_actual_hover(a,b,c,d);OBS(0x426450,1,a,b,(uintptr_t)c,d,r);return r;}
int32_t FUN_004d5ed0(void){int32_t r;OBS(0x4d5ed0,0,0,0,0,0,0);r=tool_selector_input_actual_offset();OBS(0x4d5ed0,1,0,0,0,0,r);return r;}
static uint32_t query(uint32_t fn,uint32_t a,uint32_t b,uint32_t c,uint32_t bit,uint32_t value)
{OBS(fn,0,a,b,c,0,0);if(bit!=0xffffffffu)value=p[19]|((p[18]>>bit)&1u);OBS(fn,1,a,b,c,0,value);return value;}
uint32_t FUN_00403ef0(void){return query(0x403ef0,0,0,0,0,0);}
uint32_t FUN_004f5470(int32_t a){return query(0x4f5470,a,0,0,a==0xb3?1:a==0xb2?2:a==0xb1?3:4,0);}
int32_t FUN_004f5020(void){return(int32_t)query(0x4f5020,0,0,0,0xffffffffu,p[20]);}
int32_t FUN_004f4ff0(void){return(int32_t)query(0x4f4ff0,0,0,0,0xffffffffu,p[21]);}
uint8_t FUN_004c52d0(int32_t a,int32_t b){return(uint8_t)query(0x4c52d0,a,b,0,b==1?5:b==2?6:7,0);}
uint8_t FUN_0053dd90(RestoreSpriteLibraries *a,const char *b,int c){return(uint8_t)query(0x53dd90,(uint32_t)(uintptr_t)a,(uint32_t)(uintptr_t)b,c,0xffffffffu,1);}
#pragma warning(push)
#pragma warning(disable:4100)
__declspec(naked) static void checked_call(uint32_t op,uint32_t a,uint32_t b,uint32_t out[7])
{
    __asm{
        push ebp
        push ebx
        push esi
        push edi
        mov ecx,[esp+32]
        mov [ecx+20],esp
        mov ebx,011111111h
        mov ebp,022222222h
        mov esi,033333333h
        mov edi,044444444h
        cmp dword ptr[esp+20],1
        je row_call
        call FUN_00403e00
        jmp complete
row_call:
        push dword ptr[esp+28]
        push dword ptr[esp+28]
        call FUN_00403f10
        add esp,8
complete:
        mov ecx,[esp+32]
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
static void region(uint32_t a,uint32_t n)
{DWORD old;CHECK(region_count<32);regions[region_count][0]=a;regions[region_count][1]=n;CHECK(VirtualProtect((void *)(uintptr_t)a,n,PAGE_READWRITE,&old));memset((void *)(uintptr_t)a,(p[1]+region_count*29u)&255u,n);++region_count;}
static uint32_t run(const uint32_t *q)
{
    uint32_t j,k,a,rec,out[7];memcpy(p,q,sizeof(p));used=seen=region_count=0;
    for(j=0;j<16;++j)region(fixed[j][0],fixed[j][1]);for(j=0;j<8;++j)region(0xea5ba0u+j*0x49d8u,0x500);region(0x7999d0u+p[26]*264u,280);
    for(j=0;j<38;++j)rows(j,p[16],p[17]);rows(256,p[16],p[17]);rows(0xffffffffu,p[16],p[17]);rows(p[2],p[16],p[17]);rows(p[32],p[16],p[17]);
    if(p[55]!=0xffffffffu)rows(p[55],p[53],p[17]^1u);
    U(0x5ce3b8u+p[2]*120u)=p[52];U(0x70a8d8u+p[2]*120u)=p[17];
    for(j=0;j<256;++j){a=0x5c28a8u+j*28u;if(p[13]==0&&j!=p[14]){W(a)=20000;W(a+2)=20000;W(a+4)=20001;W(a+6)=20001;}else{W(a)=(uint16_t)p[38];W(a+2)=(uint16_t)(p[39]+(p[13]==2?j*20u:0u));W(a+4)=(uint16_t)p[40];W(a+6)=(uint16_t)(p[41]+(p[13]==2?j*20u:0u));}}
    for(j=0;j<31;++j){U(0x5cf63cu+j*8u)=0x10000u+j;U(0x5cf640u+j*8u)=0x20000u+j;W(0x784ddeu+j*2u)=(uint16_t)p[49];}
    rec=0x5d19c8u+(uint32_t)(int32_t)(int16_t)p[49]*16u;W(rec)=(uint16_t)p[46];W(rec+2)=(uint16_t)p[47];W(rec+4)=(uint16_t)p[48];
    for(j=0;j<8;++j){a=j*0x49d8u;W(0xea5baau+a)=(uint16_t)(j==p[22]?p[50]:1);W(0xea5ce8u+a)=(uint16_t)(j==p[22]?p[24]:p[24]^1u);U(0xea5f9cu+a)=1000u+j;for(k=0;k<30;++k)U(0xea5fa0u+a+k*4u)=p[23];}
    U(0x7056ec)=p[55]==0xffffffffu?p[2]:p[55];U(0x7056f0)=p[3];U(0xf53f64)=p[4];U(0xf53f60)=p[5];U(0xf53f78)=p[6];U(0x605a3c)=p[7];U(0xe94dac)=p[8];U(0x7056e8)=p[9];U(0xe38e34)=p[10];U(0xe38e30)=p[11];B(0xe38e03)=(uint8_t)p[12];U(0xeaac28)=p[22];U(0xbca310)=p[25];U(0xbc9800)=p[26];U(0x6059e8)=p[51];U(0xd3b198)=1;U(0xe92e0c)=0x12345678;U(0xbca2e0)=2;
    for(j=0;j<p[54];++j){checked_call(p[0],p[2],p[27],out);CHECK(out[0]==0x11111111&&out[1]==0x22222222&&out[2]==0x33333333&&out[3]==0x44444444&&out[4]==out[5]);}return out[6];
}
static void digest(char hex[65])
{HCRYPTHASH h;DWORD n=32;uint8_t b[32];uint32_t j;static const char d[]="0123456789abcdef";CHECK(CryptCreateHash(provider,CALG_SHA_256,0,0,&h));for(j=0;j<region_count;++j){CHECK(CryptHashData(h,(const BYTE *)regions[j],8,0));CHECK(CryptHashData(h,(const BYTE *)(uintptr_t)regions[j][0],regions[j][1],0));}CHECK(CryptGetHashParam(h,HP_HASHVAL,b,&n,0));CHECK(CryptDestroyHash(h));for(j=0;j<32;++j){hex[j*2]=d[b[j]>>4];hex[j*2+1]=d[b[j]&15];}hex[64]=0;}
int main(int argc,char **argv)
{
    uint32_t q[FIELDS]={0},j,k,r;char hex[65];SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOGPFAULTERRORBOX);CHECK(restore_data_open());CHECK(CryptAcquireContextW(&provider,NULL,NULL,PROV_RSA_AES,CRYPT_VERIFYCONTEXT));
    if(argc==2&&!strcmp(argv[1],"--oracle-batch")){puts("{\"fields\":56}");fflush(stdout);for(;;){for(j=0;j<FIELDS;++j)if(scanf("%u",q+j)!=1)goto done;r=run(q);digest(hex);printf("{\"return\":%u,\"memory\":\"%s\",\"events\":[",r,hex);for(j=0;j<used;++j){if(j)putchar(',');putchar('[');for(k=0;k<30;++k){if(k)putchar(',');printf("%u",events[j][k]);}putchar(']');}puts("]}");fflush(stdout);}}
    else{q[2]=1;q[3]=1;q[4]=1024;q[5]=800;q[10]=907;q[11]=110;q[12]=1;q[15]=3;q[16]=0x6a;q[17]=256;q[26]=257;q[27]=0xffffffff;q[30]=1;q[40]=20;q[41]=16;q[49]=1;q[54]=1;q[55]=0xffffffff;r=run(q);CHECK(r==1&&U(0x7056c8)==1&&U(0x605a48)==0x405b80);puts("Tool selector input: real DWORD geometry, row enumeration, hover and selection composition.");}
done:CHECK(CryptReleaseContext(provider,0));restore_data_close();return 0;
}
