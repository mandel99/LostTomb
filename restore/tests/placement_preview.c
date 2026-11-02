#include "placement_preview.h"
#include "standalone_data.h"
#include "preview_footprint_seeds.h"
#include "startup_geometry.h"
#include <stdlib.h>
#define BASE 0x30000000u
#define ALT ((RestoreSpriteLibraries *)(BASE+0x10000u))
#define OBJ (BASE+0x1000u)
#define SRC (BASE+0x400000u)
#define ACTOR (BASE+0x500000u)
#define OUT (BASE+0x600000u)
#define I PV_I
#define U PV_U
#define W PV_W
#define B PV_B
#define REQUIRE(x) do{if(!(x)){fprintf(stderr,"Placement preview failure%d\n",__LINE__);ExitProcess(1);}}while(0)
static uint32_t p[64],events[16384][24],event_count,region_calls,result,current_record;
static const uint32_t objects[]={0x6fd5d8,0x6de650,0x6fd368,0x6f9320,0x6f5648,0x6de9e0,0x6e9188,0x6de548};
static const uint32_t vtables[]={0x56f48c,0x56f438,0x56f4dc,0x570160,0x570268,0x570228,0x5701e8,0x56f6e8};
static const uint32_t adapters[]={0x40d870,0x401420,0x416f90,0x5456e0,0x558b80,0x555e20,0x54f410,0x4e6610};
static const uint32_t offsets[]={0x4c,0x4c,0x4c,0x1e5c,0x1cbc,0x4ee0,0x5c88,0x4c};
static const uint32_t counts[]={25,16,9,167,158,437,513,2};
static const uint32_t tags[]={0xf564dc,0xf5653c,0xf56544,0xf566ac,0xf566b4,0xf56764,0xf56b24,0xf56b84,0xf56c34,0xf56c44,0xf56c94,0xf56d34,0xf56d54,0xf56d5c,0xf56d64};
static const uint32_t regions[][2]={{0x576140,12},{0x605980,240},{0x6de50c,16},{0x70e118,336},{0x70e440,84},{0x705c68,12},{0xbc96f0,152},{0xbca2c8,152},{0xd3b4d8,280},{0xe38dc8,12},{OUT,64}};
static uint32_t watch_object,watch_bytes,lookup_address,definition;
static int member_index(uint32_t mode){switch(mode){case 0xe1:return 0;case 0xe6:return 1;case 0xe3:return 2;case 0xe5:return 3;case 0xea:return 4;case 0xeb:return 5;case 0xec:return 6;case 0xe4:return 7;default:return -1;}}
static void event(uint32_t a,uint32_t n,const uint32_t *args){uint32_t *r;REQUIRE(event_count<16384&&n<=7);r=events[event_count++];memset(r,0,96);r[0]=a;r[1]=n;if(n)memcpy(r+2,args,n*4);r[10]=W(0x70e120);r[11]=U(0xbc9768);r[12]=U(0x70e13c);r[13]=U(0xd3b580);r[14]=U(0xd3b57c);r[15]=U(0xd3b578);r[16]=U(0xd3b544);r[17]=U(0x6059e8);r[18]=U(0xbc9714);r[19]=U(0xbc96f8);r[20]=U(0xbca300);}
#define EV(a,...) do{uint32_t argv[]={__VA_ARGS__};event(a,sizeof(argv)/4,argv);}while(0)
#define E0(a) event(a,0,NULL)
extern void restore_placement_project(int,int,int *,int *),restore_placement_member_adapter_body(uint32_t,void *);
extern int restore_placement_mask_global(int,int,int,uint32_t);
extern uint8_t restore_placement_predicate_body(void *,uint32_t,int32_t);
void FUN_004f0ba0(int x,int y,int *out_x,int *out_y){EV(0x4f0ba0,x,y,1,out_x==out_y?1:2);restore_placement_project(x,y,out_x,out_y);}
void FUN_004f0ab0(int row,int column,int *map_y,int *map_x){EV(0x4f0ab0,row,column,1,map_y==map_x?1:2);*map_y=(int32_t)p[61];*map_x=(int32_t)p[62];}
int16_t restore_placement_key_state(int32_t key){EV(0x56f1e4,key);return (int16_t)p[49];}
uint8_t restore_placement_predicate(void *self,uint32_t terrain,int32_t piece){uint32_t target=U(U((uint32_t)self)+16u);EV(target,(uint32_t)self,terrain,piece);return restore_placement_predicate_body(self,terrain,piece);}
void restore_placement_member_adapter(uint32_t a,void *self){EV(a,(uint32_t)self);restore_placement_member_adapter_body(a,self);}
static uint32_t record_for(RestoreSpriteLibraries *self,int image){uint32_t index=(int32_t)image<(int32_t)self->next_image?(uint32_t)image:0;return (uint32_t)self+0x180e48u+index*64u;}
int FUN_0053f4f0(int image,int x,int y,uint32_t mask){EV(0x53f4f0,0xf55bb0,image,x,y,mask);current_record=record_for(RESTORE_SPRITE_LIBRARIES,image);return restore_placement_mask_global(image,x,y,mask);}
int FUN_0053e590(int image,int x,int y){EV(0x53e590,0xf55bb0,image,x,y);if(image>0&&image<25500){I(0xd3b580)=image;I(0xd3b57c)=x;I(0xd3b578)=y;}if(p[27]&16)U(0x70e13c)=0xffffffffu;if(p[27]&32)I(0xbc96f8)=1;return (int32_t)p[28];}
int FUN_0053f250(int image,int x,int y){EV(0x53f250,0xf55bb0,image,x,y);return 1;}
void FUN_004cdad0(void){E0(0x4cdad0);B(0xd3b530)=p[30];if(p[27]&2)I(0xd3b544)=p[28]+256;if(p[27]&4)B(current_record+51)=1;if(p[27]&8)U(0xd05720u+(p[4]+I(0x5cfe10u+(p[5]/2)*144u+4u))*4u)=ACTOR;}
void FUN_004cdb80(void){E0(0x4cdb80);B(0xd3b531)=p[31];}
#define LEAF(name,a) void name(const void *pixels,uint32_t mask){EV(a,(uint32_t)pixels,mask);if(p[27]&1)U(0xd3b544)+=1;}
LEAF(FUN_004100f0,0x4100f0) LEAF(FUN_004101c0,0x4101c0) LEAF(FUN_00410700,0x410700) LEAF(FUN_004109d0,0x4109d0) LEAF(FUN_00410a70,0x410a70)
#define ZERO(name,a) void name(void){E0(a);}
ZERO(FUN_0043d720,0x43d720) ZERO(FUN_00441490,0x441490) ZERO(FUN_00541c00,0x541c00) ZERO(FUN_004debe0,0x4debe0) ZERO(FUN_004edbb0,0x4edbb0) ZERO(FUN_0043e090,0x43e090) ZERO(FUN_004bfcd0,0x4bfcd0) ZERO(FUN_00441f80,0x441f80) ZERO(FUN_0043d300,0x43d300)
#define ONE(name,a) void name(int v){EV(a,v);}
ONE(FUN_0043df10,0x43df10) ONE(FUN_0043f5b0,0x43f5b0) ONE(FUN_004c0f30,0x4c0f30) ONE(FUN_004c2f50,0x4c2f50)
void FUN_0043dd70(uint32_t a,uint32_t b){EV(0x43dd70,a,b);}
uint32_t FUN_00482650(int x,int y,int w,int h,uint32_t bits){EV(0x482650,x,y,w,h,bits);if(p[41]&&!region_calls)U(0x6059e8)=p[41];return region_calls++?p[18]:p[17];}
static uint32_t rule_token(code *pfn){uint32_t ids[]={0x405b40,0x405b60,0x405b80},i;for(i=0;i<3;++i)if(pfn==restore_placement_rule(ids[i]))return ids[i];REQUIRE(0);return 0;}
uint32_t FUN_00482720(int x,int y,int w,int h,code *rule){EV(0x482720,x,y,w,h,rule_token(rule));return p[18];}
uint32_t FUN_004828e0(int x,int y,int w,int h,uint32_t bits){EV(0x4828e0,x,y,w,h,bits);return p[18];}
uint32_t FUN_004829b0(int x,int y,int w,int h,uint32_t bits,int flag){EV(0x4829b0,x,y,w,h,bits,flag);return p[18]!=0;}
uint8_t FUN_0046f9a0(int x,int y,int flags){EV(0x46f9a0,x,y,flags);if(p[0]==3&&(p[41]&0x100))U(0xbca2e0)=p[41]&255;return (uint8_t)p[19];}
uint32_t FUN_004ec410(uint32_t v,char mode){EV(0x4ec410,v,(uint8_t)mode);return (p[12]>>0)&1;}
uint32_t FUN_00477d40(int x,int y){EV(0x477d40,x,y);return (p[12]>>1)&1;}
int FUN_0046fde0(int mode){EV(0x46fde0,mode);return (p[12]>>2)&1;}
uint32_t FUN_0047ed60(int tile){EV(0x47ed60,tile);return (p[12]>>3)&1;}
uint32_t FUN_0047b920(void){E0(0x47b920);return (p[12]>>4)&1;}
int FUN_00487f80(int mode){EV(0x487f80,0xbca350,mode);return (int)p[23];}
uint32_t FUN_00489470(int x,int y,int w,int h){EV(0x489470,0xbca350,x,y,w,h);return (p[12]>>5)&1;}
int FUN_00489050(int x,int y,int w,int h){EV(0x489050,0xbca350,x,y,w,h);return (int)p[22];}
uint32_t FUN_0051c510(int tile){EV(0x51c510,tile);return (p[12]>>6)&1;}
int FUN_0051c5f0(uint32_t tile,int image){EV(0x51c5f0,tile,image);return (int)p[20];}
uint32_t FUN_0051c390(int tile){EV(0x51c390,tile);return (p[12]>>7)&1;}
char FUN_0051c460(int tile){EV(0x51c460,tile);return (char)p[21];}
uint8_t FUN_0047ed30(int tile,uint32_t mask){EV(0x47ed30,tile,mask);return (p[12]>>8)&1;}
uint8_t FUN_0047ed00(int tile,uint32_t mask){EV(0x47ed00,tile,mask);return (p[12]>>9)&1;}
uint32_t FUN_0051c630(int tile){EV(0x51c630,tile);return (p[12]>>10)&1;}
void FUN_0047e870(void){E0(0x47e870);U(0xbc9748)=p[22]&1;}
int FUN_00484be0(int t,int n,int limit,char flag){EV(0x484be0,t,n,limit,(uint8_t)flag);return 1;}
void FUN_0047f200(int x,int y,int kind){EV(0x47f200,x,y,kind);I(0xbc96f4)=p[50];I(0xbc96f8)=p[51];I(0xbc96fc)=p[52];}
static void sprites(RestoreSpriteLibraries *s,uint32_t extra){uint32_t i;memset(s,0,sizeof(*s));s->next_image=p[35];for(i=0;i<25500;++i){uint8_t *r=s->images[i].bytes;*(uint32_t *)r=p[34]?SRC+extra:0;*(int16_t *)(r+20)=(int16_t)(p[28]+(extra?3:0));*(int16_t *)(r+22)=(int16_t)(p[29]+(extra?5:0));r[50]=(uint8_t)p[33];r[51]=(uint8_t)p[32];}}
static void piece(uint32_t ptr,uint32_t side,int type){uint32_t o;I(ptr)=1;I(ptr+4)=-1;I(ptr+8)=side;I(ptr+12)=type;for(o=0;o<4;++o){I(ptr+20+o*4)=10+(int)o*3;I(ptr+36+o*4)=20-(int)o*2;}}
static void prepare(void){uint32_t i;RestoreSpriteLibraries *s=RESTORE_SPRITE_LIBRARIES;uint32_t op=p[0],id=p[37]%8;uint32_t object,ofs;if(op==0&&member_index(p[1])>=0)id=member_index(p[1]);object=p[36]?OBJ:objects[id];ofs=offsets[id];int32_t tile=(int32_t)p[4];
 event_count=region_calls=0;result=0;memset((void *)BASE,0,0x10000);memset((void *)OUT,0xa5,64);memset((void *)0x6de000u,0,0x20000);sprites(s,0);sprites(ALT,0x1000);
 for(i=0;i<sizeof(regions)/sizeof(regions[0]);++i)if(regions[i][0]!=OUT)memset((void *)regions[i][0],0xa5,regions[i][1]);
 for(i=0;i<51984;++i){U(0xaf598c+i*4)=p[8];U(0xd05720+i*4)=0;PV_S(0xb350dc+i*2)=(int16_t)p[10];B(0xb285cc+i)=p[56]&0x80;B(0xadc36c+i)=p[56]&1;}
 if(p[9])for(i=0;i<36;++i){int32_t cell=tile+I(0x5cfe10u+(p[5]/2)*144u+i*4u);U(0xaf598cu+(uint32_t)cell*4u)=i&1?p[8]:p[8]^p[9];}
 memset((void *)ACTOR,0xa5,64);B(ACTOR+24)=p[11]==1?0:(uint8_t)p[11];if(p[11])U(0xd05720u+(uint32_t)tile*4u)=ACTOR;
 for(i=0;i<15;++i)I(tags[i])=p[24]+i*16;I(0xf56be4)=p[25];U(0xf55bb4u+p[26]*8u)=p[24];
 definition=0x5d06dcu+p[1]*20u;if(p[1]<300){I(definition)=p[6];I(definition+8)=p[26];I(definition+12)=p[63];}
 I(0x6059e8)=p[1];I(0x605a3c)=p[3];I(0xbca300)=(p[2]&1)?0:tile;I(0xe38dcc)=(p[2]&2)?256:0;B(0x605a5c)=(p[2]&4)?1:0;I(0x605994)=(p[2]&8)?256:0;I(0x6059e4)=p[53];I(0xbc9724)=p[54];I(0xbc9744)=p[57];I(0xbc9748)=p[22]&1;
 for(i=0;i<7;++i)U(0x6059b4+i*4)=0;U(0x605a48)=0;U(0x605a28)=p[59];
 switch(p[13]){case 1:U(0x6059cc)=256;break;case 2:U(0x605a48)=0x405b40+(p[58]%3)*32;break;case 3:U(0x6059c8)=256;break;case 4:U(0x6059c4)=256;break;case 5:U(0x6059c0)=256;break;case 6:U(0x6059bc)=256;break;case 7:U(0x6059b8)=256;break;case 8:U(0x6059b4)=256;break;}
 I(0xbca2d0)=228;I(0xbca2d4)=228;I(0xbca2d8)=0;I(0xbca2e0)=p[5];I(0xbca2e8)=tile%228;I(0xbca2ec)=tile/228;I(0xbca304)=p[14];I(0xbca308)=p[15];I(0x70e45c)=p[43];I(0x70e44c)=p[44];I(0x70e448)=3;I(0x70e490)=p[45];I(0x70e48c)=p[46];U(0x70e474)=2;U(0x70e478)=3;lookup_address=0x70e494+(2*231+3)*4;I(lookup_address)=p[47]?0xfff0000:tile;
 U(0xeaac28)=0;I(0xea187c)=p[7];W(0xea5cf6)=(p[59]>>8)&65535;I(0xea620c)=(p[59]>>16)&1;I(0xea5e70)=(p[59]>>17)&1;I(0xea6240)=(p[59]>>18)&1;I(0x705c6c)=(p[59]>>19)&1;I(0xbc9714)=p[48];I(0x576144)=p[55];W(0x70e120)=0x1234;U(0x70e13c)=0;memset((void *)SRC,0x55,256);
 if(!p[16])restore_placement_preview_initialize_classes();
 watch_object=object;watch_bytes=ofs+counts[id]*52u+16;memset((void *)(OBJ-8),0,0xe010);if(p[36]){U(OBJ)=vtables[id];U(OBJ+0x50)=p[40];}
 if(p[16]){restore_preview_footprint_seeds_initialize();FUN_004013f0((void *)objects[1]);FUN_0040d7e0((void *)objects[0]);FUN_00416f60((void *)objects[2]);FUN_004e6590((void *)objects[7]);FUN_005456a0((void *)objects[3]);FUN_00558b40((void *)objects[4]);FUN_00555de0((void *)objects[5]);FUN_0054f3d0((void *)objects[6]);restore_placement_preview_initialize_classes();}
 else if(op==0&&p[1]>=0xe0){for(i=0;i<8;++i){uint32_t selected=(i>=3&&i<=6)?U(objects[i]+0x50):0;piece(objects[i]+offsets[i]+selected*52,p[39],9);if(selected)piece(objects[i]+offsets[i],p[39],0);}}
 else {piece(object+ofs,p[39],9);if(p[38]>1)piece(object+ofs+52,p[39],0);}
 if(op==3&&(p[41]&1)){uint32_t grids[]={0xaf55fc,0xaf5d1c,0xaf5988,0xaf5990};int32_t center=tile-228+1;for(i=0;i<4;++i)U(grids[i]+center*4u)=2;U(0xaf598cu+center*4u)=0;}
 if(op==5){for(i=0;i<8;++i){U(objects[i])=0;memset((void *)(objects[i]-8),0xa5,8);memset((void *)(objects[i]+4),0xa5,8);}U(0x6f9320+0x50)=U(0x6f5648+0x50)=U(0x6de9e0+0x50)=U(0x6e9188+0x50)=0;}
}
static void run(void){uint32_t id=p[37]%8;void *self=(void *)(p[36]?OBJ:objects[id]);prepare();switch(p[0]){
 case 0:FUN_0043b630();break;
 case 1:FUN_004f0ba0((int)p[14],(int)p[15],(int *)(OUT+8),(int *)(OUT+(p[60]?8:24)));break;
 case 2:EV(0x53f4f0,(uint32_t)(p[36]?ALT:RESTORE_SPRITE_LIBRARIES),p[24],p[14],p[15],p[42]);current_record=record_for(p[36]?ALT:RESTORE_SPRITE_LIBRARIES,p[24]);result=restore_placement_mask_sprite(p[36]?ALT:RESTORE_SPRITE_LIBRARIES,p[24],p[14],p[15],p[42]);break;
 case 3:if(id>=3&&id<=6)restore_placement_member_00545fc0(self,(void *)((uint32_t)self+offsets[id]),p[38]);else restore_placement_member_004e5490(self,(void *)((uint32_t)self+offsets[id]),p[38]);break;
 case 4:result=restore_placement_predicate(self,p[42],p[40]);break;
 case 5:restore_placement_preview_initialize_classes();break;
 case 6:EV(0x405b40+(p[58]%3)*32,p[42]);result=(uint8_t)(*restore_placement_rule(0x405b40+(p[58]%3)*32))(p[42]);break;
 case 7:FUN_004e52b0((RestoreGeometryRecord *)((uint32_t)self+offsets[id]),(int32_t)p[38]);break;
 default:REQUIRE(0);}}
