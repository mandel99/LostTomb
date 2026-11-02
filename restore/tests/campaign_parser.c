#include "campaign_parser.h"
#include "standalone_data.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define U(a) (*(uint32_t *)(uintptr_t)(a))
static uint8_t input[65537];static size_t length,position;
static unsigned int open_ok,opens,closes,checks,scenarios,errors[1024],error_count;
#define REQUIRE(x) do{++checks;if(!(x)){fprintf(stderr,"Campaign parser failure at %d\n",__LINE__);ExitProcess(1);}}while(0)
static unsigned int hash(const char *s) {unsigned int h=2166136261u;while(*s)h=(h^(uint8_t)*s++)*16777619u;return h;}
void *FUN_00561a0a(const char *path,const char *mode) {REQUIRE(!strcmp(path,"fixture.txt")&&!strcmp(mode,"rb"));++opens;return open_ok?(void *)0x31000000u:NULL;}
char *FUN_00561e4d(char *out,int size,void *stream)
{
    size_t i=0;REQUIRE(stream==(void *)0x31000000u&&size==200);
    if(position==length)return NULL;
    while(i<(size_t)size-1&&position<length) {out[i++]=(char)input[position++];if(out[i-1]=='\n')break;}out[i]=0;return out;
}
int FUN_00561994(void *stream) {REQUIRE(stream==(void *)0x31000000u);++closes;return 0;}
void FUN_004ce6e0(const char *message,int a,int b) {REQUIRE(!a&&!b&&error_count<1024);errors[error_count++]=hash(message);}
static int parse(const void *data,size_t bytes,unsigned int seed,unsigned int available)
{
    REQUIRE(bytes<=65536);memcpy(input,data,bytes);length=bytes;position=0;opens=closes=error_count=0;open_ok=available;
    memset(RESTORE_CAMPAIGN,(uint8_t)seed,sizeof(*RESTORE_CAMPAIGN));U(0x12663c8)=0;*(char *)0x7027d4u=0;
    ++scenarios;return FUN_0041ae40(RESTORE_CAMPAIGN,"fixture.txt");
}
static int text(const char *s) {return parse(s,strlen(s),0xa5,1);}
static int32_t field(int record,int offset) {int32_t result;memcpy(&result,RESTORE_CAMPAIGN->index.records[record].bytes+offset,4);return result;}
static void hex(const void *memory,size_t count) {size_t i;putchar('"');for(i=0;i<count;++i)printf("%02x",((const uint8_t *)memory)[i]);putchar('"');}
static void oracle(void)
{
    static char encoded[131073];unsigned int seed,available,bytes,i,value;int result;
    puts("{\"object\":7331080,\"bytes\":19076}");fflush(stdout);
    while(scanf("%u %u %u",&seed,&available,&bytes)==3) {
        REQUIRE(bytes<=65536);if(bytes) {REQUIRE(scanf("%131072s",encoded)==1);REQUIRE(strlen(encoded)==bytes*2u);}
        for(i=0;i<bytes;++i) {REQUIRE(sscanf(encoded+i*2,"%2x",&value)==1);input[i]=(uint8_t)value;}
        result=parse(input,bytes,seed,available);
        printf("{\"result\":%d,\"position\":%u,\"opens\":%u,\"closes\":%u,\"object\":",result,(unsigned int)position,opens,closes);hex(RESTORE_CAMPAIGN,sizeof(*RESTORE_CAMPAIGN));
        printf(",\"errors\":[");for(i=0;i<error_count;++i)printf("%s%u",i?",":"",errors[i]);puts("]}");fflush(stdout);
    }
}
static void constructors(void)
{
    unsigned int seed;
    puts("{\"object\":7331080,\"bytes\":19076}");fflush(stdout);
    while(scanf("%u",&seed)==1) {
        memset(RESTORE_CAMPAIGN,(uint8_t)seed,sizeof(*RESTORE_CAMPAIGN));
        printf("{\"result\":%d,\"object\":",FUN_0041ac50(RESTORE_CAMPAIGN)==RESTORE_CAMPAIGN);
        hex(RESTORE_CAMPAIGN,sizeof(*RESTORE_CAMPAIGN));puts("}");fflush(stdout);
    }
}
int main(int argc,char **argv)
{
    const char *valid="[MISSION_NAMES]\r\nNubt (Naqada)\r\nThinis\r\n[PREDYNASTIC_PERIOD]\r\nmission=0,200,0,0\r\nmission=1,201,1,0\r\n";
    REQUIRE(restore_data_open());
    if(argc==2&&!strcmp(argv[1],"--constructor-batch"))constructors();
    else if(argc==2&&!strcmp(argv[1],"--oracle-batch"))oracle();
    else {
        char generated[12000];size_t n;unsigned int i;
        memset(RESTORE_CAMPAIGN,0xa5,sizeof(*RESTORE_CAMPAIGN));
        REQUIRE(FUN_0041ac50(RESTORE_CAMPAIGN)==RESTORE_CAMPAIGN&&RESTORE_CAMPAIGN->index.count==0&&RESTORE_CAMPAIGN->name_count==0);
        REQUIRE(RESTORE_CAMPAIGN->active[64]==0&&RESTORE_CAMPAIGN->state[0]==0xa5&&RESTORE_CAMPAIGN->index.records[99].bytes[0]==255);
        REQUIRE(text(valid)==1&&RESTORE_CAMPAIGN->index.count==2&&RESTORE_CAMPAIGN->name_count==2);
        REQUIRE(!strcmp(RESTORE_CAMPAIGN->names[0],"Nubt (Naqada)")&&field(1,0xc)==1&&field(1,8)==201);
        REQUIRE(text("[mission_names]\n  My City \t\r\n[predynastic_period]\n m i s s i o n = 0 , 2 , 3\n")==1);
        REQUIRE(!strcmp(RESTORE_CAMPAIGN->names[0],"  My City")&&field(0,8)==2&&field(0,0x10)==3);
        REQUIRE(text("[MISSION_NAMES]\nCity\n[PREDYNASTIC_PERIOD]\nmission=0,no,0\n")==0&&error_count==1);
        REQUIRE(text("[MISSION_NAMES]\nCity\n[PREDYNASTIC_PERIOD]\nmission=0,200,0,256\n")==0);
        REQUIRE(text("[MISSION_NAMES]\nCity\n[PREDYNASTIC_PERIOD]\nmission=0,200,0,0\nchoice=1,400,-2,65537\n")==1);
        REQUIRE(field(0,0x38)==1&&field(0,0x20)==400&&field(0,0x24)==-2&&RESTORE_CAMPAIGN->index.records[0].bytes[0x28]==1);
        REQUIRE(text("[MISSION_NAMES]\nCity\n[PREDYNASTIC_PERIOD]\nmission=0,200,0,0\nchoice=1,2,3,4\nchoice=2,3,4,5\n")==0&&error_count==2);
        REQUIRE(text("[MISSION_NAMES]\nCity\n[PREDYNASTIC_PERIOD]\nmission=0,200,0,0,9,3,8,4\n")==1);
        REQUIRE(RESTORE_CAMPAIGN->index.records[0].bytes[7]==2&&RESTORE_CAMPAIGN->index.records[0].bytes[2]==3&&RESTORE_CAMPAIGN->index.records[0].bytes[3]==4);
        REQUIRE(text("[MISSION_NAMES]\nABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwx\n")==0);
        n=(size_t)sprintf(generated,"[MISSION_NAMES]\n");for(i=0;i<101;++i)n+=(size_t)sprintf(generated+n,"City %u\n",i);
        REQUIRE(text(generated)==0&&RESTORE_CAMPAIGN->name_count==100);
        n=(size_t)sprintf(generated,"[MISSION_NAMES]\nCity\n[PREDYNASTIC_PERIOD]\n");for(i=0;i<101;++i)n+=(size_t)sprintf(generated+n,"mission=0,200,0\n");
        REQUIRE(text(generated)==0&&RESTORE_CAMPAIGN->index.count==100);
        REQUIRE(text("; comment\n\n")==1&&RESTORE_CAMPAIGN->index.count==0);
        REQUIRE(parse(valid,strlen(valid),0xa5,0)==0&&opens==1&&closes==0&&RESTORE_CAMPAIGN->index.count==(int32_t)0xa5a5a5a5u);
        printf("Campaign parser: %u checks across %u scenarios; complete records, names, paths, choices, integer widths, capacity, errors and file lifecycle verified.\n",checks,scenarios);
    }
    restore_data_close();return 0;
}
