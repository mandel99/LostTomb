#include "event_publisher.h"
#include "standalone_data.h"
#include <stdlib.h>
#define BASE 0x30000000u
#define TEXT (BASE+0x10000u)
#define MM (BASE+0x100000u)
#define ALT (BASE+0x200000u)
#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define W(a) (*(uint16_t *)(uintptr_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(a))
#define REQUIRE(x) do{if(!(x)){fprintf(stderr,"Event publisher failure line%d\n",__LINE__);ExitProcess(1);}}while(0)
static uint32_t p[24],events[128][16],count,open_count,alloc_count,positions[3],message,result;
static uint8_t *files[3];static uint32_t lengths[3];
static const uint32_t regions[][2]={{0x5c7558,16},{0x7027b0,48},{0x78d7b8,16},{0xd3a6e0,0xb00},{0xe36c68,24},{0xe38dc0,0xd0},{0xe95070,0x1a0},{0xe95220,48016},{0xea0da0,16},{0xf54110,16}};
static void event(uint32_t address,uint32_t n,const uint32_t *args){uint32_t *r;REQUIRE(count<128&&n<=5);r=events[count++];memset(r,0,64);r[0]=address;r[1]=n;if(n)memcpy(r+2,args,n*4);r[8]=U(0xe950dc);r[9]=B(0xe38dda);r[10]=U(0xe951ec);r[11]=U(0xe951e8);r[12]=U(0xe36c74);r[13]=U(0xd3a6e8);}
#define EV(a,...) do{uint32_t args[]={__VA_ARGS__};event(a,sizeof(args)/4,args);}while(0)
uint32_t FUN_004e0f90(int32_t id,const int16_t *params){EV(0x4e0f90,id,(uint32_t)params);return p[5];}
void FUN_0053b090(int32_t id){EV(0x53b090,id);}
void FUN_004ca980(void){event(0x4ca980,0,NULL);if(p[16]&1)U(0xe36c74)=ALT;}
void FUN_004c8040(void){event(0x4c8040,0,NULL);memset((void *)0xd3a6f8u,0,1000);U(0xd3a6ec)=U(0xd3a6f0)=0;}
#define ZERO(name,address) void name(void){event(address,0,NULL);}
ZERO(FUN_00417a70,0x417a70) ZERO(FUN_00404350,0x404350) ZERO(FUN_004051a0,0x4051a0) ZERO(FUN_00417aa0,0x417aa0) ZERO(FUN_004179f0,0x4179f0) ZERO(FUN_0051e990,0x51e990)
/* The authenticated original zero body is composed; the caller's unused stack argument is logged. */
#define FUN_0047d350 restore_event_native_zero
#include "../src/unit_01130.c"
#undef FUN_0047d350
uint32_t FUN_0047d350(int32_t value){EV(0x47d350,value);return restore_event_native_zero();}
uint32_t FUN_00568eab(uint32_t name,int flags){uint32_t h;EV(0x568eab,name,flags);++open_count;if(p[0]==4||p[0]==6){h=name==BASE+0x100?1:2;if((p[14]==1&&h==1)||(p[14]==2&&h==2))return 0xffffffff;positions[h]=0;return h;}return p[14]==1||(p[14]==2&&open_count==1)?0xffffffff:3;}
uint32_t FUN_005647eb(uint32_t fd){EV(0x5647eb,fd);return 0;}
uint32_t FUN_0056b1b6(uint32_t fd){EV(0x56b1b6,fd);REQUIRE(fd==1||fd==2);return lengths[fd];}
void *FUN_00561a1d(uint32_t n){uint32_t out=++alloc_count==1?TEXT:MM;EV(0x561a1d,n);REQUIRE(n<0xe0000);return (void *)(uintptr_t)out;}
void FUN_00561a2b(uint32_t address){EV(0x561a2b,address);}
int FUN_005660ed(uint32_t fd,uint32_t buffer,uint32_t n){EV(0x5660ed,fd,buffer,n);REQUIRE(fd==1||fd==2);REQUIRE(n<=lengths[fd]-positions[fd]);memcpy((void *)(uintptr_t)buffer,files[fd]+positions[fd],n);positions[fd]+=n;return n;}
bool FUN_004ce100(uint32_t name){EV(0x4ce100,name);if(p[16]&2)U(0xe951dc)=message+1;return 1;}
void FUN_004ce2d0(void){event(0x4ce2d0,0,NULL);}
extern uint32_t FUN_004cec40(uint32_t,uint32_t);
extern bool FUN_004ce3f0(uint32_t);
static void load_file(unsigned id,const char *name){FILE *f=fopen(name,"rb");REQUIRE(f);fseek(f,0,SEEK_END);lengths[id]=ftell(f);rewind(f);files[id]=malloc(lengths[id]);REQUIRE(files[id]&&fread(files[id],1,lengths[id],f)==lengths[id]);fclose(f);}
static uint32_t run(void){uint32_t i,slot=p[2],params=0;count=open_count=alloc_count=0;memset(positions,0,sizeof(positions));memset((void *)BASE,0xa5,0x300000);strcpy((char *)(BASE+0x100),"Pharaoh_Text.eng");strcpy((char *)(BASE+0x200),"Pharaoh_MM.eng");strcpy((char *)(MM+0x400),"fixture.smk");strcpy((char *)(ALT+0x400),"alternate.smk");for(i=0;i<sizeof(regions)/sizeof(regions[0]);++i)memset((void *)regions[i][0],0xa5,regions[i][1]);
 memset((void *)0xe34c30u,0xa5,8036);message=p[0]==1||p[0]==6?U(0x5794c4+p[21]*28):p[1];REQUIRE(p[0]==2||message<1000);memset((void *)0xe20998u,0xa5,0x138a8);if(message<1000){RestoreMessageRecord *r=restore_event_layout(message);r->subtype=(int16_t)p[12];*(uint32_t *)((uint8_t *)r+56)=p[13];r->media=0x400;restore_event_layout(message+1)->media=0;}
 for(i=0;i<1000;++i)restore_event_at(i)->message=1;if(slot<1000)restore_event_at(slot)->message=0;
 U(0xe950dc)=0x5a010000;U(0xe950e0)=p[6];U(0xe951f0)=p[8];U(0xe951ec)=p[9];U(0xe951e8)=0x12345678;U(0xe36c70)=p[20]?BASE+0x4000:0;U(0xe36c74)=p[0]==4||p[0]==6?(p[20]?BASE+0x5000:0):MM;B(0xe38dda)=p[3];W(0x78d7bc)=p[10];B(0x78d7c0)=p[11];
 for(i=0;i<20;++i)U(0xe95080+i*4)=0x100+i;if(p[15]<20)U(0xe95080+p[15]*4)=0;
 if(p[7]){params=BASE+0x800;if(p[7]>=3&&slot<1000)params=0xe95228+slot*48+(p[7]==3?12:18);for(i=0;i<30;++i)B(params+i)=(uint8_t)(p[19]+i*11);W(params)=p[7]==2?0xffff:17;}
 result=0;switch(p[0]){case 0:FUN_004e0cf0(message,p[17],p[18],(int16_t *)params,(uint8_t)p[4]);break;case 1:FUN_00417ae0(p[21]);break;case 2:FUN_004e14b0(p[1]);break;case 3:result=FUN_004e1750();break;case 4:result=FUN_004cec40(BASE+0x100,BASE+0x200);break;case 5:result=FUN_004ce3f0(MM+0x400);break;case 6:result=FUN_004cec40(BASE+0x100,BASE+0x200);if(result)FUN_00417ae0(p[21]);break;default:REQUIRE(0);}
 return result;
}
static void printbytes(uint32_t a,uint32_t n){static const char hex[]="0123456789abcdef";char buffer[8192];uint32_t i,k;while(n){k=n>4096?4096:n;for(i=0;i<k;++i){uint8_t v=B(a+i);buffer[i*2]=hex[v>>4];buffer[i*2+1]=hex[v&15];}REQUIRE(fwrite(buffer,2,k,stdout)==k);a+=k;n-=k;}}
static void output(void){uint32_t i,j;printf("{\"result\":%u,\"state\":\"",result);for(i=0;i<sizeof(regions)/sizeof(regions[0]);++i)printbytes(regions[i][0],regions[i][1]);if(p[0]!=2)printbytes(0xe20998,0x138a8);printbytes(BASE+0x7f8,48);if(p[0]==4||p[0]==6){printbytes(0xe34c30,8036);printbytes(TEXT-8,lengths[1]-0x1f5c+16);printbytes(MM-8,lengths[2]-0x13898+16);}printf("\",\"events\":[");for(i=0;i<count;++i){printf("%s[",i?",":"");for(j=0;j<16;++j)printf("%s%u",j?",":"",events[i][j]);putchar(']');}puts("]}");fflush(stdout);}
int main(int argc,char **argv){uint32_t i;REQUIRE(restore_data_open());REQUIRE(VirtualAlloc((void *)BASE,0x300000,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE)==(void *)BASE);load_file(1,"build/debug-startup-game/Pharaoh_Text.eng");load_file(2,"build/debug-startup-game/Pharaoh_MM.eng");if(argc==2&&!strcmp(argv[1],"--oracle-batch")){puts("{\"fields\":24}");fflush(stdout);for(;;){for(i=0;i<24;++i)if(scanf("%u",p+i)!=1)goto done;run();output();}}else{p[0]=6;p[2]=0;p[3]=1;p[5]=1;p[8]=1;p[10]=(uint32_t)-1500;p[11]=7;p[15]=20;p[21]=15;run();REQUIRE(result==1&&U(0xe36c74)==MM&&U(0xd3b124)==249&&B(0xe38dda)==5&&restore_event_at(0)->message==249);puts("Event publisher: real language-file load, tutorial249, typed48byte event and message setup passed.");}done:free(files[1]);free(files[2]);VirtualFree((void *)BASE,0,MEM_RELEASE);restore_data_close();return 0;}
