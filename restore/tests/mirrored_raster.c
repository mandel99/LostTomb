#include "mirrored_raster.h"
#include "standalone_data.h"
#include <windows.h>
#include <wincrypt.h>
#include <stdio.h>
#include <string.h>
#define U(a) (*(uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(uint32_t)(a))
#define SRC 0x31000000u
#define DST 0x31100000u
#define CAP 0x80000u
#define FIELDS 20
#define CHECK(x) do{if(!(x)){fprintf(stderr,"Mirrored raster failure at%d\n",__LINE__);ExitProcess(1);}}while(0)
/* width,height,x,y,pitch,skip,rowgap,pattern,seed,sourceOffset,frameOffset,
 mutatePhases,newWidth,newHeight,newX,newY,newPitch,newGap,newSkip,repeats */
static uint32_t p[FIELDS],events[8][10],event_count,final_source;
static uint64_t write_hash;
static uint32_t write_count;
static uint32_t plain_mode;
static HCRYPTPROV provider;
static const uint32_t regions[][2]={{SRC,CAP},{DST,CAP},{0xd3b520,224},{0xe38d88,32},{0xe92e10,32}};
char *mirrored_raster_actual_skip(char *pixels);
void mirrored_raster_plain(const void *pixels);
static void observe(uint32_t phase,uint32_t source)
{
    uint32_t *e;CHECK(event_count<8);e=events[event_count++];e[0]=phase;e[1]=source;e[2]=W(0xd3b5be);e[3]=U(0xd3b538);e[4]=U(0xd3b5d4);e[5]=U(0xd3b5e0);e[6]=U(0xd3b5dc);e[7]=U(0xe38d94);e[8]=U(0xe92e18);e[9]=U(0xd3b53c);
    if(p[11]&(1u<<phase)){W(0xd3b5be)=(uint16_t)p[12];U(0xd3b5d4)=p[13];U(0xd3b5e0)=p[14];U(0xd3b5dc)=p[15];U(0xe38d94)=p[16];U(0xd3b53c)=p[17];U(0xd3b538)=p[18];}
}
char *FUN_00410f20(char *pixels){char *result;observe(0,(uint32_t)(uintptr_t)pixels);result=mirrored_raster_actual_skip(pixels);observe(1,(uint32_t)(uintptr_t)result);return result;}
void restore_mirrored_raster_finish(uint32_t source){final_source=source;}
void restore_mirrored_raster_pixel(uint32_t address,uint16_t value){CHECK(address>=DST&&address+2u<=DST+CAP);write_hash=(write_hash^address)*UINT64_C(1099511628211);write_hash=(write_hash^value)*UINT64_C(1099511628211);++write_count;}
#pragma warning(push)
#pragma warning(disable:4100)
__declspec(naked) static void checked_call(uint32_t pixels,uint32_t out[6],uint32_t plain)
{
    __asm{
        push ebp
        push ebx
        push esi
        push edi
        mov ecx,[esp+24]
        mov [ecx+20],esp
        mov ebx,011111111h
        mov ebp,022222222h
        mov esi,033333333h
        mov edi,044444444h
        cmp dword ptr[esp+28],0
        jne plain_body
        push dword ptr[esp+20]
        call FUN_0040fc40
        jmp complete
plain_body:
        push dword ptr[esp+20]
        call mirrored_raster_plain
complete:
        add esp,4
        mov ecx,[esp+24]
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
static uint32_t positive(uint32_t value){return(int32_t)value>0?value:0;}
static void prepare_source(void)
{
    int width=(int16_t)((p[11]&1u)?p[12]:p[0]);
    uint32_t rows=positive(p[1])+positive(p[5])+positive(p[13])+positive(p[18])+2u;
    uint32_t row,col,run,i,cursor=p[9],pattern=p[7],count;
    memset((void *)SRC,pattern==6||pattern==8?1:0xa5,CAP);
    for(row=0;width>0&&row<rows;++row){
        col=0;
        while(col<(uint32_t)width){
            if(pattern==4||pattern==8||(pattern==11&&col>0))B(SRC+cursor++)=0;
            if(pattern==5){B(SRC+cursor++)=255;B(SRC+cursor++)=0;}
            run=pattern==0?1:pattern==1?254:1u+(row+col+p[8])%16u;
            if(pattern==6||pattern==8)run=(uint32_t)width+7u>254u?254u:(uint32_t)width+7u;
            if(pattern==2||pattern==7)run=255;
            if(pattern==7&&((uint32_t)width+7u)<255u)run=(uint32_t)width+7u;
            if(pattern==10||pattern==11)run=col==0?(pattern==10?130u:128u):17u;
            if(pattern!=6&&pattern!=7&&pattern!=8&&run>(uint32_t)width-col)run=(uint32_t)width-col;
            count=pattern==2||pattern==7||(pattern==3&&((row+col+p[8])%3u)==0)||(pattern==10&&col>0);
            CHECK(cursor+run*2u+4u<CAP);
            B(SRC+cursor++)=(uint8_t)(count?255u:run);
            if(count)B(SRC+cursor++)=(uint8_t)run;
            else for(i=0;i<run;++i){W(SRC+cursor)=pattern==6||pattern==8?0x0101u:(uint16_t)((row*(uint32_t)width+col+i)*37u+p[8]);cursor+=2u;}
            col+=run;
        }
    }
}
static void run(const uint32_t *q)
{
    uint32_t i,j,out[6];DWORD old;
    memcpy(p,q,sizeof(p));event_count=0;final_source=0;write_count=0;write_hash=UINT64_C(14695981039346656037);
    CHECK(VirtualProtect((void *)SRC,CAP,PAGE_READWRITE,&old));prepare_source();CHECK(VirtualProtect((void *)SRC,CAP,PAGE_READONLY,&old));
    for(i=0;i<CAP;++i)B(DST+i)=(uint8_t)((i*13u+(i>>7)*7u)^0xc7u);
    for(i=2;i<sizeof(regions)/sizeof(regions[0]);++i)memset((void *)(uintptr_t)regions[i][0],(int)(0x5au+i),regions[i][1]);
    W(0xd3b5be)=(uint16_t)p[0];U(0xd3b5d4)=p[1];U(0xd3b5e0)=p[2];U(0xd3b5dc)=p[3];U(0xe38d94)=p[4];U(0xd3b538)=p[5];U(0xd3b53c)=p[6];U(0xe92e18)=DST+p[10];
    for(j=0;j<p[19];++j){checked_call(SRC+p[9],out,plain_mode);CHECK(out[0]==0x11111111&&out[1]==0x22222222&&out[2]==0x33333333&&out[3]==0x44444444&&out[4]==out[5]);}
}
static void digest(char hex[65])
{
    HCRYPTHASH hash;DWORD length=32;uint8_t bytes[32];uint32_t i;static const char digits[]="0123456789abcdef";
    CHECK(CryptCreateHash(provider,CALG_SHA_256,0,0,&hash));for(i=0;i<sizeof(regions)/sizeof(regions[0]);++i){CHECK(CryptHashData(hash,(const BYTE *)regions[i],8,0));CHECK(CryptHashData(hash,(const BYTE *)(uintptr_t)regions[i][0],regions[i][1],0));}
    CHECK(CryptGetHashParam(hash,HP_HASHVAL,bytes,&length,0));CHECK(CryptDestroyHash(hash));for(i=0;i<32;++i){hex[i*2]=digits[bytes[i]>>4];hex[i*2+1]=digits[bytes[i]&15];}hex[64]=0;
}
int main(int argc,char **argv)
{
    uint32_t q[FIELDS]={7,2,300,8,2048,1,2034,3,123,17,0x10000,0,5,1,301,9,2048,2038,0,1},i,j;char hex[65];
    SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOGPFAULTERRORBOX);CHECK(restore_data_open());CHECK(CryptAcquireContextW(&provider,NULL,NULL,PROV_RSA_AES,CRYPT_VERIFYCONTEXT));CHECK(VirtualAlloc((void *)SRC,CAP,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE)==(void *)SRC);CHECK(VirtualAlloc((void *)DST,CAP,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE)==(void *)DST);
    if(argc==2&&(!strcmp(argv[1],"--oracle-batch")||!strcmp(argv[1],"--oracle-batch-plain"))){
        plain_mode=!strcmp(argv[1],"--oracle-batch-plain");
        puts("{\"fields\":20}");fflush(stdout);for(;;){for(i=0;i<FIELDS;++i)if(scanf("%u",q+i)!=1)goto done;run(q);digest(hex);printf("{\"memory\":\"%s\",\"source\":%u,\"write_count\":%u,\"write_hash\":\"%016llx\",\"events\":[",hex,final_source,write_count,(unsigned long long)write_hash);for(i=0;i<event_count;++i){printf("%s[",i?",":"");for(j=0;j<10;++j)printf("%s%u",j?",":"",events[i][j]);putchar(']');}puts("]}");fflush(stdout);}
    }else{char traced[65];run(q);CHECK(event_count==2&&final_source>SRC+17);CHECK(W(DST+0x10000+8u*2048u+600u)!=W(DST+0x10000+8u*2048u+602u));digest(traced);plain_mode=1;run(q);digest(hex);CHECK(!strcmp(traced,hex)&&final_source==0&&write_count==0);puts("Mirrored RLE: traced/plain actual row skip, exact leftward pixels, full coordinates and source cursor.");}
done:CryptReleaseContext(provider,0);restore_data_close();return 0;
}
