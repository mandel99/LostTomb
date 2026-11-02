#include "mission_loaders.h"
#include "mission_routes.h"
#include "chunk_io.h"
#include "mission_schema.h"
/* Synthetic native-loader fixture: C-only save recovery is checked separately. */
void restore_mission_schema_recover_loaded(void){}
#include "standalone_data.h"
#include <windows.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#define BASE 0x30000000u
#define ALT (BASE+0x10000u)
#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define H(a) (*(int16_t *)(uintptr_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(a))
#define REQUIRE(x) do { if(!(x)) {fprintf(stderr,"Mission loaders failure at %d\n",__LINE__);ExitProcess(1);} } while(0)
/* op,version,failed_chunk,signature,raw_codec,route_variant,building_flags,
   player_index,IO_failure_bits,archive_offset,header_only,editor,mission_id */
static uint32_t config[13],events[512][6],event_count;
static const uint32_t regions[][2]={{0x776fb0,16232},{0x7848d8,1624},{0x78d790,40},{0xe945bc,12},
    {0x978d9c,207936},{0xd38360,896},{0xea184c,0xbd00},{0xe39080,4},{0xe20760,16},{0x7027b0,8},
    {0x5e45b0,240},{0x5e46a0,432},{0x5c1140,528},{0x5c1350,312},{ALT,64},{ALT+0x32c40,896}};
