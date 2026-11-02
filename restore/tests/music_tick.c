#include "music_manager.h"
#include "sound_streams.h"
#include "standalone_data.h"
#include <windows.h>
#include <wincrypt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BASE 0x30000000u
#define ALT ((RestoreMusicManager *)(BASE+0x1000))
#define ALT_SOUND ((RestoreSound *)(BASE+0x80000))
#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(a))
#define CHECK(x) do{if(!(x)){fprintf(stderr,"Music tick failure: %d\n",__LINE__);ExitProcess(1);}}while(0)
static const uint32_t regions[][2]={{0xe39180,sizeof(RestoreMusicManager)+8},{BASE+0x1000,sizeof(RestoreMusicManager)+8},
 {0xe92bf8,sizeof(RestoreSound)+8},{BASE+0x80000,sizeof(RestoreSound)+8},{0xe38e60,0x80},{0xf55b88,0x14},{0x604950,4},{0xeaac28,4},{0xea1894,4},{0xea626c,4}};
static uint32_t params[16],events[256][8],event_count,now;
static RestoreMusicManager *active;static RestoreSound *active_sound;static HCRYPTPROV provider;
static void event(uint32_t a,uint32_t n,uint32_t v0,uint32_t v1,uint32_t v2,uint32_t v3)
{uint32_t *r;CHECK(event_count<256);r=events[event_count++];r[0]=a;r[1]=n;r[2]=v0;r[3]=v1;r[4]=v2;r[5]=v3;r[6]=(uint32_t)active->phase;r[7]=(uint32_t)active->current;}
uint32_t restore_music_milliseconds(void){event(0x56f12c,0,0,0,0,0);return now;}
uint32_t FUN_0055f449(RestoreSound *self,const char *path,int channel,uint32_t loops)
{event(0x55f449,4,(uint32_t)self,(uint32_t)path,(uint32_t)channel,loops);return 1;}
int __cdecl _AIL_stream_status_4(uintptr_t stream)
{event(0x56f298,1,(uint32_t)stream,0,0,0);if(params[15]&512)active_sound->streams[0]=0x56780000;return params[15]&16?4:2;}
int __cdecl _AIL_set_stream_volume_8(uintptr_t stream,uintptr_t volume)
{event(0x56f29c,2,(uint32_t)stream,(uint32_t)volume,0,0);return 0;}
extern uint32_t FUN_00562348(void);
/* op,seed,category,phase,count,ambient,current,waiting,previous,first,last,tick,duration,now,population,flags */
static uint32_t run(const uint32_t p[16])
{
 uint32_t k,i,result=0;CHECK(p[0]<=3&&p[2]<53&&p[4]<=(p[0]==2?100:50)&&p[5]<=10&&p[6]<=50);
 memcpy(params,p,sizeof(params));event_count=0;now=p[13];active=(p[15]&64)?ALT:RESTORE_MUSIC;active_sound=p[0]==1&&(p[15]&1024)?ALT_SOUND:RESTORE_SOUND;
 for(k=0;k<sizeof(regions)/sizeof(regions[0]);++k)memset((void *)regions[k][0],(uint8_t)(p[1]+k),regions[k][1]);
 for(k=0;k<2;++k){RestoreMusicManager *m=k?ALT:RESTORE_MUSIC;RestoreMusicCategory *c=&m->categories[p[2]];
  for(i=0;i<53;++i){m->categories[i].count=0;m->categories[i].ambient_count=0;}
  c->count=p[0]==2?0:(int32_t)p[4];c->ambient_count=(int32_t)p[5];
  for(i=0;i<60;++i){snprintf(c->tracks[i].path,100,"cat%u-track%u.mp3",p[2],i);c->tracks[i].delay=(int32_t)(p[1]+i*3);c->tracks[i].minimum=i*100;c->tracks[i].maximum=(i+2)*100-1;}
  for(i=0;i<50;++i)c->ambient_after[i]=(p[15]&32)?1:0;
  m->enabled=1;m->phase=(int32_t)p[3];m->current=(int32_t)p[6];m->waiting=(int32_t)p[7];m->previous=(int32_t)p[8];m->first=(int32_t)p[9];m->last=(int32_t)p[10];m->tick=p[11];
 }
 for(k=0;k<2;++k){RestoreSound *s=k?ALT_SOUND:RESTORE_SOUND;s->disabled=(p[15]&8)?1:0;s->streams[0]=(p[15]&4)?0x12340000:0;s->volumes[0]=-999;}
 U(0xf55b88)=(p[15]&1)?0x10000:0;B(0xe38e64)=(p[15]&2)?128:0;B(0xe38ea8)=(uint8_t)p[2];U(0xe38ec8)=p[14];U(0xf55b98)=p[12];U(0x604950)=p[1];U(0xeaac28)=p[1]&1;
 U(0xea1894)=(p[1]&1)?p[14]^0x13579bdf:p[14];U(0xea626c)=(p[1]&1)?p[14]:p[14]^0x13579bdf;
 if(p[0]==0)FUN_00539820(active);
 else if(p[0]==1)FUN_0055f38d(active_sound,(int32_t)p[14]);
 else if(p[0]==2)for(i=0;i<p[4];++i)result=FUN_00562348();
 else for(i=0;i<12;++i){now=p[13]+i*10000;FUN_00539820(active);}
 return result;
}
static void digest(uint32_t a,uint32_t n,char hex[65])
{HCRYPTHASH h;DWORD length=32;uint8_t b[32];uint32_t i;static const char d[]="0123456789abcdef";CHECK(CryptCreateHash(provider,CALG_SHA_256,0,0,&h));CHECK(CryptHashData(h,(const BYTE *)a,n,0));CHECK(CryptGetHashParam(h,HP_HASHVAL,b,&length,0));CHECK(CryptDestroyHash(h));for(i=0;i<32;++i){hex[i*2]=d[b[i]>>4];hex[i*2+1]=d[b[i]&15];}hex[64]=0;}
static void output(uint32_t result)
{uint32_t i,j;char hex[65];printf("{\"result\":%u,\"hashes\":[",result);for(i=0;i<sizeof(regions)/sizeof(regions[0]);++i){digest(regions[i][0],regions[i][1],hex);printf("%s\"%s\"",i?",":"",hex);}printf("],\"events\":[");for(i=0;i<event_count;++i){printf("%s[",i?",":"");for(j=0;j<8;++j)printf("%s%u",j?",":"",events[i][j]);putchar(']');}puts("]}");fflush(stdout);}
int main(int argc,char **argv)
{uint32_t p[16]={0,1,0,2,4,1,0,1,0,0,3,0,1000,1234,150,3},i;SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOGPFAULTERRORBOX);CHECK(restore_data_open());CHECK(VirtualAlloc((void *)BASE,0x100000,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE)==(void *)BASE);CHECK(CryptAcquireContextW(&provider,NULL,NULL,PROV_RSA_AES,CRYPT_VERIFYCONTEXT));
 if(argc==2&&!strcmp(argv[1],"--oracle-batch")){puts("{\"fields\":16}");fflush(stdout);for(;;){for(i=0;i<16;++i)if(scanf("%u",p+i)!=1)goto done;output(run(p));}}
 else{run(p);CHECK(active->phase==3&&active->tick==1234);p[0]=2;p[4]=1;CHECK(run(p)==41);puts("Music tick: native state machine, real selection/volume/RNG and owned clock boundary.");}
done:CryptReleaseContext(provider,0);VirtualFree((void *)BASE,0,MEM_RELEASE);restore_data_close();return 0;}
