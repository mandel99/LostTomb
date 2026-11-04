#include "ground_render.h"
#include "standalone_data.h"
#include <windows.h>
#include <wincrypt.h>
#include <stdio.h>
#include <string.h>
#define U(a) (*(uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(uint32_t)(a))
#define PIXELS 0x31000000u
#define FRAME 0x31100000u
#define FIELDS 68
#define CHECK(x) do{if(!(x)){fprintf(stderr,"Ground render failure at %d\n",__LINE__);ExitProcess(1);}}while(0)
/* Corrupt actor chains are outside the finite valid-grid fixtures. Preserve
   the production fail-closed boundary if a fixture unexpectedly reaches it. */
__declspec(noreturn) void restore_game_actor_grid_failure(uint32_t tile,int32_t actor)
{fprintf(stderr,"Unexpected corrupt actor grid %u/%d\n",tile,actor);ExitProcess(1);}
/* op,seed,h,w,camX,camY,pixelX,pixelY,dy,dx,halfX,lookupMode,tile,flags,
   edge,foot,imageOrOffset,building,type,damage,animate,special,overlay,
   selected,sentinelBlank,boundaryEnabled,boundaryTotal,neighborA,neighborB,
   occluders,actorType,actorHidden,links,imageW,imageH,format,upper,count,
   imageBaseMode,wrapperX,wrapperY,wrapperMode,wrapperActor,mutateAddress,
   mutateOccurrence,mutationBits,newTile,newRow,newCol,newDx,newDy,newW,
   newH,newImage,newType,newFlags,newFoot,newEdge,newSelected,newScreenW,
   newScreenH,newStats,repeats,transparent,tint,uniformCells,pixels,upperOffset. */
