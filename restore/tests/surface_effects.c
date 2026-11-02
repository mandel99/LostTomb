#include "graphics_session.h"
#include "standalone_data.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define FRONT ((LPDIRECTDRAWSURFACE)0x30000100u)
#define BACK ((LPDIRECTDRAWSURFACE)0x30000200u)
#define LOCAL_CONTEXT ((RestoreSurfaceContext *)0x30000800u)
#define FRONT_END 0x31041000u
#define BACK_END 0x31141000u
#define CAPACITY (0x40000u/2u-16u)
enum {P_OP,P_FORMAT,P_WIDTH,P_HEIGHT,P_STEPS,P_BRIGHTNESS,P_RANDOMIZE,P_EFFECT,P_BUFFER,P_CLIP,P_ALTERNATE,P_SEED,P_HISTORY,P_GATE,P_FORCE,P_MODE,P_COUNTER,P_ELAPSED,P_TICK,P_DIRTY,P_PREVIOUS_TICK,P_PARAMETERS};
static const uint32_t defaults[P_PARAMETERS]={0,1,17,33,2,0,0,0,1,17,0,0x12345678,99,0,0,1,1,0,0x10,0,0xfffffff0};
static const uintptr_t globals[]={0xd3b364,0xd3b368,0xd3b36c,0xd3b370,0xd3b374,0xd3b378,0xd3b380,0xd3b384,
 0xd3b1c4,0x5e06b0,0xf54054,0xe92e04,0x78d7a0,0xe92e08,0xe36c88,0xf53f60,0xf53f64,0xf53f78};
