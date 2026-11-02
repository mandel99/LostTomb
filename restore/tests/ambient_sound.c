#include "ambient_sound.h"
#include "sound_initialization.h"
#include "standalone_data.h"
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>
#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define W(a) (*(uint16_t *)(uintptr_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(a))
#define GRID 0x0070e494u
#define GRID_COUNT (231u*128u)
#define PARAMS 30
static uint32_t p[PARAMS],events[16][10],event_count,checks;
static const uint32_t regions[][2]={{0x70e3f0,0xb0},{0x604940,32},{0xe92d98,40},{0xf53f50,48},{0xe38d88,0x150},{0xf55b80,32}};
static unsigned char grid_before[GRID_COUNT*4],tables_before[0xec0],record_before[280],terrain_before[24],building_before[16];
static uintptr_t record,terrain,building;
#define CHECK(x) do{++checks;if(!(x)){fprintf(stderr,"Ambient failure line %d\n",__LINE__);ExitProcess(1);}}while(0)
uint32_t ambient_rng_impl(void);
int32_t ambient_coord_impl(int32_t,int32_t,int32_t);
static uint32_t *event(unsigned tag){uint32_t *e;CHECK(event_count<16);e=events[event_count++];memset(e,0,40);e[0]=tag;return e;}
uint32_t FUN_00562348(void)
{
    uint32_t *e=event(1);e[1]=U(0x604950);e[2]=ambient_rng_impl();e[3]=U(0x604950);return e[2];
}
int32_t FUN_00430960(int32_t x,int32_t y,int32_t unused)
{
    uint32_t *e=event(2);e[1]=(uint32_t)x;e[2]=(uint32_t)y;e[3]=(uint32_t)unused;
    e[4]=(uint32_t)ambient_coord_impl(x,y,unused);return (int32_t)e[4];
}
int FUN_0055f0e8(RestoreSound *self,const int32_t *request)
{
    uint32_t *e=event(3);unsigned i;e[1]=(uint32_t)(uintptr_t)self;e[2]=(uint32_t)(uintptr_t)request;
    for(i=0;i<5;++i)e[3+i]=(uint32_t)request[i];
    if(p[15]&1){U(0xe92da8)=0x10203040;U(0xf53f60)^=0x12340000;U(0x604950)^=0xc001d00d;}
    return (int)p[15];
}
static void prepare(void)
{
    unsigned i,j;uint32_t kind=p[10]&65535u;
    event_count=0;memset(events,0,sizeof(events));
    for(i=0;i<sizeof(regions)/sizeof(regions[0]);++i)memset((void *)(uintptr_t)regions[i][0],0xa5,regions[i][1]);
    for(i=0;i<GRID_COUNT;++i)U(GRID+i*4)=p[0]==1&&!(p[15]&2)?(i*37u+p[1])%60000u:p[18];
    U(0x70e490)=p[19];U(0x70e48c)=p[20];U(0x70e460)=p[21];U(0x70e464)=p[22];
    U(0x70e474)=p[23];U(0x70e478)=p[24];U(0x70e44c)=p[26];U(0x70e458)=p[27];U(0x70e45c)=p[28];
    U(0x604950)=p[1];U(0xf55b88)=p[2];B(0xe38ec0)=(uint8_t)p[3];U(0xf53f78)=p[4];
    U(0xf53f60)=p[5];U(0xf53f64)=p[6];U(0xe38d98)=p[7];U(0xe38ed0)=p[14];
    /* Tile zero is also populated: native coordinate rejection returns zero. */
    memset((void *)0xaf5988,0x5a,16);memset((void *)0xb4e6f8,0x5a,12);
    U(0xaf598c)=p[8];W(0xb4e6fc)=(uint16_t)p[9];
    terrain=0xaf598cu+(p[18]<65536?p[18]:0)*4u;
    building=0xb4e6fcu+(p[18]<65536?p[18]:0)*2u;
    if(terrain!=0xaf598c){memset((void *)(terrain-4),0x5a,24);U(terrain)=p[8];}
    if(building!=0xb4e6fc){memset((void *)(building-4),0x5a,16);W(building)=(uint16_t)p[9];}
    record=0x7999e8u+(uint32_t)(int32_t)(int16_t)p[9]*264u;
    memset((void *)(record-8),0x69,280);W(record)=(uint16_t)p[10];W(record+2)=(uint16_t)p[11];
    memset((void *)0xf54540,0x57,0xec0);
    for(i=0;i<0xec0/4;++i)U(0xf54540+i*4)=(int32_t)p[13]<0?p[13]:p[13]+i*17u;
    for(i=0;i<6;++i)U(0xf5457c+i*48)=p[12];
    if(kind!=57)U(0xf54680u+(uint32_t)(int32_t)(int16_t)kind*20u)=p[12];
    U(0xf549a0)=p[12];U(0xf549f0)=p[12];U(0xf54a04)=p[12];
    memcpy(grid_before,(void *)GRID,sizeof(grid_before));memcpy(tables_before,(void *)0xf54540,sizeof(tables_before));
    memcpy(record_before,(void *)(record-8),280);memcpy(terrain_before,(void *)(terrain-4),24);memcpy(building_before,(void *)(building-4),16);
    /* Keep compiler integer-only; these sentinels verify both x87 values pop. */
    j=p[25];
    __asm {
        fninit
        fldcw word ptr j
        fld1
    }
}
static uint32_t invoke(void)
{
    uint32_t result=0,sp0,sp1,bp0,bp1,bx_saved,si_saved,di_saved,op=p[0],x=p[16],y=p[17],unused=p[29];
    __asm{
        mov sp0,esp
        mov bp0,ebp
        mov ebx,11223344h
        mov esi,33445566h
        mov edi,44556677h
        cmp op,0
        jne direct
        call FUN_0053acf0
        jmp done
    direct:
        cmp op,1
        jne position
        push unused
        push y
        push x
        call FUN_00430960
        add esp,12
        mov result,eax
        jmp done
    position:
        cmp op,2
        jne rng
        push y
        push x
        call restore_ambient_position
        add esp,8
        mov result,eax
        jmp done
    rng:
        call FUN_00562348
        mov result,eax
    done:
        mov sp1,esp
        mov bp1,ebp
        mov bx_saved,ebx
        mov si_saved,esi
        mov di_saved,edi
    }
    CHECK(sp0==sp1&&bp0==bp1&&bx_saved==0x11223344&&si_saved==0x33445566&&di_saved==0x44556677);
    return result;
}
static void output(uint32_t result)
{
    unsigned i;unsigned short cw,sw;unsigned char sentinel[10];
    __asm {
        fstcw cw
        fnstsw sw
        fstp tbyte ptr sentinel
    }
    CHECK(!memcmp(grid_before,(void *)GRID,sizeof(grid_before))&&!memcmp(tables_before,(void *)0xf54540,sizeof(tables_before)));
    CHECK(!memcmp(record_before,(void *)(record-8),280)&&!memcmp(terrain_before,(void *)(terrain-4),24)&&!memcmp(building_before,(void *)(building-4),16));
    U(0x33000000)=cw;U(0x33000004)=sw&((p[0]==2&&!p[17])?0x381e:0x381f);
    fwrite(&result,4,1,stdout);fwrite((void *)0x33000000,8,1,stdout);fwrite(sentinel,10,1,stdout);
    fwrite(&event_count,4,1,stdout);fwrite(events,40,event_count,stdout);
    for(i=0;i<sizeof(regions)/sizeof(regions[0]);++i)fwrite((void *)(uintptr_t)regions[i][0],regions[i][1],1,stdout);
    fflush(stdout);
}
int main(int argc,char **argv)
{
    unsigned i;uint32_t ready=0x414d4249;
    CHECK(restore_data_open());CHECK(VirtualAlloc((void *)0x33000000,4096,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE)==(void *)0x33000000);
    if(argc>1&&!strcmp(argv[1],"--oracle-batch")){
        _setmode(_fileno(stdout),_O_BINARY);fwrite(&ready,4,1,stdout);fflush(stdout);
        while(1){for(i=0;i<PARAMS;++i)if(scanf("%u",&p[i])!=1)return 0;prepare();output(invoke());}
    }
    {uint32_t defaults[PARAMS]={0,1234,0x10000,128,0,640,800,600,0x10000,0,0,0,4,500,0x12345678,0,45,600,500,0,0,40,14,1,1,0x37f,15,58,29,0xabcdef01};memcpy(p,defaults,sizeof(p));}
    prepare();invoke();CHECK(event_count==5&&events[4][0]==3&&events[4][3]==0xffffffff&&events[4][6]<128);
    p[0]=2;p[16]=639;p[17]=640;prepare();CHECK(invoke()==127);
    p[16]=(uint32_t)-639;prepare();CHECK(invoke()==(uint32_t)-127);
    p[0]=0;p[2]=0;prepare();invoke();CHECK(event_count==0);
    printf("Ambient: %u checks; actual RNG, full-width geometry, x87 position and ordered sound descriptor passed.\n",checks);
    return 0;
}
