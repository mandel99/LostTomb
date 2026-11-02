#include "load_save_menu.h"
#include "file_list.h"
#include "campaign_profile.h"
#include "sound_streams.h"
#include "standalone_data.h"
#include <stdio.h>
#include <string.h>
#define BASE 0x30000000u
#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(a))
#define REQUIRE(x) do{if(!(x)){fprintf(stderr,"Load/save failure at %d\n",__LINE__);ExitProcess(1);}}while(0)
static uint32_t count,events[160][24],mask,existing,accepted,small_calls,big_calls;
static const uint32_t regions[][2]={{0x6057c8,28},{0x6059e8,20},{0x7056b8,4},{0x7056c0,12},{0xd3b198,4},
 {0xd3b1b3,1},{0xd3b1bc,8},{0xe20998,4},{0xe38dc9,2},{0xe38dda,1},{0xe38df8,1},{0xe37100,80},{0xe37078,132},
 {0x700998,100},{0xe38aec,4},{0xe36efc,4},{0xe20764,8},{BASE+0x1000,1200},{BASE+0x4000,19200}};
static uint32_t hash(const char *s){uint32_t n=2166136261u;while(*s)n=(n^(uint8_t)*s++)*16777619u;return n;}
static void event(uint32_t a,uint32_t n,const uint32_t *args)
{
 uint32_t *row;REQUIRE(count<160&&n<=9);row=events[count++];memset(row,0,24*4);row[0]=a;row[1]=n;if(n)memcpy(row+2,args,n*4);
 row[12]=U(0x6059f4);row[13]=U(0x6059f0);row[14]=U(0xd3b198);row[15]=B(0xd3b1b3);row[16]=B(0xe38dda);row[17]=B(0xd3b1c0);row[18]=B(0xe38dca);row[19]=U(0xe20998);row[20]=U(0x7056c8);
}
#define E(a,...) do{uint32_t args[]={__VA_ARGS__};event(a,sizeof(args)/4,args);}while(0)
void FUN_0040f8f0(void *out,int n,int v){E(0x40f8f0,(uint32_t)out,n,v);memset(out,v,n);}
char *FUN_005169c0(void){event(0x5169c0,0,NULL);return (char *)(BASE+0x100);}
char *FUN_00516a20(void){event(0x516a20,0,NULL);return (char *)(BASE+0x200);}
void FUN_004cc500(int n){E(0x4cc500,n);}
void FUN_004cc3c0(void){event(0x4cc3c0,0,NULL);}
void FUN_004cc3e0(void){event(0x4cc3e0,0,NULL);}
void FUN_004cbea0(void *out,const void *in,int n){E(0x4cbea0,(uint32_t)out,(uint32_t)in,n);memcpy(out,in,n);}
int FUN_004ce3f0(const char *path){E(0x4ce3f0,hash(path));return existing;}
void FUN_004d5100(const char *path){E(0x4d5100,hash(path));}
void FUN_004d4d80(const char *path){E(0x4d4d80,hash(path));}
void FUN_004d4e20(const char *path){E(0x4d4e20,hash(path));}
void *FUN_00561a0a(const char *path,const char *mode){E(0x561a0a,hash(path),hash(mode));return existing?(void *)37:NULL;}
void FUN_00561994(void *file){E(0x561994,(uint32_t)file);}
void FUN_0055f561(RestoreSound *self,int channel){E(0x55f561,(uint32_t)self,channel);}
uint8_t FUN_0041b8a0(const RestoreCampaignProfile *self,const char *path){E(0x41b8a0,(uint32_t)self,hash(path));return 1;}
void FUN_004cc110(int g,int i){E(0x4cc110,g,i);U(0xe20768)=BASE+0x300;}
void FUN_004cc160(int g,int i){E(0x4cc160,g,i);U(0xe20764)=BASE+0x380;}
int FUN_00538cd0(const char *first,const char *second,int flags){E(0x538cd0,hash(first),hash(second),flags);return accepted?6:7;}
uint8_t FUN_0041c740(RestoreFileList *self,const char *pattern,char flag){E(0x41c740,(uint32_t)self,hash(pattern),flag);return 1;}
int FUN_00562370(const char *a,const char *b){return strcmp(a,b);}
void FUN_0041ca10(RestoreFileList *self,RestoreFileCompare compare){REQUIRE(compare==FUN_00562370);E(0x41ca10,(uint32_t)self,0x562370);}
int FUN_004cde60(int n,int d){return n*100/d;}
int FUN_004cde40(int n,int p){return n*p/100;}
void FUN_00536820(int x,int y,int w,int h){E(0x536820,x,y,w,h);}
void FUN_004d65b0(int x,int y,int w,int h,int ox,int oy){E(0x4d65b0,x,y,w,h,ox,oy);}
void FUN_004d6250(int g,int i,int x,int y,int w,int f,int c,int ox,int oy){E(0x4d6250,g,i,x,y,w,f,c,ox,oy);}
void FUN_004d62f0(int g,int i,int x,int y,int f,int c,int ox,int oy){E(0x4d62f0,g,i,x,y,f,c,ox,oy);}
void FUN_004cc770(const char *s,int x,int y,int f,int c){E(0x4cc770,hash(s),x,y,f,c);}
void FUN_00425730(int x,int y,const void *b,int n){E(0x425730,x,y,(uint32_t)b,n);}
void FUN_004cc540(void){event(0x4cc540,0,NULL);}
void FUN_00537140(int x,int y,int c){E(0x537140,x,y,c);}
int FUN_0053e590(int id,int x,int y){E(0x53e590,id,x,y);return 1;}
int FUN_00426450(int x,int y,const void *b,int n){E(0x426450,x,y,(uint32_t)b,n);return mask&255;}
uint32_t FUN_004d6740(int x,int y,const void *b,int n,int hit){E(0x4d6740,x,y,(uint32_t)b,n,hit);return 256+hit;}
int FUN_004261d0(int x,int y,const void *b,int n){E(0x4261d0,x,y,(uint32_t)b,n);return (mask>>(8+big_calls++))&1;}
void FUN_00425bc0(int x,int y,const void *b,int n,int frames){E(0x425bc0,x,y,(uint32_t)b,n,frames);}
int FUN_00425990(int x,int y,const void *b,int n){E(0x425990,x,y,(uint32_t)b,n);return (mask>>(10+small_calls++))&1;}
/* op,count,first,action,mode,error,offset x/y,mouse x/y,flags,mask,hover,
   file exists,accept overwrite,campaign */
