#include "high_scores_menu.h"
#include "menu_input.h"
#include "standalone_data.h"
#include <direct.h>
#include <stdlib.h>
#define BASE 0x30000000u
#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(a))
#define REQUIRE(x) do { if(!(x)){fprintf(stderr,"High scores failure at %d\n",__LINE__);ExitProcess(1);} } while(0)
static const uint32_t regions[][2]={{0xd38700,0x1dc8},{0x6ff608,5016},{0xe20774,12},{0xe38da0,16},
 {0xe38dc6,16},{0xe38dd6,12},{0xe38df4,16},{0xe38e2c,16},{0xd3b194,12},{0xe38ea4,12},{0x5d2a28,12},
 {BASE+0x3ff8,0x1dc0},{0x14a439f8,92}};
static uint32_t count,events[160][20],io_open,io_bytes,io_result,advance,small_result,real_io;
static uint32_t hash(const char *s){uint32_t v=2166136261u;while(*s)v=(v^(uint8_t)*s++)*16777619u;return v;}
static void event(uint32_t address,uint32_t n,const uint32_t *args)
{
 uint32_t *r;REQUIRE(count<160&&n<=8);r=events[count++];memset(r,0,80);r[0]=address;r[1]=n;if(n)memcpy(r+2,args,n*4);
 r[12]=U(0xd3a4b8);r[13]=U(0xd3a4bc);r[14]=B(0xe38dca);r[15]=B(0xe38dda);r[16]=U(0xe20778);
}
#define E(a,...) do{uint32_t args[]={__VA_ARGS__};event(a,sizeof(args)/4,args);}while(0)
restore_legacy_FILE *FUN_00561a0a(const char *path,const char *mode)
{
 E(0x561a0a,hash(path),hash(mode));
 if(real_io)return (restore_legacy_FILE *)fopen(path,mode);
 return io_open?(restore_legacy_FILE *)(BASE+0xf000):NULL;
}
uint32_t FUN_00561ca1(void *out,uint32_t size,uint32_t n,restore_legacy_FILE *file)
{
 E(0x561ca1,(uint32_t)out,size,n,(uint32_t)file);
 if(real_io)return (uint32_t)fread(out,size,n,(FILE *)file);
 REQUIRE(size==0x1db0&&n==1&&file==(restore_legacy_FILE *)(BASE+0xf000));memcpy(out,(void *)(BASE+0x1000),io_bytes);return io_result;
}
uint32_t FUN_00561994(restore_legacy_FILE *file)
{
 E(0x561994,(uint32_t)file);return real_io?(uint32_t)fclose((FILE *)file):0x12345678;
}
void FUN_00536820(int x,int y,int w,int h){E(0x536820,x,y,w,h);}
void FUN_00536a30(int x,int y,int w,int h){E(0x536a30,x,y,w,h);}
void FUN_004ccb30(int g,int i,int x,int y,int w,int f,int c){E(0x4ccb30,g,i,x,y,w,f,c);}
void FUN_004ccbb0(int g,int i,int x,int y,int f,int c){E(0x4ccbb0,g,i,x,y,f,c);U(0xe20778)+=advance;}
void FUN_004cc770(const char *s,int x,int y,int f,int c){E(0x4cc770,hash(s),x,y,f,c);U(0xe20778)+=advance;}
void FUN_004cce00(uint32_t number,char prefix,const char *suffix,int x,int y,int f,int c){E(0x4cce00,number,(uint8_t)prefix,hash(suffix),x,y,f,c);U(0xe20778)+=advance;}
int FUN_0053e590(int image,int x,int y){E(0x53e590,0xf55bb0,image,x,y);return 1;}
int FUN_00425990(int x,int y,const void *buttons,int n){E(0x425990,x,y,(uint32_t)buttons,n);return small_result;}
static void game(uint32_t address){REQUIRE(address==0x4c5920);FUN_004c5920();}
static const RestoreMenuActionApi action_api={game,NULL,NULL,NULL};
const RestoreMenuActionApi *restore_menu_action_platform(void){return &action_api;}
/* op,mission count,first,total,valid pattern,offset x/y,local mouse x/y,
   flags(mouse held/back),action,open success,bytes copied,read result,text advance,small return. */
