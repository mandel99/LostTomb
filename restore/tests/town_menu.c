#include "town_menu.h"
#include "standalone_data.h"
#include <windows.h>
#include <stdio.h>
#include <string.h>
#define ALT ((RestoreTownList *)0x30001000u)
#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(a))
#define REQUIRE(x) do{if(!(x)){fprintf(stderr,"Town menu failure at %d\n",__LINE__);ExitProcess(1);}}while(0)
static void setup(RestoreTownList *list,uint32_t count,uint32_t seed)
{
    uint32_t i,j;memset(list,0xa5,sizeof(*list));list->count=count;
    for(i=0;i<150;++i) {
        uint8_t *r=list->records[i].bytes;static const uint8_t kinds[]={1,2,23,0};
        *(int16_t *)r=(int16_t)(i+((seed&64)!=0));r[2]=kinds[(i+seed)%4];r[0x28]=1;
        if(count&&i==seed%count)r[0x28]|=16;
        r[0x60]=seed%4;
        for(j=0;j<4;++j)*(int16_t *)(r+(unsigned[]){0x24,0x26,0x52,0x54}[j])=-1;
        if(count&&i<count)*(int16_t *)(r+(unsigned[]){0x24,0x26,0x52,0x54}[(i+seed)%4])=(seed&8)?i:(i+1)%count;
        if((seed&16)&&count>0&&count<=6&&i<count)*(int16_t *)(r+0x24)=(i+2)%count;
    }
}
static uint32_t run(const uint32_t p[6])
{
    int16_t *previous=(int16_t *)0x30000000u;REQUIRE(p[0]<4&&p[1]<=150);
    setup(RESTORE_TOWNS,p[1],p[2]);setup(ALT,p[1],p[2]+1);*previous=(int16_t)p[4];U(0x5d9a40)=258;B(0x78a634)=0x44;
    switch(p[0]) {
    case 0:return FUN_0041e3b0();
    case 1:if(p[1])FUN_0041e270(&RESTORE_TOWNS->records[p[2]%p[1]],(int)p[3]);return 0;
    case 2:return (uint32_t)FUN_0044bb90(p[4]==0xffffffffu?0:previous,(char)p[5]);
    default:return (uint32_t)restore_town_next(ALT,p[4]==0xffffffffu?0:previous,(uint8_t)p[5]);
    }
}
static void output(uint32_t result)
{
    static char hex[sizeof(RestoreTownList)*4+1];static const char digits[]="0123456789abcdef";uint32_t i,j=0;
    const uint8_t *lists[2]={(const uint8_t *)RESTORE_TOWNS,(const uint8_t *)ALT};
    for(i=0;i<sizeof(RestoreTownList)*2;++i){uint8_t value=lists[i/sizeof(RestoreTownList)][i%sizeof(RestoreTownList)];hex[j++]=digits[value>>4];hex[j++]=digits[value&15];}hex[j]=0;
    printf("{\"result\":%u,\"last\":%u,\"flag\":%u,\"state\":\"%s\"}\n",result,U(0x5d9a40),B(0x78a634),hex);fflush(stdout);
}
int main(int argc,char **argv)
{
    uint32_t p[6]={0,0,0,1,0xffffffffu,0},i,result;REQUIRE(restore_data_open());REQUIRE(VirtualAlloc((void *)0x30000000u,0x10000,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE)==(void *)0x30000000u);
    if(argc==2&&!strcmp(argv[1],"--oracle-batch")){puts("{\"fields\":6}");fflush(stdout);for(;;){for(i=0;i<6;++i)if(scanf("%u",p+i)!=1)goto done;result=run(p);output(result);}}
    else{REQUIRE(run(p)==0&&U(0x5d9a40)==257&&B(0x78a634)==1);p[1]=140;p[2]=24;run(p);puts("Town menu: native iteration, complete 124-byte record traversal, bounded recursion, cycles and explicit receiver.");}
done:VirtualFree((void *)0x30000000u,0,MEM_RELEASE);restore_data_close();return 0;
}
