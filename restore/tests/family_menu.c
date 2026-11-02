#include "restore_types.h"
#include "family_menu.h"
#include "file_list.h"
#include "campaign_profile.h"
#include "standalone_data.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define BASE 0x30000000u
#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(a))
static uint32_t files,target,available,event_count,events[32][4],checks;
#define REQUIRE(x) do{++checks;if(!(x)){fprintf(stderr,"Family menu failure at %d\n",__LINE__);ExitProcess(1);}}while(0)
static uint32_t hash(const char *s) {uint32_t v=2166136261u;while(*s)v=(v^(uint8_t)*s++)*16777619u;return v;}
static void event(uint32_t a,uint32_t b,uint32_t c,uint32_t d)
{REQUIRE(event_count<32);events[event_count][0]=a;events[event_count][1]=b;events[event_count][2]=c;events[event_count++][3]=d;}
uint8_t FUN_0041ae40(RestoreCampaign *self,const char *path)
{REQUIRE(self==RESTORE_CAMPAIGN);event(1,(uint32_t)self,hash(path),0);return 1;}
uint8_t FUN_0041b8a0(const RestoreCampaignProfile *self,const char *path)
{REQUIRE(self==RESTORE_CAMPAIGN_PROFILE);event(2,(uint32_t)self,hash(path),0);return 1;}
uint8_t FUN_0041b680(RestoreCampaignProfile *self,const char *path)
{REQUIRE(self==RESTORE_CAMPAIGN_PROFILE);event(3,(uint32_t)self,hash(path),0);return 1;}
restore_legacy_FILE *FUN_00561a0a(const char *path,const char *mode)
{REQUIRE(!strcmp(mode,"rb"));event(4,hash(path),hash(mode),available);return available?(restore_legacy_FILE *)0x31000000:NULL;}
uint32_t FUN_00561994(restore_legacy_FILE *stream)
{REQUIRE(stream==(restore_legacy_FILE *)0x31000000);event(5,0,0,0);return 0;}
BOOL restore_file_create_directory(LPCSTR path,LPSECURITY_ATTRIBUTES attributes)
{REQUIRE(!attributes);event(6,hash(path),0,0);return TRUE;}
void FUN_00516b50(const char *pattern)
{
    uint32_t i;REQUIRE(!strcmp(pattern,"Save\\*.dat"));event(7,files,0,0);
    RESTORE_FAMILY_FILES->offsets=(uint32_t *)(BASE+0x1000);RESTORE_FAMILY_FILES->strings=(char *)(BASE+0x2000);RESTORE_FAMILY_FILES->count=(int32_t)files;
    for(i=0;i<files;++i){RESTORE_FAMILY_FILES->offsets[i]=i*32;if(i==target)strcpy(RESTORE_FAMILY_FILES->strings+i*32,"Restore");else sprintf_s(RESTORE_FAMILY_FILES->strings+i*32,32,"Family%03u",i);}
}
char *FUN_0041e010(RestoreFileList *self,int index)
{REQUIRE(self==RESTORE_FAMILY_FILES&&index>=0&&index<self->count);return self->strings+self->offsets[index];}
void FUN_004ce3a0(char *path) {char *dot=strrchr(path,'.');if(dot)*dot=0;}
void FUN_004cc500(int index) {event(8,index,0,0);U(0xe20990)=index;}
void FUN_004cc3c0(void) {event(9,0,0,0);}
void FUN_004cc3e0(void) {event(10,0,0,0);}
void FUN_004cb6a0(void) {event(11,0,0,0);B(0xe38dda)=0x30;}
void FUN_0051f780(int ignored) {(void)ignored;}
int FUN_004cde60(int numerator,int denominator) {return numerator*100/denominator;}
int FUN_004cde40(int value,int percent) {return value*percent/100;}
static void run(const uint32_t p[10])
{
    uint32_t i;const char *names[]={"Restore","  Restore  ","   ",""};
    files=p[1];target=p[2];available=p[6];REQUIRE(files<=300&&p[7]<4);
    memset((void *)BASE,0,0x10000);memset((void *)0xe38e74u,0xa5,32);strcpy((char *)0xe38e74u,names[p[7]]);
    memset((void *)0xeadda0u,0xa5,520);memset((void *)0x705704u,0xa5,24);
    RESTORE_FAMILY_FILES->count=files;RESTORE_FAMILY_FILES->offsets=(uint32_t *)(BASE+0x1000);RESTORE_FAMILY_FILES->strings=(char *)(BASE+0x2000);
    for(i=0;i<files;++i){RESTORE_FAMILY_FILES->offsets[i]=i*32;if(i==target)strcpy((char *)(BASE+0x2000+i*32),"Restore");else sprintf_s((char *)(BASE+0x2000+i*32),32,"Family%03u",i);}
    U(0xeadfcc)=p[3];U(0xd3b198)=p[4];U(0xeadfc8)=p[5];B(0xe38dda)=54;B(0xe38dca)=0;B(0xd3b1b3)=0x77;U(0xe20998)=0x12345678;U(0xe94dc0)=0x12345678;
    U(0xe20990)=7;U(0xe38da8)=192;U(0xe38da4)=144;U(0xe38e34)=p[8];U(0xe38e30)=p[9];B(0xe38e00)=1;event_count=0;
    switch(p[0]) {
    case 0:FUN_00516c80();break;case 1:FUN_00516bb0();break;case 2:FUN_00516ab0();break;case 3:FUN_00516990();break;
    case 4:FUN_00516c10();break;case 5:FUN_00516890();break;case 6:FUN_005168e0();break;case 7:FUN_00516c60();break;
    }
}
static const uint32_t regions[][2]={{0xe38e74,32},{0xeadfcc,4},{0xeadfc8,4},{0xe38dda,1},{0xe38dca,1},{0xd3b1b3,1},{0xe20998,4},{0xe94dc0,4},{0xd3b198,4},{0xe20990,4},{0x705704,24},{0xeadda0,520}};
static void output(void)
{
    uint32_t i,j;printf("{\"state\":\"");for(i=0;i<sizeof(regions)/sizeof(regions[0]);++i)for(j=0;j<regions[i][1];++j)printf("%02x",*(uint8_t *)(uintptr_t)(regions[i][0]+j));
    printf("\",\"events\":[");for(i=0;i<event_count;++i)printf("%s[%u,%u,%u,%u]",i?",":"",events[i][0],events[i][1],events[i][2],events[i][3]);puts("]}");fflush(stdout);
}
int main(int argc,char **argv)
{
    uint32_t p[10]={0,260,259,0,0,0,0,1,656,300},i;
    REQUIRE(restore_data_open());REQUIRE(VirtualAlloc((void *)BASE,0x10000,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE)==(void *)BASE);
    if(argc==2&&!strcmp(argv[1],"--oracle-batch")) {
        puts("{\"fields\":10}");fflush(stdout);for(;;){for(i=0;i<10;++i)if(scanf("%u",p+i)!=1)goto done;run(p);output();}
    } else {
        run(p);REQUIRE(U(0xeadfcc)==259&&B(0xe38dda)==0x30&&!strcmp((char *)0xe38e74u,"Restore"));
        p[6]=1;run(p);REQUIRE(B(0xe38dda)==0x3f&&U(0x705704)==0x516c10);
        p[0]=1;p[1]=0;run(p);REQUIRE(B(0xe38dda)==54&&U(0xe20998)==0);
        p[0]=4;run(p);REQUIRE(B(0xe38e74)==0&&B(0xe38dda)==54);
        p[0]=5;p[1]=260;p[4]=1;run(p);REQUIRE(U(0xeadfc8)==1);
        p[0]=7;run(p);REQUIRE(B(0xe38dda)==0x3f&&U(0x705704)==0x516dd0&&U(0x705708)==2);
        printf("Family menu: %u checks; creation, duplicate prompt, selection beyond 255, name entry and scroll.\n",checks);
    }
done:VirtualFree((void *)BASE,0,MEM_RELEASE);restore_data_close();return 0;
}
