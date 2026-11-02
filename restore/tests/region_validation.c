#include "region_validation.h"
#include "standalone_data.h"
#include <windows.h>
#include <wincrypt.h>
#include <stdio.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>
#define GRID 0x00af598cu
#define CELLS 0x20000u
#define GRID_FIRST (GRID-64u)
#define GRID_BYTES (CELLS*4u+128u)
#define FIELDS 20
#define CAPACITY 8192
static const uint32_t regions[][2]={{GRID_FIRST,GRID_BYTES},{0xbca2c0,64},{0xd3b4d0,48}};
static uint32_t p[FIELDS],events[CAPACITY][8],event_count,checks;
static uint8_t cached_grid[GRID_BYTES];static uint32_t cache_key[4];static int cache_valid;
static code *rule;static HCRYPTPROV provider;
code *restore_placement_rule(uint32_t token);
#define CHECK(x) do{++checks;if(!(x)){fprintf(stderr,"Region validation failure at %d\n",__LINE__);ExitProcess(1);}}while(0)
static uint32_t callback_enter(uint32_t terrain)
{
    uint32_t *e;CHECK(event_count<CAPACITY);e=events[event_count];
    e[0]=terrain;e[1]=RV_U(RV_ROW);e[2]=RV_U(RV_COL);e[3]=RV_U(0xbca2d0);e[4]=RV_U(0xbca2d4);e[5]=RV_U(0xbca2d8);e[6]=0;e[7]=event_count+1;
    ++event_count;
    if(event_count==1){
        if(p[16]&1)RV_U(RV_COL)=p[17];if(p[16]&2)RV_U(RV_ROW)=p[18];
        if(p[16]&4){RV_U(0xbca2d0)=0x12345678;RV_U(0xbca2d4)=0x87654321;RV_U(0xbca2d8)^=0x40010000;}
        if(p[16]&8)RV_U(GRID+(p[12]%CELLS)*4u)=p[14]^0x10001u;
    }
    return event_count-1;
}
static int32_t callback(uint32_t terrain)
{
    uint32_t row=callback_enter(terrain),result;
    if(p[13]<3)result=(uint32_t)(*rule)(terrain);
    else result=!p[15]||event_count==p[15]?p[14]:0;
    events[row][6]=result&255u;return (int32_t)result;
}
static void prepare(void)
{
    uint32_t key[4]={p[1],p[11],p[9],p[12]},i,v;
    if(!cache_valid||memcmp(key,cache_key,sizeof(key))){
        for(i=0;i<GRID_BYTES;++i)cached_grid[i]=(uint8_t)((i*13u+(i>>4)*11u+p[1])^0xb7u);
        for(i=0;i<CELLS;++i){
            switch(p[11]){
            case 0:v=0;break;case 1:v=p[9];break;case 2:v=(i*0x9e3779b9u+p[1])^(i>>3);break;
            case 3:v=(i%228u+i/228u)&1?p[9]:0;break;case 4:v=i==p[12]?p[9]:0;break;
            case 5:v=i<p[12]?p[9]:0;break;default:v=p[1];break;
            }
            memcpy(cached_grid+64+i*4,&v,4);
        }
        memcpy(cache_key,key,sizeof(key));cache_valid=1;
    }
    memcpy((void *)GRID_FIRST,cached_grid,GRID_BYTES);
    for(i=0;i<64;++i)*(uint8_t *)(0xbca2c0u+i)=(uint8_t)((i*37u+p[1])^0x5du);
    for(i=0;i<48;++i)*(uint8_t *)(0xd3b4d0u+i)=(uint8_t)((i*23u+p[1])^0xc3u);
    RV_U(0xbca2d0)=p[2];RV_U(0xbca2d4)=p[3];RV_U(0xbca2d8)=p[4];event_count=0;
    if(p[13]<3)rule=restore_placement_rule(p[13]==0?0x405b40:p[13]==1?0x405b60:0x405b80);
}
static uint32_t invoke(void)
{
    uint32_t result,sp0,sp1,bp0,bp1,bx_saved,si_saved,di_saved;
    uint32_t op=p[0],x=p[5],y=p[6],footprint=p[7],radius=p[8],bits=p[9],required=p[10];code *predicate=(code *)callback;
    __asm {
        mov sp0,esp
        mov bp0,ebp
        mov ebx,11223344h
        mov esi,33445566h
        mov edi,44556677h
        push required
        cmp op,1
        jne mask_arg
        push predicate
        jmp args
    mask_arg:
        push bits
    args:
        push radius
        push footprint
        push y
        push x
        cmp op,0
        jne pred
        call FUN_00482650
        jmp done
    pred:
        cmp op,1
        jne all_bits
        call FUN_00482720
        jmp done
    all_bits:
        cmp op,2
        jne count_bits
        call FUN_004828e0
        jmp done
    count_bits:
        call FUN_004829b0
    done:
        add esp,24
        mov result,eax
        mov sp1,esp
        mov bp1,ebp
        mov bx_saved,ebx
        mov si_saved,esi
        mov di_saved,edi
    }
    CHECK(sp0==sp1&&bp0==bp1&&bx_saved==0x11223344&&si_saved==0x33445566&&di_saved==0x44556677);CHECK(result<=1);return result;
}
static void digest(uint8_t bytes[32])
{
    HCRYPTHASH hash;DWORD length=32;unsigned i;CHECK(CryptCreateHash(provider,CALG_SHA_256,0,0,&hash));
    for(i=0;i<3;++i){CHECK(CryptHashData(hash,(const BYTE *)regions[i],8,0));CHECK(CryptHashData(hash,(const BYTE *)(uintptr_t)regions[i][0],regions[i][1],0));}
    CHECK(CryptGetHashParam(hash,HP_HASHVAL,bytes,&length,0));CHECK(CryptDestroyHash(hash));
}
int main(int argc,char **argv)
{
    uint32_t i,result;uint8_t hash[32];CHECK(restore_data_open());CHECK(CryptAcquireContextW(&provider,NULL,NULL,PROV_RSA_AES,CRYPT_VERIFYCONTEXT));
    if(argc>1&&!strcmp(argv[1],"--oracle-batch")){
        uint32_t ready=0x5245474e;_setmode(_fileno(stdout),_O_BINARY);fwrite(&ready,4,1,stdout);fflush(stdout);
        for(;;){for(i=0;i<FIELDS;++i)if(scanf("%u",&p[i])!=1)return 0;prepare();result=invoke();digest(hash);fwrite(&result,4,1,stdout);fwrite(hash,32,1,stdout);fwrite(&event_count,4,1,stdout);fwrite(events,32,event_count,stdout);fflush(stdout);}
    }
    {uint32_t q[FIELDS]={0,123,400,400,65536,256,1,2,0,0x10000,1,1,0,3,0,0,0,0,0,0};memcpy(p,q,sizeof(p));}
    prepare();CHECK(invoke()==1&&RV_U(RV_COL)==256&&RV_U(RV_ROW)==1);
    p[0]=2;prepare();CHECK(invoke()==1&&RV_U(RV_COL)==258&&RV_U(RV_ROW)==3);
    p[0]=3;p[10]=0x80000000;prepare();CHECK(invoke()==0&&RV_U(RV_COL)==258&&RV_U(RV_ROW)==3);
    p[0]=1;p[2]=228;p[3]=228;p[4]=0;p[5]=2;p[6]=3;p[7]=2;p[14]=0x100;prepare();CHECK(invoke()==0&&event_count==4);
    p[14]=0x101;prepare();CHECK(invoke()==1&&event_count==1);
    printf("Region validators: %u checks; DWORD dimensions/origin, full EAX returns, AL callback, end counters and guards passed.\n",checks);return 0;
}
