#include "tool_selector.h"
#include "standalone_data.h"
#include "sprite_libraries.h"
#define U TS_U
#define I TS_I
#define W TS_W
#define B TS_B
#define CHECK(v) do{if(!(v)){fprintf(stderr,"Tool selector failure line%d\n",__LINE__);ExitProcess(1);}}while(0)
static uint32_t p[32],events[8192][20],used,result,loader_calls;
static const uint32_t regions[][2]={{0x6059b0,192},{0x7056dc,32},{0xbc9708,32},{0xbc97f8,16},{0xbca2bc,96},{0xd3b190,20},{0xe38dc4,32},{0x784b8c,20},{0x5e6968,20},{0x57613c,20},{0xeaac20,24},{0xe92e04,20}};
static uint32_t actor,record,entry;
extern int32_t restore_tool_row_count_body(void);
extern void restore_tool_release_body(int32_t),restore_tool_media_body(void);
static void event(uint32_t a,uint32_t n,const uint32_t *v){uint32_t *r;CHECK(used<8192);r=events[used++];memset(r,0,80);r[0]=a;r[1]=n;if(n)memcpy(r+2,v,n*4);r[6]=U(0x7056ec);r[7]=U(0x6059e8);r[8]=U(0x605a48);r[9]=U(0xbca310);r[10]=U(0xeaac28);r[11]=W(0xea5baau+U(0xeaac28)*0x49d8u);r[12]=U(0xbc9710);r[13]=B(0xe38dda);r[14]=U(0x7056f0);r[15]=U(0x6059cc);r[16]=U(0x6059b4);r[17]=U(0xbc9714);}
#define E0(a) event(a,0,NULL)
#define EV(a,...) do{uint32_t v[]={__VA_ARGS__};event(a,sizeof(v)/4,v);}while(0)
int32_t FUN_00404120(void){E0(0x404120);return restore_tool_row_count_body();}
void FUN_00486060(int32_t index){EV(0x486060,index);restore_tool_release_body(index);if(p[26]&1){U(0xbca310)=2;U(0x7056ec)=p[27];}}
void FUN_0053d850(void){E0(0x53d850);restore_tool_media_body();}
uint32_t FUN_00403ef0(void){E0(0x403ef0);if(p[26]&2)U(0x7056ec)=p[27];if(p[26]&4)U(0xeaac28)=p[28];return p[17]|(p[16]&1);}
uint32_t FUN_004f5470(int32_t building){uint32_t bit=building==0xb3?1:building==0xb2?2:building==0xb1?3:4;EV(0x4f5470,building);return p[17]|((p[16]>>bit)&1);}
int32_t FUN_004f5020(void){E0(0x4f5020);return p[18];}
int32_t FUN_004f4ff0(void){E0(0x4f4ff0);return p[19];}
uint8_t FUN_004c52d0(int32_t building,int32_t phase){uint32_t bit=phase==1?5:phase==2?6:7;EV(0x4c52d0,building,phase);if((p[26]&32)&&phase==1)U(0xeaac28)=p[28];return (uint8_t)(p[17]|((p[16]>>bit)&1));}
uint8_t FUN_0053dd90(RestoreSpriteLibraries *self,const char *name,int index){EV(0x53dd90,(uint32_t)self,(uint32_t)name,index);if(!loader_calls++){if(p[26]&8)U(0xeaac28)=p[28];if(p[26]&16)U(0x6059e8)=p[29];}return 1;}
static void rows(uint32_t category){uint32_t i;for(i=0;i<31;++i){U(0x5ce3b8u+(category*30u+i)*4u)=i&&i<=p[14]?p[30]:0;U(0x70a8dcu+(category*30u+i)*4u)=p[15];}}
static void prepare(void){uint32_t i;used=result=loader_calls=0;for(i=0;i<sizeof(regions)/sizeof(regions[0]);++i)memset((void *)regions[i][0],(uint8_t)(0xa5^p[31]),regions[i][1]);
 for(i=0;i<38;++i)rows(i);rows(p[1]);rows(p[27]);
 actor=0x7999d8u+p[3]*264u;memset((void *)(actor-8),0xa5,280);
 record=0x5d19c8u+(uint32_t)(int32_t)(int16_t)p[12]*16u;memset((void *)(record-8),0xa5,32);W(record)=p[9];W(record+2)=p[10];W(record+4)=p[11];W(0x784ddeu+p[2]*2u)=p[12];
 entry=0x5ce3b8u+(p[1]*30u+p[2])*4u;if(p[0]==0&&p[1]!=0x1c)U(entry)=p[13];
 for(i=0;i<31;++i){U(0x5cf63cu+i*8u)=p[24]+i;U(0x5cf640u+i*8u)=p[25]+i;W(0x784de0u+i*2u)=p[21];}
 /* The selector's WORD lookup may overlap the availability list. Reinstall it last. */
 W(0x784ddeu+p[2]*2u)=p[12];
 for(i=0;i<8;++i){uint32_t a=i*0x49d8u;memset((void *)(0xea5ba6u+a),0xa5,12);memset((void *)(0xea5ce4u+a),0xa5,12);memset((void *)(0xea5f98u+a),0xa5,144);W(0xea5baau+a)=i==p[6]?(uint16_t)p[7]:1;W(0xea5ce8u+a)=p[8];U(0xea5f9cu+a)=p[20]+i;for(uint32_t j=0;j<30;++j)U(0xea5fa0u+a+j*4u)=p[22];}
 U(0x7056ec)=p[1];U(0xd3b198)=p[2];U(0xbc9800)=p[3];U(0xbca310)=p[4];U(0x6059e8)=p[5];U(0xeaac28)=p[6];U(0xe92e0c)=0x12345678;U(0xbca2e0)=p[23];
}
static void run(void){prepare();switch(p[0]){case 0:FUN_00405200();break;case 1:result=FUN_00404120();break;case 2:FUN_00486060(p[3]);break;case 3:FUN_0053d850();break;default:CHECK(0);}}
static void hex(uint32_t address,uint32_t n){static const char d[]="0123456789abcdef";uint32_t i;for(i=0;i<n;++i){uint8_t x=B(address+i);putchar(d[x>>4]);putchar(d[x&15]);}}
static void output(void){uint32_t i,j;printf("{\"result\":%u,\"state\":\"",result);for(i=0;i<sizeof(regions)/sizeof(regions[0]);++i)hex(regions[i][0],regions[i][1]);hex(actor-8,280);hex(record-8,32);hex(entry,4);hex(0x784ddeu+p[2]*2u,2);for(i=0;i<8;++i){uint32_t a=i*0x49d8u;hex(0xea5ba6u+a,12);hex(0xea5ce4u+a,12);hex(0xea5f98u+a,144);}printf("\",\"events\":[");for(i=0;i<used;++i){printf("%s[",i?",":"");for(j=0;j<20;++j)printf("%s%u",j?",":"",events[i][j]);putchar(']');}puts("]}");fflush(stdout);}
int main(int argc,char **argv){uint32_t i;CHECK(restore_data_open());if(argc==2&&!strcmp(argv[1],"--oracle-batch")){puts("{\"fields\":32}");fflush(stdout);for(;;){for(i=0;i<32;++i)if(scanf("%u",p+i)!=1)goto done;run();output();}}else{p[1]=1;p[2]=1;p[3]=1;p[4]=1;p[5]=0x12345678;p[7]=0x1234;p[9]=1;p[12]=1;p[13]=0x6a;p[14]=3;p[15]=256;p[20]=10;p[21]=256;p[24]=0x12345678;p[25]=0x87654321;p[30]=1;run();CHECK(U(0x605a48)==0x405b80&&B(actor)==6&&B(actor+1)==0xa5&&U(0xbca310)==0);puts("Tool selector: full rule token, actor BYTE/264 stride and adjacent canaries smoke passed.");}done:restore_data_close();return 0;}
