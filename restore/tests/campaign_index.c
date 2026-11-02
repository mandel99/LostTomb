#include "campaign_index.h"
#include "standalone_data.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static RestoreCampaignIndex local;
static uint32_t random_state,checks,lookups;
static const uintptr_t addresses[]={0xe38eb0,0xe38eb4,0xe38eb8,0xe38ea8,0xe38ea9,0xe38eaa,0xe38eab,0x7027a4,
 0xea5cc0,0xea5cc0+0x49d8,0xea5cc0+2*0x49d8,0xea5cc0+3*0x49d8,0xea5cc0+4*0x49d8,0xea5cc0+5*0x49d8,0xea5cc0+6*0x49d8,0xea5cc0+7*0x49d8,
 0xe39080,0xeaac28,0x5d1988,0x5d198c,0x5d1990,0x5d1994,0x5d1998};
#define N_FIELDS (sizeof(addresses)/sizeof(addresses[0]))
#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define REQUIRE(x) do{++checks;if(!(x)){fprintf(stderr,"Campaign index failure at line %d\n",__LINE__);abort();}}while(0)
static uint32_t next_random(void) {random_state=random_state*1664525u+1013904223u;return random_state;}
static void fill(uint32_t seed,int count)
{
 int i;random_state=seed;memset(&local,0,sizeof(local));local.count=count;
 for(i=0;i<100;++i) {
  uint8_t *r=local.records[i].bytes;uint32_t n;int32_t choices;unsigned int j;
  for(j=0;j<64;++j)r[j]=(uint8_t)(next_random()>>24);
  r[0]=(uint8_t)(next_random()%12);r[1]=(uint8_t)(next_random()%7);r[7]=(uint8_t)(next_random()%6);
  for(j=0;j<5;++j)r[2+j]=(uint8_t)(next_random()%7);
  choices=(int32_t)(next_random()%3);memcpy(r+0x38,&choices,4);
  r[0x2a]=(uint8_t)(next_random()%7);r[0x36]=(uint8_t)(next_random()%7);n=next_random();r[0x3c]=(uint8_t)(n&1);
 }
}
static int run(unsigned int op,uint32_t category,int previous,int choice)
{
 ++lookups;
 if(op==0)return FUN_0041b920(&local,category,previous,choice);
 if(op==1)return FUN_0041ba20(&local,category);
 if(op==2)return FUN_0041bab0(&local);
 FUN_00417dd0((int8_t)category);return 0;
}
static unsigned int width(size_t field) {return field>=3&&field<=6?1:4;}
static void snapshot(uint32_t *out) {size_t i;for(i=0;i<N_FIELDS;++i){out[i]=0;memcpy(out+i,(void *)addresses[i],width(i));}}
static void prepare_state(unsigned int profile,unsigned int option)
{
 size_t i;REQUIRE(profile<8&&option<5);
 for(i=0;i<N_FIELDS;++i)memset((void *)addresses[i],0x5a,width(i));
 U(0xe39080)=option;U(0xeaac28)=profile;
 for(i=0;i<5;++i)U(0x5d1988u+i*4)=0x81234560u+(uint32_t)i;
 *RESTORE_CAMPAIGN_INDEX=local;
}
static void values(const uint32_t *a) {size_t i;putchar('[');for(i=0;i<N_FIELDS;++i)printf("%s%u",i?",":"",a[i]);putchar(']');}
static void hex(const void *memory,size_t n) {const unsigned char *p=memory;size_t i;putchar('"');for(i=0;i<n;++i)printf("%02x",p[i]);putchar('"');}
static void oracle(void)
{
 unsigned int op,seed,count,category,previous,choice,profile,option;uint32_t before[N_FIELDS],after[N_FIELDS];int result;size_t i;
 printf("{\"fields\":[");for(i=0;i<N_FIELDS;++i)printf("%s[%u,%u]",i?",":"",(unsigned int)addresses[i],width(i));puts("]}");fflush(stdout);
 while(scanf("%u %u %u %u %u %u %u %u",&op,&seed,&count,&category,&previous,&choice,&profile,&option)==8) {
  fill(seed,(int32_t)count);prepare_state(profile,option);snapshot(before);
  printf("{\"input\":");hex(&local,sizeof(local));printf(",\"before\":");values(before);result=run(op,category,(int32_t)previous,(int32_t)choice);snapshot(after);
  printf(",\"result\":%d,\"after\":",result);values(after);printf(",\"object_after\":");hex(op==3?RESTORE_CAMPAIGN_INDEX:&local,sizeof(local));puts("}");fflush(stdout);
 }
}
static void golden(void)
{
 unsigned int op,seed;RestoreCampaignIndex before;int32_t choices=2;
 memset(&local,0,sizeof(local));local.count=4;
 local.records[0].bytes[0]=2;local.records[0].bytes[1]=255;local.records[0].bytes[12]=42;
 local.records[1].bytes[0]=1;local.records[1].bytes[1]=4;local.records[1].bytes[7]=2;local.records[1].bytes[2]=3;local.records[1].bytes[3]=255;
 local.records[2].bytes[0]=1;local.records[2].bytes[1]=0;
 REQUIRE(FUN_0041b920(&local,1,0,0)==1);
 REQUIRE(FUN_0041b920(&local,1,-100,0)==2);
 REQUIRE(FUN_0041b920(&local,257,0,0)==-1);
 REQUIRE(FUN_0041b920(&local,1,4,0)==-1);
 memcpy(local.records[0].bytes+0x38,&choices,4);local.records[0].bytes[0x2a]=8;local.records[0].bytes[0x36]=4;
 REQUIRE(FUN_0041b920(&local,1,0,0)==2);REQUIRE(FUN_0041b920(&local,1,0,1)==1);
 REQUIRE(FUN_0041ba20(NULL,5)==1);REQUIRE(FUN_0041ba20(NULL,0x7fffffff)==1);
 local.count=0;REQUIRE(FUN_0041ba20(&local,UINT32_MAX)==0);REQUIRE(FUN_0041bab0(&local)==1);
 for(op=0;op<3;++op)for(seed=0;seed<1000;++seed){fill(seed,seed%101);before=local;run(op,seed%14,(int)(seed%102)-1,seed&1);REQUIRE(!memcmp(&before,&local,sizeof(local)));}
 memset(&local,0,sizeof(local));local.count=1;local.records[0].bytes[0]=2;local.records[0].bytes[12]=42;
 prepare_state(3,2);FUN_00417dd0(2);
 REQUIRE(U(0xe38eb0)==0x81234562u&&U(0xe38eb4)==0&&U(0xe38eb8)==0);
 REQUIRE(*(uint8_t *)0xe38eab==0&&*(uint8_t *)0xe38ea8==42&&*(uint8_t *)0xe38ea9==2&&*(uint8_t *)0xe38eaa==255);
 REQUIRE(U(0x7027a4)==0x5a5a5a00u&&U(0xea5cc0+3*0x49d8)==0&&U(0xea5cc0+2*0x49d8)==0x5a5a5a5au);
 local.count=0;prepare_state(0,0);FUN_00417dd0(-1);REQUIRE(*(uint8_t *)0xe38eab==255&&*(uint8_t *)0xe38ea8==0);
}
int main(int argc,char **argv)
{
 REQUIRE(restore_data_open());
 if(argc==2&&!strcmp(argv[1],"--oracle-batch"))oracle();else {golden();printf("Campaign C: %u checks, %u lookup scenarios, explicit objects, signed categories, choice links, alternatives and full-width initial selection verified.\n",checks,lookups);}
 restore_data_close();return 0;
}
