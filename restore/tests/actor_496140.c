#include "actor_callback_496140.h"
#include "standalone_data.h"
#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define W(a) (*(uint16_t *)(uintptr_t)(a))
#define H(a) (*(int16_t *)(uintptr_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(a))
#define ALT 0x30010000u
#define CHECK(v) do{if(!(v)){fprintf(stderr,"Actor496140 failure%d\n",__LINE__);ExitProcess(1);}}while(0)
static uint32_t p[64],events[256][32],used,result,home,queue_self,route_address;
static const uint32_t regions[][2]={{0xd056f0,20},{0xe94df8,20},{0xbca2d8,20}};
static uint32_t actor(uint32_t id){return 0xbcd5f8u+id*388u;}
static void event(uint32_t address,uint32_t n,const uint32_t *args){uint32_t *r,a=actor(U(0xd056f8));CHECK(used<256&&n<=4);r=events[used++];memset(r,0,128);r[0]=address;r[1]=n;if(n)memcpy(r+2,args,n*4);r[8]=U(0xd056f8);r[9]=B(a+56);r[10]=B(a+17);r[11]=W(a+54);r[12]=B(a+1);r[13]=B(a+14);r[14]=B(a+114);r[15]=B(a+12);r[16]=U(a+4);r[17]=U(a+384);r[18]=U(0xe94e00);r[19]=W(a+58);r[20]=W(a+36);r[21]=W(a+38);r[22]=W(a+66);r[23]=W(a+90);r[24]=W(a+94);r[25]=B(a+147);r[26]=U(a+156);r[27]=U(a+160);r[28]=U(a+172);r[29]=W(home+44);r[30]=W(home+182);r[31]=B(home);}
#define EV(a,...) do{uint32_t v[]={__VA_ARGS__};event(a,sizeof(v)/4,v);}while(0)
static void mutate(uint32_t at){uint32_t a,flags=p[36],i;if(at!=p[35])return;if(flags&1)U(0xd056f8)=p[2];a=actor(U(0xd056f8));if(flags&2)B(a+56)=p[37];if(flags&4)W(a+54)=p[38];if(flags&8)B(a+17)=p[39];if(flags&16)U(0xbca2e0)=p[41];if(flags&32)B(a+1)=p[42];if(flags&64)W(a+94)=p[43];if(flags&128)B(a+147)=p[44];if(flags&256)for(i=0;i<4;++i)U(a+156+i*4)=p[45+i];if(flags&512)W(a+90)=p[60];}
extern void restore_496140_timer_body(int32_t),restore_496140_route_body(int32_t),restore_496140_shop_body(int32_t,uint32_t,uint32_t);
extern uint32_t restore_496140_wait_body(int32_t),restore_496140_queue_body(const void *,uint32_t);
void FUN_004b1d50(int32_t id){EV(0x4b1d50,id);restore_496140_timer_body(id);mutate(0x4b1d50);}
void FUN_004b3250(int32_t id){EV(0x4b3250,id);restore_496140_route_body(id);mutate(0x4b3250);}
void FUN_00496060(int32_t id,uint32_t building,uint32_t target){EV(0x496060,id,building,target);restore_496140_shop_body(id,building,target);mutate(0x496060);}
uint32_t FUN_004a35e0(int32_t id){uint32_t r;EV(0x4a35e0,id);r=restore_496140_wait_body(id);mutate(0x4a35e0);return r;}
uint32_t restore_actor_496140_queue_contains(const void *self,uint32_t id){uint32_t r;EV(0x4f6570,(uint32_t)self,id);r=restore_496140_queue_body(self,id);mutate(0x4f6570);return r;}
void FUN_004b17d0(int32_t id){EV(0x4b17d0,id);mutate(0x4b17d0);}
void FUN_004a5da0(int32_t id,uint32_t speed){EV(0x4a5da0,id,speed);B(actor(U(0xd056f8))+17)=p[18];mutate(0x4a5da0);}
uint32_t FUN_00495cf0(int32_t id,uint32_t building,uint32_t target,int32_t slot){EV(0x495cf0,id,building,target,slot);mutate(0x495cf0);return p[34];}
uint32_t FUN_00496010(uint32_t id,uint32_t building,uint32_t target,int32_t slot){EV(0x496010,id,building,target,slot);mutate(0x496010);return p[34];}
void FUN_00495e10(int32_t id,int32_t building,uint32_t target){EV(0x495e10,id,building,target);mutate(0x495e10);}
void FUN_00495f00(int32_t id,int32_t building,uint32_t target){EV(0x495f00,id,building,target);mutate(0x495f00);}
static void fill_queue(uint32_t q){uint32_t i;U(q)=p[33];U(q+4)=p[29];U(q+8)=p[30];U(q+12)=p[28];for(i=0;i<51;++i)U(q+20+i*4)=0x12340000+i;if(p[32]<51)U(q+20+p[32]*4)=p[31];}
static void prepare(void){uint32_t i,j;used=result=0;for(i=0;i<3;++i)memset((void *)regions[i][0],(uint8_t)(0xa5^p[57]),regions[i][1]);
 home=0x7999d8u+(uint32_t)(int32_t)(int16_t)p[9]*264u;memset((void *)(home-8),0xa5,280);B(home)=p[10];W(home+44)=p[11];W(home+182)=p[12];
 memset((void *)0xeaaf60u,0xa5,50*224+16);for(i=0;i<50;++i)memset((void *)(0xeaaf68u+i*224u),0,224);
 if(p[27]<50)fill_queue(0xeaaf68u+p[27]*224u);memset((void *)(ALT-8),0xa5,240);fill_queue(ALT);queue_self=p[58]?ALT:0xeaaf68u+(p[27]<50?p[27]:0)*224u;
 route_address=0xd04e58u+(uint32_t)(int32_t)(int16_t)p[19]*2u;if((int16_t)p[19]>0){memset((void *)(route_address-8),0xa5,20);W(route_address)=p[20];}
 for(i=0;i<3;++i){uint32_t a=actor(p[1+i]);memset((void *)(a-8),0xa5,404);B(a+1)=p[6];U(a+4)=p[50]+i;B(a+10)=p[8]+i;B(a+12)=p[54];B(a+14)=p[52];B(a+17)=p[7];W(a+36)=0x1234;W(a+38)=0x2345;W(a+44)=p[13]+i;W(a+46)=p[14]+i;W(a+54)=p[5];B(a+56)=p[4];W(a+58)=p[19];W(a+66)=p[55];W(a+90)=(p[9]+i)%4000;W(a+94)=p[15]+i;B(a+114)=p[53];B(a+147)=p[22];for(j=0;j<4;++j){U(a+156+j*4)=p[23+j];U(a+172+j*4)=0x12345678+j;}U(a+384)=p[51]+i;U(0x7090ecu+(uint32_t)(uint8_t)(p[8]+i)*56u)=p[17]+i*256u;}
 U(0xd056f8)=p[1];U(0xbca2e0)=p[16];U(0xf55d74)=p[21];U(0xf55d6c)=p[21]+256;
}
static void run(void){prepare();switch(p[0]){case 0:restore_actor_00496140();break;case 1:FUN_00496060((int32_t)p[1],p[9],p[15]);break;case 2:result=(uint8_t)FUN_004a35e0((int32_t)p[1]);break;case 3:result=restore_actor_496140_queue_contains((void *)queue_self,p[1]);break;case 4:FUN_004b1d50(p[1]);break;case 5:FUN_004b3250(p[1]);break;default:CHECK(0);}}
static void hex(uint32_t a,uint32_t n){static const char d[]="0123456789abcdef";char b[8192];uint32_t i,k;while(n){k=n>4096?4096:n;for(i=0;i<k;++i){uint8_t x=B(a+i);b[2*i]=d[x>>4];b[2*i+1]=d[x&15];}CHECK(fwrite(b,2,k,stdout)==k);n-=k;a+=k;}}
static void output(void){uint32_t i,j;printf("{\"result\":%u,\"state\":\"",result);for(i=0;i<3;++i)hex(regions[i][0],regions[i][1]);for(i=0;i<3;++i){hex(actor(p[1+i])-8,404);hex(0x7090ecu+(uint32_t)(uint8_t)(p[8]+i)*56u-8,20);}hex(home-8,280);if((int16_t)p[19]>0)hex(route_address-8,20);hex(0xeaaf60u,50*224+16);hex(ALT-8,240);printf("\",\"events\":[");for(i=0;i<used;++i){printf("%s[",i?",":"");for(j=0;j<32;++j)printf("%s%u",j?",":"",events[i][j]);putchar(']');}puts("]}");fflush(stdout);}
int main(int argc,char **argv){uint32_t i;CHECK(restore_data_open());CHECK(VirtualAlloc((void *)0x30000000u,0x20000,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE)==(void *)0x30000000u);if(argc==2&&!strcmp(argv[1],"--oracle-batch")){puts("{\"fields\":64}");fflush(stdout);for(;;){for(i=0;i<64;++i)if(scanf("%u",p+i)!=1)goto done;run();output();}}else{p[1]=1;p[2]=256;p[3]=511;p[4]=0;p[6]=255;p[8]=39;p[9]=1;p[10]=1;p[11]=1;p[13]=123;p[14]=234;p[15]=345;p[17]=12;p[19]=1;p[20]=1;p[21]=1000;p[22]=0;p[23]=p[24]=p[25]=p[26]=0xffffffff;p[27]=0xffffffff;p[28]=5;p[31]=1;p[32]=0xffffffff;p[33]=256;p[50]=0x12345678;p[51]=0x87654321;p[52]=1;run();CHECK(B(actor(1)+56)==9&&W(actor(1)+36)==123&&W(actor(1)+38)==234&&W(actor(1)+58)==0&&W(actor(1)+66)==800&&B(actor(1)+114)==1&&U(actor(1)+384)==0x87654321);puts("Actor496140: returning-goal WORDs, actual route release, shadow flag and retained counter smoke passed.");}done:VirtualFree((void *)0x30000000u,0,MEM_RELEASE);restore_data_close();return 0;}