static char state[800001];
static void note(uint32_t address,uint32_t a,uint32_t b,uint32_t c)
{ REQUIRE(event_count<512);events[event_count][0]=address;events[event_count][1]=a;events[event_count][2]=b;events[event_count][3]=c;events[event_count][4]=U(0xe945c0);events[event_count++][5]=U(0xd38360); }
static void mutate_tables(uint32_t address)
{
    static const uint32_t tables[]={0x5e45b0,0x5e46a0,0x5c1140,0x5c1350};uint32_t i;
    for(i=0;i<4;++i){U(tables[i])=0xfeed0000u+address;U(tables[i]+4)=0xbeef0000u+address;}
}
static void helper(uint32_t address){note(address,0,0,0);mutate_tables(address);}
#define MIG(id) void FUN_##id(void){helper(0x##id##u);}
MIG(004206a0) MIG(004209e0) MIG(00420d10) MIG(00421010) MIG(004212f0) MIG(004bd5d0)
MIG(004d17e0) MIG(004d1840) MIG(004d1860) MIG(004d1880) MIG(004d18b0) MIG(004d18f0)
MIG(004d19e0) MIG(004d1a70) MIG(004d1ad0) MIG(004d2fa0) MIG(004d3200) MIG(004d32a0)
MIG(004d32d0) MIG(004d32f0) MIG(004d3c20) MIG(004d3c40) MIG(004d3cc0) MIG(004d3d00)
MIG(004d3d80) MIG(004d3da0) MIG(004d3f40) MIG(004d3f60) MIG(004d4040) MIG(004d4080)
MIG(004d40a0) MIG(004d40c0) MIG(004d4180) MIG(004de140)
MIG(004bc720) MIG(004d52a0) MIG(004f4520) MIG(0053d970)
void FUN_004d0a40(uint8_t value){note(0x4d0a40,value,0,0);mutate_tables(0x4d0a40);}
void FUN_004ce2d0(void){note(0x4ce2d0,0,0,0);}
void FUN_00561a2b(void *p){note(0x561a2b,(uint32_t)p,0,0);}
static void routes(void)
{
    uint32_t i;memset((void *)0x776fb0u,0,16200);
    for(i=0;i<8;++i) {
        uint32_t r=0x776fb0u+i*324;B(r)=5;B(r+321)=2;B(r+320)=1;
        if(i==0)B(r+321)=0x55,U(r+308)=0x12345678;
        if(i==2)U(r+4)=BASE+0x9000;
        if(i==3)U(r+312)=1;
        if(i==4)B(r+321)=51;
        if(i==5)B(r+320)=3;
        if(i==6)B(r)=config[5]&2?51:0;
        if(i==7)B(r+321)=0;
    }
}
uint8_t FUN_004d0e10(int index,RestoreChunkDescriptor *schema,int fd)
{
    int map=(uintptr_t)schema==0x5e17d8u;REQUIRE(map||(uintptr_t)schema==0x5e09a0u);REQUIRE(fd==37);
    note(0x4d0e10,index,(uint32_t)schema,fd);
    if((uint32_t)index==config[2])return 0;
    if(index==(map?1:4))U(0xe945c0)=config[1];
    if(index==(map?2:5))U(0xe945c4)=map?24:(config[1]<154?175:181);
    if(index==(map?23:(config[1]<154?174:180)))routes();
    return 1;
}
uint8_t FUN_004d4720(RestoreMissionRoute *route)
{ note(0x4d4720,(uint32_t)route,0,0);U((uint32_t)route+308)=333;H((uint32_t)route+316)=1;H((uint32_t)route+318)=199;return 1; }
int FUN_005660ed(int fd,void *out,uint32_t n)
{ REQUIRE(fd==37&&n==4);note(0x5660ed,fd,(uint32_t)out,n);memcpy(out,config[3]?"MAPS":"oops",4);return 4; }
uint32_t FUN_0055bcf0(int fd,void *out,uint32_t n)
{ REQUIRE(fd==37&&n==4);note(0x55bcf0,fd,(uint32_t)out,n);memcpy(out,config[3]?"MAPS":"oops",4);return 1; }
int FUN_00562370(const char *first,const char *second){note(0x562370,(uint32_t)first,(uint32_t)second,0);return strcmp(first,second);}
void FUN_004cc110(int group,int item){note(0x4cc110,group,item,0);U(0xe20768)=BASE+0x600;}
void FUN_004cc160(int group,int item){note(0x4cc160,group,item,0);U(0xe20764)=BASE+0x700;}
int FUN_00538cd0(const void *a,const void *b,int flags){note(0x538cd0,(uint32_t)a,(uint32_t)b,flags);return 0;}
int FUN_00568eab(const char *path,int flags){note(0x568eab,(uint32_t)path,flags,0);return config[8]&1?-1:37;}
uint32_t FUN_005647eb(int fd){note(0x5647eb,fd,0,0);return 0;}
uint32_t FUN_005662e3(int fd,int offset,int method){note(0x5662e3,fd,offset,method);return offset;}
int FUN_004ce480(const char *path,void *out,uint32_t bytes,int offset)
{ note(0x4ce480,(uint32_t)path,bytes,offset);REQUIRE(bytes==4);*(uint32_t *)out=config[9];return config[8]&2?0:1; }
double FUN_004f78a0(int arg){note(0x4f78a0,arg,0,0);return 345.75;}
static uint32_t run(const uint32_t p[13])
{
    uint32_t i,j,op=p[0];REQUIRE(op<8&&p[7]<2);memcpy(config,p,sizeof(config));
    for(i=0;i<sizeof(regions)/sizeof(regions[0]);++i)memset((void *)(uintptr_t)regions[i][0],0xa5,regions[i][1]);
    memset((void *)ALT,0xa5,0x33000);memset((void *)0x799ae0u,0,0x89b6e8-0x799ae0);
    memset((void *)0x776fb0u,0,16200);
    if(p[5]&1)for(i=0;i<50;i+=7)U(0x776fb4u+i*324)=BASE+0x8000+i*64,U(0x7770e8u+i*324)=0x10002;
    for(i=0;i<4;++i) {
        uint32_t table=regions[10+i][0];
        for(j=0;j<regions[10+i][1];j+=4)U(table+j)=0x12340000u+j+i*0x100;
    }
    for(i=0;i<5;++i)if(p[6]&(1u<<i)) {static const uint16_t types[]={0xe3,0xe4,0xe1,0xe6,0xe5};B(0x799ae0u+i*264)=1;H(0x799af0u+i*264)=types[i];}
    U(0xe39080)=0x12345678;U(0xeaac28)=p[7];U(0xe945c0)=0x123400a0;U(0xe945c4)=0x11112222;
    U(0x5e17d8)=p[4];U(0x5e17e0)=4;U(0x5e17e4)=1;U(0x5e17c8)=0;
    U(0x605a3c)=p[11];B(0xe38ea8)=p[12];H(0x7848dc)=-1234;U(0xd38360)=0x11223344;
    strcpy((char *)(BASE+0x500),"restore-test.map");event_count=0;
    if(op==0){FUN_004d4ae0(37);return 0;}if(op==1){FUN_004d41e0(37);return 0;}
    if(op==2)return FUN_004d5820((const char *)(BASE+0x500),p[10]);
    if(op==3)return FUN_004d58b0(2,p[10]);
    if(op==4){FUN_004bd330((RestoreLoadSettings *)ALT);return 0;}
    if(op==5){U(ALT)=-300;U(ALT+4)=258;U(ALT+8)=0x12345678;FUN_004bd360((RestoreLoadSettings *)ALT);return 0;}
    if(op==6)return (uint32_t)FUN_004bd2d0((void *)ALT);
    FUN_004bd280();return 0;
}
static void output(uint32_t result)
{
    uint32_t i,j,at=0;static const char digits[]="0123456789abcdef";
    for(i=0;i<sizeof(regions)/sizeof(regions[0]);++i)for(j=0;j<regions[i][1];++j){uint8_t b=B(regions[i][0]+j);REQUIRE(at+2<sizeof(state));state[at++]=digits[b>>4];state[at++]=digits[b&15];}state[at]=0;
    printf("{\"result\":%u,\"events\":[",result);
    for(i=0;i<event_count;++i){printf("%s[",i?",":"");for(j=0;j<6;++j)printf("%s%u",j?",":"",events[i][j]);putchar(']');}
    printf("],\"state\":\"%s\"}\n",state);fflush(stdout);
}
int main(int argc,char **argv)
{
    uint32_t p[13]={0,160,0xffffffffu,1,0,1,31,1,0,240,1,0,2},i,result;
    SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOGPFAULTERRORBOX|SEM_NOOPENFILEERRORBOX);
    REQUIRE(restore_data_open());REQUIRE(VirtualAlloc((void *)BASE,0x100000,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE)==(void *)BASE);
    if(argc==2&&!strcmp(argv[1],"--oracle-batch")) {
        puts("{\"fields\":13}");fflush(stdout);
        for(;;){for(i=0;i<13;++i)if(scanf("%u",p+i)!=1)goto done;result=run(p);output(result);}
    } else {
        REQUIRE(argc==1);run(p);REQUIRE(U(0xe945c0)==160&&U(0x7770e4)==333&&U(0x777228)==333);
        p[0]=2;p[11]=1;REQUIRE(run(p)==0&&U(0x784f10)==(uint32_t)-1234&&U(0x784c2c)==345);
        p[0]=3;p[11]=0;REQUIRE(run(p)==1&&U(0xe39080)==0x12345678&&U(0xeaa6e8)==0);
        puts("Mission loaders: full-width state, route receivers, original wrapper order and object initialization.");
    }
done:VirtualFree((void *)BASE,0,MEM_RELEASE);restore_data_close();return 0;
}
