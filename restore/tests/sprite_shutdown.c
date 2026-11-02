#include "sprite_shutdown.h"
#include "standalone_data.h"
#include "host_allocator.h"
#include <windows.h>
#include <wincrypt.h>
#include <stdio.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>
#include <stdlib.h>
#include <crtdbg.h>
#define FIELDS 24
#define PRIVATE_SELF 0x30000040u
#define HEAP_BASE 0x31000000u
#define OBJECT_BASE 0x32000000u
#define GLOBAL_SELF 0xf55bb0u
#define SIZE_MANAGER 0x30f5d8u
#define U(a) (*(uint32_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(uint32_t)(a))
static const uint32_t regions[][2]={{PRIVATE_SELF-64,SIZE_MANAGER+128},{GLOBAL_SELF-64,SIZE_MANAGER+128},{0xe92dfc,64},{0xea1348,128},{HEAP_BASE,0x10000},{OBJECT_BASE,0x3000}};
static uint32_t p[FIELDS],events[512][16],event_count,free_count,virtual_count,checks;static HCRYPTPROV provider;
static uint8_t background[0x650000];static uint32_t background_seed;static int valid_background;static void (*registered)(void);
void sprite_shutdown_free_real(void *),sprite_shutdown_delete_real(void *,uint32_t),sprite_shutdown_slot_real(int *);
int sprite_shutdown_register_real(void (*)(void));
void sprite_shutdown_release_real(int *,char),sprite_shutdown_library_destroy_real(RestoreSpriteLibrary *);
RestoreSpriteLibrary *sprite_shutdown_library_construct_real(RestoreSpriteLibrary *);
#define CHECK(x) do{++checks;if(!(x)){fprintf(stderr,"Sprite shutdown failure line%d\n",__LINE__);ExitProcess(1);}}while(0)
static RestoreSpriteLibraries *active(void){uint32_t op=p[0];return (RestoreSpriteLibraries *)(uintptr_t)((op==3||op==5||op==6||op==11||op==12||p[2])?GLOBAL_SELF:PRIVATE_SELF);}
static uint32_t object_pointer(uint32_t kind){return kind?(OBJECT_BASE+(kind==2?0x200:0x100)):0;}
static void event(uint32_t type,uint32_t arg,uint32_t extra,uint32_t ordinal)
{
 uint32_t *e;RestoreSpriteLibraries *s=active();RestoreSpriteLibrary *l=&s->libraries[p[8]%30];CHECK(event_count<512);e=events[event_count++];e[0]=type;e[1]=arg;e[2]=extra;e[3]=ordinal;e[4]=U(0xea136c);e[5]=U(0xea1370);e[6]=U(0xea1368);e[7]=U(0xe92e1c);e[8]=(uint32_t)(uintptr_t)s->alternate;e[9]=(uint32_t)(uintptr_t)s->scratch;e[10]=(uint32_t)(uintptr_t)l->pixels;e[11]=(uint8_t)l->name[0];e[12]=s->loaded;e[13]=s->next_tag;e[14]=s->next_image;e[15]=s->pixel_bytes;
}
static void free_mutation(void)
{
 RestoreSpriteLibraries *s=active();RestoreSpriteLibrary *l=&s->libraries[p[14]%30];uint32_t flags=p[12];if(p[13]&&p[13]!=free_count)return;
 if(flags&1)l->pixels=(uint8_t *)(uintptr_t)p[15];if(flags&2){l->name[0]=(char)p[15];l->pixel_bytes=p[15];l->image_count=p[15]^0x12345678;l->tag_count=p[15]^0x87654321;}
 if(flags&4)s->alternate=(void *)(uintptr_t)p[15];if(flags&8)s->scratch=(void *)(uintptr_t)p[15];if(flags&16)U(0xe92e1c)=p[16];
 if(flags&32){U(0xea136c)=p[15];U(0xea1370)=p[15]^0x12340000;}
 if(flags&64){s->loaded=p[15];s->next_tag=p[15];s->next_image=p[15];s->pixel_bytes=p[15];}
 if(flags&128)U(0xea1370)=object_pointer(p[19]);
}
void restore_sprite_shutdown_free(void *allocation){++free_count;event(0x561a2b,(uint32_t)(uintptr_t)allocation,0,free_count);free_mutation();}
void FUN_00561a2b(uint32_t allocation){restore_sprite_shutdown_free((void *)(uintptr_t)allocation);}
void restore_sprite_shutdown_delete(void *object,uint32_t flags)
{
 uint32_t o=(uint32_t)(uintptr_t)object;RestoreSpriteLibraries *s=(RestoreSpriteLibraries *)(uintptr_t)GLOBAL_SELF;++virtual_count;event(U(U(o)),o,flags,virtual_count);
 if(p[18]&&p[18]!=virtual_count)return;
 if(p[17]&1)U(0xea1370)=object_pointer(p[19]);if(p[17]&2)U(0xea136c)=object_pointer(2);
 if(p[17]&4)s->libraries[p[14]%30].pixels=(uint8_t *)(uintptr_t)p[15];if(p[17]&8){s->alternate=(void *)(uintptr_t)p[15];s->scratch=(void *)(uintptr_t)p[16];}
 if(p[17]&16)U(0xea1368)=0;if(p[17]&32)U(o+0x10)^=0x12345678;
}
void restore_sprite_shutdown_slot(int *slot){event(0x55d960,(uint32_t)(uintptr_t)slot,(uint32_t)*slot,0);CHECK(!*slot);sprite_shutdown_slot_real(slot);}
int restore_sprite_shutdown_register(void (*callback)(void)){CHECK(callback==FUN_0053da80);event(0x561953,0x53da80,p[11],0);if(!p[11])registered=callback;return (int)p[11];}
void FUN_0053f790(int *slot,char flag){event(0x53f790,(uint32_t)(uintptr_t)slot,(uint8_t)flag,0);sprite_shutdown_release_real(slot,flag);event(0x8053f790,(uint32_t)(uintptr_t)slot,(uint8_t)flag,0);}
RestoreSpriteLibrary *FUN_005404e0(RestoreSpriteLibrary *s){RestoreSpriteLibrary *r;event(0x5404e0,(uint32_t)(uintptr_t)s,0,0);r=sprite_shutdown_library_construct_real(s);event(0x805404e0,(uint32_t)(uintptr_t)s,(uint32_t)(uintptr_t)r,0);return r;}
void FUN_00540500(RestoreSpriteLibrary *s){event(0x540500,(uint32_t)(uintptr_t)s,0,0);sprite_shutdown_library_destroy_real(s);event(0x80540500,(uint32_t)(uintptr_t)s,0,0);}
void FUN_0055d8b0(void){fputs("Unreachable non-null auxiliary destructor in private proof.\n",stderr);ExitProcess(7);}
static uint32_t pointer_kind(uint32_t kind,uint32_t heap,uint32_t alternate){return kind==0?0:kind==2?p[6]:kind==3?alternate:heap;}
static void manager_fields(uint32_t address)
{
 unsigned i;RestoreSpriteLibraries *s=(RestoreSpriteLibraries *)(uintptr_t)address;
 for(i=0;i<30;++i)s->libraries[i].pixels=(uint8_t *)(uintptr_t)((p[3]&(1u<<i))?((p[7]&(1u<<i))?p[6]:HEAP_BASE+0x100+i*0x100):0);
 s->alternate=(void *)(uintptr_t)pointer_kind(p[4],HEAP_BASE+0x4000,(uint32_t)(uintptr_t)s->libraries[p[8]%30].pixels);
 s->scratch=(void *)(uintptr_t)pointer_kind(p[5],HEAP_BASE+0x5000,(uint32_t)(uintptr_t)s->alternate);
}
static void prepare(void)
{
 unsigned i,offset=0;size_t total=0;for(i=0;i<sizeof(regions)/sizeof(regions[0]);++i)total+=regions[i][1];CHECK(total<=sizeof(background));
 if(!valid_background||background_seed!=p[1]){for(i=0;i<total;++i)background[i]=(uint8_t)((i*13u+(i>>5)*7u+p[1])^0xa7u);background_seed=p[1];valid_background=1;}
 for(i=0;i<sizeof(regions)/sizeof(regions[0]);++i){memcpy((void *)(uintptr_t)regions[i][0],background+offset,regions[i][1]);offset+=regions[i][1];}
 U(0xe92e1c)=p[6];manager_fields(PRIVATE_SELF);manager_fields(GLOBAL_SELF);U(0xea136c)=object_pointer(p[9]);U(0xea1370)=object_pointer(p[10]);U(0xea1368)=0;
 U(OBJECT_BASE+0x100)=OBJECT_BASE+0x2000;U(OBJECT_BASE+0x200)=OBJECT_BASE+0x2010;U(OBJECT_BASE+0x2000)=0x36000010;U(OBJECT_BASE+0x2010)=0x36000020;U(0xea1390)=pointer_kind(p[21],HEAP_BASE+0x6000,p[6]);registered=NULL;free_count=virtual_count=event_count=0;
}
static uint32_t invoke(void)
{
 uint32_t sp0,sp1,bp0,bp1,bx_saved,si_saved,di_saved,result=0,op=p[0],flag=p[20];RestoreSpriteLibraries *s=(RestoreSpriteLibraries *)(uintptr_t)(p[2]?GLOBAL_SELF:PRIVATE_SELF);RestoreSpriteLibrary *l=&s->libraries[p[8]%30];int *slot=(int *)(uintptr_t)0xea1390;
 __asm {
 mov sp0,esp
 mov bp0,ebp
 mov ebx,11223344h
 mov esi,33445566h
 mov edi,44556677h
 cmp op,3
 je global_ctor
 cmp op,4
 je registration
 cmp op,5
 je cleanup
 cmp op,6
 je controller
 cmp op,7
 je library_ctor
 cmp op,8
 je library_dtor
 cmp op,9
 je release
 cmp op,11
 je scheduled
 cmp op,12
 je startup_schedule
 push s
 cmp op,1
 je constructor
 cmp op,2
 je destructor
 call FUN_0053db60
 add esp,4
 cmp op,10
 jne save_result
 push s
 call FUN_0053db60
 add esp,4
 jmp save_result
 constructor:
 call FUN_0053da90
 add esp,4
 jmp save_result
 destructor:
 call FUN_0053db00
 add esp,4
 jmp done
 global_ctor:
 call FUN_0053da60
 jmp save_result
 registration:
 call FUN_0053da70
 jmp save_result
 cleanup:
 call FUN_0053da80
 jmp done
 controller:
 call FUN_004e63b0
 jmp done
 library_ctor:
 push l
 call FUN_005404e0
 add esp,4
 jmp save_result
 library_dtor:
 push l
 call FUN_00540500
 add esp,4
 jmp done
 release:
 push flag
 push slot
 call FUN_0053f790
 add esp,8
 jmp done
 startup_schedule:
 call FUN_0053da60
 scheduled:
 call FUN_0053da70
 test eax,eax
 jnz done
 call registered
 jmp done
 save_result:
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
 HCRYPTHASH hash;DWORD length=32;unsigned i;CHECK(CryptCreateHash(provider,CALG_SHA_256,0,0,&hash));for(i=0;i<sizeof(regions)/sizeof(regions[0]);++i){CHECK(CryptHashData(hash,(const BYTE *)regions[i],8,0));CHECK(CryptHashData(hash,(const BYTE *)(uintptr_t)regions[i][0],regions[i][1],0));}CHECK(CryptGetHashParam(hash,HP_HASHVAL,bytes,&length,0));CHECK(CryptDestroyHash(hash));
}
static void empty_callback(void){}
int main(int argc,char **argv)
{
 uint32_t i,result;uint8_t hash[32];CHECK(restore_data_open());SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOGPFAULTERRORBOX);_set_abort_behavior(0,_WRITE_ABORT_MSG|_CALL_REPORTFAULT);
 CHECK(VirtualAlloc((void *)(uintptr_t)(PRIVATE_SELF-64),SIZE_MANAGER+128,MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE)!=NULL);CHECK(VirtualAlloc((void *)(uintptr_t)HEAP_BASE,0x10000,MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE)!=NULL);CHECK(VirtualAlloc((void *)(uintptr_t)OBJECT_BASE,0x3000,MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE)!=NULL);
 if(argc>1&&!strcmp(argv[1],"--unsupported-delete")){sprite_shutdown_delete_real((void *)(uintptr_t)OBJECT_BASE,1);return 0;}
 if(argc>1&&!strcmp(argv[1],"--unsupported-slot")){int nonnull=0x12345678;sprite_shutdown_slot_real(&nonnull);return 0;}
 CHECK(CryptAcquireContextW(&provider,NULL,NULL,PROV_RSA_AES,CRYPT_VERIFYCONTEXT));
 if(argc>1&&!strcmp(argv[1],"--oracle-batch")){uint32_t ready=0x53485554;_setmode(_fileno(stdout),_O_BINARY);fwrite(&ready,4,1,stdout);fflush(stdout);for(;;){for(i=0;i<FIELDS;++i)if(scanf("%u",&p[i])!=1)return 0;prepare();result=invoke();digest(hash);fwrite(&result,4,1,stdout);fwrite(hash,32,1,stdout);fwrite(&event_count,4,1,stdout);fwrite(events,64,event_count,stdout);fflush(stdout);}}
 {uint32_t q[FIELDS]={0,123,0,0x3fffffff,1,1,HEAP_BASE+0x7000,0,0,0,0,0,0,0,0,HEAP_BASE+0x8000,HEAP_BASE+0x9000,0,0,0,0,1,0,0};memcpy(p,q,sizeof(p));}
 prepare();CHECK(invoke()==1&&free_count==32&&active()->loaded==0&&active()->next_tag==1);p[0]=1;prepare();CHECK(invoke()==PRIVATE_SELF&&free_count==0);p[0]=2;prepare();invoke();CHECK(free_count==32);p[0]=4;prepare();CHECK(invoke()==0&&registered==FUN_0053da80);p[0]=9;p[21]=2;prepare();invoke();CHECK(U(0xea1390)==p[6]&&free_count==0);p[20]=1;prepare();invoke();CHECK(!U(0xea1390));
 {void *allocation=restore_heap_allocate(37);CHECK(allocation!=NULL);sprite_shutdown_free_real(allocation);CHECK(sprite_shutdown_register_real(empty_callback)==0);}
 printf("Sprite shutdown: %u checks; typed manager/library lifecycle, selective clear, compiled cleanup callback and host boundaries passed.\n",checks);return 0;
}
