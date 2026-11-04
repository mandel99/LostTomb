#include "music_manager.h"
#include "standalone_data.h"
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <io.h>
#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define SNAPSHOT (sizeof(RestoreMusicManager)+8)
static uint8_t data[65537];static size_t length,position;
static uint32_t p[16],events[4096][4],event_count,checks,scenarios;
#define REQUIRE(x) do{++checks;if(!(x)){fprintf(stderr,"Music manager failure at %d\n",__LINE__);ExitProcess(1);}}while(0)
static uint32_t hash(const char *s) {uint32_t v=2166136261u;while(*s)v=(v^(uint8_t)*s++)*16777619u;return v;}
static void event(uint32_t a,uint32_t n,uint32_t b,uint32_t c) {uint32_t *e;REQUIRE(event_count<4096);e=events[event_count++];e[0]=a;e[1]=n;e[2]=b;e[3]=c;}
void *FUN_00561a0a(const char *file,const char *mode)
{
    event(0x561a0a,2,hash(file),hash(mode));
    if(!strcmp(file,"music.txt"))return p[15]?(void *)0x31000000u:NULL;
    return p[13]?(void *)0x31000004u:NULL;
}
char *FUN_00561e4d(char *out,int size,void *stream)
{
    size_t i=0;REQUIRE(size==200&&stream==(void *)0x31000000u);if(position==length)return NULL;
    while(i<(size_t)size-1&&position<length) {out[i++]=(char)data[position++];if(out[i-1]=='\n')break;}out[i]=0;return out;
}
int FUN_00561994(void *stream) {event(0x561994,1,(uint32_t)(uintptr_t)stream,0);return 0;}
static void prepare(const uint32_t input[16],const uint8_t *text,size_t bytes)
{
    RestoreMusicManager *self=RESTORE_MUSIC;unsigned int i;size_t j;
    memcpy(p,input,sizeof(p));event_count=0;position=length=0;REQUIRE(bytes<=65536);
    for(j=0;j<bytes;++j) {if(text[j]=='\r'&&j+1<bytes&&text[j+1]=='\n')continue;data[length++]=text[j];}data[length]=0;
    memset(self,(uint8_t)p[1],SNAPSHOT);REQUIRE(FUN_00539550(self)==self);
    if(!p[0])return;
    for(i=0;i<53;++i)self->categories[i].ambient_count=0;
    if(p[0]==3&&(int32_t)p[2]<=53){
        uint32_t row=(uint32_t)(uintptr_t)self+p[2]*0x1b10u;
        U(row+0x1b08u)=p[3];U(row+0x1b0cu)=p[4];
    }
    if(p[0]!=5&&(int32_t)p[2]>=0&&p[2]<53) {
        RestoreMusicCategory *c=&self->categories[p[2]];c->count=(int32_t)p[3];c->ambient_count=(int32_t)p[4];
        for(i=0;i<p[3]&&i<50;++i) {
            snprintf(c->tracks[i].path,100,"track%u.mp3",i);c->tracks[i].delay=5+i;c->tracks[i].minimum=i*100;c->tracks[i].maximum=(i+2)*100-1;
        }
    }
    self->current=(int32_t)p[5];self->first=(int32_t)p[9];self->last=(int32_t)p[10];self->previous=(int32_t)p[11];self->waiting=(int32_t)p[12];self->phase=42;
    *(int8_t *)0xe38ea8u=(int8_t)p[2];U(0xeaac28)=p[7];
    if(p[7]<8)U(0xea1894u+p[7]*0x49d8u)=p[8];*(uint8_t *)0xe38e62u=(uint8_t)p[14];U(0x12663c8)=0;
}
static int run(const uint32_t input[16],const void *text,size_t bytes)
{
    prepare(input,text,bytes);++scenarios;
    switch(p[0]) {
    case 0:return 1;
    case 1:FUN_00539a40(RESTORE_MUSIC,(int32_t)p[3]);return 0;
    case 2:FUN_00539a50(RESTORE_MUSIC,(int32_t)p[9],(int32_t)p[10]);return 0;
    case 3:return FUN_005395e0(RESTORE_MUSIC,(const char *)data,(int32_t)p[2],(int32_t)p[9],(int32_t)p[10],(int32_t)p[11],(int)p[6]);
    case 4:FUN_00539730(RESTORE_MUSIC,(int)p[6]);return 0;
    case 5:FUN_00539250(RESTORE_MUSIC);return 0;
    default:REQUIRE(0);return 0;
    }
}
static void oracle(void)
{
    static char encoded[131073];uint32_t input[16];unsigned int i,j,bytes,value;int result;
    REQUIRE(_setmode(_fileno(stdout),_O_BINARY)!=-1);
    printf("{\"object\":%u,\"bytes\":%u}\n",(unsigned int)(uintptr_t)RESTORE_MUSIC,(unsigned int)SNAPSHOT);fflush(stdout);
    for(;;) {
        for(i=0;i<16;++i)if(scanf("%u",&input[i])!=1)return;
        REQUIRE(scanf("%u",&bytes)==1&&bytes<=65536);
        if(bytes) {REQUIRE(scanf("%131072s",encoded)==1&&strlen(encoded)==bytes*2u);}
        for(i=0;i<bytes;++i) {REQUIRE(sscanf(encoded+i*2,"%2x",&value)==1);data[i]=(uint8_t)value;}
        result=run(input,data,bytes);printf("{\"result\":%d,\"position\":%u,\"events\":[",result,(unsigned int)position);
        for(i=0;i<event_count;++i) {printf("%s[",i?",":"");for(j=0;j<4;++j)printf("%s%u",j?",":"",events[i][j]);putchar(']');}
        puts("]}");fwrite(RESTORE_MUSIC,1,SNAPSHOT,stdout);fflush(stdout);
    }
}
int main(int argc,char **argv)
{
    uint32_t q[16]={3,0,0,0,0,0,0,0,150,7,20,30,0,1,0,1};RestoreMusicCategory *c;char long_path[101],generated[1024];unsigned int i;size_t n;
    const char *song="fixture.mp3",*text="; comment\r\nfirst.mp3 M 0 20 0 999\r\nAMBIENT\r\nsecond.mp3 A 52\r\nthird.mp3 M 0\r\n";
    REQUIRE(restore_data_open());
    if(argc==2&&!strcmp(argv[1],"--oracle-batch"))oracle();
    else {
        REQUIRE(run(q,song,strlen(song))==1);c=&RESTORE_MUSIC->categories[0];REQUIRE(c->count==1&&!strcmp(c->tracks[0].path,song)&&c->tracks[0].minimum==20);
        q[6]=1;REQUIRE(run(q,song,strlen(song))==1&&c->ambient_count==1&&c->tracks[50].minimum==0&&c->tracks[50].maximum==0);
        q[4]=10;REQUIRE(run(q,song,strlen(song))==-1);q[4]=0;q[6]=0;q[3]=50;REQUIRE(run(q,song,strlen(song))==-1);
        q[3]=0;q[2]=53;REQUIRE(run(q,song,strlen(song))==1&&RESTORE_MUSIC->enabled!=0);q[2]=0xffffffffu;REQUIRE(run(q,song,strlen(song))==1);q[2]=0;
        memset(long_path,'x',100);long_path[100]=0;REQUIRE(run(q,long_path,100)==1&&c->count==1&&c->tracks[0].delay==7);
        q[0]=5;REQUIRE(run(q,text,strlen(text))==0&&c->count==2&&c->ambient_after[1]==1&&c->tracks[1].delay==20);
        REQUIRE(RESTORE_MUSIC->categories[52].ambient_count==1&&RESTORE_MUSIC->categories[52].tracks[50].delay==20);
        q[13]=0;REQUIRE(run(q,"a.mp3 M 0\n",10)==0&&!strcmp(c->tracks[0].path,"audio\\music\\a.mp3"));
        q[14]='D';run(q,"a.mp3 M 0\n",10);REQUIRE(!strcmp(c->tracks[0].path,"audio\\music\\a.mp3"));q[13]=1;q[14]=0;
        run(q,";bad\n\nAMBIENT\n",14);REQUIRE(c->count==0&&c->ambient_after[0]==1);
        n=0;for(i=0;i<50;++i) {memcpy(generated+n,"a.mp3 M 0\n",10);n+=10;}memcpy(generated+n,"AMBIENT\n",8);n+=8;
        run(q,generated,n);REQUIRE(c->count==1&&c->ambient_count==0);
        memset(generated,'x',95);memcpy(generated+95," M 0\n",5);run(q,generated,100);REQUIRE(c->count==1);
        q[0]=4;q[3]=5;q[5]=4;q[6]=1;q[7]=7;q[8]=150;q[11]=4;q[12]=0;run(q,"",0);
        REQUIRE(RESTORE_MUSIC->first==0&&RESTORE_MUSIC->last==1&&RESTORE_MUSIC->current==0&&RESTORE_MUSIC->phase==1);
        q[12]=1;run(q,"",0);REQUIRE(RESTORE_MUSIC->phase==42);
        q[0]=0;q[1]=0xa5;run(q,"",0);for(i=0;i<53;++i)REQUIRE(!RESTORE_MUSIC->categories[i].count&&RESTORE_MUSIC->categories[i].ambient_count==(int32_t)0xa5a5a5a5u);
        REQUIRE(U((uintptr_t)RESTORE_MUSIC+sizeof(*RESTORE_MUSIC))==0xa5a5a5a5u);
        printf("Music manager: %u checks across %u scenarios; native record/count behavior, parser defaults, ambient aliasing, population selection and local-only CD policy verified without playback.\n",checks,scenarios);
    }
    restore_data_close();return 0;
}