static uint32_t p[FIELDS],regions[48][2],region_count,events[8192][40],event_count,mutation_seen,current_image;
static HCRYPTPROV provider;
int ground_render_actual_sprite(int,int,int),ground_render_actual_upper(int,int,int);
void ground_render_actual_hclip(void),ground_render_actual_vclip(void),ground_render_actual_clear(void),ground_render_actual_empty(void);
uint32_t ground_render_actual_classify(uint32_t),ground_render_actual_boundary(int32_t);
static const uint32_t watched[]={0x70e47c,0x70e480,0x70e488,0x70e458,0x70e44c,0x70e464,0x70e460,0xd3b5c0,0x70e3e8,0x70e3ec,0xe94e0c,0x6059d0,0xd3b55c,0xd3b558,0xd3b554,0xd3b550};
static void event(uint32_t address,uint32_t self,uint32_t count,const uint32_t *args)
{
    uint32_t *e,i,b=(uint32_t)(int32_t)(int16_t)p[17]*264u;CHECK(event_count<8192);e=events[event_count++];memset(e,0,160);e[0]=address;e[1]=self;e[2]=count;for(i=0;i<count;++i)e[3+i]=args[i];
    for(i=0;i<16;++i)e[8+i]=U(watched[i]);e[24]=W(0x70e120);e[25]=W(0x7999e8+b);e[26]=W(0x7999e8);e[27]=U(0xf55b84);
    for(i=0;i<12;++i)e[28+i]=U(0xf55b50+i*4u);
    if(address==p[43]&&++mutation_seen==p[44]){
        if(p[45]&1)U(0x70e488)=p[46];
        if(p[45]&2){U(0x70e47c)=p[47];U(0x70e480)=p[48];}
        if(p[45]&4){U(0x70e458)=p[49];U(0x70e44c)=p[50];U(0x70e464)=p[51];U(0x70e460)=p[52];}
        if(p[45]&8){U(0xd3b5c0)=p[53];U(0xb7482c+4)=p[53];}
        if(p[45]&16)W(0x7999e8+b)=(uint16_t)p[54];
        if(p[45]&32){U(0xaf598c+4)=p[55];B(0xb285cc+1)=(uint8_t)p[56];B(0xb67d1c+1)=(uint8_t)p[57];}
        if(p[45]&64)U(0x6059d0)=p[58];
        if(p[45]&128){U(0xe38db0)=p[59];U(0xe38dac)=p[60];}
        if(p[45]&256){for(i=0;i<12;++i)U(0xf55b50+i*4u)=p[61];U(0xf55b84)=p[61];}
        if(p[45]&512)W(0x70e120)=0x80ff;
    }
}
void FUN_0053b670(void){event(0x53b670,0,0,NULL);ground_render_actual_clear();}
uint32_t FUN_0053b830(uint32_t type){event(0x53b830,0,1,&type);return ground_render_actual_classify(type);}
uint32_t restore_ground_boundary_at(int32_t x,int32_t y){uint32_t a[2]={(uint32_t)x,(uint32_t)y};event(0x47a1c0,0,2,a);return ground_render_actual_boundary(x);}
void FUN_0051f780(void){ground_render_actual_empty();}
void ground_render_pixel_empty(uint32_t pointer,uint32_t flags){uint32_t a[2]={pointer,flags};event(0x51f780,0,2,a);ground_render_actual_empty();}
void restore_ground_empty_at(int32_t x,int32_t y){uint32_t a[2]={(uint32_t)x,(uint32_t)y};event(0x51f780,0,2,a);ground_render_actual_empty();}
void FUN_00436b60(uint32_t actor){event(0x436b60,0,1,&actor);}
void FUN_00439740(uint32_t q){event(0x439740,0,1,&q);}
void FUN_0041d410(int32_t a,int32_t b){uint32_t v[2]={(uint32_t)a,(uint32_t)b};event(0x41d410,0,2,v);}
#define TWO(name,a) void name(int32_t x,int32_t y){uint32_t v[2]={(uint32_t)x,(uint32_t)y};event(a,0,2,v);}
TWO(FUN_00431ce0,0x431ce0) TWO(FUN_00432620,0x432620) TWO(FUN_00434e10,0x434e10)
void FUN_00433760(int32_t x,int32_t y,uint32_t n){uint32_t v[3]={(uint32_t)x,(uint32_t)y,n};event(0x433760,0,3,v);}
void FUN_00434aa0(int32_t x,int32_t y,uint32_t a,uint32_t b){uint32_t v[4]={(uint32_t)x,(uint32_t)y,a,b};event(0x434aa0,0,4,v);}
void FUN_00432d30(int32_t b,int32_t x,int32_t y,uint32_t n){uint32_t v[4]={(uint32_t)b,(uint32_t)x,(uint32_t)y,n};event(0x432d30,0,4,v);}
int FUN_0053e590(int image,int x,int y){uint32_t v[3]={(uint32_t)image,(uint32_t)x,(uint32_t)y};event(0x53e590,0xf55bb0,3,v);return ground_render_actual_sprite(image,x,y);}
int FUN_0053f250(int image,int x,int y){uint32_t v[3]={(uint32_t)image,(uint32_t)x,(uint32_t)y};event(0x53f250,0xf55bb0,3,v);return ground_render_actual_upper(image,x,y);}
void FUN_004cdad0(void){event(0x4cdad0,0,0,NULL);ground_render_actual_hclip();}
void FUN_004cdb80(void){event(0x4cdb80,0,0,NULL);ground_render_actual_vclip();}
static int pixel1(uint32_t address,uint32_t pointer){event(address,0,1,&pointer);return 77;}
static int pixel2(uint32_t address,uint32_t pointer,uint32_t color){uint32_t a[2]={pointer,color};event(address,0,2,a);return 77;}
void FUN_0040fb60(const void *q){pixel1(0x40fb60,(uint32_t)(uintptr_t)q);}
void FUN_0040ffd0(const void *q){pixel1(0x40ffd0,(uint32_t)(uintptr_t)q);}
void FUN_00410090(const void *q){pixel1(0x410090,(uint32_t)(uintptr_t)q);}
void FUN_00410230(const void *q){pixel1(0x410230,(uint32_t)(uintptr_t)q);}
void FUN_00410b10(const void *q){pixel1(0x410b10,(uint32_t)(uintptr_t)q);}
void FUN_0040fed0(const void *q){pixel1(0x40fed0,(uint32_t)(uintptr_t)q);}
void FUN_0040ff50(const void *q){pixel1(0x40ff50,(uint32_t)(uintptr_t)q);}
#define PIX1(name,a) int name(uint32_t q){return pixel1(a,q);}
PIX1(FUN_0040fc40,0x40fc40) PIX1(FUN_0040fcf0,0x40fcf0) PIX1(FUN_0040fdd0,0x40fdd0)
PIX1(FUN_004105c0,0x4105c0) PIX1(FUN_00410660,0x410660) PIX1(FUN_00410de0,0x410de0) PIX1(FUN_00410e80,0x410e80) PIX1(FUN_00410f70,0x410f70) PIX1(FUN_00410fb0,0x410fb0) PIX1(FUN_004114a0,0x4114a0) PIX1(FUN_00411fd0,0x411fd0)
#define PIX2(name,a) int name(uint32_t q,uint32_t c){return pixel2(a,q,c&65535u);}
PIX2(FUN_004100f0,0x4100f0) PIX2(FUN_004101c0,0x4101c0) PIX2(FUN_00410700,0x410700) PIX2(FUN_004109d0,0x4109d0) PIX2(FUN_00410a70,0x410a70)
int FUN_00427a20(uint32_t q,uint32_t n){return pixel2(0x427a20,q,n);}
int FUN_0042a160(uint32_t q,uint32_t n){return pixel2(0x42a160,q,n);}
int FUN_0042b440(uint32_t q,uint32_t n){return pixel2(0x42b440,q,n);}
#define PIX0(name,a) int name(void){event(a,0,0,NULL);return 77;}
PIX0(FUN_0042c740,0x42c740) PIX0(FUN_0042c9a0,0x42c9a0) PIX0(FUN_0042cee0,0x42cee0) PIX0(FUN_0042d810,0x42d810) PIX0(FUN_0042e680,0x42e680)
/* Fixtures contain already-loaded inline images. File-loading branches are
   excluded explicitly and must never be silently serviced by the oracle. */
