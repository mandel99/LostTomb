#include "graphics_53f250.h"
#include "standalone_data.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BASE 0x30000000u
#define PIXELS 0x31000000u
#define FRAME 0x31100000u
#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define W(a) (*(uint16_t *)(uintptr_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(a))
#define PARAMETERS 21
static uint32_t events[8][8],event_count,mutation,checks,scenario_count;
static uint8_t *manager_shadow;
#define CHECK(x) do{++checks;if(!(x)){fprintf(stderr,"53f250 failure at line %d\n",__LINE__);ExitProcess(1);}}while(0)
void graphics_53f250_hclip_impl(void);
void graphics_53f250_vclip_impl(void);
static uint32_t hash(const void *memory,size_t bytes)
{
    const uint8_t *p=memory;uint32_t h=2166136261u;while(bytes--)h=(h^*p++)*16777619u;return h;
}
static void put32(void *p,uint32_t v){memcpy(p,&v,4);}
static void put16(void *p,uint16_t v){memcpy(p,&v,2);}
static void event(uint32_t address,unsigned n,const void *pixels,uint16_t color)
{
    uint32_t *row;CHECK(event_count<8);row=events[event_count++];
    memset(row,0,8*sizeof(*row));row[0]=address;row[1]=n;row[2]=(uint32_t)(uintptr_t)pixels;row[3]=color;
    row[4]=U(0xd3b5e0);row[5]=U(0xd3b5dc);row[6]=W(0xd3b5bc);row[7]=W(0xd3b5be);
}
void FUN_004cdad0(void){event(0x4cdad0,0,NULL,0);graphics_53f250_hclip_impl();}
void FUN_004cdb80(void){event(0x4cdb80,0,NULL,0);graphics_53f250_vclip_impl();}
static uint32_t base(uint32_t address,const void *pixels)
{
    event(address,1,pixels,0);
    if(mutation==1){B(0xd3b531)=2;W(0x70e120)=0x9080;U(0xd3b5b8)+=0x10;}
    if(mutation==2)W(0x70e120)=0;
    if(mutation==3){B(0xd3b531)=5;W(0x70e120)=0x80ff;}
    if(mutation==4){B(0xd3b531)=1;W(0x70e120)=0xff00;U(0xd3b5b8)+=0x20;}
    return 0x9abcdef0u;
}
void FUN_00410230(const void *p){base(0x410230,p);}
uint32_t FUN_004105c0(const void *p){return base(0x4105c0,p);}
uint32_t FUN_00410660(const void *p){return base(0x410660,p);}
void FUN_00410700(const void *p,uint16_t c){event(0x410700,2,p,c);}
void FUN_004109d0(const void *p,uint16_t c){event(0x4109d0,2,p,c);}
void FUN_00410a70(const void *p,uint16_t c){event(0x410a70,2,p,c);}
static RestoreSpriteLibraries *prepare(const uint32_t p[PARAMETERS])
{
    RestoreSpriteLibraries *self=p[19]?RESTORE_SPRITE_LIBRARIES:(RestoreSpriteLibraries *)(uintptr_t)BASE;
    uint8_t *record;memset(self,0xa5,sizeof(*self));self->next_image=p[6];
    record=self->images[0].bytes;memset(record,0,64);put32(record,PIXELS+0x100);put32(record+8,4);
    put16(record+20,118);put16(record+22,70);record[0x35]=(uint8_t)p[20];
    if(p[0]>0&&p[0]<25500) {
        record=self->images[p[0]].bytes;memset(record,0,64);put32(record,p[7]);put32(record+8,p[8]);
        put16(record+20,(uint16_t)p[1]);put16(record+22,(uint16_t)p[2]);record[0x35]=(uint8_t)p[5];
    }
    memset((void *)0xd3b520u,0x5a,224);W(0x70e120)=(uint16_t)p[9];
    U(0xd3b55c)=p[10];U(0xd3b558)=p[11];U(0xd3b554)=p[12];U(0xd3b550)=p[13];
    U(0xe38d9c)=p[14];U(0xe38d94)=p[14]*2;U(0xe92e18)=FRAME+0x100;
    U(0xd3b5e0)=p[15];U(0xd3b5dc)=p[16];W(0xd3b5bc)=(uint16_t)p[17];
    event_count=0;mutation=p[18];memcpy(manager_shadow,self,sizeof(*self));return self;
}
/* Verification boundary only: sentinel registers and an ESP measurement
   check the compiled C ABI independently of the original thiscall ABI. */
