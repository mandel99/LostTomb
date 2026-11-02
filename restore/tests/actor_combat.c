#include "actor_combat.h"
#include "standalone_data.h"
#include <windows.h>
#include <wincrypt.h>
#include <stdio.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>
#define FIELDS 40
static const uint32_t regions[][2]={{0xbcb7c8,0xbcd600u+1002u*388u+64u-0xbcb7c8u},{0x7090b0,0x3900},{0xd04e38,0x820},{0xd056f0,32},{0xeaac18,48}};
static uint32_t p[FIELDS],events[64][24],event_count,checks;static HCRYPTPROV provider;
static uint8_t background[0x80000];static uint32_t background_seed;static int valid_background;
uint32_t actor_combat_direction_real(int32_t,int32_t),actor_combat_classA_real(int32_t);int32_t actor_combat_classC_real(int32_t);
void actor_combat_exit_real(int32_t),actor_combat_step_real(int32_t),actor_combat_route_real(int32_t),actor_combat_sound_real(int32_t);
#define CHECK(x) do{++checks;if(!(x)){fprintf(stderr,"Actor combat failure line%d\n",__LINE__);ExitProcess(1);}}while(0)
static void event(uint32_t address,unsigned count,const uint32_t *args,uint32_t result)
{
 uint32_t *e,a=cb_actor(p[2]),t=cb_actor((uint32_t)(int32_t)(int16_t)p[4]),f=cb_formation((int16_t)p[6]);unsigned i;CHECK(event_count<64);e=events[event_count++];memset(e,0,96);e[0]=address;e[1]=count;for(i=0;i<count;++i)e[2+i]=args[i];e[6]=result;
 e[7]=CB_U(0xd056f8);e[8]=CB_B(a+0x30);e[9]=CB_B(a+0x31);e[10]=CB_B(a+0x62);e[11]=CB_B(a+0x83);e[12]=CB_W(a+0x88);e[13]=CB_W(a+0x58);e[14]=CB_B(a+2);e[15]=CB_B(a+11);e[16]=CB_B(t+0x30);e[17]=CB_B(t+0x15);e[18]=CB_B(t+2);e[19]=CB_W(f+0x7a);e[20]=CB_W(a+0x42);e[21]=CB_W(a+0x44);e[22]=CB_B(a+0x5d);e[23]=CB_W(a+0x32);
}
static void mutation(uint32_t mask)
{
 uint32_t a=cb_actor(p[2]),t=cb_actor((uint32_t)(int32_t)(int16_t)p[4]),f=cb_formation((int16_t)p[6]),g=cb_formation((int16_t)p[7]);unsigned i;if(p[32]&&!(p[32]&mask))return;
 if(p[31]&1)CB_U(0xd056f8)=CB_U(0xd056f8)==p[3]?p[2]:p[3];
 if(p[31]&2)CB_W(a+0x58)=(uint16_t)p[7];
 if(p[31]&4){CB_B(a+2)=(uint8_t)p[33];CB_B(t+2)=(uint8_t)p[33];}
 if(p[31]&8)CB_W(a+0x88)=(uint16_t)p[35];
 if(p[31]&16){CB_W(f+4)=13;CB_W(g+4)=12;CB_W(f+0x2a)=CB_W(g+0x2a)=6;CB_W(f+0x5a)=CB_W(g+0x5a)=1;}
 if(p[31]&32)CB_U(0xeaac28)=p[34];
 if(p[31]&64){CB_B(a+7)=(uint8_t)(p[25]^0x81);CB_B(t+7)=(uint8_t)((p[25]>>8)^0x81);}
 if(p[31]&128){CB_B(a+11)=(uint8_t)(p[15]^7);CB_B(t+11)=(uint8_t)((p[29]>>16)^7);CB_B(f+0x6b)=(uint8_t)p[15];CB_B(g+0x6b)=(uint8_t)p[15];}
 if(p[31]&256)for(i=0;i<256;++i){CB_U(0x7090d4+i*56)=p[20]^0x12340000;CB_U(0x7090d8+i*56)=p[21]^0x56780000;}
 if(p[31]&512)CB_B(t+0x15)=(uint8_t)(p[23]^128);
 if(p[31]&1024)CB_W(f+0x7a)=(uint16_t)(p[30]>>16);
 if(p[31]&2048){CB_B(a+0x83)=2;CB_W(a+0x84)=(uint16_t)p[5];CB_W(a+0x86)=(uint16_t)p[4];}
}
uint32_t FUN_004b1d10(int32_t x,int32_t y){uint32_t args[2]={(uint32_t)x,(uint32_t)y},r;event(0x4b1d10,2,args,0);r=actor_combat_direction_real(x,y);event(0x804b1d10,2,args,r);mutation(1);return r;}
void FUN_004b1da0(int32_t id){uint32_t args[1]={(uint32_t)id};event(0x4b1da0,1,args,0);actor_combat_exit_real(id);event(0x804b1da0,1,args,0);mutation(64);}
void FUN_004b3f00(int32_t id){uint32_t args[1]={(uint32_t)id};event(0x4b3f00,1,args,0);actor_combat_step_real(id);event(0x804b3f00,1,args,0);mutation(2);}
void FUN_004b3250(int32_t id){uint32_t args[1]={(uint32_t)id};event(0x4b3250,1,args,0);actor_combat_route_real(id);event(0x804b3250,1,args,0);mutation(4);}
uint32_t FUN_004ea9a0(int32_t type){uint32_t args[1]={(uint32_t)type},r;event(0x4ea9a0,1,args,0);r=actor_combat_classA_real(type);event(0x804ea9a0,1,args,r);mutation(8);return r;}
int32_t FUN_004ea9c0(int32_t type){uint32_t args[1]={(uint32_t)type};int32_t r;event(0x4ea9c0,1,args,0);r=actor_combat_classC_real(type);event(0x804ea9c0,1,args,(uint32_t)r);mutation(16);return r;}
void restore_actor_combat_sound(int32_t sound){uint32_t args[1]={(uint32_t)sound};event(0x47d350,1,args,0);actor_combat_sound_real(sound);mutation(32);}
void FUN_004b5050(int32_t id){uint32_t args[1]={(uint32_t)id};event(0x4b5050,1,args,0);mutation(128);}
void FUN_004b5210(int32_t id){uint32_t args[1]={(uint32_t)id};event(0x4b5210,1,args,0);mutation(256);}
void FUN_004b7ab0(int32_t id){uint32_t args[1]={(uint32_t)id};event(0x4b7ab0,1,args,0);mutation(512);}
static void target_fields(uint32_t a,unsigned alternate)
{
 CB_B(a+2)=(uint8_t)(p[18]^(alternate?1:0));CB_B(a+6)=(uint8_t)p[alternate?11:9];CB_B(a+0x30)=(uint8_t)p[alternate?12:10];CB_B(a+7)=(uint8_t)p[25];CB_B(a+11)=(uint8_t)(p[29]>>16);CB_B(a+0x15)=(uint8_t)p[23];CB_W(a+0x58)=(uint16_t)(p[7]+alternate);CB_W(a+0x88)=(uint16_t)p[35];
}
static void formation_fields(int32_t id,unsigned target)
{
 uint32_t f=cb_formation(id);CB_W(f+4)=(uint16_t)(p[26]>>(target?16:0));CB_W(f+0x5a)=(uint16_t)(p[27]>>(target?16:0));CB_W(f+0x2a)=(uint16_t)(p[28]>>(target?16:0));CB_B(f+0x6b)=(uint8_t)(p[29]>>(target?8:0));CB_W(f+0x7a)=(uint16_t)(p[30]>>(target?16:0));
}
static void prepare(void)
{
 unsigned i,offset=0;size_t total=0;uint32_t a=cb_actor(p[2]),types[3]={p[17]&255,p[18]&255,(p[18]^1)&255};
 for(i=0;i<sizeof(regions)/sizeof(regions[0]);++i)total+=regions[i][1];CHECK(total<=sizeof(background));
 if(!valid_background||background_seed!=p[1]){for(i=0;i<total;++i)background[i]=(uint8_t)((i*13u+(i>>5)*7u+p[1])^0xa7u);background_seed=p[1];valid_background=1;}
 for(i=0;i<sizeof(regions)/sizeof(regions[0]);++i){memcpy((void *)(uintptr_t)regions[i][0],background+offset,regions[i][1]);offset+=regions[i][1];}
 CB_U(0xd056f8)=p[3];CB_U(0xeaac28)=p[24];CB_B(a+2)=(uint8_t)p[17];CB_B(a+6)=1;CB_B(a+0x30)=5;CB_B(a+8)=(uint8_t)p[39];CB_B(a+9)=(uint8_t)p[16];CB_B(a+11)=(uint8_t)p[15];CB_B(a+7)=(uint8_t)(p[25]>>8);CB_B(a+0x83)=(uint8_t)p[8];CB_B(a+0x31)=(uint8_t)p[14];CB_B(a+0x62)=(uint8_t)p[13];CB_W(a+0x58)=(uint16_t)p[6];CB_W(a+0x88)=CB_W(a+0x84)=(uint16_t)p[4];CB_W(a+0x86)=(uint16_t)p[5];CB_W(a+0x32)=(uint16_t)p[36];CB_W(a+0x42)=(uint16_t)p[20];CB_W(a+0x44)=(uint16_t)p[21];CB_B(a+0x6f)=(uint8_t)p[38];CB_B(a+0x70)=(uint8_t)(p[38]>>8);CB_B(a+0x71)=(uint8_t)(p[38]>>16);CB_B(a+0x5d)=(uint8_t)(p[38]>>24);
 target_fields(cb_actor((uint32_t)(int32_t)(int16_t)p[4]),0);target_fields(cb_actor((uint32_t)(int32_t)(int16_t)p[5]),1);formation_fields((int16_t)p[6],0);formation_fields((int16_t)p[7],1);formation_fields((int16_t)(p[7]+1),1);
 CB_W(0xd04e58u+(uint32_t)(int32_t)(int16_t)p[36]*2u)=(uint16_t)p[37];
 for(i=0;i<256;++i){CB_U(0x7090cc+i*56)=p[19]+i*256;CB_U(0x7090d0+i*56)=p[22]+i*3;CB_U(0x7090d4+i*56)=p[20]+i*257;CB_U(0x7090d8+i*56)=p[21]-i*13;}
 for(i=0;i<3;++i){uint32_t t=types[i]*56;CB_U(0x7090cc+t)=p[19];CB_U(0x7090d0+t)=p[22];CB_U(0x7090d4+t)=p[20];CB_U(0x7090d8+t)=p[21];}event_count=0;
}
static uint32_t invoke(void)
{
 uint32_t sp0,sp1,bp0,bp1,bx_saved,si_saved,di_saved,result=0,op=p[0],id=p[2],x=p[15],y=p[29],kind=p[18];
 __asm {
 mov sp0,esp
 mov bp0,ebp
 mov ebx,11223344h
 mov esi,33445566h
 mov edi,44556677h
 cmp op,1
 je direction
 cmp op,4
 je classA
 cmp op,5
 je classC
 push id
 cmp op,0
 jne exit_test
 call FUN_004b17d0
 jmp discard
 exit_test:
 cmp op,2
 jne step_test
 call FUN_004b1da0
 jmp discard
 step_test:
 cmp op,3
 jne route_test
 call FUN_004b3f00
 jmp discard
 route_test:
 call FUN_004b3250
 discard:
 add esp,4
 jmp done
 direction:
 push y
 push x
 call FUN_004b1d10
 add esp,8
 mov result,eax
 jmp done
 classA:
 push kind
 call FUN_004ea9a0
 jmp consumed
 classC:
 push kind
 call FUN_004ea9c0
 consumed:
 add esp,4
 mov result,eax
 done:
 mov sp1,esp
 mov bp1,ebp
 mov bx_saved,ebx
 mov si_saved,esi
 mov di_saved,edi
 }
 CHECK(sp0==sp1&&bp0==bp1&&bx_saved==0x11223344&&si_saved==0x33445566&&di_saved==0x44556677);return result;
}
static void digest(uint8_t bytes[32])
{
    HCRYPTHASH hash;DWORD length=32;unsigned i;CHECK(CryptCreateHash(provider,CALG_SHA_256,0,0,&hash));
    for(i=0;i<sizeof(regions)/sizeof(regions[0]);++i){CHECK(CryptHashData(hash,(const BYTE *)regions[i],8,0));CHECK(CryptHashData(hash,(const BYTE *)(uintptr_t)regions[i][0],regions[i][1],0));}
    CHECK(CryptGetHashParam(hash,HP_HASHVAL,bytes,&length,0));CHECK(CryptDestroyHash(hash));
}
int main(int argc,char **argv)
{
 uint32_t i,result;uint8_t hash[32];CHECK(restore_data_open());CHECK(CryptAcquireContextW(&provider,NULL,NULL,PROV_RSA_AES,CRYPT_VERIFYCONTEXT));
 if(argc>1&&!strcmp(argv[1],"--oracle-batch")){uint32_t ready=0x434f4d42;_setmode(_fileno(stdout),_O_BINARY);fwrite(&ready,4,1,stdout);fflush(stdout);for(;;){for(i=0;i<FIELDS;++i)if(scanf("%u",&p[i])!=1)return 0;prepare();result=invoke();digest(hash);fwrite(&result,4,1,stdout);fwrite(hash,32,1,stdout);fwrite(&event_count,4,1,stdout);fwrite(events,96,event_count,stdout);fflush(stdout);}}
 {uint32_t q[FIELDS]={0,123,1,257,4,5,1,2,2,1,1,1,1,23,5,0,2,11,12,0,20,0,100,0,0,0,12|(12<<16),1|(1<<16),1|(1<<16),0,0,0,0,82,256,0,1,1,0,8};memcpy(p,q,sizeof(p));}
 prepare();invoke();CHECK(CB_B(cb_actor(4)+0x15)==22&&CB_B(cb_actor(1)+0x62)==0);p[0]=1;p[15]=7;p[29]=0;prepare();CHECK(invoke()==1);
 p[0]=2;prepare();invoke();CHECK(CB_B(cb_actor(1)+0x83)==0&&CB_B(cb_actor(1)+0x30)==8&&CB_W(cb_actor(1)+0x32)==0);
 p[0]=3;p[20]=65535;p[21]=0;prepare();invoke();CHECK(CB_W(cb_actor(1)+0x42)==0);printf("Actor combat: %u checks; explicit-ID melee, target/formation arithmetic, real step/exit/classifiers/route and guarded records passed.\n",checks);return 0;
}
