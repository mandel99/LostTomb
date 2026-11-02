#include "actor_order_los.h"
#include "actor_order_los_case.h"
#include "standalone_data.h"
#include <windows.h>
#include <stdio.h>
#include <string.h>
#define BASE 0x30000000u
#define B(a) (*(uint8_t *)(uintptr_t)(a))
#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define CHECK(x) do{if(!(x)){fprintf(stderr,"LOS case failed line%d\n",__LINE__);ExitProcess(2);}}while(0)
static const uint8_t guards[2][10]={{0x21,0x43,0x65,0x87,0xa9,0xcb,0xed,0x8f,0xff,0x3f},{0x10,0x32,0x54,0x76,0x98,0xba,0xdc,0xfe,0x04,0xc0}};
static const uint32_t flood_regions[][2]={{0xbca2c0,48},{0xeadff0,0xcb40},{0xed4130,0xcb30},{0xeed750,0x658a0}};
static uint32_t p[L_FIELDS],events[2][6],used,result[2],snapshot[8],after_cw;
static uint32_t raw_slope(void){return BASE+0x100u+p[L_ALIGN];}
static uint32_t raw_inverse(void){return BASE+0x140u+p[L_ALIGN];}
static uint32_t raw_input(void){return BASE+0x180u+p[L_ALIGN];}
static uint32_t raw_result(void){return BASE+0x1c0u+p[L_ALIGN];}
static uint32_t output_x(void){return BASE+(p[L_ALIAS]==2?0x260u:0x250u)+p[L_ALIGN];}
static uint32_t output_y(void){return BASE+(p[L_ALIAS]==1?0x250u:p[L_ALIAS]==2?0x250u:0x260u)+p[L_ALIGN];}
static uint32_t region_count(void){return p[L_OP]<4?1u:p[L_OP]==4?3u:6u;}
static void region(uint32_t i,uint32_t *address,uint32_t *size)
{
    if(i==0){*address=BASE+0xf0;*size=0x200;return;}
    if(i==1){*address=0xaf597c;*size=207968;return;}
    if(p[L_OP]==4){CHECK(i==2);*address=0xbca2c0;*size=48;return;}
    *address=flood_regions[i-2][0];*size=flood_regions[i-2][1];
}
void restore_los_fill_trace(uint32_t destination,uint32_t bytes,uint32_t value)
{uint32_t *e;CHECK(used<2);e=events[used++];e[0]=destination;e[1]=bytes;e[2]=value;e[3]=U(0xeae004);e[4]=U(0xeae008);e[5]=U(0xeae00c);}
#pragma warning(push)
#pragma warning(disable:4100)
__declspec(naked) static void checked_call(uint32_t op,const uint32_t args[6],uint32_t snap[8])
{__asm{
    push ebp
    push ebx
    push esi
    push edi
    mov ecx,[esp+28]
    mov [ecx+16],esp
    mov ebx,011111111h
    mov ebp,022222222h
    mov esi,033333333h
    mov edi,044444444h
    mov eax,[esp+20]
    mov ecx,[esp+24]
    cmp eax,4
    jae whole_call
    cmp eax,2
    jae one_call
    push dword ptr[ecx+12]
    push dword ptr[ecx+8]
    push dword ptr[ecx+4]
    push dword ptr[ecx]
    cmp eax,1
    je intersection_call
    call restore_los_slopes80
    add esp,16
    xor eax,eax
    xor edx,edx
    jmp complete
intersection_call:
    call restore_los_intersection80
    add esp,16
    mov ecx,[esp+24]
    push dword ptr[ecx+12]
    call restore_los_trunc80
    add esp,4
    jmp complete
one_call:
    push dword ptr[ecx]
    cmp eax,2
    je compare_call
    call restore_los_trunc80
    add esp,4
    jmp complete
compare_call:
    call restore_los_equal_max80
    add esp,4
    xor edx,edx
    jmp complete
whole_call:
    push dword ptr[ecx+20]
    push dword ptr[ecx+16]
    push dword ptr[ecx+12]
    push dword ptr[ecx+8]
    push dword ptr[ecx+4]
    push dword ptr[ecx]
    cmp eax,4
    je clear_call
    call FUN_0040c180
    jmp whole_done
clear_call:
    call FUN_0040bda0
whole_done:
    add esp,24
    xor edx,edx
complete:
    mov ecx,[esp+28]
    mov [ecx],ebx
    mov [ecx+4],ebp
    mov [ecx+8],esi
    mov [ecx+12],edi
    mov [ecx+20],esp
    mov [ecx+24],eax
    mov [ecx+28],edx
    pop edi
    pop esi
    pop ebx
    pop ebp
    ret
}}
#pragma warning(pop)
static void setup(void)
{
    uint32_t i,j,address,size,low,cell,probe;int32_t dx=(int32_t)(p[L_TX]-p[L_X]),dy=(int32_t)(p[L_TY]-p[L_Y]);
    static const int32_t neighbors[9]={0,-228,228,1,-1,-227,-229,229,227};
    static const int32_t fallback[8]={-228,-227,228,-229,1,229,-1,227};
    for(i=0;i<region_count();++i){region(i,&address,&size);for(j=0;j<size;++j)B(address+j)=(uint8_t)(p[L_SEED]+j*37u+i*11u);}
    for(i=0;i<10;++i)B(raw_input()+i)=(uint8_t)p[L_R0+i];
    if(p[L_OP]>=4){
        U(0xbca2d8)=p[L_BASE];low=p[L_STYLE]==1||p[L_STYLE]==3||p[L_STYLE]==4||p[L_STYLE]==5?4u:0u;
        for(i=0;i<51984;++i)U(0xaf598cu+i*4u)=((p[L_SEED]+i*0x01020300u)&0xffffff00u)|low;
        probe=p[L_BASE]+(p[L_Y]+(dy<0?0xffffffffu:dy>0?1u:0u))*228u+p[L_X]+(dx<0?0xffffffffu:dx>0?1u:0u);
        if(p[L_STYLE]==2||p[L_STYLE]==3){cell=probe+(uint32_t)neighbors[p[L_BLOCK]%9u];CHECK(cell<51984);if(p[L_STYLE]==2)U(0xaf598cu+cell*4u)|=4u;else U(0xaf598cu+cell*4u)&=~4u;}
        if(p[L_STYLE]==4){CHECK(probe<51984);U(0xaf598cu+probe*4u)|=0x40u;}
        if(p[L_STYLE]==5&&p[L_BLOCK]<8){cell=p[L_BASE]+p[L_Y]*228u+p[L_X]+(uint32_t)fallback[p[L_BLOCK]];CHECK(cell<51984);U(0xaf598cu+cell*4u)&=~4u;}
    }
    if(p[L_OP]==5){
        memset((void *)0xed4140u,0xfd,51984);
        for(i=1;i+1<p[L_DIM];++i)for(j=1;j+1<p[L_DIM];++j)B(0xed4140u+i*228u+j)=(uint8_t)p[L_CLASS];
    }
    used=0;memset(events,0,sizeof(events));memset(result,0,sizeof(result));memset(snapshot,0,sizeof(snapshot));
}
static void run(void)
{
    uint32_t args[6]={0};uint16_t before,after,top_before,top_after,wanted=(uint16_t)p[L_CW];uint8_t g0[10],g1[10];const uint8_t *first=guards[0],*second=guards[1];
    setup();
    if(p[L_OP]==0){args[0]=p[L_X];args[1]=p[L_Y];args[2]=raw_slope();args[3]=raw_inverse();}
    else if(p[L_OP]==1){args[0]=p[L_X];args[1]=p[L_Y];args[2]=raw_input();args[3]=raw_result();}
    else if(p[L_OP]<4)args[0]=raw_input();
    else{args[0]=p[L_X];args[1]=p[L_Y];args[2]=p[L_TX];args[3]=p[L_TY];args[4]=output_x();args[5]=output_y();}
    __asm {fnstcw before}
    __asm {fnstsw top_before}
    __asm {fldcw wanted}
    __asm {mov eax,second}
    __asm {fld tbyte ptr[eax]}
    __asm {mov eax,first}
    __asm {fld tbyte ptr[eax]}
    checked_call(p[L_OP],args,snapshot);
    __asm {fnstcw after}
    __asm {lea eax,g0}
    __asm {fstp tbyte ptr[eax]}
    __asm {lea eax,g1}
    __asm {fstp tbyte ptr[eax]}
    __asm {fnstsw top_after}
    __asm {fldcw before}
    CHECK(snapshot[0]==0x11111111&&snapshot[1]==0x22222222&&snapshot[2]==0x33333333&&snapshot[3]==0x44444444&&snapshot[4]==snapshot[5]);
    CHECK(after==wanted&&((top_before^top_after)&0x3800u)==0&&memcmp(g0,guards[0],10)==0&&memcmp(g1,guards[1],10)==0);
    result[0]=snapshot[6];result[1]=snapshot[7];after_cw=after;
}
static void output(void)
{
    static char encoded[2u*207968u];static const char hex[]="0123456789abcdef";
    uint32_t i,j,address,size,chunk,offset;printf("{\"return\":[%u,%u],\"cw\":%u,\"guards\":true,\"state\":\"",result[0],result[1],after_cw);
    for(i=0;i<region_count();++i){region(i,&address,&size);for(offset=0;offset<size;offset+=chunk){chunk=size-offset;if(chunk>sizeof(encoded)/2u)chunk=sizeof(encoded)/2u;for(j=0;j<chunk;++j){uint8_t value=B(address+offset+j);encoded[j*2u]=hex[value>>4];encoded[j*2u+1u]=hex[value&15u];}CHECK(fwrite(encoded,2u,chunk,stdout)==chunk);}}
    printf("\",\"events\":[");for(i=0;i<used;++i){printf("%s[",i?",":"");for(j=0;j<6;++j)printf("%s%u",j?",":"",events[i][j]);putchar(']');}puts("]}");fflush(stdout);
}
int main(int argc,char **argv)
{
    uint32_t i;SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOGPFAULTERRORBOX);CHECK(restore_data_open());CHECK(VirtualAlloc((void *)BASE,0x10000,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE)==(void *)BASE);
    if(argc==2&&!strcmp(argv[1],"--oracle-batch")){printf("{\"fields\":%d}\n",L_FIELDS);fflush(stdout);for(;;){for(i=0;i<L_FIELDS;++i)if(scanf("%u",p+i)!=1)goto done;run();output();}}
    else{memset(p,0,sizeof(p));p[L_OP]=4;p[L_CW]=0x37f;p[L_X]=p[L_Y]=4;p[L_TX]=p[L_TY]=8;p[L_DIM]=16;run();CHECK(result[0]==32&&U(output_x())==8&&U(output_y())==8);puts("LOS exact numeric guards and whole clear-line result passed.");}
done:VirtualFree((void *)BASE,0,MEM_RELEASE);restore_data_close();return 0;
}
