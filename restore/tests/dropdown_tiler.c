#include "dropdown_tiler.h"
#include "standalone_data.h"
#include <windows.h>
#include <stdio.h>
#include <string.h>
#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(a))
#define CHECK(q) do{if(!(q)){fprintf(stderr,"Dropdown tiler failure line%d\n",__LINE__);ExitProcess(1);}}while(0)
static uint32_t p[16],events[8192][9],used;
static const uint32_t regions[][2]={{0xd3b5b8,56},{0xf56834,24}};
int FUN_0053e590(int image,int x,int y)
{
    uint32_t *e;CHECK(used<8192);e=events[used++];e[0]=0xf55bb0;e[1]=(uint32_t)image;e[2]=(uint32_t)x;e[3]=(uint32_t)y;e[4]=U(0xd3b5c0);e[5]=U(0xd3b5e0);e[6]=U(0xd3b5dc);e[7]=U(0xf5683c);e[8]=p[8];
    if(p[5]&1)U(0xf5683c)+=p[6];
    if(p[5]&2){U(0xd3b5c0)=p[9];U(0xd3b5e0)=p[10];U(0xd3b5dc)=p[11];}
    if(p[5]&4)U(0xf5683c)=p[7];
    return (int)p[8];
}
static void run(void){used=0;for(uint32_t i=0;i<2;++i)memset((void *)regions[i][0],(uint8_t)(0xa5u^p[12]),regions[i][1]);U(0xf5683c)=p[4];FUN_00536990((int32_t)p[0],(int32_t)p[1],(int32_t)p[2],(int32_t)p[3]);}
static void hex(uint32_t a,uint32_t n){static const char d[]="0123456789abcdef";for(uint32_t i=0;i<n;++i){uint8_t v=B(a+i);putchar(d[v>>4]);putchar(d[v&15]);}}
static void output(void){printf("{\"state\":\"");for(uint32_t i=0;i<2;++i)hex(regions[i][0],regions[i][1]);printf("\",\"events\":[");for(uint32_t i=0;i<used;++i){printf("%s[",i?",":"");for(uint32_t j=0;j<9;++j)printf("%s%u",j?",":"",events[i][j]);putchar(']');}puts("]}");fflush(stdout);}
int main(int argc,char **argv){CHECK(restore_data_open());if(argc==2&&!strcmp(argv[1],"--oracle-batch")){puts("{\"fields\":16}");fflush(stdout);for(;;){for(uint32_t i=0;i<16;++i)if(scanf("%u",p+i)!=1)goto done;run();output();}}else{p[0]=400;p[1]=300;p[2]=11;p[3]=11;p[4]=0x12345678;run();CHECK(used==121&&events[0][1]==0x12345685&&events[120][1]==0x12345685&&events[120][2]==560&&events[120][3]==460);CHECK(U(0xd3b5b8)==0xa5a5a5a5&&U(0xd3b5c4)==0xa5a5a5a5&&U(0xd3b5d8)==0xa5a5a5a5&&U(0xd3b5e4)==0xa5a5a5a5);puts("Dropdown tiler: fullDWORD art/coordinates,10column/10row cycles and neighboring canaries passed.");}done:restore_data_close();return 0;}
