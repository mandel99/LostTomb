#include "family_menu.h"
#include "campaign_index.h"
#include "sound_streams.h"
#include "standalone_data.h"
#include <windows.h>
#include <stdio.h>
#include <string.h>
#define BASE 0x30000000u
#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(a))
static uint32_t count,events[32][12],checks,answer,files;
#define REQUIRE(x) do{++checks;if(!(x)){fprintf(stderr,"Family options failure at %d\n",__LINE__);ExitProcess(1);}}while(0)
static void event(uint32_t a,uint32_t n,const uint32_t *v)
{
    REQUIRE(count<32&&n<=5);memset(events[count],0,sizeof(events[count]));events[count][0]=a;events[count][1]=n;
    if(n)memcpy(events[count]+2,v,n*4);
    events[count][7]=U(0x605a3c);events[count][8]=B(0x6de515);events[count][9]=U(0x704dac);events[count][10]=U(0x704db0);events[count][11]=B(0xe38dda);++count;
}
void FUN_0055f561(RestoreSound *self,int channel) {uint32_t v[2]={(uint32_t)self,channel};event(0x55f561,2,v);}
void FUN_004d5100(const char *path) {uint32_t v[1]={(uint32_t)path};REQUIRE(path==(const char *)0x700998u);event(0x4d5100,1,v);B(0xe38dda)=0x55;B(0xe38dca)=0x77;}
void FUN_0041f870(void) {event(0x41f870,0,NULL);B(0xe38dda)=61;B(0xe38dca)=0x77;}
void FUN_00402450(void) {event(0x402450,0,NULL);B(0xe38dda)=10;U(0xd3b1bc)=0x12345678;}
void FUN_004cc110(int g,int i) {uint32_t v[2]={g,i};REQUIRE(g==5&&i==141);event(0x4cc110,2,v);U(0xe20768)=BASE+0x1000;}
void FUN_004cc160(int g,int i) {uint32_t v[2]={g,i};REQUIRE(g==5&&i==142);event(0x4cc160,2,v);U(0xe20764)=BASE+0x2000;}
int FUN_00538cd0(const char *title,const char *message,uint32_t flags)
{uint32_t v[3]={(uint32_t)title,(uint32_t)message,flags};event(0x538cd0,3,v);return (int)answer;}
void FUN_00516b50(const char *pattern) {uint32_t v[1]={(uint32_t)pattern};REQUIRE(!strcmp(pattern,"Save\\*.dat"));event(0x516b50,1,v);U(0xeadfb0)=files;}
void FUN_00516ab0(void) {event(0x516ab0,0,NULL);if(files)strcpy((char *)0xe38e74u,"Restore");B(0xe38dca)=1;}
void FUN_004cc500(int index) {uint32_t v[1]={index};event(0x4cc500,1,v);}
void FUN_004cc3c0(void) {event(0x4cc3c0,0,NULL);}
void FUN_004cc3e0(void) {event(0x4cc3e0,0,NULL);}
static const uint32_t regions[][2]={{0x605a3c,4},{0x6de515,1},{0x7027ac,8},{0x6fdc98,4},{0x704dac,8},{0xe38ebc,4},{0xe38dda,1},{0xe38dca,1},{0xd3b1bc,4},{0xe20764,8},{0xeadfb0,4},{0xeadfc8,8},{0xd3b198,4},{0xd3b1b3,1},{0xe20998,4},{0xe94dc0,4},{0xe38e74,32}};
static uint32_t run(const uint32_t p[10])
{
    uint32_t i,result=0;REQUIRE(p[0]<6&&((int32_t)p[2]<=100));count=0;answer=p[4];files=p[8];
    memset(RESTORE_CAMPAIGN_INDEX,0,sizeof(*RESTORE_CAMPAIGN_INDEX));RESTORE_CAMPAIGN_INDEX->count=(int32_t)p[2];
    if(p[3]<p[2]&&p[3]<100)RESTORE_CAMPAIGN_INDEX->records[p[3]].bytes[0x3c]=0x80;
    for(i=0;i<sizeof(regions)/sizeof(regions[0]);++i)memset((void *)(uintptr_t)regions[i][0],0xa5,regions[i][1]);
    B(0xd3b188)=(p[1]&1)!=0;B(0xd3b189)=(p[1]&2)!=0;B(0xd3b18a)=(p[1]&4)!=0;B(0xd3b18b)=(p[1]&8)!=0;
    U(0x605a3c)=p[7];U(0x7027ac)=p[5];U(0x7027b0)=p[6];U(0xeadfcc)=p[9];B(0xe38dda)=48;B(0xe38dca)=0x44;
    switch(p[0]) {case 0:FUN_004cb800();break;case 1:FUN_004cb860();break;case 2:FUN_004cb910();break;case 3:FUN_004cb940();break;case 4:FUN_004cb970();break;case 5:result=FUN_0041ba80(RESTORE_CAMPAIGN_INDEX);break;}
    return result;
}
static void output(uint32_t result)
{
    uint32_t i,j;printf("{\"result\":%u,\"state\":\"",result);
    for(i=0;i<sizeof(regions)/sizeof(regions[0]);++i)for(j=0;j<regions[i][1];++j)printf("%02x",B(regions[i][0]+j));
    printf("\",\"events\":[");for(i=0;i<count;++i){printf("%s[",i?",":"");for(j=0;j<12;++j)printf("%s%u",j?",":"",events[i][j]);putchar(']');}puts("]}");fflush(stdout);
}
int main(int argc,char **argv)
{
    uint32_t p[10]={0,0,100,99,7,258,260,0x12345678,0,0},i,result;
    REQUIRE(restore_data_open());
    if(argc==2&&!strcmp(argv[1],"--oracle-batch")) {
        puts("{\"fields\":10}");fflush(stdout);for(;;){for(i=0;i<10;++i)if(scanf("%u",p+i)!=1)goto done;result=run(p);output(result);}
    } else {
        run(p);REQUIRE(B(0xe38dda)==23&&U(0x7027ac)==0&&U(0x7027b0)==0&&U(0x6fdc98)==0);
        p[1]=8;run(p);REQUIRE(B(0xe38dda)==1&&U(0x605a3c)==0&&B(0x6de515)==0);
        p[0]=1;p[1]=1;run(p);REQUIRE(B(0xe38dda)==61&&U(0x704dac)==2&&U(0x704db0)==1);
        p[3]=0xffffffff;run(p);REQUIRE(B(0xe38dda)==48);p[4]=6;run(p);REQUIRE(B(0xe38dda)==61);
        p[0]=2;p[1]=4;run(p);REQUIRE(B(0xe38dda)==10&&U(0xd3b1bc)==48);
        p[0]=3;run(p);REQUIRE(U(0xe38ebc)==2&&U(0x704dac)==3&&U(0x704db0)==0);
        p[0]=4;run(p);REQUIRE(B(0xe38dda)==54&&U(0xe20998)==0);
        printf("Family options: %u checks; all five actions, explicit completion predicate, prompt choices, nested-call order and full metadata.\n",checks);
    }
done:restore_data_close();return 0;
}