static uint32_t run(const uint32_t p[16])
{
 uint32_t i,result=0;RestoreHighScores *payload=(RestoreHighScores *)(BASE+0x1000);
 REQUIRE(p[0]<=8&&(p[1]<=100||(int32_t)p[1]<0)&&p[12]<=7600);count=0;io_open=p[11];io_bytes=p[12];io_result=p[13];advance=p[14];small_result=p[15];
 memset((void *)BASE,0,0x10000);
 for(i=0;i<sizeof(regions)/sizeof(regions[0]);++i)memset((void *)(uintptr_t)regions[i][0],0xa5,regions[i][1]);
 memset(payload,0x5a,sizeof(*payload));strcpy((char *)(BASE+0x100),"fixture-scores.bin");
 for(i=0;i<100;++i){char *mission=(char *)(0x6ff60cu+i*50u);RestoreHighScore *r=payload->records+i;
  sprintf(mission,"Mission%03u",i);r->score=100000+i*17;r->mission=i;sprintf(r->family,"Family%03u",i);r->difficulty=i%5;
  r->valid=p[4]==0?0:(p[4]==1?1:(p[4]==2?(i%2?256:0):(i%3?0:(int32_t)0x80000000u)));
 }
 memcpy(RESTORE_HIGH_SCORES,payload,sizeof(*payload));if(p[0]<=1||p[0]==8)memset(RESTORE_HIGH_SCORES,0x6d,sizeof(*payload));
 U(0x14a43a48)=1; /* Valid record whose native mission-address arithmetic wraps to NULL. */
 U(0x700994)=p[1];U(0xd3a4b8)=p[2];U(0xd3a4bc)=p[3];U(0xe38da8)=p[5];U(0xe38da4)=p[6];
 U(0xe38e34)=p[5]+p[7];U(0xe38e30)=p[6]+p[8];B(0xe38e00)=(p[9]&1)!=0;B(0xe38df8)=(p[9]&2)!=0;
 U(0xd3b198)=p[10];B(0xe38dca)=0;B(0xe38dda)=0x53;U(0xe20778)=0x12345678;
 U(0xf56f84)=1005;U(0xf56f8c)=335;U(0xf56f94)=871;U(0xf56fb4)=18765;U(0xf56494)=49000;
 switch(p[0]){case 0:result=FUN_004c5920();break;case 1:result=restore_high_scores_read((RestoreHighScores *)(BASE+0x4000),(const char *)(BASE+0x100));break;
 case 2:FUN_004c5400();break;case 3:FUN_004c5450();break;case 4:FUN_004c54c0();break;case 5:FUN_004c5660();break;
 case 6:result=FUN_004c57d0();break;case 7:restore_high_scores_input();break;case 8:FUN_00417ff0();break;}
 return result;
}
static void output(uint32_t result)
{
 uint32_t i,j;printf("{\"result\":%u,\"state\":\"",result);
 for(i=0;i<sizeof(regions)/sizeof(regions[0]);++i)for(j=0;j<regions[i][1];++j)printf("%02x",B(regions[i][0]+j));
 printf("\",\"events\":[");for(i=0;i<count;++i){printf("%s[",i?",":"");for(j=0;j<20;++j)printf("%s%u",j?",":"",events[i][j]);putchar(']');}puts("]}");fflush(stdout);
}
static void smoke(void)
{
 char directory[MAX_PATH],*slash;FILE *file;uint32_t p[16]={0,80,7,37,2,192,144,584,200,1,0,1,7600,1,17,0};
 uint8_t expected[7600];uint32_t i;
 GetModuleFileNameA(NULL,directory,MAX_PATH);slash=strrchr(directory,'\\');REQUIRE(slash);*slash=0;strcat(directory,"\\high-scores-fixtures");CreateDirectoryA(directory,NULL);REQUIRE(SetCurrentDirectoryA(directory));CreateDirectoryA("save",NULL);
 run(p);memcpy(expected,(void *)(BASE+0x1000),sizeof(expected));real_io=1;
 DeleteFileA("save\\highscore.jas");count=0;REQUIRE(FUN_004c5920()==0&&U(0xd3a4bc)==0);
 file=fopen("save\\highscore.jas","wb");REQUIRE(file);REQUIRE(fwrite(expected,7600,1,file)==1);REQUIRE(fclose(file)==0);
 memset(RESTORE_HIGH_SCORES,0,7600);U(0xd3a4b8)=7;count=0;REQUIRE(FUN_004c5920()==1&&U(0xd3a4bc)==40&&U(0xd3a4b8)==7);REQUIRE(!memcmp(RESTORE_HIGH_SCORES,expected,7600));
 FUN_004c5660();REQUIRE(B(0xe38dca)==1&&count>30);B(0xe38e00)=1;U(0xe38e34)=192+584;U(0xe38e30)=144+401;REQUIRE(FUN_004c57d0()==1&&U(0xd3a4b8)==20);
 U(0xd3b198)=0;FUN_004c5400();REQUIRE(U(0xd3a4b8)==19);B(0xe38df8)=1;restore_high_scores_input();REQUIRE(B(0xe38dda)==0&&B(0xe38dca)==1);
 file=fopen("save\\highscore.jas","wb");REQUIRE(file);REQUIRE(fwrite(expected,123,1,file)==1);REQUIRE(fclose(file)==0);memset(RESTORE_HIGH_SCORES,0x6d,7600);count=0;
 REQUIRE(FUN_004c5920()==0&&U(0xd3a4bc)==0);REQUIRE(!memcmp(RESTORE_HIGH_SCORES,expected,123));for(i=123;i<7600;++i)REQUIRE(((uint8_t *)RESTORE_HIGH_SCORES)[i]==0x6d);
 REQUIRE(DeleteFileA("save\\highscore.jas"));real_io=0;
 puts("High scores: actual missing/full/partial file reads, score rows, scrolling, drag and back input.");
}
int main(int argc,char **argv)
{
 uint32_t p[16],i;REQUIRE(restore_data_open());REQUIRE(VirtualAlloc((void *)BASE,0x10000,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE)==(void *)BASE);
 REQUIRE(VirtualAlloc((void *)0x14a40000u,0x10000,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE)==(void *)0x14a40000u);
 if(argc==2&&!strcmp(argv[1],"--oracle-batch")){puts("{\"fields\":16}");fflush(stdout);for(;;){for(i=0;i<16;++i)if(scanf("%u",p+i)!=1)goto done;output(run(p));}}
 else smoke();
 done:VirtualFree((void *)0x14a40000u,0,MEM_RELEASE);VirtualFree((void *)BASE,0,MEM_RELEASE);restore_data_close();return 0;
}