static uint32_t run(const uint32_t p[16])
{
 uint32_t i,result=0;REQUIRE(p[0]<=10&&p[1]<=300);count=small_calls=big_calls=0;mask=p[11];existing=p[13];accepted=p[14];
 for(i=0;i<sizeof(regions)/sizeof(regions[0]);++i)memset((void *)(uintptr_t)regions[i][0],0xa5,regions[i][1]);
 memset((void *)BASE,0,0x1000);strcpy((char *)(BASE+0x100),"Save/Restore");strcpy((char *)(BASE+0x200),"Save/Restore.dat");
 strcpy((char *)(BASE+0x300),"Overwrite?");strcpy((char *)(BASE+0x380),"Saved city");
 for(i=0;i<300;++i){char *name=(char *)(BASE+0x4000+i*64);U(BASE+0x1000+i*4)=i*64;strcpy(name,"city000");name[4]=(char)('0'+i/100);name[5]=(char)('0'+i/10%10);name[6]=(char)('0'+i%10);if(p[0]<3)strcat(name,".sav");}
 RESTORE_SAVE_FILES->offsets=(uint32_t *)(BASE+0x1000);RESTORE_SAVE_FILES->strings=(char *)(BASE+0x4000);RESTORE_SAVE_FILES->count=p[1];
 U(0x6059f8)=p[1];U(0x6059f4)=p[2];U(0x6059f0)=7;U(0x6059ec)=p[10]&1;U(0x6059e8)=0x1234;
 U(0xd3b198)=p[3];B(0xd3b1c0)=p[4];B(0xd3b1b3)=p[5];U(0xd3b1bc)=48;
 U(0xe38da8)=p[6];U(0xe38da4)=p[7];U(0xe38e34)=p[6]+p[8];U(0xe38e30)=p[7]+p[9];
 B(0xe38e00)=(p[10]&2)!=0;B(0xe38df8)=(p[10]&4)!=0;U(0xe20998)=(p[10]&8)?1:0;
 B(0xe38dda)=(p[10]&16)?16:13;B(0xe38dca)=0;B(0xe38dc9)=0;B(0x7027b4)=p[15];U(0x7056c8)=p[12];
 U(0xf56f8c)=1005;U(0xf56f94)=335;U(0xf56f9c)=871;U(0xf56fb4)=18765;U(0xf56494)=49000;
 strcpy((char *)0xe370bcu,"Remembered");strcpy((char *)0xe37100u,"Entered");strcpy((char *)0xe37078u,"Map.name");memcpy((void *)0xe38aecu,"xyz",4);
 U(0xe20768)=BASE+0x300;U(0xe20764)=BASE+0x380;
 switch(p[0]){case 0:FUN_00402450();break;case 1:FUN_00402540();break;case 2:FUN_00402630();break;case 3:FUN_00402720();break;
 case 4:FUN_00402980();break;case 5:result=FUN_004029d0();break;case 6:FUN_00402a80();break;case 7:FUN_00532340();break;
 case 8:FUN_005325f0();break;case 9:FUN_00532830();break;case 10:restore_load_save_input();break;}
 return result;
}
static void output(uint32_t result)
{
 uint32_t i,j;printf("{\"result\":%u,\"state\":\"",result);
 for(i=0;i<sizeof(regions)/sizeof(regions[0]);++i)for(j=0;j<regions[i][1];++j)printf("%02x",B(regions[i][0]+j));
 printf("\",\"events\":[");for(i=0;i<count;++i){printf("%s[",i?",":"");for(j=0;j<24;++j)printf("%s%u",j?",":"",events[i][j]);putchar(']');}puts("]}");fflush(stdout);
}
int main(int argc,char **argv)
{
 uint32_t p[16]={7,20,1,1,1,0,192,144,472,180,2,0,256,1,1,0},i,result;
 REQUIRE(restore_data_open());REQUIRE(VirtualAlloc((void *)BASE,0x10000,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE)==(void *)BASE);
 if(argc==2&&!strcmp(argv[1],"--oracle-batch")){puts("{\"fields\":16}");fflush(stdout);for(;;){for(i=0;i<16;++i)if(scanf("%u",p+i)!=1)goto done;result=run(p);output(result);}}
 else {run(p);REQUIRE(count>20&&B(0xe38dc9)==1);p[0]=0;run(p);REQUIRE(B(0xe38dda)==13&&B(0xd3b1c0)==1);p[0]=6;p[2]=0;run(p);REQUIRE(!strcmp((char *)0xe37100u,"city001"));puts("Load/save: entry, filename selection, scrolling, rendering and confirmation contracts.");}
done:VirtualFree((void *)BASE,0,MEM_RELEASE);restore_data_close();return 0;
}
