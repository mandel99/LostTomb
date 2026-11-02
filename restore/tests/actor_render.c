#include "actor_render.h"
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
#define FIELDS 92
#define CHECK(x) do{if(!(x)){fprintf(stderr,"Actor render failure at%d\n",__LINE__);ExitProcess(1);}}while(0)
/* op,seed,actor,x,y,selection,type,upper,lower,facing,fallback,moving,zflag,
 fraction,elevation,moveX,moveY,z,attached,attachedType,attachedZ,offsetX,
 offsetY,owner,blocked,rotation,count,halfX,halfY,baseX,baseY,upperX,upperY,
 upperHeight,cargo,ship,shipState,shipFlag,meter,meterBase,cargoBase,traderX,
 traderY,traderFlag,quantity,predicate,coordinateX,coordinateY,counter,
 counterFlag,height,width,camY,camX,pixelY,pixelX,stepX,lookupMode,tile,
 head,links,visible,overlay,special,format,imageW,imageH,transparent,tint,
 mutateAddress,mutateOccurrence,mutationBits,newType,newOffsetX,newOffsetY,
 newRow,newCol,newTile,newCount,newStep,newHeight,newWidth,newNext,
 newShipFlag,newQuantity,newCargoBase,newMetaHeight,repeats,pixels,
 newVisible,upperOffset,newImage. */
