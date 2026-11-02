#include "startup_geometry.h"
#include "standalone_data.h"
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <io.h>
#define BASE 0x30000000u
#define U(a) (*(uint32_t *)(uintptr_t)(a))
static unsigned int checks,cases;static uint32_t random_state;
#define REQUIRE(x) do{++checks;if(!(x)){fprintf(stderr,"Startup geometry failure at %d\n",__LINE__);ExitProcess(1);}}while(0)
static uint32_t next(void) {random_state=random_state*1664525u+1013904223u;return random_state;}
static void prepare(const uint32_t p[6])
{
    uint32_t *data=(uint32_t *)BASE;unsigned int i,j;random_state=p[3];
    for(i=0;i<0x20000/4;++i)data[i]=next();
    for(i=0;i<100;++i)for(j=0;j<13;++j)U(0x601dd8u+i*52+j*4)=next();
    for(i=0;i<116;++i) {
        for(j=0;j<13;++j)U(0x5e2ad8u+i*52+j*4)=next();
        U(0x5e2ad8u+i*52)=next()%23;U(0x5e2adcu+i*52)=next()%9;U(0x5e2ae0u+i*52)=next()%8;
    }
    U(0x70e44c)=p[4];U(0x70e45c)=p[5];
}
static void run(const uint32_t p[6])
{
    static void (*const methods[])(void *)={FUN_004013f0,FUN_0040d7e0,FUN_00416f60,FUN_004e6590,FUN_005456a0,FUN_00558b40,FUN_00555de0,FUN_0054f3d0};
    prepare(p);++cases;
    if(p[0]==0)FUN_004e5190((RestoreGeometryRecord *)BASE);
    else if(p[0]==1)FUN_004e52b0((RestoreGeometryRecord *)BASE,(int32_t)p[1]);
    else if(p[0]==2) {REQUIRE(p[2]<8);methods[p[2]]((void *)BASE);}
    else if(p[0]==3)FUN_00541d50();
    else if(p[0]==4)FUN_004dee10();
    else REQUIRE(0);
}
static void oracle(void)
{
    uint32_t p[6];unsigned int i;
    REQUIRE(_setmode(_fileno(stdout),_O_BINARY)!=-1);puts("{\"bytes\":142304}");fflush(stdout);
    for(;;) {
        for(i=0;i<6;++i)if(scanf("%u",&p[i])!=1)return;
        run(p);fwrite((void *)BASE,1,0x20000,stdout);fwrite((void *)0x601dd8u,1,5200,stdout);fwrite((void *)0x5e2ad8u,1,6032,stdout);fflush(stdout);
    }
}
int main(int argc,char **argv)
{
    uint32_t p[6]={0,1,0,123,1024,255};RestoreGeometryRecord record={{3,5,7,9}};uint32_t before;
    REQUIRE(restore_data_open());REQUIRE(VirtualAlloc((void *)BASE,0x20000,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE)==(void *)BASE);
    if(argc==2&&!strcmp(argv[1],"--oracle-batch"))oracle();
    else {
        U(0x70e44c)=1024;U(0x70e45c)=255;FUN_004e5190(&record);
        REQUIRE(record.value[4]==1143&&record.value[5]==0xfffffe00u&&record.value[9]==8192&&record.value[12]==0xfffff800u);
        REQUIRE(record.value[2]==7&&record.value[3]==9);
        p[0]=1;p[1]=0;prepare(p);before=U(BASE);FUN_004e52b0((RestoreGeometryRecord *)BASE,0);REQUIRE(U(BASE)==before);
        p[1]=513;run(p);REQUIRE(U(BASE+4*4)==U(BASE)+U(BASE+4)*228u);
        p[0]=2;for(p[2]=0;p[2]<8;++p[2])run(p);
        p[0]=3;run(p);REQUIRE(U(0x601de8u)==U(0x601dd8u)+U(0x601ddcu)*228u);
        p[0]=4;run(p);
        printf("Startup geometry: %u checks, %u scenarios; explicit object receivers, complete tables, word-width scales, wraparound and 52-byte rotation frames.\n",checks,cases);
    }
    VirtualFree((void *)BASE,0,MEM_RELEASE);restore_data_close();return 0;
}
