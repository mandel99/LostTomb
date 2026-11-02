#include "mission_routes.h"
#include "standalone_data.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BASE 0x30000000u
#define ROUTE ((RestoreMissionRoute *)(BASE+0x1000))
#define OLD ((uint8_t *)(BASE+0x10000))
#define HEAP ((uint8_t *)(BASE+0x100000))
#define H(p,o) (*(int16_t *)((uint8_t *)(p)+(o)))
#define D(p,o) (*(int32_t *)((uint8_t *)(p)+(o)))
#define REQUIRE(x) do { if(!(x)) { fprintf(stderr,"Mission routes failure at %d\n",__LINE__);ExitProcess(1); } } while(0)
static uint32_t events[4][2],event_count,requested,fail;
static char hex[4200001];
void *FUN_00561a1d(uint32_t bytes)
{ REQUIRE(bytes<=0x100000&&event_count<4);requested=bytes;events[event_count][0]=0x561a1d;events[event_count++][1]=bytes;return fail?NULL:HEAP; }
void FUN_00561a2b(void *p)
{ REQUIRE(p==OLD&&event_count<4);events[event_count][0]=0x561a2b;events[event_count++][1]=(uint32_t)p; }
/* count,step,shape,seed,capacity,has_buffer,fail,graphics_count,sprite_id,
   matching_cities,x_offset,y_offset */
static void point(const uint32_t p[12],uint32_t i,int16_t *x,int16_t *y)
{
    int32_t a=0,b=0;
    switch(p[2]) {
    case 0:a=i*7+p[3]%5;b=p[3]%11;break;
    case 1:a=p[3]%7;b=i*9;break;
    case 2:a=i*3;b=i*4;break;
    case 3:a=i*3;b=(i&1)?7:0;break;
    case 4:a=3;b=-7;break;
    case 5:a=i?32767:-32768;b=i?1:-1;break;
    case 6:a=-170+(p[3]*17+i*19)%340;b=-170+(p[3]*7+i*31)%340;break;
    case 7:a=i*500;b=i*500;break;
    }
    *x=(int16_t)(a+(int16_t)p[10]);*y=(int16_t)(b+(int16_t)p[11]);
}
static uint32_t run(const uint32_t p[12])
{
    uint32_t i;int16_t first_x,first_y,last_x,last_y;uint8_t *data=ROUTE->bytes;
    REQUIRE(p[0]<=51&&p[1]<=255&&p[2]<8&&p[4]<=100000&&p[5]<2&&p[6]<2&&p[7]<=258&&p[8]<=65535&&p[9]<4);
    memset(data,0xa5,356);memset(OLD,0xa7,0xf0000);memset(HEAP,0xcc,0x100000);
    memset((void *)0x77f748u,0,19600);memset((void *)0x10d6a00u,0,258*64);
    event_count=requested=0;fail=p[6];data[0]=(uint8_t)p[1];data[321]=(uint8_t)p[0];
    D(data,4)=p[5]?(int32_t)OLD:0;D(data,308)=-123;D(data,312)=p[4];H(data,316)=0x2222;H(data,318)=0x3333;
    for(i=0;i<p[0]&&i<50;++i)point(p,i,(int16_t *)(data+8+i*6),(int16_t *)(data+10+i*6));
    *(uint32_t *)0x126517cu=p[7];
    for(i=0;i<258;++i) { H((void *)0x10d6a00u,i*64+12)=i==0?7:(i==1?300:(i==256?4:999));H((void *)0x10d6a00u,i*64+14)=i==0?8:(i==1?300:(i==256?5:999)); }
    if(p[9]) {
        static const int indices[]={1,7,199};
        point(p,0,&first_x,&first_y);point(p,p[0]?p[0]-1:0,&last_x,&last_y);
        for(i=0;i<3;++i) {
            uint8_t *city=(uint8_t *)(0x77f748u+indices[i]*98u);city[0]=1;city[1]=1;H(city,12)=(int16_t)p[8];
            H(city,4)=p[9]==2?first_x-2:(i==2?last_x-1:first_x-1);
            H(city,6)=p[9]==2?first_y-2:(i==2?last_y-1:first_y-1);
            if(p[9]==3)city[0]=i==0?2:1,city[1]=i==2?0:1;
        }
    }
    return FUN_004d4720(ROUTE);
}
static uint32_t append(const uint8_t *data,uint32_t bytes,uint32_t at)
{
    uint32_t i;static const char digits[]="0123456789abcdef";REQUIRE(at+bytes*2<sizeof(hex));
    for(i=0;i<bytes;++i){hex[at++]=digits[data[i]>>4];hex[at++]=digits[data[i]&15];}return at;
}
static void output(const uint32_t p[12],uint32_t result)
{
    uint32_t i,at=append(ROUTE->bytes,356,0);
    if(p[5])at=append(OLD,p[4]*6+32,at);
    if(requested)at=append(HEAP,requested+32,at);hex[at]=0;
    printf("{\"result\":%u,\"events\":[",result);
    for(i=0;i<event_count;++i)printf("%s[%u,%u]",i?",":"",events[i][0],events[i][1]);
    printf("],\"state\":\"%s\"}\n",hex);fflush(stdout);
}
int main(int argc,char **argv)
{
    uint32_t p[12]={2,5,0,0,0,0,0,258,257,1,0,0},i,result;
    SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOGPFAULTERRORBOX|SEM_NOOPENFILEERRORBOX);
    REQUIRE(restore_data_open());REQUIRE(VirtualAlloc((void *)BASE,0x300000,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE)==(void *)BASE);
    if(argc==2&&!strcmp(argv[1],"--oracle-batch")) {
        puts("{\"fields\":12}");fflush(stdout);
        for(;;){for(i=0;i<12;++i)if(scanf("%u",p+i)!=1)goto done;result=run(p);output(p,result);}
    } else {
        REQUIRE(argc==1);REQUIRE(run(p)==1&&D(ROUTE,308)==2&&H(HEAP,0)==0&&H(HEAP,6)==5);
        p[2]=7;p[1]=1;REQUIRE(run(p)==0&&requested==551*6&&HEAP[requested]==0xcc);
        p[6]=1;REQUIRE(run(p)==0);p[0]=51;REQUIRE(run(p)==0&&!event_count);
        puts("Mission routes: explicit receiver, native interpolation, full-width graphics metadata and bounded buffers.");
    }
done:VirtualFree((void *)BASE,0,MEM_RELEASE);restore_data_close();return 0;
}