void FUN_004ce310(char *p0,const char *ext){(void)p0;(void)ext;CHECK(0);}
int FUN_004ce480(const char *s,uint32_t a,uint32_t n,uint32_t o){(void)s;(void)a;(void)n;(void)o;CHECK(0);return 0;}
int FUN_0053f7d0(const char *s,uint32_t a,uint32_t n,uint32_t r,uint32_t o){(void)s;(void)a;(void)n;(void)r;(void)o;CHECK(0);return 0;}
void FUN_0053f790(uint32_t *p0,char b){(void)p0;(void)b;CHECK(0);}
void FUN_004cdf20(uint32_t p0,int n){uint32_t v[2]={p0,(uint32_t)n};event(0x4cdf20,0,2,v);}
void FUN_004cdeb0(uint32_t p0,int n){uint32_t v[2]={p0,(uint32_t)n};event(0x4cdeb0,0,2,v);}
#pragma warning(push)
#pragma warning(disable:4100)
__declspec(naked) static void checked_call(uint32_t op,const uint32_t *q,uint32_t out[6])
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
        je overlay
        cmp eax,2
        je wrapper
        call FUN_00430ef0
        jmp complete
overlay:
        call FUN_004316f0
        jmp complete
wrapper:
        mov ecx,[esp+24]
        push dword ptr[ecx+168]
        push dword ptr[ecx+164]
        push dword ptr[ecx+160]
        push dword ptr[ecx+156]
        call FUN_00430e60
        add esp,16
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
static void region(uint32_t a,uint32_t n)
{
    uint32_t i;DWORD old;for(i=0;i<region_count;++i)if(regions[i][0]==a)return;CHECK(region_count<48);CHECK(VirtualProtect((void *)(uintptr_t)a,n,PAGE_READWRITE,&old));regions[region_count][0]=a;regions[region_count][1]=n;memset((void *)(uintptr_t)a,(int)((p[1]+region_count)&255),n);++region_count;
}
static void run(const uint32_t *q)
{
    uint32_t i,a,b,row,col,index,n,out[6],ids[3]={1,257,1000};memcpy(p,q,sizeof(p));CHECK(p[0]<=2&&p[32]<=2&&p[62]>=1&&p[62]<=2);region_count=event_count=mutation_seen=0;
    region(0x6059c8,16);region(0x6de510,16);region(0x70e118,16);region(0x70e0f0,0x775620-0x70e0f0);region(0x78d7a8,16);
    for(i=0;i<6;++i){static const uint32_t bases[]={0xaf598c,0xb285cc,0xb350dc,0xb4e6fc,0xb67d1c,0xb7482c};static const uint32_t widths[]={4,1,2,2,1,4};region(bases[i]-8,8*widths[i]+16);}
    region(0xd3b520,224);region(0xe38aa0,0x150);region(0xe38d90,0x150);region(0xe92e10,24);region(0xe94e04,16);region(0xf55b48,0x68);region(0xf55bb0,8000);region(0x10d69b0,4097*64+96);region(0x1265178,16);
    b=(uint32_t)(int32_t)(int16_t)p[17];region(0x7999d0,280);region(0x7999d0+b*264u,280);region(0x7999d0+9*264u,280);region(0x7999d0+11*264u,280);
    for(i=0;i<3;++i)region(0xbcd5f8+ids[i]*388u,404);
    for(i=0;i<1000;++i){U(0xf55bb0+i*8)=0;U(0xf55bb4+i*8)=100+i*4u;}
    current_image=p[16];if(p[38]){static const uint32_t asset[]={0,0xf56bd4,0xf56d24,0xf57224,0xf572ec,0xf56bdc};CHECK(p[38]<=5);current_image+=U(asset[p[38]]);}
    for(i=0;i<4097;++i){a=0x10d69f8+i*64u;memset((void *)(uintptr_t)a,0,64);U(a)=p[66];U(a+4)=0x100;U(a+8)=p[67];U(a+16)=1;W(a+20)=(uint16_t)p[33];W(a+22)=(uint16_t)p[34];B(a+0x32)=(uint8_t)p[35];B(a+0x33)=(uint8_t)p[63];B(a+0x35)=(uint8_t)p[36];}
    U(0x126517c)=p[37];for(i=0;i<231u*457u;++i)U(0x70e494+i*4u)=0x0fff0000;
    U(0x70e440)=p[8]*4u;U(0x70e444)=p[8]*3u;U(0x70e448)=p[8]*2u;U(0x70e44c)=p[8];U(0x70e450)=p[9]*3u;U(0x70e454)=p[9]*2u;U(0x70e458)=p[9];U(0x70e45c)=p[10];U(0x70e460)=p[2];U(0x70e464)=p[3];U(0x70e478)=p[4];U(0x70e474)=p[5];U(0x70e490)=p[6];U(0x70e48c)=p[7];U(0x70e3c4)=p[20];U(0xe94e0c)=p[22];B(0x6de515)=(uint8_t)p[21];U(0x6059d0)=p[23];U(0xe38e54)=p[24];U(0x78d7b0)=p[25];U(0x78d7ac)=p[26];
    U(0x70e430)=768;U(0x70e434)=0;U(0x70e438)=1024;U(0x70e43c)=0;
    row=p[5]-8u;col=p[4]-4u;if((int32_t)row>=0&&row<457&&(int32_t)col>=0&&col<231){index=row*231u+col;if(p[11]!=2)U(0x70e494+index*4u)=p[12];if(p[11]==1){U(0x70e494+(index+1)*4u)=2;if(row<456)U(0x70e494+(index+231)*4u)=2;}if(p[11]==3)for(i=0;i<231u*457u;++i)U(0x70e494+i*4u)=p[12];U(0x70e0f4+index*4u)=p[27];U(0x70e0f8+index*4u)=p[28];U(0x70e0fc+index*4u)=p[28];}
    for(i=0;i<8;++i){n=(i==1||p[65])?0:1;U(0xaf598c+i*4u)=p[13]^(n?0x100u:0);B(0xb285cc+i)=(uint8_t)(p[15]^(n?7u:0));W(0xb350dc+i*2u)=0;W(0xb4e6fc+i*2u)=(uint16_t)p[17];B(0xb67d1c+i)=(uint8_t)(p[14]^(n?0x40u:0));U(0xb7482c+i*4u)=current_image+(n?i*31u:0);}
    W(0xb4e6fc+6)=9;W(0xb4e6fc+8)=11;a=0x7999d8+b*264u;memset((void *)(uintptr_t)a,0,264);W(a+16)=(uint16_t)p[18];B(a+0xbf)=(uint8_t)p[19];
    for(i=0;i<2;++i){a=0x7999d8+(i?11:9)*264u;memset((void *)(uintptr_t)a,0,264);n=(p[29]>>(i*8))&255;if(n>=1&&n<=6&&n!=2){W(a+16)=n==6?182:183;B(a+0x56)=n==3?0:1;U(a+0x68)=n==4?0:1;B(a+0x59)=n==5?4:3;}if(n==2)W(0xb350dc+(i?4:3)*2)=1;}
    for(i=0;i<3;++i){a=0xbcd600+ids[i]*388u;memset((void *)(uintptr_t)a,0,388);W(a)=i<p[32]?(uint16_t)ids[i+1]:0;B(a+2)=(uint8_t)p[30];B(a+0x5d)=(uint8_t)p[31];}
    W(0x70e120)=(uint16_t)p[64];U(0xd3b55c)=0;U(0xd3b558)=1024;U(0xd3b554)=0;U(0xd3b550)=768;U(0xe38d9c)=1024;U(0xe38d98)=768;U(0xe38db0)=1024;U(0xe38dac)=768;U(0xe38d94)=2048;U(0xe92e18)=FRAME+0x100;U(0xe92e1c)=FRAME+0x200;B(0xe38dda)=0;
    for(i=0;i<p[62];++i){checked_call(p[0],p,out);CHECK(out[0]==0x11111111&&out[1]==0x22222222&&out[2]==0x33333333&&out[3]==0x44444444&&out[4]==out[5]);}
}
static void digest(char hex[65])
{
    HCRYPTHASH h;DWORD length=32;uint8_t b[32];uint32_t i;static const char d[]="0123456789abcdef";CHECK(CryptCreateHash(provider,CALG_SHA_256,0,0,&h));for(i=0;i<region_count;++i){CHECK(CryptHashData(h,(const BYTE *)regions[i],8,0));CHECK(CryptHashData(h,(const BYTE *)(uintptr_t)regions[i][0],regions[i][1],0));}CHECK(CryptGetHashParam(h,HP_HASHVAL,b,&length,0));CHECK(CryptDestroyHash(h));for(i=0;i<32;++i){hex[i*2]=d[b[i]>>4];hex[i*2+1]=d[b[i]&15];}hex[64]=0;
}
int main(int argc,char **argv)
{
    uint32_t q[FIELDS]={0},i,j;char hex[65];DWORD old;SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOGPFAULTERRORBOX);CHECK(restore_data_open());CHECK(CryptAcquireContextW(&provider,NULL,NULL,PROV_RSA_AES,CRYPT_VERIFYCONTEXT));CHECK(VirtualAlloc((void *)PIXELS,0x10000,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE)==(void *)PIXELS);CHECK(VirtualAlloc((void *)FRAME,0x10000,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE)==(void *)FRAME);memset((void *)PIXELS,0x6b,0x10000);memset((void *)FRAME,0xc7,0x10000);CHECK(VirtualProtect((void *)PIXELS,0x10000,PAGE_READONLY,&old));CHECK(VirtualProtect((void *)FRAME,0x10000,PAGE_READONLY,&old));
    if(argc==2&&!strcmp(argv[1],"--oracle-batch")){printf("{\"fields\":%u}\n",FIELDS);fflush(stdout);for(;;){for(i=0;i<FIELDS;++i)if(scanf("%u",q+i)!=1)goto done;run(q);digest(hex);printf("{\"memory\":\"%s\",\"events\":[",hex);for(i=0;i<event_count;++i){printf("%s[",i?",":"");for(j=0;j<40;++j)printf("%s%u",j?",":"",events[i][j]);putchar(']');}puts("]}");fflush(stdout);}}
    else{q[2]=0xfffffff3u;q[3]=0xfffffffau;q[4]=4;q[5]=8;q[6]=320;q[7]=144;q[8]=15;q[9]=58;q[10]=29;q[12]=1;q[14]=64;q[16]=2000;q[23]=1;q[27]=q[28]=0xfff0000;q[30]=13;q[33]=58;q[34]=60;q[35]=1;q[36]=1;q[37]=4096;q[62]=1;q[66]=PIXELS+0x100;q[67]=4;run(q);CHECK(U(0xf55b84)==1&&U(0xf55b68)==1&&U(0x70e3ec)==80);puts("Ground renderer: coherent normal/overlay passes, counters, animation, occlusion and preview wrapper.");}
done:CryptReleaseContext(provider,0);restore_data_close();return 0;
}