static uint32_t p[FIELDS],regions[32][2],region_count,events[8192][44],event_count,mutation_seen;
static HCRYPTPROV provider;
void actor_render_actual_renderer(int32_t,uint32_t,uint32_t,uint32_t);
int actor_render_actual_sprite(int,int,int);
void actor_render_actual_hclip(void),actor_render_actual_vclip(void);
static const uint32_t watched[]={0x70e47c,0x70e480,0x70e488,0x70e458,0x70e44c,0x70e464,0x70e460,0xe94e00,0x70e3e4,0x70e3e0,0xbca2e0,0x78d5ec,0x126517c,0xd3b5c0,0xe94e0c,0x978d78};
static void event(uint32_t address,uint32_t self,uint32_t count,const uint32_t *args)
{
    uint32_t i,*e,actor=p[2]*388u,ship=(uint32_t)(int32_t)(int16_t)p[35]*144u;
    CHECK(event_count<8192&&count<=7);e=events[event_count++];memset(e,0,176);e[0]=address;e[1]=self;e[2]=count;
    for(i=0;i<count;++i)e[3+i]=args[i];for(i=0;i<16;++i)e[10+i]=U(watched[i]);
    e[26]=W(0x70e120);e[27]=B(0xbcd602+actor);e[28]=B(0xbcd664+actor);e[29]=B(0xbcd665+actor);e[30]=W(0xbcd600+actor);
    e[31]=U(0xbcd778+actor);e[32]=U(0xbcd5fc+actor);e[33]=U(0xbcba4c+ship);e[34]=B(0xbcb9ca+ship);e[35]=B(0xbcd603+actor);
    e[36]=W(0x10d6a0e+p[7]*64u);e[37]=U(0x978d78);e[38]=B(0x605b3d);e[39]=B(0xbcd66f+actor);e[40]=B(0xbcd609+actor);e[41]=W(0xbcd718+actor);e[42]=U(0xf56b1c);e[43]=U(0xf56814);
    if(address==p[69]&&++mutation_seen==p[70]){
        if(p[71]&1u)B(0xbcd602+actor)=(uint8_t)p[72];
        if(p[71]&2u){B(0xbcd664+actor)=(uint8_t)p[73];B(0xbcd665+actor)=(uint8_t)p[74];}
        if(p[71]&4u){U(0x70e47c)=p[75];U(0x70e480)=p[76];U(0x70e488)=p[77];}
        if(p[71]&8u){U(0x70e458)=p[79];U(0x70e44c)=p[79]+1u;U(0x70e460)=p[80];U(0x70e464)=p[81];}
        if(p[71]&16u){W(0xbcd600+actor)=(uint16_t)p[82];B(0xbcd66f+actor)=(uint8_t)p[89];}
        if(p[71]&32u){U(0x126517c)=p[78];W(0x10d6a0e+p[7]*64u)=(uint16_t)p[86];U(0xf56b1c)=p[85]-5u;U(0xf56814)=p[85];}
        if(p[71]&64u){U(0xbcd778+actor)=p[91];U(0xbcd5fc+actor)=p[91]+1u;}
        if(p[71]&128u){U(0xbcba4c+ship)=p[83];B(0xbcd603+actor)=(uint8_t)p[84];B(0xbcb9ca+ship)=(uint8_t)p[84];W(0xbcba42+ship)=(uint16_t)p[84];}
        if(p[71]&256u){B(0xbcd609+actor)=7;U(0xbca2e0)=4;U(0xe94e00)=0x12345678;U(0x978d78)=1;}
        if(p[71]&512u){U(0x70e3e4)=0x12345678;U(0x70e3e0)=0x87654321;W(0x70e120)=0x80ff;}
    }
}
void FUN_00438e50(int32_t actor,uint32_t x,uint32_t y,uint32_t selection){uint32_t a[4]={(uint32_t)actor,x,y,selection};event(0x438e50,0,4,a);actor_render_actual_renderer(actor,x,y,selection);}
uint8_t FUN_004387d0(int32_t actor){uint32_t a=(uint32_t)actor;event(0x4387d0,0,1,&a);return(uint8_t)(p[45]&1u);}
void FUN_004f0d30(int32_t actor,uint32_t *x,uint32_t *y){uint32_t a[3]={(uint32_t)actor,*x,*y};CHECK(x!=y);event(0x4f0d30,0,3,a);*x=p[46];*y=p[47];}
void FUN_004cce00(uint32_t value,uint8_t separator,const char *suffix,uint32_t x,uint32_t y,uint32_t font,uint32_t color){uint32_t a[7]={value,separator,(uint32_t)(uintptr_t)suffix,x,y,font,color};event(0x4cce00,0,7,a);}
int FUN_0053e590(int image,int x,int y){uint32_t a[3]={(uint32_t)image,(uint32_t)x,(uint32_t)y};event(0x53e590,0xf55bb0,3,a);return actor_render_actual_sprite(image,x,y);}
void FUN_004cdad0(void){event(0x4cdad0,0,0,NULL);actor_render_actual_hclip();}
void FUN_004cdb80(void){event(0x4cdb80,0,0,NULL);actor_render_actual_vclip();}
void actor_render_pixel_empty(uint32_t pointer,uint32_t flags){uint32_t a[2]={pointer,flags};event(0x51f780,0,2,a);}
static int pixel1(uint32_t address,uint32_t pointer){event(address,0,1,&pointer);return 77;}
static int pixel2(uint32_t address,uint32_t pointer,uint32_t color){uint32_t a[2]={pointer,color};event(address,0,2,a);return 77;}
void FUN_0040fb60(const void *p0){pixel1(0x40fb60,(uint32_t)(uintptr_t)p0);}
void FUN_0040ffd0(const void *p0){pixel1(0x40ffd0,(uint32_t)(uintptr_t)p0);}
void FUN_00410090(const void *p0){pixel1(0x410090,(uint32_t)(uintptr_t)p0);}
void FUN_00410230(const void *p0){pixel1(0x410230,(uint32_t)(uintptr_t)p0);}
void FUN_00410b10(const void *p0){pixel1(0x410b10,(uint32_t)(uintptr_t)p0);}
#define PIX1(name,a) int name(uint32_t pointer){return pixel1(a,pointer);}
PIX1(FUN_0040fc40,0x40fc40) PIX1(FUN_0040fcf0,0x40fcf0) PIX1(FUN_0040fdd0,0x40fdd0) PIX1(FUN_0040fed0,0x40fed0) PIX1(FUN_0040ff50,0x40ff50)
PIX1(FUN_004105c0,0x4105c0) PIX1(FUN_00410660,0x410660) PIX1(FUN_00410de0,0x410de0) PIX1(FUN_00410e80,0x410e80) PIX1(FUN_00410f70,0x410f70) PIX1(FUN_00410fb0,0x410fb0) PIX1(FUN_004114a0,0x4114a0) PIX1(FUN_00411fd0,0x411fd0)
#define PIX2(name,a) int name(uint32_t pointer,uint32_t color){return pixel2(a,pointer,color&65535u);}
PIX2(FUN_004100f0,0x4100f0) PIX2(FUN_004101c0,0x4101c0) PIX2(FUN_00410700,0x410700) PIX2(FUN_004109d0,0x4109d0) PIX2(FUN_00410a70,0x410a70)
int FUN_00427a20(uint32_t pointer,uint32_t n){return pixel2(0x427a20,pointer,n);}
int FUN_0042a160(uint32_t pointer,uint32_t n){return pixel2(0x42a160,pointer,n);}
int FUN_0042b440(uint32_t pointer,uint32_t n){return pixel2(0x42b440,pointer,n);}
#define PIX0(name,a) int name(void){event(a,0,0,NULL);return 77;}
PIX0(FUN_0042c740,0x42c740) PIX0(FUN_0042c9a0,0x42c9a0) PIX0(FUN_0042cee0,0x42cee0) PIX0(FUN_0042d810,0x42d810) PIX0(FUN_0042e680,0x42e680)
void FUN_004cdf20(uint32_t pointer,int n){uint32_t a[2]={pointer,(uint32_t)n};event(0x4cdf20,0,2,a);}
void FUN_004cdeb0(uint32_t pointer,int n){uint32_t a[2]={pointer,(uint32_t)n};event(0x4cdeb0,0,2,a);}
/* Loaded-inline fixtures only: excluded filesystem paths fail visibly. */
void FUN_004ce310(char *s,const char *ext){(void)s;(void)ext;CHECK(0);}
int FUN_004ce480(const char *s,uint32_t a,uint32_t n,uint32_t o){(void)s;(void)a;(void)n;(void)o;CHECK(0);return 0;}
int FUN_0053f7d0(const char *s,uint32_t a,uint32_t n,uint32_t r,uint32_t o){(void)s;(void)a;(void)n;(void)r;(void)o;CHECK(0);return 0;}
void FUN_0053f790(uint32_t *pointer,char flag){(void)pointer;(void)flag;CHECK(0);}
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
        mov ecx,[esp+24]
        cmp dword ptr[esp+20],0
        jne scanner
        push dword ptr[ecx+20]
        push dword ptr[ecx+16]
        push dword ptr[ecx+12]
        push dword ptr[ecx+8]
        call FUN_00438e50
        add esp,16
        jmp complete