static int checked_call(RestoreSpriteLibraries *self,int image,int x,int y,int facade)
{
    uint32_t sp_before,sp_after,bp_before,bp_after,bx_after,si_after,di_after;int result;
    if(facade) {
        __asm {
            mov sp_before,esp
            mov bp_before,ebp
            mov ebx,11223344h
            mov esi,33445566h
            mov edi,44556677h
            push y
            push x
            push image
            call FUN_0053f250
            add esp,12
            mov result,eax
            mov sp_after,esp
            mov bp_after,ebp
            mov bx_after,ebx
            mov si_after,esi
            mov di_after,edi
        }
    } else {
        __asm {
            mov sp_before,esp
            mov bp_before,ebp
            mov ebx,11223344h
            mov esi,33445566h
            mov edi,44556677h
            push y
            push x
            push image
            push self
            call restore_sprite_draw_upper
            add esp,16
            mov result,eax
            mov sp_after,esp
            mov bp_after,ebp
            mov bx_after,ebx
            mov si_after,esi
            mov di_after,edi
        }
    }
    CHECK(sp_before==sp_after&&bp_before==bp_after);
    CHECK(bx_after==0x11223344u&&si_after==0x33445566u&&di_after==0x44556677u);
    return result;
}
static int run(const uint32_t p[PARAMETERS])
{
    RestoreSpriteLibraries *self=prepare(p);int result;++scenario_count;
    result=checked_call(self,(int32_t)p[0],(int32_t)p[3],(int32_t)p[4],p[19]!=0);
    CHECK(memcmp(self,manager_shadow,sizeof(*self))==0);
    CHECK(hash((void *)PIXELS,0x10000)==0x42399dc5u);
    CHECK(hash((void *)FRAME,0x10000)==0x94699dc5u);
    return result;
}
static void hex(const void *p,size_t n){size_t i;putchar('"');for(i=0;i<n;++i)printf("%02x",((const uint8_t *)p)[i]);putchar('"');}
static void oracle(void)
{
    uint32_t p[PARAMETERS];unsigned i,j;int result;
    puts("{\"base\":805306368}");fflush(stdout);
    while(1) {
        for(i=0;i<PARAMETERS;++i)if(scanf("%u",&p[i])!=1)return;
        result=run(p);printf("{\"result\":%d,\"globals\":",result);hex((void *)0xd3b520u,224);
        printf(",\"tint\":%u,\"events\":[",W(0x70e120));
        for(i=0;i<event_count;++i){printf("%s[",i?",":"");for(j=0;j<8;++j)printf("%s%u",j?",":"",events[i][j]);putchar(']');}
        puts("]}");fflush(stdout);
    }
}
int main(int argc,char **argv)
{
    uint32_t p[PARAMETERS]={5,58,45,20,50,1,16,PIXELS+0x100,4,0x1234,0,640,0,480,640,7,8,9,0,0,1};
    DWORD previous;
    CHECK(restore_data_open());CHECK(VirtualAlloc((void *)BASE,0x400000,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE)==(void *)BASE);
    manager_shadow=VirtualAlloc(NULL,sizeof(RestoreSpriteLibraries),MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);CHECK(manager_shadow);
    CHECK(VirtualAlloc((void *)PIXELS,0x10000,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE)==(void *)PIXELS);
    CHECK(VirtualAlloc((void *)FRAME,0x10000,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE)==(void *)FRAME);
    memset((void *)PIXELS,0x6b,0x10000);memset((void *)FRAME,0xc7,0x10000);
    CHECK(VirtualProtect((void *)PIXELS,0x10000,PAGE_READONLY,&previous));
    CHECK(VirtualProtect((void *)FRAME,0x10000,PAGE_READONLY,&previous));
    if(argc==2&&!strcmp(argv[1],"--oracle-batch"))oracle();
    else {
        CHECK(run(p)==1&&event_count==4&&events[0][0]==0x4cdad0&&events[1][0]==0x4cdb80&&events[2][0]==0x410230&&events[3][0]==0x410700);
        CHECK(U(0xd3b5b8)==PIXELS+0x104&&W(0xd3b5bc)==29&&U(0xd3b5e0)==20&&U(0xd3b5dc)==35);
        p[3]=0xfffffffdu;CHECK(run(p)==1&&events[2][0]==0x4105c0&&events[3][0]==0x4109d0);
        p[3]=630;CHECK(run(p)==1&&events[2][0]==0x410660&&events[3][0]==0x410a70);
        p[3]=640;CHECK(run(p)==1&&event_count==2);
        p[0]=0;CHECK(run(p)==0&&event_count==0);
        p[0]=25500;CHECK(run(p)==0&&event_count==0);
        p[0]=509;p[6]=683;p[3]=20;p[19]=1;CHECK(run(p)==1&&U(0xd3b5b8)==PIXELS+0x104);
        p[0]=16;p[6]=16;CHECK(run(p)==1&&W(0xd3b5be)==118);
        p[20]=0;CHECK(run(p)==0&&event_count==0);
        p[0]=5;p[6]=16;p[5]=0;CHECK(run(p)==0&&event_count==0);
        p[5]=1;p[7]=0x80000000u;CHECK(run(p)==0&&U(0xd3b5b8)==0x80000000u);
        p[7]=PIXELS+0x100;p[18]=1;CHECK(run(p)==1&&events[3][0]==0x410a70&&events[3][2]==PIXELS+0x114&&events[3][3]==0x9080);
        printf("53f250: %u checks in %u scenarios; receiver, widths, fallback record, return, endpoint order/args, source/frame guards, stack/saved registers passed. Raster endpoints controlled.\n",checks,scenario_count);
    }
    return 0;
}
