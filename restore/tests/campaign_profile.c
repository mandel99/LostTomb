#include "restore_types.h"
#include "campaign_profile.h"
#include "standalone_data.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define BASE 0x30000000u
#define SELF ((RestoreCampaignProfile *)BASE)
#define STREAM ((restore_legacy_FILE *)0x31000000u)
static uint8_t input[0x4aa0],written[0x4a9c];
static uint32_t position,length,available,write_ok,event_count,events[16][3],checks;
#define REQUIRE(x) do{++checks;if(!(x)){fprintf(stderr,"Profile failure at %d\n",__LINE__);ExitProcess(1);}}while(0)
static void event(uint32_t a,uint32_t b,uint32_t c)
{REQUIRE(event_count<16);events[event_count][0]=a;events[event_count][1]=b;events[event_count++][2]=c;}
restore_legacy_FILE *FUN_00561a0a(const char *path,const char *mode)
{REQUIRE(!strcmp(path,"fixture.dat"));event(1,!strcmp(mode,"wb"),available);return available?STREAM:NULL;}
uint32_t FUN_00561ca1(void *out,uint32_t size,uint32_t count,restore_legacy_FILE *stream)
{uint32_t bytes;REQUIRE(stream==STREAM&&count==1&&position<=length);bytes=length-position;if(bytes>size)bytes=size;memcpy(out,input+position,bytes);position+=bytes;event(2,size,bytes);return bytes/size;}
uint32_t FUN_00561f5f(const void *in,uint32_t size,uint32_t count,restore_legacy_FILE *stream)
{REQUIRE(stream==STREAM&&count==1&&size==sizeof(written));memcpy(written,in,size);event(3,size,write_ok);return write_ok;}
int FUN_00561ea4(restore_legacy_FILE *stream,int offset,int origin)
{REQUIRE(stream==STREAM&&!offset&&!origin);position=0;event(4,0,0);return 0;}
uint32_t FUN_00561994(restore_legacy_FILE *stream)
{REQUIRE(stream==STREAM);event(5,0,0);return 0;}
static uint32_t run(const uint32_t p[8])
{
    uint32_t i,value;
    REQUIRE(p[2]<=sizeof(input)&&p[5]<=65);
    memset(SELF,(uint8_t)p[4],sizeof(*SELF));SELF->campaign.name_count=(int32_t)p[3];
    for(i=0;i<sizeof(input);++i)input[i]=(uint8_t)(i*37+p[4]);
    memset(input+0x2c90,'R',65);if(p[5]<65)input[0x2c90+p[5]]=0;
    for(i=0;i<100;++i){value=(i%3)?i+1:0;memcpy(input+0x2d1c+i*0x4c,&value,4);}
    position=0;length=p[2];available=p[1];write_ok=p[6];event_count=0;memset(written,0,sizeof(written));
    return p[0]?FUN_0041b8a0(SELF,"fixture.dat"):FUN_0041b680(SELF,"fixture.dat");
}
static void output(uint32_t result)
{
    uint32_t i;printf("{\"result\":%u,\"position\":%u,\"object\":\"",result,position);
    for(i=0;i<sizeof(*SELF);++i)printf("%02x",((uint8_t *)SELF)[i]);
    printf("\",\"written\":\"");for(i=0;i<sizeof(written);++i)printf("%02x",written[i]);
    printf("\",\"events\":[");for(i=0;i<event_count;++i)printf("%s[%u,%u,%u]",i?",":"",events[i][0],events[i][1],events[i][2]);puts("]}");fflush(stdout);
}
int main(int argc,char **argv)
{
    uint32_t p[8]={0,1,0x4a9c,100,17,7,1,0},i,result;
    REQUIRE(restore_data_open());REQUIRE(VirtualAlloc((void *)BASE,0x10000,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE)==(void *)BASE);
    if(argc==2&&!strcmp(argv[1],"--oracle-batch")) {
        puts("{\"fields\":8}");fflush(stdout);
        for(;;){for(i=0;i<8;++i)if(scanf("%u",p+i)!=1)goto done;result=run(p);output(result);}
    } else {
        REQUIRE(run(p)==1&&SELF->campaign.active[7]==0&&SELF->campaign.index.records[0].bytes[0x3c]==0&&SELF->campaign.index.records[1].bytes[0x3c]==1);
        p[2]=0x4a6c;REQUIRE(run(p)==1&&SELF->suffix[0]==0);for(i=0x4a6c;i<0x4a84;++i)REQUIRE(((uint8_t *)SELF)[i]==0);
        p[2]=0x4a6b;REQUIRE(run(p)==0);p[2]=0x4a9c;p[5]=65;REQUIRE(run(p)==0);
        p[5]=7;p[3]=101;REQUIRE(run(p)==0);p[3]=100;p[1]=0;REQUIRE(run(p)==0&&SELF->suffix[0]==0);
        p[0]=1;p[1]=1;REQUIRE(run(p)==1&&!memcmp(written,SELF,sizeof(*SELF)));p[6]=0;REQUIRE(run(p)==0);
        printf("Campaign profiles: %u checks; full/old/partial reads, explicit receiver, suffix, completion flags, bounds and writes.\n",checks);
    }
done:VirtualFree((void *)BASE,0,MEM_RELEASE);restore_data_close();return 0;
}
