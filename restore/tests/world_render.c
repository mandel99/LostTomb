#include "world_render.h"
#include "graphics_53f250.h"
#include "standalone_data.h"
#include <windows.h>
#include <wincrypt.h>
#include <stdio.h>
#include <string.h>
#define U(a) (*(uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(uint32_t)(a))
#define CHECK(x) do{if(!(x)){fprintf(stderr,"World render failure at %d\n",__LINE__);ExitProcess(1);}}while(0)
#define ALT 0x30000000u
#define PIXELS 0x31000000u
#define FRAME 0x31100000u
#define FIELDS 67
static uint32_t p[FIELDS],regions[48][2],region_count,events[8192][32],event_count,mutation_seen;
static HCRYPTPROV provider;
void world_render_actual_hclip(void),world_render_actual_vclip(void);
int world_render_actual_upper_facade(int,int,int);
int32_t world_render_actual_record_facade(int32_t);
uint32_t world_render_actual_base_facade(int32_t);
void world_render_actual_city_empty(void);
static const uint32_t watched[]={0x70e47c,0x70e480,0x70e488,0x70e458,0x70e464,0x70e460,0xd3b5c0,0xe94e0c,0x126517c,0xd3b5e0,0xd3b5dc};
static void event(uint32_t address,uint32_t self,uint32_t count,const uint32_t *args)
{
    uint32_t *e,i,b=(uint32_t)(int32_t)(int16_t)p[15]*264u;
    CHECK(event_count<8192);e=events[event_count++];memset(e,0,128);e[0]=address;e[1]=self;e[2]=count;for(i=0;i<count;++i)e[3+i]=args[i];
    for(i=0;i<11;++i)e[8+i]=U(watched[i]);
    e[19]=W(0x70e120);e[20]=W(0x775688);e[21]=W(0x7999e8+b);e[22]=W(0x7999ea+b);e[23]=B(0x7999dd+b);
    e[24]=B(0x799a5f+b);e[25]=B(0x799a97+b);e[26]=W(0xd3b5bc);e[27]=W(0xd3b5be);e[28]=B(0xd3b530);e[29]=B(0xd3b531);e[30]=U(0xd3b5b8);e[31]=U(0xbca2e0);
    if(address==p[42]&&++mutation_seen==p[43]){
        if(p[44]&1)U(0x70e488)=p[45];
        if(p[44]&2){U(0x70e47c)=p[46];U(0x70e480)=p[47];}
        if(p[44]&4)U(0x70e458)=p[48];
        if(p[44]&8){W(0x7999e8+b)=(uint16_t)p[49];W(0x7999ea+b)=(uint16_t)p[50];}
        if(p[44]&16)W(0x70e120)=(uint16_t)p[51];
        if(p[44]&32){U(0xd3b5c0)=p[52];U(0xb7482c+4)=p[52];}
        if(p[44]&64){W(0x10d6a16+p[14]*64u)=(uint16_t)p[53];U(0x126517c)=1;}
        if(p[44]&128){B(0x7999dd+b)=1;B(0x799a5f+b)=2;}
    }
}
#define E0(name,a) void name(void){event(a,0,0,NULL);}
#define E2(name,a) void name(int32_t x,int32_t y){uint32_t v[2]={(uint32_t)x,(uint32_t)y};event(a,0,2,v);}
#define E3(name,a) void name(int32_t b,int32_t x,int32_t y){uint32_t v[3]={(uint32_t)b,(uint32_t)x,(uint32_t)y};event(a,0,3,v);}
#define E4(name,a) int32_t name(int32_t i,int32_t f,int32_t x,int32_t y){uint32_t v[4]={(uint32_t)i,(uint32_t)f,(uint32_t)x,(uint32_t)y};event(a,0,4,v);return 0x12345678;}
E2(FUN_00420a30,0x420a30) E2(FUN_0041fff0,0x41fff0) E2(FUN_00420050,0x420050)
E2(FUN_004206f0,0x4206f0) E2(FUN_00420d60,0x420d60) E2(FUN_00421060,0x421060)
E3(FUN_004e5e90,0x4e5e90) E3(FUN_00542e10,0x542e10) E3(FUN_004c0160,0x4c0160) E3(FUN_00436310,0x436310)
E4(FUN_00438470,0x438470) E4(FUN_004384d0,0x4384d0) E4(FUN_00438590,0x438590)
E4(FUN_00438650,0x438650) E4(FUN_004386b0,0x4386b0) E4(FUN_00438710,0x438710) E4(FUN_00438770,0x438770)
void FUN_004ef550(int32_t image,int32_t x,int32_t y,int32_t b){uint32_t v[4]={(uint32_t)image,(uint32_t)x,(uint32_t)y,(uint32_t)b};event(0x4ef550,0,4,v);}
void FUN_00401f70(int32_t b,int32_t x,int32_t y){uint32_t v[3]={(uint32_t)b,(uint32_t)x,(uint32_t)y};event(0x401f70,0x6de650,3,v);}
void FUN_00421920(int32_t x,int32_t y,int32_t b){uint32_t v[3]={(uint32_t)x,(uint32_t)y,(uint32_t)b};event(0x421920,0,3,v);}
void FUN_00438e50(int32_t b,int32_t x,int32_t y,uint32_t q){uint32_t v[4]={(uint32_t)b,(uint32_t)x,(uint32_t)y,q};event(0x438e50,0,4,v);}
uint32_t FUN_00442280(int32_t b,uint32_t tile,int32_t x,int32_t y){uint32_t v[4]={(uint32_t)b,tile,(uint32_t)x,(uint32_t)y};event(0x442280,0,4,v);return 0x12345678;}
uint32_t FUN_0040f2c0(int32_t image,int32_t tile){uint32_t v[2]={(uint32_t)image,(uint32_t)tile};event(0x40f2c0,0,2,v);return p[35];}
int32_t FUN_00488030(int32_t b){uint32_t v=(uint32_t)b;event(0x488030,0xbca350,1,&v);return (int32_t)p[36];}
uint32_t FUN_00488050(uint32_t b){event(0x488050,0xbca350,1,&b);return p[37];}
void FUN_00488c20(int32_t b,int32_t x,int32_t y,int32_t n){uint32_t v[4]={(uint32_t)b,(uint32_t)x,(uint32_t)y,(uint32_t)n};event(0x488c20,0xbca350,4,v);}
void FUN_00489500(int32_t b,int32_t frame,int32_t x,int32_t y){uint32_t v[4]={(uint32_t)b,(uint32_t)frame,(uint32_t)x,(uint32_t)y};event(0x489500,0xbca350,4,v);world_render_actual_city_empty();}
int32_t FUN_004381b0(int32_t image,int32_t x,int32_t y,int32_t dx,int32_t dy){uint32_t v[5]={(uint32_t)image,(uint32_t)x,(uint32_t)y,(uint32_t)dx,(uint32_t)dy};event(0x4381b0,0,5,v);return 0x12345678;}
void FUN_00438210(int32_t image,int32_t frame,int32_t x,int32_t y,uint32_t a){uint32_t v[5]={(uint32_t)image,(uint32_t)frame,(uint32_t)x,(uint32_t)y,a};event(0x438210,0,5,v);}
int32_t FUN_00438350(int32_t image,int32_t frame,int32_t x,int32_t y,int32_t a){uint32_t v[5]={(uint32_t)image,(uint32_t)frame,(uint32_t)x,(uint32_t)y,(uint32_t)a};event(0x438350,0,5,v);return 0x12345678;}
int32_t FUN_00438180(int32_t image){uint32_t v=(uint32_t)image;event(0x438180,0xf55bb0,1,&v);return restore_world_image_record(RESTORE_SPRITE_LIBRARIES,image);}
uint32_t FUN_004381a0(int32_t group){uint32_t v=(uint32_t)group;event(0x4381a0,0xf55bb0,1,&v);return restore_world_image_base(RESTORE_SPRITE_LIBRARIES,group);}
int FUN_0053e590(int image,int x,int y){uint32_t v[3]={(uint32_t)image,(uint32_t)x,(uint32_t)y};event(0x53e590,0xf55bb0,3,v);return 1;}
int FUN_0053f250(int image,int x,int y){uint32_t v[3]={(uint32_t)image,(uint32_t)x,(uint32_t)y};event(0x53f250,0xf55bb0,3,v);return world_render_actual_upper_facade(image,x,y);}
void FUN_004cdad0(void){event(0x4cdad0,0,0,NULL);world_render_actual_hclip();}
void FUN_004cdb80(void){event(0x4cdb80,0,0,NULL);world_render_actual_vclip();}
void FUN_00410230(const void *p0){uint32_t v=(uint32_t)(uintptr_t)p0;event(0x410230,0,1,&v);}
uint32_t FUN_004105c0(const void *p0){uint32_t v=(uint32_t)(uintptr_t)p0;event(0x4105c0,0,1,&v);return 0x12345678;}
uint32_t FUN_00410660(const void *p0){uint32_t v=(uint32_t)(uintptr_t)p0;event(0x410660,0,1,&v);return 0x12345678;}
void FUN_00410700(const void *p0,uint16_t color){uint32_t v[2]={(uint32_t)(uintptr_t)p0,color};event(0x410700,0,2,v);}
void FUN_004109d0(const void *p0,uint16_t color){uint32_t v[2]={(uint32_t)(uintptr_t)p0,color};event(0x4109d0,0,2,v);}
void FUN_00410a70(const void *p0,uint16_t color){uint32_t v[2]={(uint32_t)(uintptr_t)p0,color};event(0x410a70,0,2,v);}
#pragma warning(push)
#pragma warning(disable:4100)
__declspec(naked) static void checked_call(uint32_t op,uint32_t arg,uint32_t self,uint32_t out[7])
{
    __asm{
        push ebp
        push ebx
        push esi
        push edi
        mov ecx,[esp+32]
        mov [ecx+24],esp
        mov ebx,011111111h
        mov ebp,022222222h
        mov esi,033333333h
        mov edi,044444444h
        mov eax,[esp+20]
        mov edx,[esp+24]
        mov ecx,[esp+28]
        push edx
        cmp eax,1
        je global_record
        cmp eax,2
        je global_base
        cmp eax,3
        je member_record
        cmp eax,4
        je member_base
        mov ecx,0badc0deh
        call FUN_00436b60
        jmp cleanup_one
global_record:
        call world_render_actual_record_facade
        jmp cleanup_one
global_base:
        call world_render_actual_base_facade
        jmp cleanup_one
member_record:
        push ecx
        call restore_world_image_record
        add esp,4
        jmp cleanup_one
member_base:
        push ecx
        call restore_world_image_base
        add esp,4
cleanup_one:
        add esp,4
        mov ecx,[esp+32]
        mov [ecx],eax
        mov [ecx+4],ebx
        mov [ecx+8],ebp
        mov [ecx+12],esi
        mov [ecx+16],edi
        mov [ecx+20],esp
        pop edi
        pop esi
        pop ebx
        pop ebp
        ret
    }
}
#pragma warning(pop)
static void region(uint32_t a,uint32_t n)
{
    uint32_t i;DWORD old;for(i=0;i<region_count;++i)if(regions[i][0]==a)return;CHECK(region_count<48);CHECK(VirtualProtect((void *)(uintptr_t)a,n,PAGE_READWRITE,&old));
    regions[region_count][0]=a;regions[region_count][1]=n;memset((void *)(uintptr_t)a,(int)((p[0]+region_count)&255),n);++region_count;
}
static uint32_t prepare_and_run(const uint32_t *params)
{
    uint32_t i,a,b,row,col,index,ids[3],n,out[7],result=0;memcpy(p,params,sizeof(p));CHECK(p[33]<=2&&p[63]>=1&&p[63]<=2&&p[64]<=4);region_count=event_count=mutation_seen=0;
    region(0x6de510,16);region(0x70e118,16);region(0x70e0f0,0x775620-0x70e0f0);region(0x775680,24);
    for(i=0;i<7;++i){static const uint32_t bases[]={0xadc36c,0xaf598c,0xb285cc,0xb350dc,0xb4e6fc,0xb67d1c,0xb7482c};static const uint32_t widths[]={1,4,1,2,2,1,4};region(bases[i]-8,8*widths[i]+16);}
    region(0xbca2d8,24);region(0xd3b520,224);region(0xe38d90,24);region(0xe92e10,16);region(0xe94e04,16);region(0xf55bb0,8000);region(0x10d69b0,4097*64+96);region(0x1265178,16);
    b=(uint32_t)(int32_t)(int16_t)p[15];region(0x7999d0+b*264u,280);region(0x7999d0+9*264u,280);region(0x7999d0+11*264u,280);
    ids[0]=p[27]?p[27]:1;ids[1]=ids[0]==257?1000:257;ids[2]=ids[0]==1000?1999:1000;
    for(i=0;i<3;++i)region(0xbcd5f8+ids[i]*388u,404);region(0xbcd5f8+388u,404);
    if(p[24])region(0xbcd5f8+p[24]*388u,404);if(p[26])region(0xbcd5f8+(uint32_t)(int32_t)(int16_t)p[26]*388u,404);
    for(i=0;i<1000;++i){U(0xf55bb0+i*8u)=0;U(0xf55bb4+i*8u)=100+i*4u;}
    for(i=0;i<4097;++i){a=0x10d69f8+i*64u;memset((void *)(uintptr_t)a,0,64);U(a)=PIXELS+0x100;U(a+8)=4;W(a+0x14)=(uint16_t)p[39];W(a+0x16)=(uint16_t)p[40];W(a+0x1e)=(uint16_t)p[34];W(a+0x22)=0xfffb;W(a+0x24)=7;B(a+0x35)=(uint8_t)p[41];}
    U(0x10d69f8)=PIXELS+0x200;W(0x10d69f8+0x14)=118;W(0x10d69f8+0x16)=70;W(0x10d69f8+0x1e)=p[34]?(uint16_t)(p[34]+1u):0;W(0x10d69f8+0x22)=0xfff5;W(0x10d69f8+0x24)=13;
    U(0x126517c)=p[38];for(i=0;i<231u*457u;++i)U(0x70e494+i*4u)=0x0fff0000;
    U(0x70e440)=p[7]*4u;U(0x70e444)=p[7]*3u;U(0x70e448)=p[7]*2u;U(0x70e44c)=p[7];U(0x70e450)=p[8]*3u;U(0x70e454)=p[8]*2u;U(0x70e458)=p[8];U(0x70e45c)=p[9];
    U(0x70e460)=p[1];U(0x70e464)=p[2];U(0x70e478)=p[3];U(0x70e474)=p[4];U(0x70e490)=p[5];U(0x70e48c)=p[6];W(0x775688)=(uint16_t)p[62];U(0xe94e0c)=p[55];B(0x6de515)=(uint8_t)p[56];U(0xbca2e0)=p[57];
    row=p[4]-8u;col=p[3]-4u;
    if((int32_t)row>=0&&row<457&&(int32_t)col>=0&&col<231){index=row*231u+col;if(p[58]!=2)U(0x70e494+index*4u)=1;if(p[58]==1){U(0x70e494+(index+1)*4u)=2;if(row<456)U(0x70e494+(index+231)*4u)=2;}if(p[58]==3)for(i=0;i<231u*457u;++i)U(0x70e494+i*4u)=1;U(0x70e0f4+index*4u)=p[59];U(0x70e0f8+index*4u)=p[59];U(0x70e0fc+index*4u)=p[60];}
    for(i=0;i<8;++i){B(0xadc36c+i)=(uint8_t)p[13];U(0xaf598c+i*4u)=p[10];B(0xb285cc+i)=(uint8_t)p[12];W(0xb350dc+i*2u)=0;W(0xb4e6fc+i*2u)=(uint16_t)p[15];B(0xb67d1c+i)=(uint8_t)p[11];U(0xb7482c+i*4u)=p[14];}
    W(0xb4e6fc+3*2)=9;W(0xb4e6fc+4*2)=11;
    a=0x7999d8+b*264u;memset((void *)(uintptr_t)a,0,264);W(a+0x10)=(uint16_t)p[16];W(a+0x12)=(uint16_t)p[17];B(a+5)=(uint8_t)p[19];B(a+0x87)=(uint8_t)p[20];B(a+0xbf)=(uint8_t)p[18];B(a+0x56)=(uint8_t)p[21];U(a+0x68)=p[22];B(a+0x59)=(uint8_t)p[23];U(a+0x5c)=p[24];U(a+0x64)=p[25];W(a+0xb4)=(uint16_t)p[26];
    for(i=0;i<2;++i){a=0x7999d8+(i?11:9)*264u;memset((void *)(uintptr_t)a,0,264);n=(p[61]>>(8*i))&255;if(n>=1&&n<=6&&n!=2){W(a+16)=n==6?182:183;B(a+0x56)=n==3?0:1;U(a+0x68)=n==4?0:1;B(a+0x59)=n==5?4:3;}if(n==2)W(0xb350dc+(i?4:3)*2)=1;}
    if(p[66]){W(0xb4e6fc+4)=9;W(0x7999e8+9*264)=(uint16_t)p[66];}
    memset((void *)(0xbcd600u+388u),0,388);B(0xbcd602u+388u)=(uint8_t)p[28];
    for(i=0;i<3;++i){a=0xbcd600+ids[i]*388u;memset((void *)(uintptr_t)a,0,388);W(a)=i<p[33]?(uint16_t)ids[i+1]:0;B(a+2)=(uint8_t)p[28];B(a+0x5d)=(uint8_t)p[29];W(a+0x118)=(uint16_t)p[30];B(a+9)=(uint8_t)p[31];B(a+0x30)=(uint8_t)p[32];}
    if(p[27])W(0xb350dc+2)=(uint16_t)p[27];if(p[24])B(0xbcd609+p[24]*388u)=(uint8_t)p[31];if(p[26])B(0xbcd630+(uint32_t)(int32_t)(int16_t)p[26]*388u)=(uint8_t)p[32];
    U(0xd3b55c)=0;U(0xd3b558)=1024;U(0xd3b554)=0;U(0xd3b550)=768;U(0xe38d9c)=1024;U(0xe38d94)=2048;U(0xe92e18)=FRAME+0x100;
    for(i=0;i<(p[64]?1:p[63]);++i){checked_call(p[64],p[64]?p[65]:p[54],p[64]>=3?ALT:0xf55bb0,out);CHECK(out[1]==0x11111111&&out[2]==0x22222222&&out[3]==0x33333333&&out[4]==0x44444444&&out[5]==out[6]);if(p[64])result=out[0];}
    return result;
}
static void digest(char hex[65])
{
    HCRYPTHASH h;DWORD length=32;uint8_t b[32];uint32_t i;static const char d[]="0123456789abcdef";CHECK(CryptCreateHash(provider,CALG_SHA_256,0,0,&h));
    for(i=0;i<region_count;++i){CHECK(CryptHashData(h,(const BYTE *)regions[i],8,0));CHECK(CryptHashData(h,(const BYTE *)(uintptr_t)regions[i][0],regions[i][1],0));}
    CHECK(CryptGetHashParam(h,HP_HASHVAL,b,&length,0));CHECK(CryptDestroyHash(h));for(i=0;i<32;++i){hex[i*2]=d[b[i]>>4];hex[i*2+1]=d[b[i]&15];}hex[64]=0;
}
int main(int argc,char **argv)
{
    uint32_t q[FIELDS]={0},i,j,result;char hex[65];DWORD old;SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOGPFAULTERRORBOX);CHECK(restore_data_open());CHECK(CryptAcquireContextW(&provider,NULL,NULL,PROV_RSA_AES,CRYPT_VERIFYCONTEXT));CHECK(VirtualAlloc((void *)ALT,0x400000,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE)==(void *)ALT);CHECK(VirtualAlloc((void *)PIXELS,0x10000,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE)==(void *)PIXELS);CHECK(VirtualAlloc((void *)FRAME,0x10000,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE)==(void *)FRAME);memset((void *)PIXELS,0x6b,0x10000);memset((void *)FRAME,0xc7,0x10000);CHECK(VirtualProtect((void *)PIXELS,0x10000,PAGE_READONLY,&old));CHECK(VirtualProtect((void *)FRAME,0x10000,PAGE_READONLY,&old));
    U(ALT+0x30f5cc)=16;for(i=0;i<1000;++i)U(ALT+i*8+4)=500+i*3;
    if(argc==2&&!strcmp(argv[1],"--oracle-batch")){printf("{\"fields\":%u}\n",FIELDS);fflush(stdout);for(;;){for(i=0;i<FIELDS;++i)if(scanf("%u",q+i)!=1)goto done;result=prepare_and_run(q);digest(hex);printf("{\"result\":%u,\"memory\":\"%s\",\"events\":[",result,hex);for(i=0;i<event_count;++i){printf("%s[",i?",":"");for(j=0;j<32;++j)printf("%s%u",j?",":"",events[i][j]);putchar(']');}puts("]}");fflush(stdout);}}
    else{q[1]=0xffffffefu;q[2]=0xfffffffau;q[3]=4;q[4]=8;q[5]=320;q[6]=144;q[7]=15;q[8]=58;q[9]=29;q[10]=1;q[11]=64;q[14]=2000;q[15]=7;q[38]=4096;q[39]=58;q[40]=60;q[41]=1;q[59]=q[60]=0xfff0000;q[63]=1;prepare_and_run(q);CHECK(W(0x775688)==1);puts("World rendering: staged complete four-pass controller and explicit sprite-manager helpers.");}
done:CryptReleaseContext(provider,0);restore_data_close();return 0;
}
