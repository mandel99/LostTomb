#include "family_menu.h"
#include "file_list.h"
#include "host_allocator.h"
#include "standalone_data.h"
#include <stdio.h>
#include <string.h>
#define BASE 0x30000000u
static uint32_t files,name_length,allocation,event_count,events[1024][3],checks,failures;
#define REQUIRE(x) do{++checks;if(!(x)){fprintf(stderr,"Family delete failure at %d\n",__LINE__);ExitProcess(1);}}while(0)
static uint32_t hash(const char *s) {uint32_t v=2166136261u;while(*s)v=(v^(uint8_t)*s++)*16777619u;return v;}
static void event(uint32_t a,uint32_t b,uint32_t c) {REQUIRE(event_count<1024);events[event_count][0]=a;events[event_count][1]=b;events[event_count++][2]=c;}
int restore_file_unlink(const char *path) {event(1,hash(path),0);return failures?-1:0;}
BOOL restore_file_remove_directory(LPCSTR path) {event(2,hash(path),0);return failures?FALSE:TRUE;}
void restore_heap_free(void *pointer) {event(3,(uint32_t)pointer,0);}
uint8_t FUN_0041c740(RestoreFileList *self,const char *pattern,char append)
{
    unsigned int i;char expected[260];REQUIRE(!append&&self->legacy_vtable==0x56f548&&!self->offsets&&!self->strings&&!self->count&&!self->capacity&&!self->string_capacity&&!self->used);
    snprintf(expected,sizeof(expected),"%s\\*.sav",FUN_005169c0());REQUIRE(!strcmp(pattern,expected));event(4,hash(pattern),files);
    self->count=files;
    if(files||allocation) {self->offsets=(uint32_t *)(BASE+0x1000);self->strings=(char *)(BASE+0x3000);}
    for(i=0;i<files;++i) {
        char *name=self->strings+i*256;self->offsets[i]=i*256;memset(name,'X',name_length);sprintf_s(name,256,"Save%03u",i);
        if(name_length>7)memset(name+7,'X',name_length-7);memcpy(name+name_length,".sav",5);
    }
    return 1;
}
void FUN_00516bb0(void) {event(5,0,0);*(uint8_t *)0xe38ddau=47;}
static void run(const uint32_t p[7])
{
    unsigned int i;char *family=(char *)0xe38e74u;
    REQUIRE(p[1]<=100&&p[3]>=7&&p[3]<=220&&p[2]<=31);
    files=p[1];name_length=p[3];allocation=p[4];failures=p[5];event_count=0;memset((void *)BASE,0,0x10000);
    memset(family,0,32);for(i=0;i<p[2];++i)family[i]='A'+i%26;
    memset((void *)0xeadda0u,0xa5,520);*(uint8_t *)0xd3b439u=(uint8_t)p[0];*(uint8_t *)0xe38dcau=0x44;*(uint8_t *)0xe38ddau=63;*(uint32_t *)0x704db0u=p[6];
    FUN_00516dd0();
}
static void output(void)
{
    unsigned int i;printf("{\"mode\":%u,\"dirty\":%u,\"events\":[",*(uint8_t *)0xe38ddau,*(uint8_t *)0xe38dcau);
    for(i=0;i<event_count;++i)printf("%s[%u,%u,%u]",i?",":"",events[i][0],events[i][1],events[i][2]);puts("]}");fflush(stdout);
}
int main(int argc,char **argv)
{
    uint32_t p[7]={1,100,31,100,0,0,1},i;
    REQUIRE(restore_data_open());REQUIRE(VirtualAlloc((void *)BASE,0x10000,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE)==(void *)BASE);
    if(argc==2&&!strcmp(argv[1],"--oracle-batch")) {
        puts("{\"fields\":7}");fflush(stdout);for(;;){for(i=0;i<7;++i)if(scanf("%u",p+i)!=1)goto done;run(p);output();}
    } else {
        run(p);REQUIRE(event_count==106&&events[0][0]==1&&events[1][0]==4&&events[102][0]==2&&events[103][1]==BASE+0x3000&&events[104][1]==BASE+0x1000);
        p[0]=0;run(p);REQUIRE(event_count==1&&events[0][0]==5);
        p[0]=1;p[1]=0;run(p);REQUIRE(event_count==4);
        p[4]=1;run(p);REQUIRE(event_count==6);
        p[4]=0;p[1]=1;p[3]=220;run(p);REQUIRE(event_count==6); /* Oversize native path is skipped safely. */
        printf("Family delete: %u checks; confirmation, .sav enumeration, explicit object lifetime, free order, empty lists and mode flags.\n",checks);
    }
done:VirtualFree((void *)BASE,0,MEM_RELEASE);restore_data_close();return 0;
}
