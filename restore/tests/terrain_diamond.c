#include "terrain_diamond.h"
#include "standalone_data.h"
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>
#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define W(a) (*(uint16_t *)(uintptr_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(a))
#define SOURCE 0x30000000u
#define FRAME 0x31000000u
#define SOURCE_BYTES 0x20000u
#define FRAME_BYTES 0x100000u
static uint32_t trace[512][7],trace_count,checks;
#define CHECK(c) do{++checks;if(!(c)){fprintf(stderr,"Terrain diamond failure line%d\n",__LINE__);ExitProcess(1);}}while(0)
void terrain_hclip_impl(void);
void terrain_vclip_impl(void);
void terrain_diamond_test_trace(uint32_t entry,const void *pixels,int clip)
{
    uint32_t *row;CHECK(trace_count<512);row=trace[trace_count++];
    row[0]=entry;row[1]=(uint32_t)(uintptr_t)pixels;row[2]=(uint32_t)clip;
    row[3]=U(0xd3b5e0);row[4]=U(0xd3b5dc);row[5]=W(0xd3b5be);row[6]=W(0xd3b5bc);
}
void FUN_004cdad0(void){terrain_diamond_test_trace(0x4cdad0,0,0);terrain_hclip_impl();}
void FUN_004cdb80(void){terrain_diamond_test_trace(0x4cdb80,0,0);terrain_vclip_impl();}
static void prepare(const uint32_t p[12])
{
    uint32_t i;DWORD previous;
    CHECK(VirtualProtect((void *)SOURCE,SOURCE_BYTES,PAGE_READWRITE,&previous));
    for(i=0;i<SOURCE_BYTES;++i)B(SOURCE+i)=(uint8_t)((i*37u+(i>>8)*19u+p[11]*11u)^((i>>4)*13u));
    CHECK(VirtualProtect((void *)SOURCE,SOURCE_BYTES,PAGE_READONLY,&previous));
    memset((void *)FRAME,0xc7,FRAME_BYTES);memset((void *)0xd3b520u,0x5a,224);
    U(0xd3b5e0)=p[2];U(0xd3b5dc)=p[3];U(0xe38d94)=p[4];U(0xe38d9c)=p[4]/2;
    U(0xe92e18)=FRAME;U(0xd3b5b8)=SOURCE+0x100;U(0xe36c80)=p[10];
    U(0xd3b55c)=p[5];U(0xd3b558)=p[6];U(0xd3b554)=p[7];U(0xd3b550)=p[8];
    W(0x70e120)=(uint16_t)p[9];trace_count=0;
}
static uint32_t invoke(const uint32_t p[12])
{
    uint32_t result=0,sp_before,sp_after,bp_before,bp_after,bx_saved,si_saved,di_saved;
    const void *pixels=(const void *)(uintptr_t)(SOURCE+0x100+p[10]);int clip=(int)p[1];
    void (*draw)(const void *,int)=NULL;int (*compose)(void)=NULL;
    switch(p[0]){
    case 0x427a20:draw=FUN_00427a20;break;case 0x428c00:draw=FUN_00428c00;break;
    case 0x42a160:draw=FUN_0042a160;break;case 0x42aa00:draw=FUN_0042aa00;break;
    case 0x42b440:draw=FUN_0042b440;break;case 0x42bcf0:draw=FUN_0042bcf0;break;
    case 0x42c740:compose=FUN_0042c740;break;case 0x42c9a0:compose=FUN_0042c9a0;break;
    case 0x42cee0:compose=FUN_0042cee0;break;case 0x42d810:compose=FUN_0042d810;break;
    case 0x42e680:compose=FUN_0042e680;break;default:CHECK(0);
    }
    if(draw){
        __asm {
            mov sp_before,esp
            mov bp_before,ebp
            mov ebx,11223344h
            mov esi,33445566h
            mov edi,44556677h
            push clip
            push pixels
            call draw
            add esp,8
            mov sp_after,esp
            mov bp_after,ebp
            mov bx_saved,ebx
            mov si_saved,esi
            mov di_saved,edi
        }
    }else{
        __asm {
            mov sp_before,esp
            mov bp_before,ebp
            mov ebx,11223344h
            mov esi,33445566h
            mov edi,44556677h
            call compose
            mov result,eax
            mov sp_after,esp
            mov bp_after,ebp
            mov bx_saved,ebx
            mov si_saved,esi
            mov di_saved,edi
        }
    }
    CHECK(sp_before==sp_after&&bp_before==bp_after);
    CHECK(bx_saved==0x11223344u&&si_saved==0x33445566u&&di_saved==0x44556677u);
    return result;
}
static uint32_t run(const uint32_t p[12]){prepare(p);return invoke(p);}
static void oracle(void)
{
    uint32_t p[12],result;unsigned i;const uint32_t ready=0x54444941;
    _setmode(_fileno(stdout),_O_BINARY);fwrite(&ready,4,1,stdout);fflush(stdout);
    while(1){for(i=0;i<12;++i)if(scanf("%u",&p[i])!=1)return;result=run(p);
        fwrite(&result,4,1,stdout);fwrite(&trace_count,4,1,stdout);fwrite((void *)0xd3b520u,224,1,stdout);
        fwrite((void *)FRAME,FRAME_BYTES,1,stdout);fwrite(trace,sizeof(trace[0]),trace_count,stdout);fflush(stdout);
    }
}
int main(int argc,char **argv)
{
    uint32_t p[12]={0x427a20,0,80,40,1024,0,400,0,240,0,0,17},result;
    CHECK(restore_data_open());CHECK(VirtualAlloc((void *)SOURCE,SOURCE_BYTES,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE)==(void *)SOURCE);
    CHECK(VirtualAlloc((void *)FRAME,FRAME_BYTES,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE)==(void *)FRAME);
    if(argc>1&&!strcmp(argv[1],"--oracle-batch")){oracle();return 0;}
    result=run(p);CHECK(result==0&&trace_count==1);CHECK(W(FRAME+40*1024+108*2)==W(SOURCE+0x100));
    CHECK(B(FRAME+40*1024+107*2)==0xc7&&B(FRAME+39*1024+108*2)==0xc7);
    p[0]=0x42e680;p[9]=0xff00;result=run(p);CHECK(result==58&&trace_count>72);
    p[2]=0xfffff000u;result=run(p);CHECK(result==58&&trace_count==73);
    printf("Terrain diamond: %u checks; packed pixels, guards, full source pointers, tint, six-cell footprint and ABI passed.\n",checks);
    return 0;
}