scanner:
        push dword ptr[ecx+20]
        call FUN_00439740
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
static void region(uint32_t a,uint32_t n)
{
    uint32_t i;DWORD old;for(i=0;i<region_count;++i)if(regions[i][0]==a)return;
    CHECK(region_count<32);regions[region_count][0]=a;regions[region_count][1]=n;
    CHECK(VirtualProtect((void *)(uintptr_t)a,n,PAGE_READWRITE,&old));
    memset((void *)(uintptr_t)a,(int)((p[1]+region_count)&255u),n);++region_count;
}
static void run(const uint32_t *q)
{
    uint32_t ids[3],i,j,a,o,row,col,index,out[6],ship;
    memcpy(p,q,sizeof(p));event_count=mutation_seen=region_count=0;ids[0]=p[2];ids[1]=p[2]+1u;ids[2]=p[2]+257u;
    region(0x5c8500,0x2300);region(0x605b30,32);region(0x6de510,16);region(0x70e118,16);region(0x70e3d0,0x775630-0x70e3d0);
    region(0x78d5e0,32);region(0x978d70,16);region(0xb350dc-8,8*2+16);region(0xbca2d0,48);region(0xd3b520,224);
    region(0xe38aa0,0x150);region(0xe38d90,0x150);region(0xe92e10,24);region(0xe94df8,32);region(0xf5680c,24);region(0xf56b14,24);
    region(0xf55bb0,8000);region(0x10d69b0,4097*64+96);region(0x1265178,16);
    for(i=0;i<3;++i)region(0xbcd5f0+ids[i]*388u,412);
    region(0xbcd5f0+(uint32_t)(int32_t)(int16_t)p[18]*388u,412);
    ship=(uint32_t)(int32_t)(int16_t)p[35]*144u;region(0xbcb9b8+ship,168);
    for(i=0;i<0x2300;++i)B(0x5c8500+i)=(uint8_t)((i*37u+p[1]*13u)%255u);
    for(i=0;i<1000;++i){U(0xf55bb0+i*8u)=0;U(0xf55bb4+i*8u)=100+i*4u;}
    for(i=0;i<4097;++i){a=0x10d69f8+i*64u;memset((void *)(uintptr_t)a,0,64);U(a)=p[88];U(a+4)=0x100;U(a+8)=p[90];U(a+16)=1;W(a+20)=(uint16_t)p[65];W(a+22)=(uint16_t)p[66];W(a+34)=(uint16_t)(i%23u);W(a+36)=(uint16_t)(i%17u);B(a+0x32)=(uint8_t)p[64];B(a+0x33)=(uint8_t)p[67];B(a+0x35)=1;}
    W(0x10d6a1a+p[8]*64u)=(uint16_t)p[29];W(0x10d6a1c+p[8]*64u)=(uint16_t)p[30];
    W(0x10d6a1a+p[7]*64u)=(uint16_t)p[31];W(0x10d6a1c+p[7]*64u)=(uint16_t)p[32];W(0x10d6a0e+p[7]*64u)=(uint16_t)p[33];
    for(i=0;i<3;++i){
        o=ids[i]*388u;a=0xbcd5f8+o;memset((void *)(uintptr_t)a,0,388);
        U(0xbcd5fc+o)=p[8]+i;W(0xbcd600+o)=i<p[60]?(uint16_t)ids[i+1u]:0;B(0xbcd602+o)=(uint8_t)(p[6]+i);B(0xbcd603+o)=(uint8_t)p[44];B(0xbcd604+o)=(uint8_t)p[11];
        B(0xbcd609+o)=(uint8_t)p[9];B(0xbcd60a+o)=(uint8_t)p[10];W(0xbcd60c+o)=(uint16_t)p[41];W(0xbcd60e+o)=(uint16_t)p[42];B(0xbcd614+o)=(uint8_t)p[17];B(0xbcd631+o)=(uint8_t)p[13];B(0xbcd639+o)=(uint8_t)p[12];
        W(0xbcd642+o)=(uint16_t)p[15];W(0xbcd644+o)=(uint16_t)p[16];W(0xbcd652+o)=(uint16_t)p[18];W(0xbcd658+o)=(uint16_t)p[35];B(0xbcd664+o)=(uint8_t)p[21];B(0xbcd665+o)=(uint8_t)p[22];B(0xbcd66f+o)=(uint8_t)p[61];B(0xbcd670+o)=(uint8_t)p[14];W(0xbcd718+o)=(uint16_t)p[23];U(0xbcd778+o)=p[7];B(0xbcd5f9+o)=(uint8_t)p[43];
    }
    o=(uint32_t)(int32_t)(int16_t)p[18]*388u;B(0xbcd602+o)=(uint8_t)p[19];B(0xbcd670+o)=(uint8_t)p[20];
    B(0xbcb9ca+ship)=(uint8_t)p[34];W(0xbcba42+ship)=(uint16_t)p[38];W(0xbcba4a+ship)=(uint16_t)p[36];U(0xbcba4c+ship)=p[37];
    for(i=0;i<231u*457u;++i)U(0x70e494+i*4u)=0x0fff0000;
    for(i=0;i<8;++i)W(0xb350dc+i*2u)=(uint16_t)(i==p[58]?p[59]:0);
    row=p[52]-8u;col=p[53]-4u;
    if(row<457&&col<231){index=row*231u+col;if(p[57])U(0x70e494+index*4u)=p[58];if(p[57]>=2){if(col<230)U(0x70e494+(index+1u)*4u)=p[58];if(row<456)U(0x70e494+(index+231u)*4u)=p[58];}}
    U(0x978d78)=p[24];U(0xbca2e0)=p[25];U(0x126517c)=p[26];U(0x70e45c)=p[27];U(0x70e44c)=p[28];U(0xf56b1c)=p[39];U(0xf56814)=p[40];U(0x78d5ec)=p[48];B(0x605b3d)=(uint8_t)p[49];
    U(0x70e460)=p[50];U(0x70e464)=p[51];U(0x70e474)=p[52];U(0x70e478)=p[53];U(0x70e48c)=p[54];U(0x70e490)=p[55];U(0x70e458)=p[56];U(0xe94e0c)=p[62];B(0x6de515)=(uint8_t)p[63];
    W(0x70e120)=(uint16_t)p[68];U(0xd3b55c)=0;U(0xd3b558)=1024;U(0xd3b554)=0;U(0xd3b550)=768;U(0xe38d9c)=1024;U(0xe38d98)=768;U(0xe38db0)=1024;U(0xe38dac)=768;U(0xe38d94)=2048;U(0xe92e18)=FRAME+0x100;U(0xe92e1c)=FRAME+0x200;B(0xe38dda)=0;
    for(j=0;j<p[87];++j){checked_call(p[0],p,out);CHECK(out[0]==0x11111111&&out[1]==0x22222222&&out[2]==0x33333333&&out[3]==0x44444444&&out[4]==out[5]);}
}
static void digest(char hex[65])
{
    HCRYPTHASH h;DWORD length=32;uint8_t bytes[32];uint32_t i;static const char digits[]="0123456789abcdef";CHECK(CryptCreateHash(provider,CALG_SHA_256,0,0,&h));
    for(i=0;i<region_count;++i){CHECK(CryptHashData(h,(const BYTE *)regions[i],8,0));CHECK(CryptHashData(h,(const BYTE *)(uintptr_t)regions[i][0],regions[i][1],0));}
    CHECK(CryptGetHashParam(h,HP_HASHVAL,bytes,&length,0));CHECK(CryptDestroyHash(h));for(i=0;i<32;++i){hex[i*2]=digits[bytes[i]>>4];hex[i*2+1]=digits[bytes[i]&15];}hex[64]=0;
}
int main(int argc,char **argv)
{
    uint32_t q[FIELDS]={0},i,j;char hex[65];DWORD old;SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOGPFAULTERRORBOX);CHECK(restore_data_open());CHECK(CryptAcquireContextW(&provider,NULL,NULL,PROV_RSA_AES,CRYPT_VERIFYCONTEXT));
    CHECK(VirtualAlloc((void *)PIXELS,0x10000,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE)==(void *)PIXELS);CHECK(VirtualAlloc((void *)FRAME,0x10000,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE)==(void *)FRAME);memset((void *)PIXELS,0x6b,0x10000);memset((void *)FRAME,0xc7,0x10000);CHECK(VirtualProtect((void *)PIXELS,0x10000,PAGE_READONLY,&old));CHECK(VirtualProtect((void *)FRAME,0x10000,PAGE_READONLY,&old));
    if(argc==2&&!strcmp(argv[1],"--oracle-batch")){
        printf("{\"fields\":%u}\n",FIELDS);fflush(stdout);for(;;){for(i=0;i<FIELDS;++i)if(scanf("%u",q+i)!=1)goto done;run(q);digest(hex);printf("{\"memory\":\"%s\",\"events\":[",hex);for(i=0;i<event_count;++i){printf("%s[",i?",":"");for(j=0;j<44;++j)printf("%s%u",j?",":"",events[i][j]);putchar(']');}puts("]}");fflush(stdout);}
    }else{
        q[2]=q[59]=1;q[3]=320;q[4]=144;q[6]=1;q[8]=2000;q[25]=0;q[26]=4096;q[27]=29;q[28]=15;q[39]=2600;q[40]=2700;q[50]=0xfffffff3u;q[51]=0xfffffffau;q[52]=8;q[53]=4;q[54]=144;q[55]=320;q[56]=58;q[57]=1;q[58]=1;q[61]=1;q[64]=1;q[65]=58;q[66]=60;q[87]=1;q[88]=PIXELS+0x100;q[90]=4;run(q);CHECK(event_count>=2&&U(0xe94e00)==0);puts("Actor renderer/scanner: real composition, full art/record state, signed geometry and ordered draws.");
    }
done:CryptReleaseContext(provider,0);restore_data_close();return 0;
}
