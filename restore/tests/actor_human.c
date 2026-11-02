#include "actor_callbacks_human.h"
#include "standalone_data.h"
#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define I(a) (*(int32_t *)(uintptr_t)(a))
#define W(a) (*(uint16_t *)(uintptr_t)(a))
#define H(a) (*(int16_t *)(uintptr_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(a))
#define CHECK(v) do{if(!(v)){fprintf(stderr,"Human actor failure line%d\n",__LINE__);ExitProcess(1);}}while(0)
static uint32_t p[64],events[128][32],used,result,home,cap_address,route_address;
static const uint32_t regions[][2]={{0xd056f0,20},{0xe94df8,20},{0xbcb998,20},{0xeaac20,20},{0xd3b4d8,24},{0xbca2d8,20},{0x978d68,20},{0x78d7a4,24}};
static const uint32_t tags[]={0xf55d64,0xf55d5c,0xf55d04,0xf55cfc,0xf56144,0xf571c4,0xf56e14,0xf5613c,0xf571bc};
static uint32_t actor(uint32_t id){return 0xbcd5f8u+id*388u;}
static void event(uint32_t address,uint32_t count,const uint32_t *args){uint32_t *r,a=actor(U(0xd056f8));CHECK(used<128&&count<=6);r=events[used++];memset(r,0,128);r[0]=address;r[1]=count;if(count)memcpy(r+2,args,count*4);r[8]=U(0xd056f8);r[9]=B(a+56);r[10]=B(a+17);r[11]=W(a+54);r[12]=B(a+1);r[13]=B(a+14);r[14]=B(a+100);r[15]=B(a+64);r[16]=U(a+4);r[17]=U(a+384);r[18]=U(0xe94e00);r[19]=U(0xeaac28);r[20]=W(home+28);r[21]=W(home+30);r[22]=W(home+46);r[23]=B(home+135);r[24]=B(a+101);r[25]=B(a+12);r[26]=W(a+58);r[27]=B(a+114);r[28]=W(0xd3b4e4);r[29]=W(0xd3b4e0);r[30]=U(0xbcb9a0);}
#define EV(a,...) do{uint32_t v[]={__VA_ARGS__};event(a,sizeof(v)/4,v);}while(0)
static void mutate(uint32_t address){uint32_t flags=p[50],a,tile;if(address!=p[49])return;if(flags&1)U(0xd056f8)=p[2];a=actor(U(0xd056f8));if(flags&2)B(a+56)=p[46];if(flags&4)W(a+54)=p[47];if(flags&8)B(a+17)=p[48];if(flags&16)U(0xe94e00)=p[51];if(flags&32)U(0xeaac28)=p[52];if(flags&64)B(a+64)=p[53];if(flags&128){tile=U(actor(p[1])+32);U(0xaf598cu+tile*4u)^=4;}if(flags&256)W(home+46)=0;if(flags&512)B(a+1)=p[54];if(flags&1024)B(a+100)=p[57];if(flags&2048)U(0xbca2e0)=p[51];}
extern void restore_human_timer_body(int32_t),restore_human_route_body(int32_t);
extern uint8_t restore_human_boundary_body(int32_t,int32_t);
void FUN_004b1d50(int32_t id){EV(0x4b1d50,id);restore_human_timer_body(id);mutate(0x4b1d50);}
void FUN_004b3250(int32_t id){EV(0x4b3250,id);restore_human_route_body(id);mutate(0x4b3250);}
uint8_t restore_actor_human_boundary(int32_t x,int32_t y){uint8_t result0;EV(0x47a1c0,x,y);result0=restore_human_boundary_body(x,y);mutate(0x47a1c0);return result0;}
void FUN_004b17d0(int32_t id){EV(0x4b17d0,id);mutate(0x4b17d0);}
uint32_t FUN_0047fe60(int32_t x,int32_t y,uint32_t kind,int32_t distance){EV(0x47fe60,x,y,kind,distance);W(0xd3b4e4)=p[22];W(0xd3b4e0)=p[23];mutate(0x47fe60);return p[18];}
void FUN_004a5da0(int32_t id,uint32_t speed){EV(0x4a5da0,id,speed);B(actor(U(0xd056f8))+17)=p[20];mutate(0x4a5da0);}
void FUN_004b4220(int32_t id,int32_t x,int32_t y,int32_t target_x,int32_t target_y,int32_t mode){EV(0x4b4220,id,x,y,target_x,target_y,mode);B(actor((uint32_t)id)+64)=p[21];mutate(0x4b4220);}
int32_t FUN_004b3ff0(int32_t id,int32_t mode){EV(0x4b3ff0,id,mode);B(actor(U(0xd056f8))+64)=p[21];mutate(0x4b3ff0);return (int32_t)p[19];}
void FUN_0046cea0(int32_t id,int16_t kind){EV(0x46cea0,id,(int32_t)kind);mutate(0x46cea0);}
void FUN_004e90f0(int32_t player,int32_t amount){EV(0x4e90f0,player,amount);mutate(0x4e90f0);}
static void prepare(void){uint32_t i;used=result=0;for(i=0;i<sizeof(regions)/sizeof(regions[0]);++i)memset((void *)regions[i][0],(uint8_t)(0xa5^p[55]),regions[i][1]);
 home=0x7999d8u+(uint32_t)(int32_t)(int16_t)p[9]*264u;memset((void *)(home-8),0xa5,280);
 B(home)=p[10];B(home+3)=p[30];B(home+4)=p[33];B(home+5)=p[12];W(home+6)=p[28];W(home+8)=p[29];W(home+18)=p[31];W(home+28)=p[34];W(home+30)=p[61];W(home+32)=p[62];W(home+46)=p[11];B(home+135)=p[36];
 cap_address=0x70ce14u+(uint32_t)(int32_t)(int16_t)p[31]*88u;memset((void *)(cap_address-8),0xa5,20);U(cap_address)=p[32];route_address=0xd04e58u+(uint32_t)(int32_t)(int16_t)p[41]*2u;
 if((int16_t)p[41]>0){memset((void *)(route_address-8),0xa5,20);W(route_address)=p[42];}
 for(i=0;i<3;++i){uint32_t id=p[1+i],a=actor(id),tile=p[13]+i*228u;memset((void *)(a-8),0xa5,404);B(a+1)=p[6];U(a+4)=0x12345678+i;B(a+10)=p[8]+i;B(a+12)=3;B(a+14)=p[59];B(a+17)=p[7];W(a+20)=p[26]+i;W(a+22)=p[27]+i;U(a+32)=tile;W(a+36)=123;W(a+38)=234;W(a+54)=p[5];B(a+56)=p[4];B(a+57)=0x55;W(a+58)=p[41];B(a+64)=p[21];W(a+68)=0x1234;W(a+74)=p[24]+i;W(a+76)=p[25]+i;W(a+92)=(p[9]+i)%4000u;B(a+100)=p[35]+i;B(a+101)=0x55;B(a+108)=0x55;B(a+109)=0x55;B(a+114)=p[60];U(a+384)=p[58]+i;U(0x7090ecu+(uint32_t)B(a+10)*56u)=p[17]+i*256u;U(0xaf598cu+tile*4u)=p[14]^(i?4u:0u);B(0xadc36cu+tile)=p[15];}
 for(i=0;i<9;++i)U(tags[i])=p[45]+i*256u;
 for(i=0;i<8;++i){memset((void *)(0xea3fe4u+i*0x49d8u),0xa5,12);W(0xea3fe8u+i*0x49d8u)=p[39]+i;W(0xea3feau+i*0x49d8u)=p[40]+i;}
 U(0xd056f8)=p[1];U(0xbca2e0)=p[16];U(0x978d70)=p[37];U(0xeaac28)=p[38];W(0xd3b4e4)=0x1234;W(0xd3b4e0)=0x2345;U(0x78d7ac)=p[43];U(0x78d7b0)=p[44];
}
static void run(void){prepare();switch(p[0]){case 0:restore_actor_00489ca0();break;case 1:restore_actor_0048a260();break;case 2:FUN_004b1d50((int32_t)p[1]);break;case 3:FUN_004b3250((int32_t)p[1]);break;case 4:result=restore_actor_human_boundary((int16_t)p[26],(int16_t)p[27]);break;default:CHECK(0);}}
static void hex(uint32_t address,uint32_t n){static const char d[]="0123456789abcdef";uint32_t i;for(i=0;i<n;++i){uint8_t x=B(address+i);putchar(d[x>>4]);putchar(d[x&15]);}}
static void output(void){uint32_t i,j;printf("{\"result\":%u,\"state\":\"",result);for(i=0;i<sizeof(regions)/sizeof(regions[0]);++i)hex(regions[i][0],regions[i][1]);for(i=0;i<3;++i){uint32_t a=actor(p[1+i]),tile=p[13]+i*228u;hex(a-8,404);hex(0xaf598cu+tile*4u-8,20);hex(0xadc36cu+tile-4,9);hex(0x7090ecu+(uint32_t)(uint8_t)(p[8]+i)*56u-8,20);}hex(home-8,280);hex(cap_address-8,20);if((int16_t)p[41]>0)hex(route_address-8,20);for(i=0;i<8;++i)hex(0xea3fe4u+i*0x49d8u,12);printf("\",\"events\":[");for(i=0;i<used;++i){printf("%s[",i?",":"");for(j=0;j<32;++j)printf("%s%u",j?",":"",events[i][j]);putchar(']');}puts("]}");fflush(stdout);}
int main(int argc,char **argv){uint32_t i;CHECK(restore_data_open());if(argc==2&&!strcmp(argv[1],"--oracle-batch")){puts("{\"fields\":64}");fflush(stdout);for(;;){for(i=0;i<64;++i)if(scanf("%u",p+i)!=1)goto done;run();output();}}else{p[1]=1;p[2]=256;p[3]=511;p[4]=8;p[5]=1;p[6]=255;p[8]=1;p[9]=1;p[10]=1;p[11]=1;p[12]=1;p[13]=2300;p[17]=12;p[18]=256;p[22]=10;p[23]=20;p[26]=30;p[27]=40;p[28]=31;p[29]=41;p[30]=2;p[32]=20;p[35]=10;p[39]=20;p[40]=30;p[43]=228;p[45]=1000;p[58]=0x12345678;p[59]=1;p[60]=255;run();CHECK(B(actor(1)+56)==9&&W(actor(1)+36)==10&&W(actor(1)+38)==20&&U(actor(1)+384)==0&&B(actor(1)+114)==0);puts("Human actor callbacks: WORD countdown, full-EAX route result,388-byte actor layout and frame guards smoke passed.");}done:restore_data_close();return 0;}
