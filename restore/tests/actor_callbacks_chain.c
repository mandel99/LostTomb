#include "actor_callbacks_chain.h"
#include "standalone_data.h"
#include <windows.h>
#include <wincrypt.h>
#include <stdio.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>
#define FIELDS32 32
static const uint32_t regions[][2]={{0xbcd2d8,1005u*388u+64u},{0x7998b0,302u*264u+64u},{0xb350bc,0x40040},{0x5cc46c,0x8040},{0xf55cd0,0x100},{0xd056f0,32},{0xbca2d0,48},{0xe94df0,48},{0x784ca0,48}};
static uint32_t p[FIELDS32],events[64][24],event_count,checks;static HCRYPTPROV provider;
static uint8_t background[0xe0000];static uint32_t background_seed;static int valid_background;
int32_t actor_chain_root_real(int32_t),actor_chain_tile_real(int32_t,uint32_t);
void actor_chain_sound_real(int32_t);
#define CHECK(x) do{++checks;if(!(x)){fprintf(stderr,"Actor chain failure line%d\n",__LINE__);ExitProcess(1);}}while(0)
static void event(uint32_t address,unsigned count,const uint32_t *args,uint32_t result)
{
 uint32_t *e,a=ac_current();unsigned i;CHECK(event_count<64);e=events[event_count++];memset(e,0,96);e[0]=address;e[1]=count;for(i=0;i<count;++i)e[2+i]=args[i];
 e[6]=result;e[7]=ac_id();e[8]=AC_B(a+0x30);e[9]=AC_B(a-7);e[10]=AC_B(a+9);e[11]=AC_B(a+10);e[12]=AC_B(a+0x31);e[13]=AC_B(a+0x5d);e[14]=AC_B(a+0x6a);e[15]=AC_B(a+4);e[16]=AC_B(a+6);e[17]=AC_U(a-4);e[18]=AC_U(a+0x178);e[19]=AC_U(0xbca2e0);e[20]=AC_U(0xe94e00);e[21]=AC_W(a+0x52);e[22]=AC_W(a+0x60);e[23]=AC_W(a+0x56);
}
static void mutation(uint32_t mask)
{
 uint32_t a;unsigned i;if(p[25]&&!(p[25]&mask))return;
 if(p[22]&1)AC_U(0xd056f8)=ac_id()==p[2]?p[3]:p[2];a=ac_current();
 if(p[22]&2)AC_B(a+0x30)=(uint8_t)p[23];if(p[22]&4)AC_B(a+9)=(uint8_t)p[24];
 if(p[22]&8)for(i=0;i<0x100;i+=4)AC_U(0xf55cd0+i)^=0x120000;
 if(p[22]&16){AC_U(0xbca2e0)=p[28];AC_U(0xe94e00)=p[28]^0x12340000;}
 if(p[22]&32)AC_B(0x784cb0)^=1;
 if(p[22]&64){AC_W(a+0x60)=(uint16_t)p[6];AC_W(a+0x52)=(uint16_t)(p[4]+1);}
 if(p[22]&128)AC_B(ac_actor((uint32_t)(int32_t)(int16_t)p[5])+0x5d)=1;
 if(p[22]&256){AC_B(ac_actor((uint32_t)(int32_t)(int16_t)p[5])+0x30)=(uint8_t)p[23];for(i=0;i<9;++i)AC_B(ac_actor(16+i)+0x30)=(uint8_t)p[23];}
 if(p[22]&512)for(i=0;i<3;++i)AC_B(ac_actor(100+i)+0x30)=(uint8_t)p[23];
}
int32_t FUN_0049b3c0(int32_t id){int32_t r;uint32_t args[1]={(uint32_t)id};event(0x49b3c0,1,args,0);r=actor_chain_root_real(id);event(0x8049b3c0,1,args,(uint32_t)r);mutation(8);return r;}
int32_t FUN_004b57e0(int32_t id,uint32_t type){int32_t r;uint32_t args[2]={(uint32_t)id,type};event(0x4b57e0,2,args,0);r=actor_chain_tile_real(id,type);event(0x804b57e0,2,args,(uint32_t)r);mutation(16);return r;}
int32_t FUN_004b3ff0(int32_t id,int32_t steps){uint32_t args[2]={(uint32_t)id,(uint32_t)steps};event(0x4b3ff0,2,args,0);mutation(1);return (int32_t)p[21];}
void FUN_004b22e0(int32_t id,int32_t prev,int32_t steps){uint32_t args[3]={(uint32_t)id,(uint32_t)prev,(uint32_t)steps};event(0x4b22e0,3,args,0);mutation(2);}
void restore_actor_chain_sound(int32_t sound){uint32_t args[1]={(uint32_t)sound};event(0x47d350,1,args,0);actor_chain_sound_real(sound);mutation(4);}
static void actor_fields(uint32_t id,unsigned alternate)
{
 uint32_t a=ac_actor(id);AC_B(a+2)=(uint8_t)(p[0]==0?74:66);AC_B(a+6)=0;AC_B(a+0x30)=(uint8_t)p[12];AC_B(a-7)=(uint8_t)(p[13]+alternate*5);AC_B(a+9)=(uint8_t)(p[14]+alternate*3);AC_B(a+10)=(uint8_t)(p[15]+alternate*3);AC_B(a+0x31)=(uint8_t)p[17];AC_B(a+0x72)=(uint8_t)p[18];
 AC_W(a+0x52)=(uint16_t)(p[4]+alternate);AC_W(a+0x60)=(uint16_t)p[5];AC_W(a+0x2e)=(uint16_t)p[16];AC_U(a+0x18)=p[31]+alternate;AC_U(a-4)=0xc001d00d+alternate;AC_U(a+0x178)=0xd001f00d+alternate;
}
static void prepare(void)
{
 unsigned i,offset=0;size_t total=0;uint32_t a,previous=(uint32_t)(int32_t)(int16_t)p[5],id,next,type,head;
 for(i=0;i<sizeof(regions)/sizeof(regions[0]);++i)total+=regions[i][1];CHECK(total<=sizeof(background));
 if(!valid_background||background_seed!=p[1]){for(i=0;i<total;++i)background[i]=(uint8_t)((i*13u+(i>>5)*7u+p[1])^0xa7u);background_seed=p[1];valid_background=1;}
 for(i=0;i<sizeof(regions)/sizeof(regions[0]);++i){memcpy((void *)(uintptr_t)regions[i][0],background+offset,regions[i][1]);offset+=regions[i][1];}
 for(i=0;i<0x100;i+=4)AC_U(0xf55cd0+i)=0x10000u+i*257u+p[1];
 AC_U(0xd056f8)=p[2];AC_U(0xbca2e0)=p[19];AC_B(0x784cb0)=(uint8_t)p[20];actor_fields(p[2],0);actor_fields(p[3],1);
 for(i=0;i<10;++i){id=i?15+i:previous;next=i==9?0:16+i;a=ac_actor(id);AC_W(a+0x60)=(uint16_t)next;AC_B(a+2)=(uint8_t)((i+1==p[7]&&i)?39:66);AC_B(a+6)=1;AC_B(a+0x30)=(uint8_t)p[29];AC_B(a+0x5d)=0;}
 a=ac_actor(previous);AC_B(a+2)=(uint8_t)p[10];AC_B(a+6)=(uint8_t)p[9];AC_B(a+0x30)=(uint8_t)p[8];AC_B(a+0x5d)=(uint8_t)p[11];
 head=p[26]==6?0xffff:(p[26]?100:0);AC_W(0xb350dc+p[31]*2)=AC_W(0xb350dc+(p[31]+1)*2)=(uint16_t)head;
 for(i=0;i<3;++i){a=ac_actor(100+i);type=p[26]==1?(uint32_t[]){70,68,69}[i]:p[26]==2?(uint32_t[]){68,69,70}[i]:p[26]==3?(uint32_t[]){69,7,7}[i]:p[26]==4?(uint32_t[]){7,70,68}[i]:(uint32_t[]){39,7,69}[i];AC_W(a)=(uint16_t)(i<2?101+i:(p[26]==7?0xffff:0));AC_B(a+2)=(uint8_t)type;AC_B(a+0x30)=(uint8_t)p[27];AC_W(a+12)=(uint16_t)(300+i*13);AC_W(a+14)=(uint16_t)(0x100+i*19);}
 AC_W(0x799a2eu+(uint32_t)(int32_t)(int16_t)p[4]*264u+(uint32_t)(uint8_t)p[18]*2u)=(uint16_t)p[6];
 for(i=0;i<0x8040;++i)AC_B(0x5cc46c+i)=(uint8_t)((i*37+p[1])^0x6d);event_count=0;
}
static uint32_t invoke(void)
{
    void (*body)(void)=NULL;uint32_t sp0,sp1,bp0,bp1,bx_saved,si_saved,di_saved,result=0,op=p[0],id=p[2],wanted_type=p[30];
    if(op<2)body=(void (*const[2])(void)){FUN_004a2e90,FUN_0049b400}[op];
    __asm {
        mov sp0,esp
        mov bp0,ebp
        mov ebx,11223344h
        mov esi,33445566h
        mov edi,44556677h
        cmp op,2
        jae helper
        call body
        jmp done
    helper:
        cmp op,2
        jne tile
        push id
        call FUN_0049b3c0
        add esp,4
        mov result,eax
        jmp done
    tile:
        push wanted_type
        push id
        call FUN_004b57e0
        add esp,8
        mov result,eax
        jmp done
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
 if(argc>1&&!strcmp(argv[1],"--oracle-batch")){uint32_t ready=0x43484149;_setmode(_fileno(stdout),_O_BINARY);fwrite(&ready,4,1,stdout);fflush(stdout);for(;;){for(i=0;i<FIELDS32;++i)if(scanf("%u",&p[i])!=1)return 0;prepare();result=invoke();digest(hash);fwrite(&result,4,1,stdout);fwrite(hash,32,1,stdout);fwrite(&event_count,4,1,stdout);fwrite(events,96,event_count,stdout);fflush(stdout);}}
 {uint32_t q[FIELDS32]={0,123,1,257,3,4,100,2,1,1,66,0,1,4,7,2,10,103,11,2,1,0,0,4,9,0,1,1,256,1,70,257};memcpy(p,q,sizeof(p));}
 prepare();invoke();CHECK(AC_B(ac_actor(100)+0x30)==12&&AC_B(ac_actor(1)+6)==2&&AC_W(ac_actor(3)+0x56)==100);
 p[0]=1;prepare();invoke();CHECK(AC_B(ac_actor(1)+0x6a)==1&&AC_U(ac_actor(1)+0x178)==0);
 p[0]=2;prepare();CHECK(invoke()==16);p[0]=3;prepare();CHECK(invoke()==100);
 printf("Actor chain callbacks: %u checks; complete arrow/carrier algorithms, chain/query composition and guarded full-width fields passed.\n",checks);return 0;
}
