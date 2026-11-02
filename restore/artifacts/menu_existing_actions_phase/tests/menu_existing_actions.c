#include "restore_types.h"
#include "standalone_data.h"
#include "file_list.h"
#include "existing_case.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <wincrypt.h>
#define ALT 0x31000000u
#define U(a) (*(uint32_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(uint32_t)(a))
#define CHECK(x) do{if(!(x)){fprintf(stderr,"Existing menu action failure at %d\n",__LINE__);ExitProcess(1);}}while(0)
static uint32_t p[P_FIELDS],events[2048][32],used,seen,path_used;
static struct{uint32_t fn,phase;char hex[521];}paths[256];static HCRYPTPROV provider;
static const uint32_t regions[][2]={{0xe38db0,0x130},{0xe370a0,0xd0},{0xe20980,0x30},{0xd3b1a0,0x30},{0x6057b0,0x2b0},{0xeade90,0x80},{0xe2078c,0x1c0},{0xe36ee0,0x30},{ALT,0x5000}};
static const char *const filenames[]={"Alpha.sav","Beta.extra.sav",".hidden.sav","no_extension","Gamma.SAV","zeta.sav","  spaced.name.sav","Last."};
void existing_actual_00402450(void),existing_actual_00402540(void),existing_actual_00402630(void),existing_actual_00402bb0(void);
void existing_actual_0040f8f0(undefined4 *,uint,undefined1),existing_actual_004cc500(int),existing_actual_004cc3c0(void),existing_actual_004cc3e0(void),existing_actual_004cdff0(uint32_t,char),existing_actual_004ce3a0(char *);
char *existing_actual_005169c0(void);
static uint32_t length(const char *s){uint32_t n;for(n=0;n<260&&s[n];++n){}CHECK(n<260);return n;}
static uint32_t hash(const char *s){uint32_t h=2166136261u,j,n=length(s);for(j=0;j<n;++j)h=(h^(uint8_t)s[j])*16777619u;return h;}
static void make_text(char *s,uint32_t n,uint32_t style,uint32_t max){static const char *const alphabet[]={"ABCDEFGHIJKLMNOPQRSTUVWXYZ","abcdefghijklmnopqrstuvwxyz0123456789","Ab 9_-.","0123456789"};uint32_t j;CHECK(n<=max&&style<4);for(j=0;j<n;++j)s[j]=alphabet[style][j%(uint32_t)strlen(alphabet[style])];s[n]=0;}
static void path_event(uint32_t fn,uint32_t phase,const char *s){static const char d[]="0123456789abcdef";uint32_t j,n=length(s);CHECK(path_used<256);paths[path_used].fn=fn;paths[path_used].phase=phase;for(j=0;j<n;++j){paths[path_used].hex[2*j]=d[(uint8_t)s[j]>>4];paths[path_used].hex[2*j+1]=d[(uint8_t)s[j]&15];}paths[path_used++].hex[n*2]=0;}
static void observe(uint32_t fn,uint32_t phase,uint32_t a,uint32_t b,uint32_t c,uint32_t d,uint32_t r)
{uint32_t *e,n;CHECK(used<2048);e=events[used++];e[0]=fn;e[1]=phase;e[2]=a;e[3]=b;e[4]=c;e[5]=d;e[6]=r;e[7]=B(0xe38dda);e[8]=B(0xe38dca);e[9]=U(0x6059e8);e[10]=U(0xe20998);e[11]=U(0xd3b1bc);e[12]=B(0xd3b1c0);e[13]=U(0xe20990);e[14]=U(0x6057d0);e[15]=U(0x6059f8);e[16]=U(0x6059f4);e[17]=U(0x6059f0);e[18]=B(0xd3b1b3);e[19]=U(0x6059ec);e[20]=hash((const char *)0xe38e74u);e[21]=hash((const char *)0xeadea4u);e[22]=hash((const char *)0xe370bcu);e[23]=hash((const char *)0xe37100u);e[24]=length((const char *)0xe370bcu);e[25]=length((const char *)0xe37100u);e[26]=U(0xe207ec+2*4);e[27]=U(0xe208a0+2*4);e[28]=U(0xe208f0+2*4);e[29]=U(0xe207ec+3*4);e[30]=U(0xe208a0+3*4);e[31]=U(0xe208f0+3*4);
 if(fn==p[P_MFN]&&phase==p[P_MPHASE]&&++seen==p[P_MOCC]){n=p[P_MBITS];if(n&1)make_text((char *)0xe38e74u,p[P_NEWPROFILE_LENGTH],p[P_NEWPROFILE_STYLE],31);if(n&2)make_text((char *)0xe370bcu,p[P_NEWNAME_LENGTH],p[P_NEWNAME_STYLE],63);if(n&4)make_text((char *)0xe37100u,p[P_NEWNAME_LENGTH],p[P_NEWNAME_STYLE],63);if(n&8){B(0xe38dda)=(uint8_t)p[P_NEWMODE];B(0xe38dca)=(uint8_t)p[P_NEWDIRTY];U(0x6059e8)=p[P_NEWCHOOSER];U(0xe20998)=p[P_NEWVALUE];U(0xd3b1bc)=p[P_NEWBACKMODE];B(0xd3b1c0)=(uint8_t)p[P_NEWACTION];}if(n&16){CHECK(p[P_NEWINDEX]<20);U(0xe20990)=p[P_NEWINDEX];U(0xe208f0+3*4)=p[P_NEWMAX];U(0xe208a0+3*4)=p[P_NEWLENGTH];U(0xe207ec+3*4)=p[P_NEWCURSOR];}if(n&32){CHECK((int32_t)p[P_NEWFILECOUNT]<=8);U(0x6057d0)=p[P_NEWFILECOUNT];}if(n&64)make_text((char *)0xeadea4u,p[P_NEWNAME_LENGTH],p[P_NEWNAME_STYLE],63);}}
