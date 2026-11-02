#include "actor_callbacks_small.h"
#include "standalone_data.h"
#include <windows.h>
#include <wincrypt.h>
#include <stdio.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>
#define FIELDS 32
static const uint32_t regions[][2]={{0xbcd5d8,1002u*388u+64u},{0x7998b0,302u*264u+64u},{0x5cc46c,0x8040},{0xf53dc0,0x840},{0xf55bb0,0x900},{0x7090d0,0x3900},{0xd056f0,32},{0xbca2d0,48},{0xe94df0,48},{0xd3b4d0,48}};
static uint32_t p[FIELDS],events[64][24],event_count,checks;static HCRYPTPROV provider;
static uint8_t background[0x90000];static uint32_t background_seed;static int valid_background;
int32_t actor_small_query_825_real(int32_t),actor_small_query_826_real(int32_t);
uint32_t actor_small_classify_real(int32_t);
void actor_small_state_adapter_real(int32_t);
#define CHECK(x) do{++checks;if(!(x)){fprintf(stderr,"Small actor callbacks failure line%d\n",__LINE__);ExitProcess(1);}}while(0)
static uint32_t *event(uint32_t address,unsigned count,const uint32_t *args,uint32_t result)
{
    uint32_t *e,a=as_current();unsigned i;CHECK(event_count<64);e=events[event_count++];memset(e,0,96);e[0]=address;e[1]=count;for(i=0;i<count;++i)e[2+i]=args[i];
    e[6]=result;e[7]=AS_U(0xd056f8);e[8]=AS_B(a+0x30);e[9]=AS_B(a-7);e[10]=AS_B(a+9);e[11]=AS_W(a+0x3c);e[12]=AS_W(a+0x3a);e[13]=AS_B(a+0x6a);e[14]=AS_B(a+4);e[15]=AS_B(a+6);e[16]=AS_U(a-4);e[17]=AS_U(0xbca2e0);e[18]=AS_U(0xe94e00);e[19]=AS_W(a+0x1c);e[20]=AS_W(a+0x1e);e[21]=AS_W(a+0x52);return e;
}
static void toggle_current(void){AS_U(0xd056f8)=AS_U(0xd056f8)==p[2]?p[3]:p[2];}
static void mutate_image(void)
{
    unsigned i;if(p[22]&256)for(i=0;i<0x900;i+=4)AS_U(0xf55bb0+i)^=0x00120000;
    if(p[22]&512){AS_U(0xbca2e0)=p[28];AS_U(0xe94e00)=p[28]^0x12340000;}
}
static void after_call(uint32_t address)
{
    uint32_t a;
    if(address==0x493610&&(p[22]&4))toggle_current();
    if(address==0x4578b0&&(p[22]&8))toggle_current();
    if(address==0x4579d0&&(p[22]&16))toggle_current();
    if(address==0x4c5120){if(p[22]&32)toggle_current();mutate_image();}
    if(address==0x493610||address==0x4578b0||address==0x4579d0){a=as_current();if(p[22]&1024)AS_B(a+0x30)=(uint8_t)p[23];if(p[22]&2048)AS_B(a+9)=(uint8_t)p[24];}
}
void restore_actor_small_state(int32_t unused)
{
    uint32_t args[1]={(uint32_t)unused};event(0x493610,1,args,0);actor_small_state_adapter_real(unused);event(0x80493610,1,args,0);after_call(0x493610);
}
int32_t FUN_004578b0(int32_t home)
{
    int32_t result;uint32_t args[1]={(uint32_t)home};event(0x4578b0,1,args,0);result=actor_small_query_825_real(home);event(0x804578b0,1,args,(uint32_t)result);after_call(0x4578b0);return result;
}
int32_t FUN_004579d0(int32_t home)
{
    int32_t result;uint32_t args[1]={(uint32_t)home};event(0x4579d0,1,args,0);result=actor_small_query_826_real(home);event(0x804579d0,1,args,(uint32_t)result);after_call(0x4579d0);return result;
}
uint32_t FUN_004c5120(int32_t home)
{
    uint32_t args[1]={(uint32_t)home},result;event(0x4c5120,1,args,0);result=actor_small_classify_real(home);event(0x804c5120,1,args,result);after_call(0x4c5120);return result;
}
static void deep(uint32_t address,unsigned count,const uint32_t *args,uint32_t mask)
{
    uint32_t a;event(address,count,args,0);
    if(!p[25]||(p[25]&mask)){
        if((p[22]&1)||(address==0x4b3250&&(p[22]&2)))toggle_current();a=as_current();
        if(p[22]&64)AS_B(a+0x30)=(uint8_t)p[23];if(p[22]&128)AS_B(a+9)=(uint8_t)p[24];mutate_image();
    }
}
void FUN_004b17d0(int32_t actor){uint32_t args[1]={(uint32_t)actor};deep(0x4b17d0,1,args,32);}
void FUN_004b1d50(int32_t actor){uint32_t args[1]={(uint32_t)actor};deep(0x4b1d50,1,args,16);}
void FUN_004b1ef0(int32_t actor,int32_t flag){uint32_t args[2]={(uint32_t)actor,(uint32_t)flag};deep(0x4b1ef0,2,args,8);}
void FUN_004b3250(int32_t actor){uint32_t args[1]={(uint32_t)actor};deep(0x4b3250,1,args,4);}
void FUN_004a5da0(int32_t actor,uint32_t speed){uint32_t args[2]={(uint32_t)actor,speed};deep(0x4a5da0,2,args,2);}
int32_t FUN_0047fe60(int32_t x,int32_t y,int32_t footprint,int32_t radius)
{
    uint32_t args[4]={(uint32_t)x,(uint32_t)y,(uint32_t)footprint,(uint32_t)radius};deep(0x47fe60,4,args,1);AS_U(0xd3b4e4)=p[26];AS_U(0xd3b4e0)=p[27];return (int32_t)p[21];
}
static void actor_fields(uint32_t id,int alternate)
{
    uint32_t a=as_actor(id),type=p[0]<4?(uint32_t[]){65,40,26,27}[p[0]]:p[30];
    AS_B(a+2)=(uint8_t)(type+(alternate?1:0));AS_B(a+0x30)=(uint8_t)p[10];AS_B(a-7)=(uint8_t)(p[11]+(alternate?5:0));AS_B(a+9)=(uint8_t)(p[12]+(alternate?3:0));
    AS_W(a+0x52)=(uint16_t)(p[4]+(alternate?1:0));AS_W(a+0x3c)=(uint16_t)(p[13]+(alternate?7:0));AS_W(a+0x2e)=(uint16_t)p[14];AS_W(a+0x3a)=(uint16_t)(p[15]+(alternate?11:0));
    AS_B(a+6)=0;AS_U(a-4)=0xc001d00d+(alternate?1:0);AS_U(0x7090ec+(uint32_t)AS_B(a+2)*56u)=p[29]^(alternate?0x12340000u:0);
}
static void building_fields(int32_t home,int alternate)
{
    uint32_t b=as_building(home);int32_t j,value;
    AS_B(b)=(uint8_t)p[5];AS_B(b+3)=(uint8_t)(3u+((uint32_t)home&3));AS_W(b+6)=(uint16_t)((uint32_t)home*3u+300u);AS_W(b+8)=(uint16_t)(257u-(uint32_t)home);
    AS_W(b+16)=(uint16_t)p[6];AS_W(b+0x4a)=(uint16_t)p[7];AS_W(b+0x2a)=(uint16_t)p[8];AS_W(b+0xb4)=(uint16_t)p[9];
    for(j=-1;j<12;++j){
        switch(p[19]){case 0:value=0;break;case 1:value=(int16_t)p[20];break;case 2:value=(int32_t)(((uint32_t)(j+1)*17u+p[1])%512u)-256;break;case 3:value=j*2-7;break;case 4:value=j==(int32_t)p[31]?32767:-1;break;default:value=(int16_t)(p[20]|0x8000);break;}
        AS_W(b+0x56u+(uint32_t)j*2u)=(uint16_t)(value+(alternate?1:0));
    }
}
static void prepare(void)
{
    unsigned i,j,offset=0;size_t total=0;static const uint32_t resource[]={0,1,2,3,4,5,6,7,8,13,15,17,19,9,0xffffffff,20};
    for(i=0;i<sizeof(regions)/sizeof(regions[0]);++i)total+=regions[i][1];CHECK(total<=sizeof(background));
    if(!valid_background||background_seed!=p[1]){for(i=0;i<total;++i)background[i]=(uint8_t)((i*13u+(i>>5)*7u+p[1])^0xa7u);background_seed=p[1];valid_background=1;}
    for(i=0;i<sizeof(regions)/sizeof(regions[0]);++i){memcpy((void *)(uintptr_t)regions[i][0],background+offset,regions[i][1]);offset+=regions[i][1];}
    for(i=0;i<0x900;i+=4)AS_U(0xf55bb0+i)=0x10000u+i*257u+p[1];
    AS_U(0xd056f8)=p[2];AS_U(0xbca2e0)=p[16];actor_fields(p[2],0);actor_fields(p[3],1);building_fields((int32_t)p[4],0);building_fields((int32_t)(p[4]+1u),1);
    AS_U(0xf53dd8)=p[17];for(i=0;i<512;++i)AS_U(0xf53ddc+i*4)=p[18]==0x80000000u?resource[i%16]:p[18];
    /* Every signed WORD timer/2 index is mapped and distinguishable. */
    for(j=0;j<0x8040;++j)*(uint8_t *)(uintptr_t)(0x5cc46cu+j)=(uint8_t)((j*37u+p[1])^0x6du);
    event_count=0;
}
static uint32_t invoke(void)
{
    void (*body)(void)=NULL;uint32_t sp0,sp1,bp0,bp1,bx_saved,si_saved,di_saved,result=0,op=p[0],home=p[4];
    if(op<4)body=(void (*const[4])(void)){FUN_0049b390,FUN_00496f20,FUN_00493ec0,FUN_00494010}[op];
    __asm {
        mov sp0,esp
        mov bp0,ebp
        mov ebx,11223344h
        mov esi,33445566h
        mov edi,44556677h
        cmp op,4
        jae helper
        call body
        jmp done
    helper:
        cmp op,4
        jne query
        push 1
        call restore_actor_small_state
        add esp,4
        jmp done
    query:
        push home
        cmp op,5
        jne other_food
        call FUN_004578b0
        jmp finish_query
    other_food:
        cmp op,6
        jne classify
        call FUN_004579d0
        jmp finish_query
    classify:
        call FUN_004c5120
    finish_query:
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
    if(argc>1&&!strcmp(argv[1],"--oracle-batch")){
        uint32_t ready=0x41534342;_setmode(_fileno(stdout),_O_BINARY);fwrite(&ready,4,1,stdout);fflush(stdout);
        for(;;){for(i=0;i<FIELDS;++i)if(scanf("%u",&p[i])!=1)return 0;prepare();result=invoke();digest(hash);fwrite(&result,4,1,stdout);fwrite(hash,32,1,stdout);fwrite(&event_count,4,1,stdout);fwrite(events,96,event_count,stdout);fflush(stdout);}
    }
    {uint32_t q[FIELDS]={0,123,1,257,1,1,70,0,1,1,1,10,7,95,10,99,2,16,0x80000000,0,0,1,0,2,9,0,0x1234,0xabcd,0x100,0x12345678,65,0};memcpy(p,q,sizeof(p));}
    prepare();invoke();CHECK(AS_B(as_actor(1)+0x6a)==0&&AS_B(as_actor(1)+0x5d)==1&&AS_B(as_actor(1)+4)==1&&event_count==0);
    p[0]=1;prepare();invoke();CHECK(AS_W(as_actor(1)+0x3a)==128&&AS_B(as_actor(1)-7)==11&&AS_U(0xe94e00)==5);
    p[0]=2;p[13]=96;prepare();invoke();CHECK(AS_B(as_actor(1)+0x30)==2&&AS_W(as_actor(1)+0x3c)==0);
    p[0]=3;p[6]=60;p[10]=8;prepare();invoke();CHECK(AS_B(as_actor(1)+0x30)==1&&event_count==2);
    p[0]=5;p[6]=70;p[19]=4;p[31]=8;p[18]=15;p[17]=1;prepare();CHECK(invoke()==32767);
    printf("Small actor callbacks: %u checks; global actor ABI,388/264-byte records, real state/stock/classifier graph and image fields passed.\n",checks);return 0;
}
