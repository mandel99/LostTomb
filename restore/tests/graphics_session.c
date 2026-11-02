#include "graphics_session.h"
#include "standalone_data.h"
#include "display_configuration.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define DRAW ((LPDIRECTDRAW)0x30000000u)
#define FRONT ((LPDIRECTDRAWSURFACE)0x30000100u)
#define BACK ((LPDIRECTDRAWSURFACE)0x30000200u)
#define CLIP ((LPDIRECTDRAWCLIPPER)0x30000300u)
#define UNKNOWN ((IUnknown *)0x30000400u)
#define WINDOW ((HWND)0xf1234567u)
static IDirectDrawVtbl draw_methods;
static IDirectDrawSurfaceVtbl surface_methods;
static IDirectDrawClipperVtbl clip_methods;
static IUnknownVtbl unknown_methods;
static uint32_t events[256][8],count,variant,format,creates,displays,current_style,cases_run;
static uint32_t red,green,blue;
static const struct {uintptr_t address;unsigned int width;} fields[]={
 {0x5e06b0,4},{0x5e06d4,1},{0x5e06d8,4},{0xe36c88,4},{0xe38d98,4},{0xe38d9c,4},
 {0xe38dac,4},{0xe38db0,4},{0xe38db4,4},{0xe38db8,4},{0xe38dca,1},{0xe38dda,1},
 {0xe38e58,4},{0xe38e5c,4},{0xe38e68,4},{0xe390e0,4},{0xe390e4,4},{0xe390e8,4},
 {0xe3912c,4},{0xe39148,4},{0xe3914c,4},{0xe39170,4},{0xe39174,4},{0xe39178,4},
 {0xe92dfc,4},{0xe92e00,4},{0xe92e04,4},{0xe92e18,4},{0xe92e4c,4},
 {0x6de518,4},{0x6de51c,4},{0x6de520,4},{0x6de524,4},{0x6de528,4},{0x6de52c,4},
 {0x6de530,4},{0x6de534,4},{0x6de538,4},{0x6de53c,4},{0x6de540,4}};
