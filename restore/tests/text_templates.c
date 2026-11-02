#include "text_templates.h"
#include "standalone_data.h"
#include <windows.h>
#include <stdio.h>
#include <string.h>
#define BASE 0x30000000u
extern void FUN_00448370(const char *,char *,int);
extern void FUN_00448330(int,char *,int);
static const char *inputs[]={"","abc","[foo]","x[foo]y","[foobar]","[]","[fo]","[FOO]","[unknown]","[foo][bar]","[foo","a[bar]b[foo]c","[bar]","text]end","[foo]]","[foo[bar]"};
static uint32_t variant,count,events[32][2],checks;
#define REQUIRE(x) do{++checks;if(!(x)){fprintf(stderr,"Text template failure at %d\n",__LINE__);ExitProcess(1);}}while(0)
const char *restore_text_template_callback(uint32_t address)
{
    REQUIRE(count<32&&(address==BASE+0xf000||address==BASE+0xf010));events[count][0]=address;events[count++][1]=*(uint32_t *)0xd3b18cu;
    return variant==1?NULL:variant==2?"":(const char *)(BASE+(address==BASE+0xf000?0x5000:0x5100));
}
unsigned int FUN_00566682(unsigned int c) {REQUIRE(0);return c;}
static uint32_t run(const uint32_t p[5])
{
    RestoreTextTemplateEntry *entries=(RestoreTextTemplateEntry *)(BASE+0x1000);RestoreMessageCatalog *catalog=(RestoreMessageCatalog *)0x784f20u;
    unsigned int i;uint32_t result=0;char *output=(char *)(BASE+0x3000);
    REQUIRE(p[0]<3&&(p[1]<16||p[0]==2));variant=p[3];count=0;memset((void *)BASE,0,0x10000);memset(output,0xa5,128);
    strcpy((char *)(BASE+0x4000),"foo");strcpy((char *)(BASE+0x4100),"bar");strcpy((char *)(BASE+0x5000),"VALUE");strcpy((char *)(BASE+0x5100),"Other");
    entries[0].id=0;entries[0].key=(char *)(BASE+0x4000);entries[0].callback=BASE+0xf000;
    entries[1].id=1;entries[1].key=(char *)(BASE+0x4100);entries[1].callback=BASE+0xf010;entries[2].id=-1;
    ((RestoreTextTemplateContext *)BASE)->entries=entries;((RestoreTextTemplateContext *)0x784f34u)->entries=entries;
    catalog->entries=(const char **)(BASE+0x2000);catalog->count=16;
    for(i=0;i<16;++i){catalog->entries[i]=(char *)(BASE+0x6000+i*256);strcpy((char *)catalog->entries[i],inputs[i]);}
    *(uint32_t *)0xd3b18cu=p[4];*(uint32_t *)0x126648cu=0;
    if(p[0]==0)result=FUN_004cb980((RestoreTextTemplateContext *)BASE,catalog->entries[p[1]],output,(int)p[2]);
    else if(p[0]==1)FUN_00448370(catalog->entries[p[1]],output,(int)p[2]);else FUN_00448330((int)p[1],output,(int)p[2]);
    return result;
}
static void output(uint32_t result)
{
    uint32_t i;printf("{\"result\":%u,\"context\":%u,\"bytes\":\"",result,*(uint32_t *)0xd3b18cu);
    for(i=0;i<128;++i)printf("%02x",*(uint8_t *)(BASE+0x3000+i));
    printf("\",\"events\":[");for(i=0;i<count;++i)printf("%s[%u,%u]",i?",":"",events[i][0],events[i][1]);puts("]}");fflush(stdout);
}
int main(int argc,char **argv)
{
    uint32_t p[5]={0,3,128,0,0x12345678},i,result;
    REQUIRE(restore_data_open());REQUIRE(VirtualAlloc((void *)BASE,0x10000,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE)==(void *)BASE);
    if(argc==2&&!strcmp(argv[1],"--oracle-batch")) {
        puts("{\"fields\":5}");fflush(stdout);for(;;){for(i=0;i<5;++i)if(scanf("%u",p+i)!=1)goto done;result=run(p);output(result);}
    } else {
        REQUIRE(run(p)==1&&!strcmp((char *)(BASE+0x3000),"xVALUEy")&&*(uint32_t *)0xd3b18cu==p[4]);
        p[1]=8;REQUIRE(run(p)==0);p[1]=10;REQUIRE(run(p)==0);p[1]=3;p[2]=3;REQUIRE(run(p)==0&&*(uint8_t *)(BASE+0x3003)==0xa5);
        p[2]=128;p[0]=1;run(p);REQUIRE(!strcmp((char *)(BASE+0x3000),"xVALUEy"));
        p[0]=2;run(p);REQUIRE(!strcmp((char *)(BASE+0x3000),"xVALUEy"));
        printf("Text templates: %u checks; explicit context, local wrappers, prefix/case matching, bounds and callback context restoration.\n",checks);
    }
done:VirtualFree((void *)BASE,0,MEM_RELEASE);restore_data_close();return 0;
}
