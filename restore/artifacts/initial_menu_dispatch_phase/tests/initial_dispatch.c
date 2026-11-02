#include "restore_types.h"
#include "standalone_data.h"
#include <stdio.h>
#include <string.h>
#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(a))
#define CHECK(x) do{if(!(x)){fprintf(stderr,"Initial dispatcher line%d\n",__LINE__);ExitProcess(1);}}while(0)
static const uint32_t regions[][2]={{0x605930,64},{0xd3a6b0,80},{0xe38dc0,48}};
static uint32_t p[22],events[32][12],reads[128][3],writes[128][3],used,read_used,write_used,plain;
extern void initial_dispatch_trace(int),initial_dispatch_plain(int);
uint32_t restore_initial_dispatch_read(uint32_t a,uint32_t n)
{
 uint32_t value=n==1?B(a):U(a);CHECK(n==1||n==4);CHECK(read_used<128);
 reads[read_used][0]=a;reads[read_used][1]=n;reads[read_used++][2]=value;return value;
}
void restore_initial_dispatch_write(uint32_t a,uint32_t v)
{
 CHECK(write_used<128);writes[write_used][0]=a;writes[write_used][1]=1;writes[write_used++][2]=(uint8_t)v;B(a)=(uint8_t)v;
}
static void mutate(uint32_t token,uint32_t ordinal,uint32_t phase)
{
 if(p[11]!=phase||(p[9]&&p[9]!=token)||(p[10]&&p[10]!=ordinal))return;
 if(p[12]&1)B(0xe38dca)=(uint8_t)p[13];if(p[12]&2)U(0xd3a6e4)=p[14];
 if(p[12]&4)U(0xd3a6c4)=p[15];if(p[12]&8)B(0xe38dda)=(uint8_t)p[16];
 if(p[12]&16)U(0x605954)=p[17];if(p[12]&32)U(0x60594c)=p[18];if(p[12]&64)U(0x605950)=p[19];
}
static void event(uint32_t token,uint32_t count,uint32_t a,uint32_t b,uint32_t c)
{
 uint32_t *e;CHECK(used<32);mutate(token,used+1,1);e=events[used++];
 e[0]=token;e[1]=count;e[2]=a;e[3]=b;e[4]=c;e[5]=B(0xe38dda);e[6]=B(0xe38dca);
 e[7]=U(0xd3a6e4);e[8]=U(0xd3a6c4);e[9]=U(0x605954);e[10]=U(0x60594c);e[11]=U(0x605950);
 mutate(token,used,2);
}
#include "renderer_boundaries.inc"
#pragma warning(push)
#pragma warning(disable:4100)
__declspec(naked) static void checked_call(void (__cdecl *fn)(int),uint32_t force,uint32_t out[6])
{
 __asm{
  push ebp
  push ebx
  push esi
  push edi
  mov edx,[esp+28]
  mov [edx+20],esp
  mov eax,[esp+20]
  mov ecx,[esp+24]
  mov ebx,011111111h
  mov ebp,022222222h
  mov esi,033333333h
  mov edi,044444444h
  push ecx
  call eax
  add esp,4
  mov ecx,[esp+28]
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
static void run(void)
{
 uint32_t i,j,out[6];DWORD old;
 for(i=0;i<3;++i){CHECK(VirtualProtect((void *)(uintptr_t)regions[i][0],regions[i][1],PAGE_READWRITE,&old));for(j=0;j<regions[i][1];++j)B(regions[i][0]+j)=(uint8_t)(p[8]+i*29u+j*37u);}
 U(0xe38dca)=p[1];U(0xd3a6e4)=p[2];U(0xd3a6c4)=p[3];B(0xe38dda)=(uint8_t)p[4];
 U(0x605954)=p[5];U(0x60594c)=p[6];U(0x605950)=p[7];used=read_used=write_used=0;CHECK(p[21]>0&&p[21]<=3);
 for(i=0;i<p[21];++i){checked_call(plain?initial_dispatch_plain:initial_dispatch_trace,p[0],out);CHECK(out[0]==0x11111111&&out[1]==0x22222222&&out[2]==0x33333333&&out[3]==0x44444444&&out[4]==out[5]);}
}
static void triples(const char *name,uint32_t data[][3],uint32_t count)
{
 uint32_t i;printf(",\"%s\":[",name);for(i=0;i<count;++i)printf("%s[%u,%u,%u]",i?",":"",data[i][0],data[i][1],data[i][2]);putchar(']');
}
static void output(void)
{
 uint32_t i,j;static const char hex[]="0123456789abcdef";printf("{\"memory\":\"");
 for(i=0;i<3;++i)for(j=0;j<regions[i][1];++j){uint8_t b=B(regions[i][0]+j);putchar(hex[b>>4]);putchar(hex[b&15]);}
 printf("\",\"events\":[");for(i=0;i<used;++i){printf("%s[",i?",":"");for(j=0;j<12;++j)printf("%s%u",j?",":"",events[i][j]);putchar(']');}putchar(']');
 triples("reads",reads,read_used);triples("writes",writes,write_used);puts("}");fflush(stdout);
}
int main(int argc,char **argv)
{
 uint32_t i;CHECK(restore_data_open());SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOGPFAULTERRORBOX);
 if(argc==2&&(!strcmp(argv[1],"--oracle-batch")||!strcmp(argv[1],"--oracle-plain-batch"))){plain=!strcmp(argv[1],"--oracle-plain-batch");puts("{\"fields\":22}");fflush(stdout);for(;;){for(i=0;i<22;++i)if(scanf("%u",p+i)!=1)goto done;run();output();}}
 else{
  p[0]=1;p[4]=15;p[5]=70000;p[6]=0x12345678;p[7]=0x87654321;p[21]=1;run();CHECK(used==1&&events[0][0]==0x530d70&&events[0][2]==70000&&events[0][3]==0x12345678&&events[0][4]==0x87654321&&B(0xe38dca)==0);
  p[2]=256;p[3]=256;run();CHECK(used==0&&B(0xe38dca)==0);p[3]=15;run();CHECK(used==1);plain=1;run();CHECK(used==1&&read_used==0&&write_used==0);
  puts("Initial menu dispatch: alternate DWORD mode, all widths, popup arguments, trace/plain ABI.");
 }
done:restore_data_close();return 0;
}
