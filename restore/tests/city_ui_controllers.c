#include "city_ui_controllers.h"
#include "standalone_data.h"
#include "city_ui_fixture.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(a))
static uint32_t events[128][11],event_count,variant,predicate,checks;
#define CHECK(x) do{++checks;if(!(x)){fprintf(stderr,"City UI failure line %d\n",__LINE__);ExitProcess(1);}}while(0)
static uint32_t endpoint(uint32_t address,unsigned n,uint32_t a,uint32_t b,uint32_t c,uint32_t d,uint32_t e,uint32_t f,uint32_t g,uint32_t h,uint32_t i)
{
    uint32_t *row;CHECK(event_count<128);row=events[event_count++];
    row[0]=address;row[1]=n;row[2]=a;row[3]=b;row[4]=c;row[5]=d;row[6]=e;row[7]=f;row[8]=g;row[9]=h;row[10]=i;
    if(address==0x4d62f0||address==0x4d6250||address==0x4d6860||address==0x4cce90||address==0x4cccb0||address==0x4cc770)
        U(0xe20778)=((a&15u)+7u)+(variant==2?512u:0u);
    if(address==0x53e590&&variant==1){U(0xe38db0)=800;U(0xf53f78)=1;}
    return address==0x44bd00?predicate:0x9abcdef0u;
}
#define E0(fn,a) void fn(void){endpoint(a,0,0,0,0,0,0,0,0,0,0);}
#define E2(fn,a) void fn(uint32_t p,uint32_t q){endpoint(a,2,p,q,0,0,0,0,0,0,0);}
#define E4(fn,a) void fn(uint32_t p,uint32_t q,uint32_t r,uint32_t s){endpoint(a,4,p,q,r,s,0,0,0,0,0);}
#define E5(fn,a) void fn(uint32_t p,uint32_t q,uint32_t r,uint32_t s,uint32_t t){endpoint(a,5,p,q,r,s,t,0,0,0,0);}
#define E8(fn,a) void fn(uint32_t p,uint32_t q,uint32_t r,uint32_t s,uint32_t t,uint32_t u,uint32_t v,uint32_t w){endpoint(a,8,p,q,r,s,t,u,v,w,0);}
#define E9(fn,a) void fn(uint32_t p,uint32_t q,uint32_t r,uint32_t s,uint32_t t,uint32_t u,uint32_t v,uint32_t w,uint32_t x){endpoint(a,9,p,q,r,s,t,u,v,w,x);}
E0(FUN_004dddc0,0x4dddc0)
E0(FUN_00425900,0x425900)
E0(FUN_0051f780,0x51f780)
E2(FUN_00425330,0x425330)
E2(FUN_00536670,0x536670)
E4(FUN_00425730,0x425730)
E4(FUN_0043e810,0x43e810)
E5(FUN_004cd510,0x4cd510)
E5(FUN_004cd9d0,0x4cd9d0)
E5(FUN_004cda40,0x4cda40)
E5(FUN_004cc770,0x4cc770)
E8(FUN_004d62f0,0x4d62f0)
E8(FUN_004cce90,0x4cce90)
E9(FUN_004d6250,0x4d6250)
E9(FUN_004d6860,0x4d6860)
void FUN_004cccb0(uint32_t a,uint32_t b,uint32_t c,uint32_t d,uint32_t e,uint32_t f){endpoint(0x4cccb0,6,a,b,c,d,e,f,0,0,0);}
int FUN_0053e590(uint32_t a,uint32_t b,uint32_t c){return endpoint(0x53e590,3,a,b,c,0,0,0,0,0,0);}
int FUN_0044bd00(void){return endpoint(0x44bd00,0,0,0,0,0,0,0,0,0,0);}
static void invoke(uint32_t entry,uint32_t a,uint32_t b)
{
    void (*draw)(void)=NULL;
    uint32_t sp_before,sp_after,bp_before,bp_after,bx_saved,si_saved,di_saved;
    switch(entry){
    case 0x51e990:draw=(void (*)(void))FUN_0051e990;break;
    case 0x51ec90:draw=(void (*)(void))FUN_0051ec90;break;
    case 0x51ed30:draw=(void (*)(void))FUN_0051ed30;break;
    case 0x51edd0:draw=(void (*)(void))FUN_0051edd0;break;
    case 0x51efb0:draw=(void (*)(void))FUN_0051efb0;break;
    case 0x51f0a0:draw=(void (*)(void))FUN_0051f0a0;break;
    case 0x51f680:draw=(void (*)(void))FUN_0051f680;break;
    case 0x533e70:draw=(void (*)(void))FUN_00533e70;break;
    case 0x537100:draw=(void (*)(void))FUN_00537100;break;
    case 0x537120:draw=(void (*)(void))FUN_00537120;break;
    default:CHECK(0);
    }
    /* Only the test boundary uses assembly: measure the compiled C ABI. */
    __asm {
        mov sp_before,esp
        mov bp_before,ebp
        mov ebx,11223344h
        mov esi,33445566h
        mov edi,44556677h
        push b
        push a
        call draw
        add esp,8
        mov sp_after,esp
        mov bp_after,ebp
        mov bx_saved,ebx
        mov si_saved,esi
        mov di_saved,edi
    }
    CHECK(sp_before==sp_after&&bp_before==bp_after);
    CHECK(bx_saved==0x11223344u&&si_saved==0x33445566u&&di_saved==0x44556677u);
}
static void run(uint32_t entry,uint32_t a,uint32_t b){event_count=0;invoke(entry,a,b);}
static void state(void)
{
    unsigned i,j,k;size_t used=0;static char text[0x10000];const char digits[]="0123456789abcdef";
    for(i=0;i<CITY_UI_REGION_COUNT;++i)for(j=0;j<city_ui_regions[i].bytes;++j){unsigned v=B(city_ui_regions[i].address+j);text[used++]=digits[v>>4];text[used++]=digits[v&15];}
    text[used]=0;printf("{\"state\":\"%s",text);
    printf("\",\"events\":[");for(i=0;i<event_count;++i){printf("%s[",i?",":"");for(k=0;k<11;++k)printf("%s%u",k?",":"",events[i][k]);putchar(']');}puts("]}");fflush(stdout);
}
static void oracle(void)
{
    uint32_t entry,a,b;unsigned i,j;char text[0x4401];
    puts("{\"ready\":true}");fflush(stdout);
    while(scanf("%u %u %u %u %u",&entry,&a,&b,&variant,&predicate)==5){
        for(i=0;i<CITY_UI_REGION_COUNT;++i){CHECK(scanf("%17408s",text)==1);CHECK(strlen(text)==city_ui_regions[i].bytes*2);
            for(j=0;j<city_ui_regions[i].bytes;++j){unsigned hi=(unsigned char)text[2*j],lo=(unsigned char)text[2*j+1];hi=hi>='a'?hi-'a'+10:hi-'0';lo=lo>='a'?lo-'a'+10:lo-'0';CHECK(hi<16&&lo<16);B(city_ui_regions[i].address+j)=(uint8_t)((hi<<4)|lo);}}
        run(entry,a,b);state();
    }
}
int main(int argc,char **argv)
{
    unsigned i;CHECK(restore_data_open());
    if(argc>1&&!strcmp(argv[1],"--oracle-batch")){oracle();return 0;}
    for(i=0;i<CITY_UI_REGION_COUNT;++i)memset((void *)(uintptr_t)city_ui_regions[i].address,0,city_ui_regions[i].bytes);
    U(0xe38db0)=1024;U(0xe38dac)=768;U(0xe38d98)=759;U(0xf53f64)=864;U(0xf53f60)=1000;U(0xf567e4)=12000;B(0xe38dda)=0;
    run(0x51e990,0,0);CHECK(events[0][0]==0x53e590&&events[0][2]==12002&&events[0][3]==1000);
    CHECK(events[1][0]==0x4cd9d0&&events[1][3]==759&&events[1][4]==1024&&events[1][5]==9);
    U(0xf53fd0)=0x100;run(0x51efb0,0,0);CHECK(U(0xf53fd0)==0&&event_count==1&&events[0][2]==877);
    U(0x605a3c)=1;run(0x51efb0,1,0);CHECK(event_count==4&&events[0][2]==872&&events[1][4]==1017);
    printf("City UI: %u checks; composed HUD, full-width bottom clear, art IDs, minimap origins/borders, and full dirty flag clear passed. External drawing/text controlled.\n",checks);
    return 0;
}