static void bytes(uint32_t a,uint32_t n){static const char hex[]="0123456789abcdef";char b[8192];uint32_t i,k;while(n){k=n>4096?4096:n;for(i=0;i<k;++i){uint8_t v=B(a+i);b[2*i]=hex[v>>4];b[2*i+1]=hex[v&15];}REQUIRE(fwrite(b,2,k,stdout)==k);a+=k;n-=k;}}
static void output(void){uint32_t i,j;printf("{\"result\":%u,\"state\":\"",result);for(i=0;i<sizeof(regions)/sizeof(regions[0]);++i)bytes(regions[i][0],regions[i][1]);bytes(lookup_address-8,20);if(p[1]<300)bytes(definition,20);for(i=0;i<8;++i){bytes(objects[i]-8,20);bytes(objects[i]+0x48,16);}if(p[0]==3||p[0]==4||p[0]==7||(p[0]==0&&member_index(p[1])>=0))bytes(watch_object,watch_bytes);if(p[0]==2)bytes(current_record-8,80);if(p[0]==5)bytes(0x6de000u,0x20000);bytes(ACTOR,64);for(i=0;i<36;++i)bytes(0xd05720u+(p[4]+U(0x5cfe10u+(p[5]/2)*144u+i*4u))*4u,4);printf("\",\"events\":[");for(i=0;i<event_count;++i){printf("%s[",i?",":"");for(j=0;j<24;++j)printf("%s%u",j?",":"",events[i][j]);putchar(']');}puts("]}");fflush(stdout);}
int main(int argc,char **argv){uint32_t i;REQUIRE(restore_data_open());REQUIRE(VirtualAlloc((void *)BASE,0x800000,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE)==(void *)BASE);if(argc==3&&!strcmp(argv[1],"--reject-rule")){_set_abort_behavior(0,_WRITE_ABORT_MSG|_CALL_REPORTFAULT);restore_placement_rule((uint32_t)strtoul(argv[2],NULL,0));REQUIRE(0);}if(argc==2&&!strcmp(argv[1],"--oracle-batch")){puts("{\"fields\":64}");fflush(stdout);for(;;){for(i=0;i<64;++i)if(scanf("%u",p+i)!=1)goto done;run();output();}}else{p[0]=1;p[4]=2300;p[5]=0;p[6]=2;p[7]=0;p[14]=10;p[15]=10;p[18]=1;p[24]=335;p[25]=100;p[28]=8;p[29]=9;p[34]=1;p[35]=25500;p[38]=1;p[39]=1;p[43]=29;p[44]=15;p[45]=192;p[46]=144;run();REQUIRE(I(OUT+8)==192&&I(OUT+24)==429);p[0]=2;p[42]=0xfffff863u;run();REQUIRE(result==8&&event_count==4&&events[3][0]==0x4100f0&&events[3][3]==0xfffff863u);p[0]=0;p[1]=0xe5;p[16]=1;run();REQUIRE(event_count>100&&events[0][0]==0x5456e0&&events[0][2]==0x6f9320&&U(0x6f9320)==0x570160&&U(0x6f9320+0x50)==0xa6);puts("Placement preview: typed projection outputs, full mask and recovered-seed class startup smoke passed.");}done:VirtualFree((void *)BASE,0,MEM_RELEASE);restore_data_close();return 0;}
