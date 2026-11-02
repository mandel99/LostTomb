#include "preview_footprint_seeds.h"
#include "startup_geometry.h"
#include "standalone_data.h"
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <io.h>
#define FIRST 0x006de500u
#define BYTES 0x1f640u
#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(a))
static const uint32_t objects[8]={0x6fd5d8,0x6de650,0x6fd368,0x6de548,0x6f9320,0x6f5648,0x6de9e0,0x6e9188};
static const uint32_t offsets[8]={0x4c,0x4c,0x4c,0x4c,0x1e5c,0x1cbc,0x4ee0,0x5c88},counts[8]={25,16,9,2,167,158,437,513};
static void (*const geometry[8])(void *)={FUN_0040d7e0,FUN_004013f0,FUN_00416f60,FUN_004e6590,FUN_005456a0,FUN_00558b40,FUN_00555de0,FUN_0054f3d0};
static uint32_t checks;
#define CHECK(x) do{++checks;if(!(x)){fprintf(stderr,"Footprint seed failure line %d\n",__LINE__);ExitProcess(1);}}while(0)
static void seed_abi(void)
{
    uint32_t sp0,sp1,bp0,bp1,bx_saved,si_saved,di_saved;
    __asm {
        mov sp0,esp
        mov bp0,ebp
        mov ebx,11223344h
        mov esi,33445566h
        mov edi,44556677h
        call restore_preview_footprint_seeds_initialize
        mov sp1,esp
        mov bp1,ebp
        mov bx_saved,ebx
        mov si_saved,esi
        mov di_saved,edi
    }
    CHECK(sp0==sp1&&bp0==bp1&&bx_saved==0x11223344&&si_saved==0x33445566&&di_saved==0x44556677);
}
static void prepare(uint32_t seed,uint32_t a,uint32_t b)
{
    uint32_t i;for(i=0;i<BYTES;++i)B(FIRST+i)=(uint8_t)((i*13u+(i>>5)*7u+seed)^0xa7u);
    for(i=0;i<48;++i)B(0x70e440+i)=(uint8_t)((i*31u+seed)^0x69u);
    U(0x70e44c)=a;U(0x70e45c)=b;
}
static void run(uint32_t op,uint32_t seed,uint32_t a,uint32_t b)
{
    unsigned i;prepare(seed,a,b);seed_abi();
    if(op==1||op==3)for(i=0;i<8;++i)geometry[i]((void *)(uintptr_t)objects[i]);
    if(op==2||op==3)seed_abi();
}
int main(int argc,char **argv)
{
    uint32_t p[4],i,j,k,total=0;static uint8_t allowed[BYTES],before[BYTES];
    CHECK(restore_data_open());
    if(argc>1&&!strcmp(argv[1],"--oracle-batch")){
        uint32_t ready=0x53454544;_setmode(_fileno(stdout),_O_BINARY);fwrite(&ready,4,1,stdout);fflush(stdout);
        for(;;){for(i=0;i<4;++i)if(scanf("%u",&p[i])!=1)return 0;run(p[0],p[1],p[2],p[3]);fwrite((void *)FIRST,BYTES,1,stdout);fwrite((void *)0x70e440,48,1,stdout);fflush(stdout);}
    }
    prepare(0x12345678,15,29);memcpy(before,(void *)FIRST,BYTES);seed_abi();
    for(i=0;i<8;++i)for(j=0;j<counts[i];++j){uint32_t offset=objects[i]+offsets[i]+j*52-FIRST;CHECK(offset+52<=BYTES);++total;for(k=0;k<16;++k)allowed[offset+k]=1;}
    CHECK(total==1327);for(i=0;i<BYTES;++i)if(!allowed[i])CHECK(B(FIRST+i)==before[i]);
    CHECK((int32_t)U(0x6fd624)==-3&&(int32_t)U(0x6fd628)==-4&&U(0x6fd62c)==1&&U(0x6fd630)==23);
    for(i=0;i<8;++i)geometry[i]((void *)(uintptr_t)objects[i]);
    CHECK((int32_t)U(0x6fd634)==-915&&U(0x6fd638)==30);
    printf("Preview footprints: %u checks;1327 exact seed tuples, non-seed canaries, all8 geometry methods and ABI passed.\n",checks);return 0;
}