static struct {uintptr_t address;unsigned int width;} fields[142];
static size_t field_count,pixel_count;
static uint16_t *front_pixels,*back_pixels;
static IDirectDrawSurfaceVtbl surface_methods;
static uint32_t events[16][8],event_count,clock_value,cases_run;
static void require_at(int value,int line) {if(!value){fprintf(stderr,"Surface effects check failed at line %d\n",line);abort();}}
#define REQUIRE(x) require_at(!!(x),__LINE__)
static uint32_t hash_bytes(const void *memory,size_t size)
{const unsigned char *p=memory;uint32_t h=2166136261u;while(size--)h=(h^*p++)*16777619u;return h;}
static uint32_t image_hash(const uint16_t *p) {return hash_bytes(p-16,(pixel_count+16)*2);}
static void event(uint32_t id,uint32_t a,uint32_t b,uint32_t c,uint32_t d,uint32_t e,uint32_t f,uint32_t g)
{uint32_t row[8]={id,a,b,c,d,e,f,g};REQUIRE(event_count<16);memcpy(events[event_count++],row,sizeof(row));}
void FUN_0051e990(void)
{
 event(1,0,0,0,0,0,0,0);
 /* A controlled renderer writes one pixel before the frame effect. */
 if(pixel_count)back_pixels[pixel_count-1]^=0x1234;
}
static void present(uint32_t id,const RECT *source)
{event(id,RESTORE_SURFACE_CONTEXT->steps,image_hash(front_pixels),image_hash(back_pixels),(uint32_t)source->left,(uint32_t)source->top,(uint32_t)source->right,(uint32_t)source->bottom);}
static HRESULT WINAPI blt(LPDIRECTDRAWSURFACE self,LPRECT destination,LPDIRECTDRAWSURFACE from,LPRECT source,DWORD flags,LPDDBLTFX effects)
{REQUIRE(self==FRONT&&from==BACK&&destination&&source&&!flags&&!effects);present(2,source);return DD_OK;}
static HRESULT WINAPI blt_fast(LPDIRECTDRAWSURFACE self,DWORD x,DWORD y,LPDIRECTDRAWSURFACE from,LPRECT source,DWORD flags)
{REQUIRE(self==FRONT&&from==BACK&&!x&&!y&&source==(RECT *)0xe39130u&&!flags);present(3,source);return DD_OK;}
static DWORD WINAPI ticks(void) {return clock_value;}
static const RestoreGraphicsApi api={.ticks=ticks};
const RestoreGraphicsApi *restore_graphics_platform(void) {return &api;}
static void initialize_fields(void)
{
 size_t i;
 for(i=0;i<sizeof(globals)/sizeof(globals[0]);++i){fields[field_count].address=globals[i];fields[field_count++].width=4;}
 for(i=0;i<100;++i){fields[field_count].address=0xd3b1c8u+i*4;fields[field_count++].width=4;}
 for(i=0;i<11;++i){fields[field_count].address=(uintptr_t)RESTORE_SURFACE_CONTEXT+i*4;fields[field_count++].width=4;}
 for(i=0;i<11;++i){fields[field_count].address=(uintptr_t)LOCAL_CONTEXT+i*4;fields[field_count++].width=4;}
 fields[field_count].address=0xe38dc8;fields[field_count++].width=1;
 fields[field_count].address=0xe38dda;fields[field_count++].width=1;
 REQUIRE(field_count==142);
}
static void setup(const uint32_t *p)
{
 size_t i;RestoreSurfaceContext value;RECT rect={17,19,1041,787};
 int32_t width=(int32_t)p[P_WIDTH],height=(int32_t)p[P_HEIGHT];
 pixel_count=width>0&&height>0?(size_t)width*(size_t)height:0;REQUIRE(pixel_count<=CAPACITY);
 front_pixels=(uint16_t *)(FRONT_END-pixel_count*2);back_pixels=(uint16_t *)(BACK_END-pixel_count*2);
 for(i=0;i<16;++i){(front_pixels-16)[i]=(uint16_t)(0xa100u+i);(back_pixels-16)[i]=(uint16_t)(0xb200u+i);}
 for(i=0;i<pixel_count;++i){front_pixels[i]=(uint16_t)(i+p[P_SEED]);back_pixels[i]=(uint16_t)~(i+p[P_SEED]);}
 for(i=0;i<sizeof(globals)/sizeof(globals[0]);++i)GFX_U(globals[i])=0xaabb0000u+(uint32_t)i;
 for(i=0;i<100;++i)GFX_U(0xd3b1c8u+i*4)=0x5a000000u+(uint32_t)i;
 GFX_U(0xd3b1c4)=p[P_HISTORY];GFX_U(0xd3b384)=p[P_SEED];GFX_U(0xd3b380)=p[P_SEED]^0x9e3779b9u;
 GFX_U(0x5e06b0)=p[P_GATE]&65535u;GFX_U(0xf54054)=(p[P_GATE]>>16)&255u;GFX_U(0xe92e04)=p[P_GATE]&0x1000000u;
 GFX_U(0x78d7a0)=p[P_COUNTER];GFX_U(0xe92e08)=p[P_ELAPSED];GFX_U(0xe36c88)=p[P_PREVIOUS_TICK];clock_value=p[P_TICK];
 GFX_B(0xe38dc8)=(uint8_t)p[P_DIRTY];GFX_B(0xe38dda)=(uint8_t)p[P_MODE];
 GFX_U(0xf53f64)=p[P_CLIP];GFX_U(0xf53f60)=p[P_CLIP]-3u;GFX_U(0xf53f78)=p[P_ALTERNATE];
 memset(&value,0,sizeof(value));value.steps=(int32_t)p[P_STEPS];value.brightness=(int32_t)p[P_BRIGHTNESS];
 value.width=width;value.height=height;value.format=(int32_t)p[P_FORMAT];value.effect=(int32_t)p[P_EFFECT];
 value.randomize=(uint8_t)p[P_RANDOMIZE];value.padding[0]=0xab;value.padding[1]=0xcd;value.padding[2]=0xef;
 value.front=FRONT;value.back=BACK;value.front_pixels=front_pixels;value.back_pixels=back_pixels;
 *RESTORE_SURFACE_CONTEXT=value;*LOCAL_CONTEXT=value;
 *(RECT *)0xe39130u=rect;GFX_U(0xe38db0)=p[P_WIDTH];GFX_U(0xe38dac)=p[P_HEIGHT];GFX_U(0xe38e5c)=p[P_ALTERNATE];
 GFX_P(LPDIRECTDRAWSURFACE,0xe39178)=FRONT;GFX_P(LPDIRECTDRAWSURFACE,0xe39174)=BACK;event_count=0;
}
static void run(const uint32_t *p)
{
 RestoreSurfaceContext *self=p[P_OP]>=3?LOCAL_CONTEXT:RESTORE_SURFACE_CONTEXT;++cases_run;
 if(p[P_OP]==2)FUN_004d0280((int32_t)p[P_FORCE]);
 else if(p[P_OP]==1||p[P_OP]==4)FUN_004d05f0(self,(int32_t)p[P_BUFFER],(int32_t)p[P_STEPS],(int32_t)p[P_BRIGHTNESS],(uint8_t)p[P_RANDOMIZE],(int32_t)p[P_EFFECT]);
 else FUN_004d0650(self,(int32_t)p[P_BUFFER]);
}
static void snapshot(uint32_t *out) {size_t i;for(i=0;i<field_count;++i){out[i]=0;memcpy(out+i,(void *)fields[i].address,fields[i].width);}}
static void print_values(const uint32_t *v) {size_t i;putchar('[');for(i=0;i<field_count;++i)printf("%s%u",i?",":"",v[i]);putchar(']');}
static void print_image(const uint16_t *p)
{const unsigned char *bytes=(const unsigned char *)(p-16);size_t i;putchar('"');for(i=0;i<(pixel_count+16)*2;++i)printf("%02x",bytes[i]);putchar('"');}
static void oracle(void)
{
 uint32_t p[P_PARAMETERS],before[142],after[142];size_t i,j;
 printf("{\"fields\":[");for(i=0;i<field_count;++i)printf("%s[%u,%u]",i?",":"",(unsigned int)fields[i].address,fields[i].width);puts("]}");fflush(stdout);
 while(scanf("%u",p)==1){for(i=1;i<P_PARAMETERS;++i)REQUIRE(scanf("%u",p+i)==1);setup(p);snapshot(before);run(p);snapshot(after);
  printf("{\"before\":");print_values(before);printf(",\"after\":");print_values(after);printf(",\"images\":[");print_image(front_pixels);putchar(',');print_image(back_pixels);
  printf("],\"events\":[");for(i=0;i<event_count;++i){printf("%s[",i?",":"");for(j=0;j<8;++j)printf("%s%u",j?",":"",events[i][j]);putchar(']');}puts("]}");fflush(stdout);
 }
}
static void golden(void)
{
 uint32_t p[P_PARAMETERS],before[142],after[142],front_hash,back_hash;size_t i;unsigned int format,effect,randomize;
 memcpy(p,defaults,sizeof(p));p[P_WIDTH]=1;p[P_HEIGHT]=32;p[P_CLIP]=1;p[P_SEED]=0;
 for(format=0;format<2;++format){p[P_FORMAT]=format;setup(p);back_pixels[29]=back_pixels[30]=back_pixels[31]=0xffff;run(p);
  REQUIRE(back_pixels[29]==0xffff&&back_pixels[30]==(format?0xffdf:0x7fff)&&back_pixels[31]==back_pixels[30]);REQUIRE(RESTORE_SURFACE_CONTEXT->steps==1);}
 p[P_OP]=4;p[P_STEPS]=(uint32_t)-1;setup(p);front_hash=image_hash(front_pixels);back_hash=image_hash(back_pixels);run(p);
 REQUIRE(LOCAL_CONTEXT->steps==0&&RESTORE_SURFACE_CONTEXT->steps==-1&&image_hash(front_pixels)==front_hash&&image_hash(back_pixels)==back_hash);
 p[P_STEPS]=16;setup(p);run(p);REQUIRE(LOCAL_CONTEXT->steps==14&&RESTORE_SURFACE_CONTEXT->steps==16&&LOCAL_CONTEXT->padding[2]==0xef);
 p[P_OP]=0;p[P_FORMAT]=(uint32_t)-1;setup(p);snapshot(before);run(p);snapshot(after);REQUIRE(!memcmp(before,after,sizeof(before)));
 memcpy(p,defaults,sizeof(p));p[P_OP]=2;p[P_FORCE]=1;setup(p);run(p);REQUIRE(event_count==2&&events[0][0]==1&&events[1][0]==2&&RESTORE_SURFACE_CONTEXT->steps==0&&GFX_U(0xe92e08)==0);
 p[P_COUNTER]=136;setup(p);run(p);REQUIRE(event_count==9&&RESTORE_SURFACE_CONTEXT->steps==6&&GFX_U(0xe36c88)==p[P_PREVIOUS_TICK]&&GFX_U(0xd3b1c4)==8);
 p[P_GATE]=1;setup(p);snapshot(before);run(p);snapshot(after);REQUIRE(!memcmp(before,after,sizeof(before))&&event_count==0);
 p[P_GATE]=0x100;setup(p);run(p);REQUIRE(event_count==9);
 p[P_COUNTER]=1;p[P_FORCE]=0;p[P_GATE]=0;p[P_ELAPSED]=17;setup(p);run(p);REQUIRE(event_count==0&&GFX_U(0xe92e08)==49);
 p[P_ELAPSED]=18;setup(p);run(p);REQUIRE(event_count==2&&GFX_U(0xe92e08)==0);
 p[P_OP]=0;p[P_WIDTH]=512;p[P_HEIGHT]=158;p[P_CLIP]=511;p[P_BUFFER]=1;p[P_STEPS]=2;
 for(format=0;format<2;++format)for(effect=0;effect<2;++effect)for(randomize=0;randomize<2;++randomize){p[P_FORMAT]=format;p[P_EFFECT]=effect;p[P_RANDOMIZE]=randomize;setup(p);front_hash=image_hash(front_pixels);run(p);
  REQUIRE(front_hash==image_hash(front_pixels));for(i=0;i<512*30;++i)REQUIRE(back_pixels[i]==(uint16_t)~(i+p[P_SEED]));
  for(i=511;i<pixel_count;i+=512)REQUIRE(back_pixels[i]==(uint16_t)~(i+p[P_SEED]));
  for(i=0;i<16;++i)REQUIRE((back_pixels-16)[i]==0xb200u+i);}
 for(i=0;i<256;++i){memcpy(p,defaults,sizeof(p));p[P_OP]=2;p[P_MODE]=(uint32_t)i;p[P_FORCE]=1;setup(p);run(p);REQUIRE(event_count==((i==1||i==3||i==30||i==31||i==32||i==73)?2u:1u));}
}
static void reserve_buffer(uintptr_t base)
{REQUIRE(VirtualAlloc((void *)base,0x50000,MEM_RESERVE,PAGE_NOACCESS)==(void *)base);REQUIRE(VirtualAlloc((void *)(base+0x1000),0x40000,MEM_COMMIT,PAGE_READWRITE)==(void *)(base+0x1000));}
int main(int argc,char **argv)
{
 REQUIRE(restore_data_open());REQUIRE(VirtualAlloc((void *)0x30000000u,0x10000,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE)==(void *)0x30000000u);
 reserve_buffer(0x31000000);reserve_buffer(0x31100000);surface_methods.Blt=blt;surface_methods.BltFast=blt_fast;FRONT->lpVtbl=BACK->lpVtbl=&surface_methods;
 initialize_fields();if(argc==2&&!strcmp(argv[1],"--oracle-batch"))oracle();else {golden();printf("Surface effects C: %u scenarios, both pixel formats, all pixel values, clipping, guard pages, local receivers, random history and frame gates verified; no game/window/device used.\n",cases_run);}
 VirtualFree((void *)0x31000000,0,MEM_RELEASE);VirtualFree((void *)0x31100000,0,MEM_RELEASE);VirtualFree((void *)0x30000000,0,MEM_RELEASE);restore_data_close();return 0;
}
