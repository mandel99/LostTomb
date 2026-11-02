#include "actor_coordinates.h"
#include "standalone_data.h"
#include <windows.h>
#include <wincrypt.h>
#include <stdio.h>
#include <string.h>
#define U(a) (*(uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(uint32_t)(a))
#define COORD_OUT 0x31000000u
#define FIELDS 60
#define CHECK(x) do{if(!(x)){fprintf(stderr,"Actor coordinates failure at%d\n",__LINE__);ExitProcess(1);}}while(0)
/* op,seed,building,step,inputX,inputY,layout,variant,part,orientation,low,
 high,active,halfX,halfY,rotation,origin414,origin418,cameraRow,cameraCol,
 tile,mapOriginX,mapOriginY,pixelX,pixelY,sourceBuilding,targetBuilding,
 sourceStep,targetStep,fraction,total,quarter,actor,alias,initialX,initialY,
 parentMapX,parentMapY,tileX,tileY,mutateFunction,mutatePhase,occurrence,
 mutationBits,newLayout,newRotation,newQuarter,newPart,newActive,newVariant,
 newFraction,newTotal,deltaX,deltaY,newHalfX,newHalfY,newMapX,newMapY,
 newStep,repeats. */
static uint32_t p[FIELDS],regions[24][2],region_count,events[128][28],event_count,mutation_seen;
static HCRYPTPROV provider;
void actor_coordinates_actual_offset(int32_t,int32_t,int32_t *,int32_t *);
void actor_coordinates_actual_inverse(int32_t,int32_t,int32_t *,int32_t *);
void actor_coordinates_actual_world(int32_t,int32_t,int32_t *,int32_t *);
void actor_coordinates_actual_actor(int32_t,int32_t *,int32_t *);
static const uint32_t watched[]={0x5e4ef4,0x70e44c,0x70e45c,0xbca2e0,0x70e414,0x70e418,0x70e474,0x70e478,0xbca2e8,0xbca2ec,0x70e490,0x70e48c,0xe94e04};
static uint32_t pointer_tag(const int32_t *value)
{
    uint32_t a=(uint32_t)(uintptr_t)value,i;
    for(i=0;i<region_count;++i)if(regions[i][0]<=a&&a+4u<=regions[i][0]+regions[i][1])return a;
    return 0;
}
static void observe(uint32_t address,uint32_t phase,int32_t first,int32_t second,int32_t *x,int32_t *y)
{
    uint32_t *e,i,r=p[2]*264u,a=p[32]*388u;
    CHECK(event_count<128);e=events[event_count++];memset(e,0,112);e[0]=address;e[1]=phase;e[2]=(uint32_t)first;e[3]=(uint32_t)second;e[4]=pointer_tag(x);e[5]=pointer_tag(y);e[6]=x==y;
    if(phase){e[7]=(uint32_t)*x;e[8]=(uint32_t)*y;}
    for(i=0;i<13;++i)e[9+i]=U(watched[i]);
    e[22]=W(0x7999ea+r);e[23]=B(0x799a2c+r);e[24]=U(0x799a40+r);e[25]=W(0xbcd64a+a);e[26]=W(0xbcd64c+a);e[27]=W(0xbcd648+a);
    if(address==p[40]&&phase==p[41]&&++mutation_seen==p[42]){
        if(p[43]&1u)U(0x5e4ef4)=p[44];
        if(p[43]&2u)U(0xbca2e0)=p[45];
        if(p[43]&4u)U(0xe94e04)=p[46];
        if(p[43]&8u){W(0x7999ea+r)=(uint16_t)p[47];B(0x799a2c+r)=(uint8_t)p[49];U(0x799a40+r)=p[48];}
        if(p[43]&16u){W(0xbcd64a+a)=(uint16_t)p[50];W(0xbcd64c+a)=(uint16_t)p[51];W(0xbcd644+a)=(uint16_t)p[58];W(0xbcd648+a)=(uint16_t)p[58];}
        if(p[43]&32u){U(0x70e45c)=p[54];U(0x70e44c)=p[55];U(0xbca2e8)=p[56];U(0xbca2ec)=p[57];}
        if(p[43]&64u){U(0x70e474)=1;U(0x70e478)=2;}
        if((p[43]&128u)&&phase){*x=ac_add(*x,(int32_t)p[52]);*y=ac_add(*y,(int32_t)p[53]);}
        if(p[43]&256u){r=p[25]*264u;W(0x7999de+r)=(uint16_t)p[56];W(0x7999e0+r)=(uint16_t)p[57];r=p[26]*264u;W(0x7999de+r)=(uint16_t)(p[56]+1u);W(0x7999e0+r)=(uint16_t)(p[57]+1u);}
    }
}
#define WRAP4(name,actual,address) void name(int32_t first,int32_t second,int32_t *x,int32_t *y){observe(address,0,first,second,x,y);actual(first,second,x,y);observe(address,1,first,second,x,y);}
WRAP4(FUN_004f08c0,actor_coordinates_actual_offset,0x4f08c0)
WRAP4(FUN_004f0ab0,actor_coordinates_actual_inverse,0x4f0ab0)
WRAP4(FUN_004f0ba0,actor_coordinates_actual_world,0x4f0ba0)
void FUN_004f0d30(int32_t actor,int32_t *x,int32_t *y){observe(0x4f0d30,0,actor,0,x,y);actor_coordinates_actual_actor(actor,x,y);observe(0x4f0d30,1,actor,0,x,y);}
#pragma warning(push)
#pragma warning(disable:4100)
__declspec(naked) static void checked_call(uint32_t mode,const uint32_t *q,int32_t *const pointers[2],uint32_t out[6])
{
    __asm{
        push ebp
        push ebx
        push esi
        push edi
        mov ecx,[esp+32]
        mov [ecx+20],esp
        mov ebx,011111111h
        mov ebp,022222222h
        mov esi,033333333h
        mov edi,044444444h
        mov eax,[esp+20]
        mov ecx,[esp+24]
        mov edx,[esp+28]
        push dword ptr[edx+4]
        push dword ptr[edx]
        cmp eax,2
        je actor_body
        cmp eax,0
        je offset_body
        push dword ptr[ecx+20]
        push dword ptr[ecx+16]
        cmp eax,1
        je inverse_body
        call FUN_004f0ba0
        jmp four_done
inverse_body:
        call FUN_004f0ab0
        jmp four_done
offset_body:
        push dword ptr[ecx+12]
        push dword ptr[ecx+8]
        call FUN_004f08c0
four_done:
        add esp,16
        jmp complete
actor_body:
        push dword ptr[ecx+128]
        call FUN_004f0d30
        add esp,12
complete:
        mov ecx,[esp+32]
        mov [ecx],ebx
        mov [ecx+4],ebp
        mov [ecx+8],esi
        mov [ecx+12],edi
        mov [ecx+16],esp
        pop edi
        pop esi
        pop ebx
        pop ebp
        ret
    }
}
#pragma warning(pop)
static void region(uint32_t a,uint32_t n)
{
    uint32_t i;DWORD old;for(i=0;i<region_count;++i)if(regions[i][0]==a)return;
    CHECK(region_count<24);CHECK(VirtualProtect((void *)(uintptr_t)a,n,PAGE_READWRITE,&old));regions[region_count][0]=a;regions[region_count][1]=n;memset((void *)(uintptr_t)a,(int)((p[1]+region_count*29u)&255u),n);++region_count;
}
static void run(const uint32_t *q)
{
    uint32_t i,j,a,r,ids[5],out[6];int32_t *pointers[2];
    memcpy(p,q,sizeof(p));event_count=mutation_seen=region_count=0;
    region(0x5e3000,0x7000);region(0x5f0000,0x8000);region(0x70e400,0x775630-0x70e400);region(0xbca2d0,64);region(0xe94df8,32);region(COORD_OUT,256);
    ids[0]=p[2];ids[1]=p[25];ids[2]=p[26];ids[3]=p[26]-(uint32_t)(int32_t)(int16_t)p[8];ids[4]=p[26]-(uint32_t)(int32_t)(int16_t)p[47];
    for(i=0;i<5;++i)region(0x7999d0+ids[i]*264u,288);
    region(0xbcd5f0+p[32]*388u,412);
    for(i=0;i<0x7000;i+=4)U(0x5e3000+i)=(i/4u+p[1])%11u;
    for(i=0;i<0x8000;i+=4)U(0x5f0000+i)=(i/4u*37u+p[1])%121u-60u;
    for(i=0;i<5;++i){
        r=ids[i]*264u;W(0x7999ea+r)=(uint16_t)p[8];B(0x799a2c+r)=(uint8_t)(ids[i]==p[2]?p[7]:(p[7]+1u)%4u);B(0x799a2f+r)=(uint8_t)p[10];B(0x799a30+r)=(uint8_t)p[11];U(0x799a40+r)=ids[i]==p[2]?p[12]:p[12]^1u;
        W(0x7999de+r)=(uint16_t)(p[38]+i*3u);W(0x7999e0+r)=(uint16_t)(p[39]-i*5u);W(0x7999fe+r)=(uint16_t)(p[36]+i*7u);W(0x799a00+r)=(uint16_t)(p[37]-i*11u);
    }
    a=(uint32_t)(int32_t)(int16_t)p[8];U(0x5e4ef8u+(p[7]*144u+a)*8u)=p[9];U(0x5e6578u+(p[7]*36u+a)*8u)=p[9];
    for(i=0;i<231u*457u;++i)U(0x70e494+i*4u)=p[20];
    U(0x5e4ef4)=p[6];U(0x70e45c)=p[13];U(0x70e44c)=p[14];U(0xbca2e0)=p[15];U(0x70e414)=p[16];U(0x70e418)=p[17];U(0x70e474)=p[18];U(0x70e478)=p[19];U(0xbca2e8)=p[21];U(0xbca2ec)=p[22];U(0x70e490)=p[23];U(0x70e48c)=p[24];U(0xe94e04)=p[31];
    r=p[32]*388u;W(0xbcd642+r)=(uint16_t)p[25];W(0xbcd644+r)=(uint16_t)p[27];W(0xbcd646+r)=(uint16_t)p[26];W(0xbcd648+r)=(uint16_t)p[28];W(0xbcd64a+r)=(uint16_t)p[29];W(0xbcd64c+r)=(uint16_t)p[30];
    U(COORD_OUT)=p[34];U(COORD_OUT+4)=p[35];pointers[0]=(int32_t *)COORD_OUT;pointers[1]=(int32_t *)(COORD_OUT+4);
    switch(p[33]){
    case 1:pointers[1]=pointers[0];break;case 2:pointers[1]=(int32_t *)(COORD_OUT+2);break;
    case 3:pointers[0]=(int32_t *)0x70e44c;break;case 4:pointers[0]=(int32_t *)0xbca2e0;break;case 5:pointers[1]=(int32_t *)0xbca2e0;break;
    case 6:pointers[0]=(int32_t *)(uintptr_t)(0x799a40+p[2]*264u);break;case 7:pointers[1]=(int32_t *)(uintptr_t)(0x799a40+p[2]*264u);break;
    case 8:pointers[0]=(int32_t *)(uintptr_t)(0xbcd64a+p[32]*388u);break;case 9:pointers[1]=(int32_t *)0xe94e04;break;
    default:break;
    }
    for(j=0;j<p[59];++j){checked_call(p[0],p,pointers,out);CHECK(out[0]==0x11111111&&out[1]==0x22222222&&out[2]==0x33333333&&out[3]==0x44444444&&out[4]==out[5]);}
}
static void digest(char hex[65])
{
    HCRYPTHASH hash;DWORD length=32;uint8_t bytes[32];uint32_t i;static const char digits[]="0123456789abcdef";CHECK(CryptCreateHash(provider,CALG_SHA_256,0,0,&hash));
    for(i=0;i<region_count;++i){CHECK(CryptHashData(hash,(const BYTE *)regions[i],8,0));CHECK(CryptHashData(hash,(const BYTE *)(uintptr_t)regions[i][0],regions[i][1],0));}CHECK(CryptGetHashParam(hash,HP_HASHVAL,bytes,&length,0));CHECK(CryptDestroyHash(hash));for(i=0;i<32;++i){hex[i*2]=digits[bytes[i]>>4];hex[i*2+1]=digits[bytes[i]&15];}hex[64]=0;
}
int main(int argc,char **argv)
{
    uint32_t q[FIELDS]={0},i,j;char hex[65];
    SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOGPFAULTERRORBOX);CHECK(restore_data_open());CHECK(VirtualAlloc((void *)COORD_OUT,4096,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE)==(void *)COORD_OUT);CHECK(CryptAcquireContextW(&provider,NULL,NULL,PROV_RSA_AES,CRYPT_VERIFYCONTEXT));
    if(argc==2&&!strcmp(argv[1],"--oracle-batch")){puts("{\"fields\":60}");fflush(stdout);for(;;){for(i=0;i<FIELDS;++i)if(scanf("%u",q+i)!=1)goto done;run(q);digest(hex);printf("{\"memory\":\"%s\",\"events\":[",hex);for(i=0;i<event_count;++i){printf("%s[",i?",":"");for(j=0;j<28;++j)printf("%s%u",j?",":"",events[i][j]);putchar(']');}puts("]}");fflush(stdout);}}
    else{q[0]=2;q[2]=q[25]=1;q[26]=2;q[3]=q[27]=q[28]=1;q[6]=3;q[8]=0;q[9]=1;q[13]=29;q[14]=15;q[18]=10;q[19]=5;q[20]=0xfff0000;q[23]=320;q[24]=144;q[29]=1;q[30]=2;q[32]=257;q[36]=7;q[37]=9;q[38]=4;q[39]=6;q[59]=1;run(q);CHECK(event_count>=8&&U(COORD_OUT)!=0);q[0]=0;q[3]=0;q[33]=1;run(q);CHECK(U(COORD_OUT)==15);puts("Actor coordinates: actual offset/inverse/world/actor chain, strides, alias-safe stores and wrapped arithmetic.");}
done:CryptReleaseContext(provider,0);restore_data_close();return 0;
}
