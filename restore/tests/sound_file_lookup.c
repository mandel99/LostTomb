#include "sound_file_lookup.h"
#include <windows.h>
#include <stdio.h>
#include <string.h>
#define BASE 0x30000000u
#define HEAP 0x31000000u
static unsigned int checks,cases,duplicates,fail_duplicate;
#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define REQUIRE(x) do{++checks;if(!(x)){fprintf(stderr,"Sound file lookup failure at %d\n",__LINE__);ExitProcess(1);}}while(0)
uint32_t FUN_0056b177(const char *text) {++duplicates;if(fail_duplicate)return 0;strcpy((char *)HEAP,text);return HEAP;}
void FUN_00561bdc(void *allocation){(void)allocation;REQUIRE(0);}
static const char *names[]={"new.wav","cached.wav","same.wav","Same.wav",""};
static int run(const uint32_t p[8])
{
    RestoreSoundFileTable *self=(RestoreSoundFileTable *)BASE;unsigned int i;
    memset((void *)BASE,(uint8_t)p[6],0x1000);memset((void *)HEAP,0xcd,0x1000);
    self->capacity=(int32_t)p[0];self->count=(int32_t)p[1];self->entries=p[2]?BASE+0x100:0;
    for(i=0;i<4;++i) {U(BASE+0x100+i*8)=0;U(BASE+0x104+i*8)=0;}
    strcpy((char *)(BASE+0x400),"cached.wav");strcpy((char *)(BASE+0x440),"same.wav");strcpy((char *)(BASE+0x800),names[p[7]]);
    U(BASE+0x100)=p[5]?0:BASE+0x400;U(BASE+0x104)=1;U(BASE+0x108)=BASE+0x440;
    duplicates=0;fail_duplicate=p[4];++cases;
    return FUN_0055dd3f(self,(const char *)(BASE+0x800),(int32_t)p[3]);
}
static void hex(const void *data,size_t n) {size_t i;putchar('"');for(i=0;i<n;++i)printf("%02x",((const uint8_t *)data)[i]);putchar('"');}
int main(int argc,char **argv)
{
    uint32_t p[8]={4,2,1,0,0,0,0xa5,0};unsigned int i;int result;
    REQUIRE(VirtualAlloc((void *)BASE,0x1000,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE)==(void *)BASE);
    REQUIRE(VirtualAlloc((void *)HEAP,0x1000,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE)==(void *)HEAP);
    if(argc==2&&!strcmp(argv[1],"--oracle-batch")) {
        puts("{\"object\":805306368}");fflush(stdout);
        for(;;) {
            for(i=0;i<8;++i)if(scanf("%u",&p[i])!=1)goto done;
            REQUIRE(p[7]<5);result=run(p);printf("{\"result\":%d,\"duplicates\":%u,\"memory\":",result,duplicates);hex((void *)BASE,0x1000);
            printf(",\"heap\":");hex((void *)HEAP,0x1000);puts("}");fflush(stdout);
        }
    } else {
        REQUIRE(run(p)==2&&U(BASE+4)==3&&U(BASE+0x110)==BASE+0x800&&!duplicates);
        p[7]=1;REQUIRE(run(p)==0&&U(BASE+4)==2&&!duplicates);
        p[7]=2;p[3]=1;REQUIRE(run(p)==1&&!duplicates);
        p[7]=3;REQUIRE(run(p)==2&&duplicates==1&&U(BASE+0x110)==HEAP&&!strcmp((char *)HEAP,"Same.wav"));
        p[4]=1;REQUIRE(run(p)==2&&U(BASE+4)==3&&U(BASE+0x110)==0&&U(BASE+0x114)==1);p[4]=0;
        p[1]=4;REQUIRE(run(p)==-1&&!duplicates);p[1]=2;p[2]=0;REQUIRE(run(p)==-1);p[2]=1;
        p[1]=0xffffffffu;REQUIRE(run(p)==-1&&duplicates==1&&U(BASE+4)==0&&U(BASE+0xf8)==HEAP&&U(BASE+0xfc)==1);
        printf("Sound file lookup: %u checks across %u cases; IDs, exact duplicates, raw ownership, capacity, null entries, failed-copy insertion and negative-count addressing verified.\n",checks,cases);
    }
done:
    VirtualFree((void *)BASE,0,MEM_RELEASE);VirtualFree((void *)HEAP,0,MEM_RELEASE);return 0;
}
