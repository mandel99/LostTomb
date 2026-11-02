#include "text_selection.h"
#include "standalone_data.h"
#include <windows.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define BASE 0x30001000u
#define REQUIRE(x) do{if(!(x)){fprintf(stderr,"Text selection failure at line %d\n",__LINE__);abort();}}while(0)
static const uint32_t offsets[4]={16,80,160,256};
static uint32_t label[5];
extern void FUN_004ccbb0(int,int,uint32_t,uint32_t,uint32_t,uint32_t);
void FUN_004cc770(uint32_t text,uint32_t x,uint32_t y,uint32_t font,uint32_t color)
{label[0]=text;label[1]=x;label[2]=y;label[3]=font;label[4]=color;}
static void prepare(void)
{
 unsigned int i;unsigned char *data=(unsigned char *)BASE;
 memset(data,'Q',512);
 memcpy(data+16,"Alpha\0Beta\0Gamma\0Tail\0",22);
 memcpy(data+80,"\1\0Delta\0Epsilon\0Tail\0",21);
 memcpy(data+160,"\0\0Oak\0 Pine\0Tail\0",17);
 memcpy(data+256,"\200\377 \0last\0tail\0",15);
 for(i=0;i<4;++i){data[offsets[i]-1]=0;U(0xe34c38u+i*8)=offsets[i];U(0xe34c3cu+i*8)=0xeeeeeeee;}
 U(0xe36c70)=BASE;U(0xe20768)=0xdeadbeef;U(0xe20764)=0xfeedface;
}
int main(int argc,char **argv)
{
 unsigned int op,group,entry,i,cases=0;
 REQUIRE(restore_data_open());REQUIRE(VirtualAlloc((void *)0x30000000u,0x10000,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE)==(void *)0x30000000u);
 if(argc==2&&!strcmp(argv[1],"--oracle-batch")) {
  puts("{\"base\":805310464}");fflush(stdout);
  while(scanf("%u %u %u",&op,&group,&entry)==3){prepare();REQUIRE(group<4);if(op)FUN_004cc160(group,(int32_t)entry);else FUN_004cc110(group,(int32_t)entry);
   printf("{\"primary\":%u,\"secondary\":%u}\n",U(0xe20768),U(0xe20764));fflush(stdout);}
 } else {
  for(op=0;op<2;++op)for(group=0;group<4;++group)for(entry=0;entry<3;++entry){prepare();if(op)FUN_004cc160(group,entry);else FUN_004cc110(group,entry);
   REQUIRE(U(op?0xe20768:0xe20764)==(op?0xdeadbeef:0xfeedface));REQUIRE(U(op?0xe20764:0xe20768)>=BASE+offsets[group]);++cases;}
  prepare();FUN_004cc110(0,1);REQUIRE(!strcmp((char *)(uintptr_t)U(0xe20768),"Beta"));
  prepare();FUN_004cc160(1,1);REQUIRE(!strcmp((char *)(uintptr_t)U(0xe20764),"Epsilon"));
  prepare();FUN_004cc110(2,2);REQUIRE(!strcmp((char *)(uintptr_t)U(0xe20768),"Oak"));
  prepare();FUN_004cc110(3,-1);REQUIRE(*(unsigned char *)(uintptr_t)U(0xe20768)==0x80);
  for(i=0;i<10000;++i){FUN_004cc110(0,2);REQUIRE(!strcmp((char *)(uintptr_t)U(0xe20768),"Gamma"));}
  prepare();U(0xd3b504)=0xabcdef01;FUN_004ccbb0(1,1,500,300,18765,0xeeee);
  REQUIRE(!strcmp((char *)(uintptr_t)label[0],"Epsilon")&&label[1]==500&&label[2]==300&&label[3]==18765&&label[4]==0xeeee&&U(0xd3b504)==0);
  printf("Text selection C: %u matrix cases, full pointer slots, eight-byte group stride, byte scans, controls and repeated lookups verified.\n",cases);
 }
 VirtualFree((void *)0x30000000u,0,MEM_RELEASE);restore_data_close();return 0;
}