#define OBS(f,ph,a,b,c,d,r) observe(f,ph,(uint32_t)(a),(uint32_t)(b),(uint32_t)(c),(uint32_t)(d),(uint32_t)(r))
#define WRAP0(a) void FUN_##a(void){OBS(0x##a,0,0,0,0,0,0);existing_actual_##a();OBS(0x##a,1,0,0,0,0,0);}
WRAP0(00402450) WRAP0(00402540) WRAP0(00402630) WRAP0(00402bb0) WRAP0(004cc3c0) WRAP0(004cc3e0)
void FUN_004cc500(int index){OBS(0x4cc500,0,index,0,0,0,0);existing_actual_004cc500(index);OBS(0x4cc500,1,index,0,0,0,0);}
void FUN_0040f8f0(undefined4 *out,uint n,undefined1 value){OBS(0x40f8f0,0,(uintptr_t)out,n,value,0,0);existing_actual_0040f8f0(out,n,value);OBS(0x40f8f0,1,(uintptr_t)out,n,value,0,0);}
char *FUN_005169c0(void){char *r;OBS(0x5169c0,0,0,0,0,0,0);r=existing_actual_005169c0();OBS(0x5169c0,1,0,0,0,0,(uintptr_t)r);return r;}
void FUN_004cdff0(uint32_t unused,char maps){const char *s=(const char *)(uintptr_t)unused;path_event(0x4cdff0,0,s);OBS(0x4cdff0,0,hash(s),length(s),(uint8_t)maps,0,0);existing_actual_004cdff0(unused,maps);path_event(0x4cdff0,1,s);OBS(0x4cdff0,1,hash(s),length(s),(uint8_t)maps,0,0);}
void FUN_004ce3a0(char *s){OBS(0x4ce3a0,0,(uintptr_t)s,0,0,0,0);existing_actual_004ce3a0(s);OBS(0x4ce3a0,1,(uintptr_t)s,0,0,0,0);}
int FUN_00562370(const char *a,const char *b){(void)a;(void)b;CHECK(0);return 0;}
uint8_t FUN_0041c740(RestoreFileList *self,const char *pattern,char flag)
{uint32_t j;CHECK(self==RESTORE_SAVE_FILES&&(int32_t)p[P_FILECOUNT]<=8);path_event(0x41c740,0,pattern);OBS(0x41c740,0,(uintptr_t)self,hash(pattern),length(pattern),(uint8_t)flag,0);self->offsets=(uint32_t *)(ALT+0x1000);self->count=(int32_t)p[P_FILECOUNT];self->capacity=8;self->strings=(char *)(ALT+0x2000);self->string_capacity=512;self->used=self->count>0?self->count*64:0;for(j=0;j<8;++j){self->offsets[j]=j*64;strcpy(self->strings+j*64,filenames[j]);}path_event(0x41c740,1,pattern);OBS(0x41c740,1,(uintptr_t)self,hash(pattern),length(pattern),(uint8_t)flag,p[P_ENUMRET]&255u);return(uint8_t)p[P_ENUMRET];}
void FUN_0041ca10(RestoreFileList *self,RestoreFileCompare compare){CHECK(self==RESTORE_SAVE_FILES&&compare==FUN_00562370);OBS(0x41ca10,0,(uintptr_t)self,0x562370,0,0,0);OBS(0x41ca10,1,(uintptr_t)self,0x562370,0,0,0);}
#pragma warning(push)
#pragma warning(disable:4100)
__declspec(naked) static void checked_call(uint32_t op,const uint32_t *q,uint32_t out[6])
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
 cmp eax,0
 je load_call
 cmp eax,1
 je save_call
 cmp eax,2
 je delete_call
 cmp eax,3
 je quit_call
 cmp eax,4
 je selector_call
 cmp eax,5
 je profile_call
 cmp eax,6
 je memset_call
 cmp eax,7
 je count_call
 cmp eax,8
 je home_call
 cmp eax,9
 je end_call
 push 031002000h
 call FUN_004ce3a0
 add esp,4
 jmp complete
