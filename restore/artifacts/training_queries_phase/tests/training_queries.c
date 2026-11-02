#include "training_queries.h"
#include "query_case.h"
#include "standalone_data.h"
#include <stdio.h>
#include <string.h>
#include <wincrypt.h>
#define U(a) (*(uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(uint32_t)(a))
#define AUX 0x4ac20000u
#define CHECK(x) do{if(!(x)){fprintf(stderr,"Training query failure at %d\n",__LINE__);ExitProcess(1);}}while(0)
static uint32_t p[P_FIELDS],events[8192][40],used,seen;static HCRYPTPROV provider;
static const uint32_t regions[][2]={{0x77644c,0x9350},{0x784d22,0x24},{0xea61e4,0x12790},{0x4ac261e4,0x30}};
uint32_t queries_actual_00443950(int),queries_actual_004439c0(int),queries_actual_00443a20(int),queries_actual_00443b20(int),queries_actual_00443b90(int),queries_actual_00443d90(int),queries_actual_004c5190(void);
bool queries_actual_00447790(int,int);int queries_actual_004c5240(int);
static int valid_city(uint32_t city){return city<4||city==65536;}
static uint32_t fee_address(uint32_t category,uint32_t resource){return 0x77646cu+(category*36u+resource)*4u;}
static void timers(uint32_t city,uint32_t t0,uint32_t t1,uint32_t t2){uint32_t offset=city*0x49d8u;CHECK(valid_city(city));W(0xea61f4u+offset)=(uint16_t)t0;W(0xea61f6u+offset)=(uint16_t)t1;W(0xea61f8u+offset)=(uint16_t)t2;}
static uint64_t flags(uint32_t low,uint32_t high){return(uint64_t)low|((uint64_t)high<<32);}
static void record(uint32_t row,uint32_t enabled,uint32_t type,uint32_t category,uint32_t present,uint64_t supply,uint64_t demand)
{uint32_t a,j;CHECK(row<61);a=0x77de00u+row*106u;memset((void *)(uintptr_t)a,0,106);B(a)=(uint8_t)enabled;B(a+2)=(uint8_t)type;B(a+4)=(uint8_t)category;B(a+5)=(uint8_t)present;for(j=0;j<36;++j){B(a+0x2a+j)=(uint8_t)((supply>>j)&1u);B(a+6+j)=(uint8_t)((demand>>j)&1u);}}
static void observe(uint32_t fn,uint32_t phase,uint32_t count,uint32_t a,uint32_t b,uint32_t result)
{
 uint32_t *e,j,n,city=U(0xeaac28),offset=city*0x49d8u,first=0x77de00u+p[P_ROW]*106u,zero=0x77de00u+p[P_ROW2]*106u,changed=0x77de00u+p[P_MUTROW]*106u;CHECK(valid_city(city)&&used<8192&&p[P_RESOURCE]<40);e=events[used++];e[0]=fn;e[1]=phase;e[2]=count;e[3]=a;e[4]=b;e[5]=result;e[6]=city;e[7]=W(0xea61f4u+offset);e[8]=W(0xea61f6u+offset);e[9]=W(0xea61f8u+offset);e[10]=W(0x784d32);e[11]=W(0x784d34);for(j=0;j<4;++j){static const uint32_t fields[]={0,2,4,5};e[12+j]=B(first+fields[j]);e[18+j]=B(zero+fields[j]);e[32+j]=B(changed+fields[j]);}e[16]=B(first+0x2a+p[P_RESOURCE]);e[17]=B(first+6+p[P_RESOURCE]);e[22]=B(zero+0x2a+p[P_RESOURCE]);e[23]=B(zero+6+p[P_RESOURCE]);e[24]=U(fee_address(B(first+4),p[P_RESOURCE]));e[25]=U(fee_address(B(zero+4),p[P_RESOURCE]));e[26]=p[P_ROW];e[27]=p[P_ROW2];e[28]=U(fee_address(p[P_CATEGORY]&255u,p[P_RESOURCE]));offset=p[P_CITY]*0x49d8u;e[29]=W(0xea61f4u+offset);e[30]=W(0xea61f6u+offset);e[31]=W(0xea61f8u+offset);e[36]=B(changed+0x2a+p[P_RESOURCE]);e[37]=B(changed+6+p[P_RESOURCE]);e[38]=U(fee_address(p[P_NEWCATEGORY]&255u,p[P_COSTRESOURCE]));e[39]=p[P_RESOURCE];
 if(fn==p[P_MFN]&&phase==p[P_MPHASE]&&++seen==p[P_MOCC]){n=p[P_MBITS];if(n&1){CHECK(valid_city(p[P_NEWCITY]));U(0xeaac28)=p[P_NEWCITY];timers(p[P_NEWCITY],p[P_NEWT0],p[P_NEWT1],p[P_NEWT2]);}if(n&2)record(p[P_MUTROW],p[P_NEWENABLED],p[P_NEWTYPE],p[P_NEWCATEGORY],p[P_NEWPRESENT],flags(p[P_NEWSUP0],p[P_NEWSUP1]),flags(p[P_NEWDEM0],p[P_NEWDEM1]));if(n&4){CHECK(p[P_COSTRESOURCE]<40);U(fee_address(p[P_NEWCATEGORY]&255u,p[P_COSTRESOURCE]))=p[P_NEWFEE];}if(n&8){W(0x784d32)=(uint16_t)p[P_NEWGATE10];W(0x784d34)=(uint16_t)p[P_NEWGATE28];}}
}
#define Q1(a) uint32_t FUN_##a(int resource){uint32_t r;observe(0x##a,0,1,(uint32_t)resource,0,0);r=queries_actual_##a(resource);observe(0x##a,1,1,(uint32_t)resource,0,r);CHECK(r<=1);return r;}
Q1(00443950) Q1(004439c0) Q1(00443a20) Q1(00443b20) Q1(00443b90) Q1(00443d90)
bool FUN_00447790(int row,int resource){bool r;observe(0x447790,0,2,(uint32_t)row,(uint32_t)resource,0);r=queries_actual_00447790(row,resource);observe(0x447790,1,2,(uint32_t)row,(uint32_t)resource,r);CHECK(r<=1);return r;}
uint32_t FUN_004c5190(void){uint32_t r;observe(0x4c5190,0,0,0,0,0);r=queries_actual_004c5190();observe(0x4c5190,1,0,0,0,r);CHECK((int32_t)r>=-3&&(int32_t)r<=1);return r;}
int FUN_004c5240(int value){int r;observe(0x4c5240,0,1,(uint32_t)value,0,0);r=queries_actual_004c5240(value);observe(0x4c5240,1,1,(uint32_t)value,0,(uint32_t)r);return r;}
#pragma warning(push)
#pragma warning(disable:4100)
__declspec(naked) static void checked_call(uint32_t op,const uint32_t *q,uint32_t out[7])
{__asm{
 push ebp
 push ebx
 push esi
 push edi
 mov ecx,[esp+28]
 mov [ecx+20],esp
 mov ebx,011111111h
 mov ebp,022222222h
 mov esi,033333333h
 mov edi,044444444h
 mov eax,[esp+20]
 mov ecx,[esp+24]
 cmp eax,8
 je modifier_call
 cmp eax,7
 je threshold_call
 cmp eax,6
 je eligible_call
 push dword ptr[ecx+P_RESOURCE*4]
 cmp eax,0
 je supply_present
 cmp eax,1
 je supply_call
 cmp eax,2
 je demand_call
 cmp eax,3
 je demand_present
 cmp eax,4
 je available_call
 call FUN_00443d90
 jmp one_done
supply_present:call FUN_00443950
 jmp one_done
supply_call:call FUN_004439c0
 jmp one_done
demand_call:call FUN_00443a20
 jmp one_done
demand_present:call FUN_00443b20
 jmp one_done
available_call:call FUN_00443b90
one_done:add esp,4
 jmp complete
eligible_call:push dword ptr[ecx+P_RESOURCE*4]
 push dword ptr[ecx+P_ROW*4]
 call FUN_00447790
 add esp,8
 movzx eax,al
 jmp complete
threshold_call:push dword ptr[ecx+P_ARGUMENT*4]
 call FUN_004c5240
 add esp,4
 jmp complete
modifier_call:call FUN_004c5190
complete:mov ecx,[esp+28]
 mov [ecx],ebx
 mov [ecx+4],ebp
 mov [ecx+8],esi
 mov [ecx+12],edi
 mov [ecx+16],esp
 mov [ecx+24],eax
 pop edi
 pop esi
 pop ebx
 pop ebp
 ret
}}
#pragma warning(pop)
static void prepare(const uint32_t *q)
{static const uint32_t cities[]={0,1,2,3,65536};uint32_t j,k,r=p[P_RESOURCE],first,zero,category;uint64_t supply,demand,zmask;DWORD old;memcpy(p,q,sizeof(p));used=seen=0;r=p[P_RESOURCE];CHECK(r<40&&p[P_ROW]<61&&p[P_ROW2]<61&&p[P_ROW]!=p[P_ROW2]&&valid_city(p[P_CITY])&&p[P_MUTROW]<61);for(j=0;j<sizeof(regions)/sizeof(regions[0]);++j){CHECK(VirtualProtect((void *)(uintptr_t)regions[j][0],regions[j][1],PAGE_READWRITE,&old));memset((void *)(uintptr_t)regions[j][0],(uint8_t)(p[P_SEED]+j*29u),regions[j][1]);}for(j=0;j<256;++j)for(k=0;k<36;++k)U(fee_address(j,k))=p[P_FEE];memset((void *)0x77de00u,0,61*106);supply=flags(p[P_SUP0],p[P_SUP1]);demand=flags(p[P_DEM0],p[P_DEM1]);zmask=flags(p[P_ZERO0],p[P_ZERO1]);if(p[P_SCENARIO]!=UINT32_MAX){uint32_t s=p[P_SCENARIO];CHECK(s<512);supply=0;zmask=0;if(s&1)zmask|=(uint64_t)1<<1;if(s&2)zmask|=(uint64_t)1<<11;if(s&4)supply|=(uint64_t)1<<9;if(s&8)supply|=(uint64_t)1<<11;if(s&16)zmask|=(uint64_t)1<<9;if(s&32)zmask|=(uint64_t)1<<12;if(s&64)supply|=(uint64_t)1<<31;if(s&128)zmask|=(uint64_t)1<<34;if(s&256)supply|=(uint64_t)1<<13;demand=supply;}record(p[P_ROW],p[P_ENABLED],p[P_TYPE],p[P_CATEGORY],p[P_PRESENT],supply,demand);record(p[P_ROW2],p[P_ZERO_ENABLED],0,0,0,zmask,0);first=0x77de00u+p[P_ROW]*106;zero=0x77de00u+p[P_ROW2]*106;if(p[P_SCENARIO]==UINT32_MAX){B(first+0x2a+r)=(uint8_t)p[P_SUPPLY_VALUE];B(first+6+r)=(uint8_t)p[P_DEMAND_VALUE];B(zero+0x2a+r)=(uint8_t)p[P_ZERO_VALUE];}category=B(first+4);for(j=0;j<36;++j)U(fee_address(category,j))=p[P_FEE];U(fee_address(category,r))=p[P_FEE];for(j=0;j<5;++j)timers(cities[j],p[P_T0],p[P_T1],p[P_T2]);U(0xeaac28)=p[P_CITY];W(0x784d32)=(uint16_t)p[P_GATE10];W(0x784d34)=(uint16_t)p[P_GATE28];}
static uint32_t run(const uint32_t *q){uint32_t j,out[7];prepare(q);for(j=0;j<p[P_REPEATS];++j){checked_call(p[P_OP],p,out);CHECK(out[0]==0x11111111&&out[1]==0x22222222&&out[2]==0x33333333&&out[3]==0x44444444&&out[4]==out[5]);}return out[6];}
static void digest(char hex[65]){HCRYPTHASH h;uint8_t b[32];DWORD n=32;uint32_t j;static const char d[]="0123456789abcdef";CHECK(CryptCreateHash(provider,CALG_SHA_256,0,0,&h));for(j=0;j<sizeof(regions)/sizeof(regions[0]);++j){CHECK(CryptHashData(h,(const BYTE *)regions[j],8,0));CHECK(CryptHashData(h,(const BYTE *)(uintptr_t)regions[j][0],regions[j][1],0));}CHECK(CryptGetHashParam(h,HP_HASHVAL,b,&n,0));CHECK(CryptDestroyHash(h));for(j=0;j<32;++j){hex[j*2]=d[b[j]>>4];hex[j*2+1]=d[b[j]&15];}hex[64]=0;}
int main(int argc,char **argv){uint32_t q[P_FIELDS]={0},j,k,result;char hex[65];SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOGPFAULTERRORBOX);CHECK(restore_data_open());CHECK(VirtualAlloc((void *)AUX,0x10000,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE));CHECK(CryptAcquireContextW(&provider,NULL,NULL,PROV_RSA_AES,CRYPT_VERIFYCONTEXT));if(argc==2&&!strcmp(argv[1],"--oracle-batch")){printf("{\"fields\":%u}\n",(uint32_t)P_FIELDS);fflush(stdout);for(;;){for(j=0;j<P_FIELDS;++j)if(scanf("%u",q+j)!=1)goto done;result=run(q);digest(hex);printf("{\"return\":%u,\"memory\":\"%s\",\"events\":[",result,hex);for(j=0;j<used;++j){if(j)putchar(',');putchar('[');for(k=0;k<40;++k){if(k)putchar(',');printf("%u",events[j][k]);}putchar(']');}puts("]}");fflush(stdout);}}else{q[P_SEED]=q[P_REPEATS]=q[P_ENABLED]=q[P_TYPE]=q[P_PRESENT]=q[P_SUPPLY_VALUE]=q[P_DEMAND_VALUE]=q[P_ZERO_VALUE]=q[P_RESOURCE]=1;q[P_ROW2]=60;q[P_SUP0]=q[P_DEM0]=q[P_SCENARIO]=UINT32_MAX;q[P_SUP1]=q[P_DEM1]=15;q[P_FEE]=15;q[P_GATE10]=q[P_GATE28]=256;CHECK(run(q)==1);q[P_OP]=6;q[P_FEE]=0xffffff00;CHECK(run(q)==0);q[P_OP]=8;q[P_CITY]=65536;q[P_T1]=q[P_T2]=1;CHECK(run(q)==0xfffffffdu);q[P_OP]=7;q[P_T0]=1;q[P_T1]=q[P_T2]=0;q[P_ARGUMENT]=25;CHECK(run(q)==40);puts("Training queries: real records, recursive availability, fee policy and selected-city modifier; zero controls.");}done:CHECK(CryptReleaseContext(provider,0));restore_data_close();return 0;}
