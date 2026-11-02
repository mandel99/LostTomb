#include <windows.h>
#include <wincrypt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "map_file_load.h"
#include "standalone_data.h"
#define CHECK(x) do{if(!(x)){fprintf(stderr,"check:%u\n",__LINE__);exit(2);}}while(0)
static uint32_t p[16],abi[8],used;
static uint32_t words[100000];
static const uint32_t regions[][2]={{0x57625c,0x50},{0x5e06cc,0x60},{0xe36b84,0x380},{0xe37068,0x80},{0xe38adc,0x30},{0xd3b188,0x58},{0xe38dba,0xb8}};
static HCRYPTPROV provider;static uint8_t saved_prefix[0x50],saved_constants[0x60],io[16000];static uint32_t io_used,io_count,opens,chdirs;
static void put(uint32_t a,uint32_t v,uint32_t n){memcpy((void *)(uintptr_t)a,&v,n);}
static int legacy(uint32_t a,uint32_t n){uint32_t i;for(i=0;i<sizeof(regions)/sizeof(regions[0]);++i)if(a>=regions[i][0]&&a+n>=a&&a+n<=regions[i][0]+regions[i][1])return 1;return 0;}
static uint32_t token(uint32_t a){return !a||legacy(a,1)?a:0xc0000000u;}
static void event(uint32_t k,uint32_t a,uint32_t n,uint32_t v,uint32_t z){CHECK(used+5<=100000);words[used++]=k;words[used++]=a;words[used++]=n;words[used++]=v;words[used++]=z;}
#ifdef MAP_FILE_LOAD_TRACE
uint32_t map_file_read(uint32_t a,uint32_t n){uint32_t v=0;memcpy(&v,(void *)(uintptr_t)a,n);if(legacy(a,n))event(0,a,n,v,0);return v;}
void map_file_write(uint32_t a,uint32_t n,uint32_t v){if(n==1)v&=255;event(1,a,n,v,0);put(a,v,n);}
void map_file_enter(uint32_t a,uint32_t x,uint32_t y,uint32_t z){if(a==0x4cbea0){x=token(x);y=token(y);}else if(a!=0x4099b0&&a!=0x4ce2d0)x=token(x);event(2,a,x,y,z);}
#endif
static void io_word(uint32_t v){CHECK(io_used+4<sizeof(io));memcpy(io+io_used,&v,4);io_used+=4;}
static void io_string(const char *s){uint32_t n=s?(uint32_t)strlen(s)+1:0;CHECK(io_used+4+n<sizeof(io));io_word(n);if(n){memcpy(io+io_used,s,n);io_used+=n;}}
static void io_event(uint32_t a,uint32_t x,uint32_t y,uint32_t z,uint32_t result,const char *first,const char *second){io_word(a);io_word(x);io_word(y);io_word(z);io_word(result);io_string(first);io_string(second);++io_count;}
int32_t map_file_open(char *path,uint32_t flags){int32_t r=(int32_t)(opens++?p[6]:p[5]);io_event(0x568eab,token((uint32_t)(uintptr_t)path),flags,0,(uint32_t)r,path,0);return r;}
int32_t map_file_close(int32_t fd){io_event(0x5647eb,(uint32_t)fd,0,0,p[15],0,0);return (int32_t)p[15];}
int32_t map_file_chdir(char *path){int32_t r=p[7]>>chdirs++&1?-1:0;io_event(0x56b2dd,token((uint32_t)(uintptr_t)path),0,0,(uint32_t)r,path,0);return r;}
char *map_file_getcwd(char *dst,int32_t n){if(!p[8])strcpy(dst,"Z:\\fixture");io_event(0x5623fc,(uint32_t)(uintptr_t)dst,(uint32_t)n,0,p[8]?0:(uint32_t)(uintptr_t)dst,0,dst);return p[8]?0:dst;}
static void post_load(void){if(p[9]&1)strcpy((char *)(uintptr_t)0xe37078,"mutated.after.map");if(p[9]&2)put(0xd3b1c0,0,1);if(p[9]&4){put(0xe38dda,0x80,1);put(0xe38dca,0x81,1);}}
uint32_t map_file_deep_load(char *path){io_event(0x4d5520,token((uint32_t)(uintptr_t)path),0,0,p[10],path,0);post_load();return p[10];}
void map_file_deep_start(void){io_event(0x4de600,0,0,0,0,0,0);post_load();}
void map_file_diagnostic(char *text,int32_t detail,int32_t mode){io_event(0x4ce6e0,(uint32_t)(uintptr_t)text,(uint32_t)detail,(uint32_t)mode,0,text,(char *)(uintptr_t)(uint32_t)detail);}
static const char *names[]={"", "name", "a.b.c", ".hidden", "last.part", "a.555", "b.sMk", "c.wav", "d.zzz", "plain", "n\x80" "me", "x.a", "no.ext.more", 0, "abc"};
static const char *extensions[]={"map","555","sMk","wav","zzz","a", "", "\x80" "Az"};
static void prepare(void)
{
 uint32_t i;char long_name[91];
 for(i=0;i<sizeof(regions)/sizeof(regions[0]);++i)memset((void *)(uintptr_t)regions[i][0],(p[11]+37*i)&255,regions[i][1]);
 memcpy((void *)(uintptr_t)0x57625c,saved_prefix,sizeof(saved_prefix));memcpy((void *)(uintptr_t)0x5e06cc,saved_constants,sizeof(saved_constants));
 memset(long_name,'L',90);long_name[90]=0;strcpy((char *)(uintptr_t)0xe37078,p[3]==13?long_name:names[p[3]]);
 memset((void *)(uintptr_t)0xe38aec,0,4);strcpy((char *)(uintptr_t)0xe38aec,extensions[p[4]]);
 strcpy((char *)(uintptr_t)0xe36b94,"Q:\\original");strcpy((char *)(uintptr_t)0xe36e84,"stale");put(0xe38e62,'R',1);
 put(0xd3b198,p[1],4);put(0xd3b1c0,p[2],1);put(0xd3b1b3,p[13],1);put(0xe38dda,p[12],1);put(0xe38dca,p[11],1);used=io_used=io_count=opens=chdirs=0;
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
        call FUN_004099b0
        xor eax,eax
        jmp result
other:
        push 0e37078h
        cmp dword ptr [p],1
        je access_check
        cmp dword ptr [p],2
        je select_directory
        cmp dword ptr [p],3
        je restore_directory
        cmp dword ptr [p],4
        je trim
        cmp dword ptr [p],5
        je append
        cmp dword ptr [p],6
        je extract
        cmp dword ptr [p],7
        je upper
        pop eax
        push dword ptr [p+56]
        push 0e38aech
        push 057626ch
        call FUN_004cbea0
        add esp,12
        xor eax,eax
        jmp result
access_check:
        call FUN_004ce3f0
        add esp,4
        jmp result
select_directory:
        call FUN_004ce100
        add esp,4
        jmp result
restore_directory:
        pop eax
        call FUN_004ce2d0
        xor eax,eax
        jmp result
trim:
        call FUN_004ce3a0
        jmp no_result
append:
        call FUN_004ce3c0
        jmp no_result
extract:
        call FUN_004ce330
        jmp no_result
upper:
        pop eax
        push 0e38aech
        call FUN_004cbec0
no_result:
        add esp,4
        xor eax,eax
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
static void io_digest(char hex[65]){HCRYPTHASH h;DWORD n=32;uint8_t b[32];uint32_t i;const char *d="0123456789abcdef";CHECK(CryptCreateHash(provider,CALG_SHA_256,0,0,&h));if(io_used)CHECK(CryptHashData(h,io,io_used,0));CHECK(CryptGetHashParam(h,HP_HASHVAL,b,&n,0));CryptDestroyHash(h);for(i=0;i<32;++i){hex[2*i]=d[b[i]>>4];hex[2*i+1]=d[b[i]&15];}hex[64]=0;}
int main(int argc,char **argv)
{
    uint32_t i;char memory[65],trace[65],io_hex[65];CHECK(restore_data_open());memcpy(saved_prefix,(void *)(uintptr_t)0x57625c,sizeof(saved_prefix));memcpy(saved_constants,(void *)(uintptr_t)0x5e06cc,sizeof(saved_constants));CHECK(CryptAcquireContext(&provider,0,0,PROV_RSA_AES,CRYPT_VERIFYCONTEXT));
    if(argc==2&&!strcmp(argv[1],"--oracle-batch")){
        puts("{\"fields\":16}");fflush(stdout);
        for(;;){
            for(i=0;i<16;++i)if(scanf("%u",p+i)!=1)goto done;
            CHECK(p[0]<=8&&p[3]<=14&&p[4]<=7);
            prepare();
#ifdef MAP_FILE_LOAD_TRACE
            if(p[0]==4)map_file_enter(0x4ce3a0,0xe37078,0,0);
            if(p[0]==5)map_file_enter(0x4ce3c0,0xe37078,0,0);
            if(p[0]==6)map_file_enter(0x4ce330,0xe37078,0,0);
            if(p[0]==7)map_file_enter(0x4cbec0,0xe38aec,0,0);
            if(p[0]==8)map_file_enter(0x4cbea0,0x57626c,0xe38aec,p[14]);
#endif
            invoke();CHECK(abi[0]==0x11111111&&abi[1]==0x22222222&&abi[2]==0x33333333&&abi[3]==0x44444444&&abi[4]==abi[5]&&abi[6]==0xbaadf00d);
            digest(memory,0);digest(trace,1);io_digest(io_hex);printf("{\"result\":%u,\"memory\":\"%s\",\"trace\":\"%s\",\"events\":%u,\"io\":\"%s\",\"io_count\":%u}\n",abi[7],memory,trace,used/5,io_hex,io_count);fflush(stdout);
        }
    }else puts("map file load C ready");
done:CryptReleaseContext(provider,0);restore_data_close();return 0;
}