static void require_at(int value,int line) {if(!value) {fprintf(stderr,"Graphics reconstruction failure at line %d, variant %u\n",line,variant); abort();}}
#define REQUIRE(x) require_at(!!(x),__LINE__)
#define E(id,a,b,c,d,e,f,g) event(id,(uintptr_t)(a),(uintptr_t)(b),(uintptr_t)(c),(uintptr_t)(d),(uintptr_t)(e),(uintptr_t)(f),(uintptr_t)(g))
static void event(uint32_t id,uintptr_t a,uintptr_t b,uintptr_t c,uintptr_t d,uintptr_t e,uintptr_t f,uintptr_t g)
{uint32_t row[8]={id,(uint32_t)a,(uint32_t)b,(uint32_t)c,(uint32_t)d,(uint32_t)e,(uint32_t)f,(uint32_t)g}; REQUIRE(count<256); memcpy(events[count++],row,sizeof(row));}
static uint32_t text_hash(const char *s) {uint32_t h=2166136261u;while(*s) h=(h^(unsigned char)*s++)*16777619u;return h;}
static unsigned int occurrences(uint32_t id) {unsigned int i,n=0;for(i=0;i<count;++i)n+=events[i][0]==id;return n;}
void FUN_004ce6e0(char *text,int detail,int number) {REQUIRE(detail==0);E(1,text_hash(text),number,0,0,0,0,0);}
void FUN_004e2970(int id,int a,int b) {REQUIRE(id==0x39&&!a&&!b);E(2,id,a,b,0,0,0,0);}
void FUN_00414bd0(void) {E(3,0,0,0,0,0,0,0);}
void FUN_004e2ab0(void) {E(4,0,0,0,0,0,0,0);}
void FUN_004cfdc0(uint32_t mode) {E(5,mode,0,0,0,0,0,0);restore_display_configure(mode);}
static HRESULT WINAPI create_draw(GUID *guid,LPDIRECTDRAW *out,IUnknown *outer)
{REQUIRE(!guid&&!outer);E(10,0,0,0,0,0,0,0);if(variant&1)return DDERR_GENERIC;*out=DRAW;return DD_OK;}
static HRESULT WINAPI cooperative(LPDIRECTDRAW self,HWND window,DWORD flags)
{REQUIRE(self==DRAW&&window==WINDOW&&(flags==8||flags==19));E(11,self,window,flags,0,0,0,0);return variant&2?DDERR_GENERIC:DD_OK;}
static HRESULT WINAPI display_mode(LPDIRECTDRAW self,DWORD width,DWORD height,DWORD bits)
{REQUIRE(self==DRAW&&bits==16);E(12,self,width,height,bits,0,0,0);return displays++<((variant>>2)&3)?DDERR_GENERIC:DD_OK;}
static HRESULT WINAPI create_surface(LPDIRECTDRAW self,LPDDSURFACEDESC desc,LPDIRECTDRAWSURFACE *out,IUnknown *outer)
{
 REQUIRE(self==DRAW&&!outer&&desc->dwSize==108);
 E(13,self,desc->dwFlags,desc->dwWidth,desc->dwHeight,desc->ddsCaps.dwCaps,0,0);++creates;
 if(desc->ddsCaps.dwCaps==DDSCAPS_PRIMARYSURFACE) {if((variant&16)||((variant&524288)&&creates==1))return DDERR_GENERIC;*out=FRONT;}
 else {REQUIRE(desc->dwFlags==7&&(desc->ddsCaps.dwCaps==0x40||desc->ddsCaps.dwCaps==0x840));if(variant&32)return DDERR_GENERIC;*out=BACK;}
 return DD_OK;
}
static HRESULT WINAPI create_clipper(LPDIRECTDRAW self,DWORD flags,LPDIRECTDRAWCLIPPER *out,IUnknown *outer)
{REQUIRE(self==DRAW&&!flags&&!outer);E(14,self,0,0,0,0,0,0);if(variant&256)return DDERR_GENERIC;*out=CLIP;return DD_OK;}
static HRESULT WINAPI clip_window(LPDIRECTDRAWCLIPPER self,DWORD flags,HWND window)
{REQUIRE(self==CLIP&&!flags&&window==WINDOW);E(15,self,window,0,0,0,0,0);return variant&512?DDERR_GENERIC:DD_OK;}
static HRESULT WINAPI attach(LPDIRECTDRAWSURFACE self,LPDIRECTDRAWCLIPPER clipper)
{REQUIRE(self==FRONT&&clipper==CLIP);E(16,self,clipper,0,0,0,0,0);return variant&1024?DDERR_GENERIC:DD_OK;}
static void describe(LPDDSURFACEDESC desc,LPDIRECTDRAWSURFACE self)
{desc->dwWidth=1024;desc->dwHeight=768;desc->lPitch=2048;desc->lpSurface=(void *)(self==FRONT?0x31000000u:0x31010000u);desc->ddpfPixelFormat.dwRBitMask=red;desc->ddpfPixelFormat.dwGBitMask=green;desc->ddpfPixelFormat.dwBBitMask=blue;}
static HRESULT WINAPI lock(LPDIRECTDRAWSURFACE self,LPRECT rect,LPDDSURFACEDESC desc,DWORD flags,HANDLE handle)
{REQUIRE((self==FRONT||self==BACK)&&!rect&&desc->dwSize==108&&flags==1&&!handle);E(17,self,flags,0,0,0,0,0);if(variant&(self==FRONT?64:128))return DDERR_GENERIC;describe(desc,self);return DD_OK;}
static HRESULT WINAPI unlock(LPDIRECTDRAWSURFACE self,LPVOID ptr)
{REQUIRE((self==FRONT||self==BACK)&&!ptr);E(18,self,0,0,0,0,0,0);return DD_OK;}
static HRESULT WINAPI surface_desc(LPDIRECTDRAWSURFACE self,LPDDSURFACEDESC desc)
{REQUIRE(self==FRONT&&desc->dwSize==108&&desc->dwFlags==4096&&desc->ddpfPixelFormat.dwSize==32);E(19,self,0,0,0,0,0,0);if(variant&2048)return DDERR_GENERIC;describe(desc,self);return DD_OK;}
static ULONG WINAPI release_draw(LPDIRECTDRAW self) {REQUIRE(self==DRAW);E(20,self,0,0,0,0,0,0);return 0;}
static ULONG WINAPI release_surface(LPDIRECTDRAWSURFACE self) {REQUIRE(self==FRONT||self==BACK);E(20,self,0,0,0,0,0,0);return 0;}
static ULONG WINAPI release_clipper(LPDIRECTDRAWCLIPPER self) {REQUIRE(self==CLIP);E(20,self,0,0,0,0,0,0);return 0;}
static ULONG WINAPI release_unknown(IUnknown *self) {REQUIRE(self==UNKNOWN);E(20,self,0,0,0,0,0,0);return 0;}
static HRESULT WINAPI blt(LPDIRECTDRAWSURFACE self,LPRECT dest,LPDIRECTDRAWSURFACE src,LPRECT source,DWORD flags,LPDDBLTFX effects)
{
 REQUIRE(self==FRONT||self==BACK);
 if(flags==0x1000400) {REQUIRE(!dest&&!src&&!source&&effects&&effects->dwSize==100&&effects->dwFillColor==0);E(21,self,flags,effects->dwSize,effects->dwFillColor,0,0,0);return variant&(self==BACK?4096:8192)?DDERR_GENERIC:DD_OK;}
 REQUIRE(self==FRONT&&src==BACK&&dest&&source&&!flags&&!effects);
 E(22,self,dest->left,dest->top,dest->right,dest->bottom,0,0);E(23,source->left,source->top,source->right,source->bottom,0,0,0);return DD_OK;
}
static HRESULT WINAPI blt_fast(LPDIRECTDRAWSURFACE self,DWORD x,DWORD y,LPDIRECTDRAWSURFACE src,LPRECT rect,DWORD flags)
{REQUIRE(self==FRONT&&src==BACK&&!x&&!y&&!flags&&rect==(RECT *)0xe39130u);E(24,self,rect->left,rect->top,rect->right,rect->bottom,0,0);return DD_OK;}
static HDC WINAPI get_dc(HWND window) {REQUIRE(!window);return (HDC)0xfedcba98u;}
static int WINAPI caps(HDC dc,int key) {REQUIRE(dc==(HDC)0xfedcba98u);REQUIRE(key==24||key==14||key==12);return key==24?256:key==14?1:(variant&65536?24:16);}
static int WINAPI release_dc(HWND window,HDC dc) {REQUIRE(!window&&dc==(HDC)0xfedcba98u);return 1;}
static int WINAPI metrics(int key) {REQUIRE(key==0||key==1);return key==0?(variant&32768?320:1920):1080;}
static UINT WINAPI palette(HPALETTE p,UINT first,UINT n,LPPALETTEENTRY entries) {REQUIRE(!p&&!first&&n==256&&entries==(PALETTEENTRY *)0x70d508u);E(25,0,0,0,0,0,0,0);return n;}
static LONG WINAPI window_long(HWND w,int key) {REQUIRE(w==WINDOW&&(key==GWL_STYLE||key==GWL_EXSTYLE));return key==GWL_STYLE?(LONG)current_style:(LONG)0x92345678u;}
static LONG WINAPI set_long(HWND w,int key,LONG value) {LONG old=current_style;REQUIRE(w==WINDOW&&key==GWL_STYLE);current_style=value;E(26,w,key,value,0,0,0,0);return old;}
static HMENU WINAPI menu(HWND w) {REQUIRE(w==WINDOW);return (HMENU)0xfedcba98u;}
static BOOL WINAPI adjust(LPRECT r,DWORD style,BOOL has_menu,DWORD ex) {REQUIRE(has_menu&&ex==0x92345678u&&style==current_style);r->left-=4;r->top-=5;r->right+=6;r->bottom+=7;return 1;}
static BOOL WINAPI position(HWND w,HWND after,int x,int y,int width,int height,UINT flags) {REQUIRE(w==WINDOW);E(27,after,x,y,width,height,flags,0);return 1;}
static BOOL WINAPI parameters(UINT key,UINT value,PVOID out,UINT flags) {RECT r={11,13,1900,1000};REQUIRE(key==SPI_GETWORKAREA&&!value&&!flags);memcpy(out,&r,sizeof(r));return 1;}
static BOOL WINAPI window_rect(HWND w,LPRECT r) {REQUIRE(w==WINDOW);r->left=-10;r->top=-20;r->right=500;r->bottom=400;return 1;}
static BOOL WINAPI update(HWND w) {REQUIRE(w==WINDOW);E(28,w,0,0,0,0,0,0);return 1;}
static void WINAPI debug(LPCSTR text) {E(29,text_hash(text),0,0,0,0,0,0);}
static DWORD WINAPI ticks(void) {return 0x10;}
static const RestoreGraphicsApi api={get_dc,caps,release_dc,metrics,palette,create_draw,window_long,set_long,menu,adjust,position,parameters,window_rect,update,debug,ticks};
const RestoreGraphicsApi *restore_graphics_platform(void) {return &api;}
static void reset(uint32_t full,uint32_t v,uint32_t f,uint32_t previous,uint32_t mode)
{
 size_t i;RECT rect={17,19,1041,787};
 for(i=0;i<sizeof(fields)/sizeof(fields[0]);++i)memset((void *)fields[i].address,0,fields[i].width);
 count=creates=displays=0;variant=v;format=f;current_style=0x81234567u;
 red=f==1?0x7c00:f==3?0xfc00:f==4?0x1234:0xf800;
 green=f==1?0x3e0:f==2?0x3e0:f==3?0x3e0:f==4?0x1234:0x7e0;blue=f==2?0x3f:0x1f;
 GFX_P(HWND,0xe39140)=WINDOW;GFX_U(0xe38e5c)=full;GFX_U(0xe38e68)=3;GFX_U(0xe92e4c)=2;
 GFX_U(0xe38d98)=GFX_U(0xe38dac)=768;GFX_U(0xe38d9c)=GFX_U(0xe38db0)=1024;
 GFX_U(0xe92dfc)=previous;GFX_B(0xe38dda)=(uint8_t)mode;GFX_U(0xe36c88)=0xfffffff0u;
 GFX_U(0x5e06d8)=0;GFX_B(0x5e06d4)=0xff;GFX_U(0xe92e04)=0xaabbccddu;GFX_U(0xe92e18)=0xdeadbeefu;
 GFX_P(LPDIRECTDRAW,0xe39148)=v&16384?DRAW:0;
 GFX_U(0xe390e0)=v&131072?0xfedcba98u:0;
 GFX_P(IUnknown *,0xe92e00)=v&262144?UNKNOWN:0;
 *(RECT *)0xe39130u=rect;
 RESTORE_SURFACE_CONTEXT->steps=7;RESTORE_SURFACE_CONTEXT->brightness=-3;
 RESTORE_SURFACE_CONTEXT->front_pixels=(void *)0x11111111u;RESTORE_SURFACE_CONTEXT->back_pixels=(void *)0x22222222u;
}
static uint32_t run(uint32_t op)
{
 uint32_t result=0;++cases_run;
 if(op!=0&&op!=7&&op!=10)GFX_P(LPDIRECTDRAW,0xe39148)=DRAW;
 if((op>=3&&op<=6)||op==8||(op==7&&(variant&131072))) {GFX_P(LPDIRECTDRAWSURFACE,0xe39178)=FRONT;GFX_P(LPDIRECTDRAWSURFACE,0xe39174)=BACK;GFX_P(LPDIRECTDRAWCLIPPER,0xe39170)=CLIP;}
 switch(op) {
 case 0:return FUN_00426b40();case 1:return FUN_00426ff0();case 2:return (uint32_t)(uintptr_t)FUN_00427480(0x12345,0x23456,!!GFX_U(0xe38e5c),0);
 case 3:return FUN_00426ee0(!!GFX_U(0xe38e5c));case 4:return FUN_00426f60();
 case 5:return FUN_004d0480(RESTORE_SURFACE_CONTEXT,FRONT,BACK);case 6:FUN_004d03f0();break;
 case 7:return FUN_004d0220();case 8:FUN_004d01e0(!!GFX_U(0xe38e5c));break;
 case 9:return FUN_004d0040();case 10:return FUN_004d0060();default:abort();
 }return result;
}
static void snapshot(uint32_t *out) {size_t i;for(i=0;i<sizeof(fields)/sizeof(fields[0]);++i){out[i]=0;memcpy(out+i,(void *)fields[i].address,fields[i].width);}}
static void values(const uint32_t *v) {size_t i;putchar('[');for(i=0;i<sizeof(fields)/sizeof(fields[0]);++i)printf("%s%u",i?",":"",v[i]);putchar(']');}
static void oracle(void)
{
 unsigned int op,full,v,f,p,m,i,j;uint32_t before[40],after[40],result;
 printf("{\"fields\":[");for(i=0;i<40;++i)printf("%s[%u,%u]",i?",":"",(unsigned int)fields[i].address,fields[i].width);puts("]}");fflush(stdout);
 while(scanf("%u %u %u %u %u %u",&op,&full,&v,&f,&p,&m)==6) {
  reset(full,v,f,p,m);
  /* Match the public run's prepared input, before executing either implementation. */
  if(op!=0&&op!=7&&op!=10)GFX_P(LPDIRECTDRAW,0xe39148)=DRAW;
  if((op>=3&&op<=6)||op==8||(op==7&&(variant&131072))){GFX_P(LPDIRECTDRAWSURFACE,0xe39178)=FRONT;GFX_P(LPDIRECTDRAWSURFACE,0xe39174)=BACK;GFX_P(LPDIRECTDRAWCLIPPER,0xe39170)=CLIP;}
  snapshot(before);result=run(op);snapshot(after);printf("{\"before\":");values(before);printf(",\"after\":");values(after);
  printf(",\"return\":%u,\"events\":[",result);for(i=0;i<count;++i){printf("%s[",i?",":"");for(j=0;j<8;++j)printf("%s%u",j?",":"",events[i][j]);putchar(']');}puts("]}");fflush(stdout);
 }
}
static void golden(void)
{
 unsigned int f,v,op;uint32_t result;
 reset(0,0,0,0,0);REQUIRE(run(0)==1&&GFX_P(LPDIRECTDRAW,0xe39148)==DRAW&&GFX_U(0xe38e58)==16&&GFX_I(0xe390e8)==448&&GFX_I(0xe390e4)==156&&occurrences(27)==3);
 reset(0,65536,0,0,0);REQUIRE(run(0)==1&&GFX_U(0xe38e5c)==1&&occurrences(2)==1&&occurrences(12)==1);
 reset(0x100,8,0,0,0);REQUIRE(run(0)==1&&displays==3&&GFX_U(0xe38e68)==1&&occurrences(5)==2);
 reset(1,12,0,0,0);REQUIRE(run(0)==0&&!GFX_P(LPDIRECTDRAW,0xe39148)&&displays==3);
 reset(0,0,0,0,0);REQUIRE(run(1)==1&&GFX_P(void *,0xe92e18)==(void *)0x31010000u&&GFX_U(0xe3914c)==0x565&&GFX_U(0xe92dfc)==2&&occurrences(14)==1);
 reset(0x100,0,0,0,0);REQUIRE(run(1)==1&&occurrences(14)==0);
 for(f=0;f<4;++f){reset(1,0,f,0,0);REQUIRE(run(1)==1&&GFX_U(0xe92dfc)==(f==1?0:2));}
 reset(0,0,0,0,7);REQUIRE(run(1)==1&&GFX_U(0xe92dfc)==0&&GFX_U(0xe3914c)==0x565);
 reset(1,0,0,0,0);REQUIRE(run(2)==(uint32_t)(uintptr_t)BACK&&events[0][5]==0x840);
 reset(0,0,0,0,0);REQUIRE(run(3)==1&&GFX_P(LPDIRECTDRAW,0xe39148)==DRAW&&!GFX_P(void *,0xe92e18)&&occurrences(20)==3);
 reset(0x100,0,0,0,0);REQUIRE(run(3)==1&&!GFX_P(LPDIRECTDRAW,0xe39148)&&occurrences(20)==4);
 reset(0,4096,0,0,0);REQUIRE(run(4)==0&&occurrences(21)==1&&events[count-1][2]==1);
 reset(0,8192,0,0,0);REQUIRE(run(4)==0&&occurrences(21)==2&&events[count-1][2]==2);
 reset(0,0,0,0,0);REQUIRE(run(5)==1&&RESTORE_SURFACE_CONTEXT->format==1&&RESTORE_SURFACE_CONTEXT->width==1024&&RESTORE_SURFACE_CONTEXT->height==768&&RESTORE_SURFACE_CONTEXT->front_pixels==(void *)0x31000000u);
 reset(0,128,0,0,0);REQUIRE(run(5)==0&&RESTORE_SURFACE_CONTEXT->back_pixels==(void *)0x22222222u&&RESTORE_SURFACE_CONTEXT->width==0&&RESTORE_SURFACE_CONTEXT->format==-1);
 reset(0,0,0,0,0);run(6);REQUIRE(occurrences(22)==1&&occurrences(24)==0);
 reset(0x100,0,0,0,0);run(6);REQUIRE(occurrences(24)==1);
 reset(0,0,0,0,0);REQUIRE(run(7)==1&&GFX_U(0xe390e0)==1&&GFX_U(0xe3912c)==1&&GFX_U(0x5e06b0)==0&&GFX_B(0xe38dca)==1);
 reset(0,1,0,0,0);REQUIRE(run(7)==0&&GFX_U(0x5e06b0)==1&&GFX_U(0xe390e0)==0);
 reset(0,524288,0,0,0);REQUIRE(run(7)==1&&GFX_U(0xe38e68)==2&&GFX_U(0xe92e4c)==2&&occurrences(20)==1);
 reset(0,0,0,0,0);REQUIRE(run(9)==32&&GFX_U(0xe36c88)==0x10);
 for(op=0;op<=10;++op)for(f=0;f<4;++f)for(v=0;v<=18;++v){reset(f&1?0x100:0,v?1u<<(v-1):0,f,0,0);result=run(op);(void)result;}
}
int main(int argc,char **argv)
{
 REQUIRE(restore_data_open());REQUIRE(VirtualAlloc((void *)0x30000000u,0x1000,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE)==(void *)0x30000000u);
 draw_methods.Release=release_draw;draw_methods.SetCooperativeLevel=cooperative;draw_methods.SetDisplayMode=display_mode;draw_methods.CreateSurface=create_surface;draw_methods.CreateClipper=create_clipper;DRAW->lpVtbl=&draw_methods;
 surface_methods.Release=release_surface;surface_methods.Lock=lock;surface_methods.Unlock=unlock;surface_methods.GetSurfaceDesc=surface_desc;surface_methods.SetClipper=attach;surface_methods.Blt=blt;surface_methods.BltFast=blt_fast;FRONT->lpVtbl=BACK->lpVtbl=&surface_methods;
 clip_methods.Release=release_clipper;clip_methods.SetHWnd=clip_window;CLIP->lpVtbl=&clip_methods;unknown_methods.Release=release_unknown;UNKNOWN->lpVtbl=&unknown_methods;
 if(argc==2&&!strcmp(argv[1],"--oracle-batch"))oracle();else {golden();printf("Graphics C: %u cases; complete pointers, mode fallback, surfaces, COM ABI, context, release order and timer wrap verified; no game/window/graphics device used.\n",cases_run);}
 VirtualFree((void *)0x30000000u,0,MEM_RELEASE);restore_data_close();return 0;
}