load_call:call FUN_00402450
 jmp complete
save_call:call FUN_00402540
 jmp complete
delete_call:call FUN_00402630
 jmp complete
quit_call:call FUN_00402bb0
 jmp complete
selector_call:push dword ptr[ecx+P_MAPS*4]
 push 031003a00h
 call FUN_004cdff0
 add esp,8
 jmp complete
profile_call:call FUN_005169c0
 jmp complete
memset_call:push dword ptr[ecx+P_FILL*4]
 push dword ptr[ecx+P_SIZE*4]
 push 031004000h
 call FUN_0040f8f0
 add esp,12
 jmp complete
count_call:push dword ptr[ecx+P_INDEX*4]
 call FUN_004cc500
 add esp,4
 jmp complete
home_call:call FUN_004cc3c0
 jmp complete
end_call:call FUN_004cc3e0
complete:mov ecx,[esp+28]
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
}}
#pragma warning(pop)
static void prepare(const uint32_t *q){uint32_t j;DWORD old;memcpy(p,q,sizeof(p));used=seen=path_used=0;for(j=0;j<sizeof(regions)/sizeof(regions[0]);++j){CHECK(VirtualProtect((void *)(uintptr_t)regions[j][0],regions[j][1],PAGE_READWRITE,&old));memset((void *)(uintptr_t)regions[j][0],(uint8_t)(p[P_SEED]+j*29u),regions[j][1]);}CHECK(p[P_INDEX]<20&&(int32_t)p[P_FILECOUNT]<=8&&p[P_SIZE]<=80);B(0xe38dda)=(uint8_t)p[P_INITIAL_MODE];B(0xe38dca)=(uint8_t)p[P_DIRTY];U(0x6059e8)=p[P_CHOOSER];U(0xe20998)=p[P_VALUE];U(0xd3b1bc)=p[P_BACKMODE];B(0xd3b1c0)=(uint8_t)p[P_ACTION];U(0xe20990)=p[P_INDEX];make_text((char *)0xe38e74u,p[P_PROFILE_LENGTH],p[P_PROFILE_STYLE],31);make_text((char *)0xe370bcu,p[P_NAME_LENGTH],p[P_NAME_STYLE],63);strcpy((char *)0xe37100u,"EnteredPrevious");strcpy((char *)0xeadea4u,"previous-profile-path");strcpy((char *)(ALT+0x3a00),"outer-unused");for(j=0;j<20;++j){U(0xe2079c+j*4)=ALT+0x3000+j*100;U(0xe208f0+j*4)=p[P_MAX];U(0xe208a0+j*4)=p[P_LENGTH];U(0xe207ec+j*4)=p[P_CURSOR];make_text((char *)(ALT+0x3000+j*100),p[P_NAME_LENGTH],p[P_NAME_STYLE],63);}U(0xe2079c+2*4)=0xe37100;RESTORE_SAVE_FILES->offsets=(uint32_t *)(ALT+0x1000);RESTORE_SAVE_FILES->count=(int32_t)p[P_FILECOUNT];RESTORE_SAVE_FILES->capacity=8;RESTORE_SAVE_FILES->strings=(char *)(ALT+0x2000);RESTORE_SAVE_FILES->string_capacity=512;RESTORE_SAVE_FILES->used=(int32_t)p[P_FILECOUNT]>0?p[P_FILECOUNT]*64:0;for(j=0;j<8;++j){U(ALT+0x1000+j*4)=j*64;strcpy((char *)(ALT+0x2000+j*64),filenames[j]);}}
static void run(const uint32_t *q){uint32_t j,out[6];prepare(q);for(j=0;j<p[P_REPEATS];++j){checked_call(p[P_OP],p,out);CHECK(out[0]==0x11111111&&out[1]==0x22222222&&out[2]==0x33333333&&out[3]==0x44444444&&out[4]==out[5]);}}
static void digest(char hex[65]){HCRYPTHASH h;uint8_t bytes[32];DWORD n=32;uint32_t j;static const char d[]="0123456789abcdef";CHECK(CryptCreateHash(provider,CALG_SHA_256,0,0,&h));for(j=0;j<sizeof(regions)/sizeof(regions[0]);++j){CHECK(CryptHashData(h,(const BYTE *)regions[j],8,0));CHECK(CryptHashData(h,(const BYTE *)(uintptr_t)regions[j][0],regions[j][1],0));}CHECK(CryptGetHashParam(h,HP_HASHVAL,bytes,&n,0));CHECK(CryptDestroyHash(h));for(j=0;j<32;++j){hex[j*2]=d[bytes[j]>>4];hex[j*2+1]=d[bytes[j]&15];}hex[64]=0;}
int main(int argc,char **argv){uint32_t q[P_FIELDS]={0},j,k;char hex[65];SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOGPFAULTERRORBOX);CHECK(restore_data_open());CHECK(VirtualAlloc((void *)ALT,0x5000,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE));CHECK(CryptAcquireContextW(&provider,NULL,NULL,PROV_RSA_AES,CRYPT_VERIFYCONTEXT));if(argc==2&&!strcmp(argv[1],"--oracle-batch")){printf("{\"fields\":%u}\n",(uint32_t)P_FIELDS);fflush(stdout);for(;;){for(j=0;j<P_FIELDS;++j)if(scanf("%u",q+j)!=1)goto done;run(q);digest(hex);printf("{\"memory\":\"%s\",\"events\":[",hex);for(j=0;j<used;++j){if(j)putchar(',');putchar('[');for(k=0;k<32;++k){if(k)putchar(',');printf("%u",events[j][k]);}putchar(']');}printf("],\"paths\":[");for(j=0;j<path_used;++j){if(j)putchar(',');printf("[%u,%u,\"%s\"]",paths[j].fn,paths[j].phase,paths[j].hex);}puts("]}");fflush(stdout);}}else{q[P_SEED]=q[P_REPEATS]=1;q[P_MAX]=32;q[P_NAME_LENGTH]=31;q[P_PROFILE_LENGTH]=31;q[P_INDEX]=2;for(j=0;j<3;++j){q[P_OP]=j;run(q);CHECK(B(0xe38dda)==13&&B(0xe38dca)==1&&B(0xd3b1c0)==(j==0?1:j==1?0:2)&&U(0xe207ec+8)==31);}q[P_OP]=3;run(q);CHECK(B(0xe38dda)==0&&B(0xe38dca)==1);puts("Existing load/save/delete/quit actions: actual profile, selector, memset and caret algorithms.");}done:CHECK(CryptReleaseContext(provider,0));restore_data_close();return 0;}
