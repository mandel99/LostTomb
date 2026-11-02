#include <stdint.h>
#include "map_refresh_arithmetic.h"
#include "map_x87_conversion.h"
#include "map_refresh_postpass1.h"
#include "map_refresh_postpass3.h"
#include "map_refresh_postpass5.h"
void FUN_00475cc0(int32_t,int32_t,int32_t,int32_t,uint32_t);
#ifdef MAP_REFRESH_WHOLE_TRACE
uint64_t map_whole_read(uint32_t,uint32_t);
void map_whole_write(uint32_t,uint32_t,uint32_t);
void map_whole_fp(uint32_t,MRFloat);
void map_whole_child(uint32_t,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t);
#define MW_R(a,n) map_whole_read((a),(n))
#define MW_W(a,n,v) map_whole_write((a),(n),(v))
#define MW_FP(a,v) map_whole_fp((a),(v))
#define MW_CHILD(a,p,n,x,y,z,t,e,r) map_whole_child((a),(p),(n),(uint32_t)(x),(uint32_t)(y),(uint32_t)(z),(uint32_t)(t),(uint32_t)(e),(uint32_t)(r))
#else
static __inline uint64_t mw_read(uint32_t a,uint32_t n){if(n==1)return *(volatile uint8_t *)(uintptr_t)a;if(n==4)return *(volatile uint32_t *)(uintptr_t)a;return *(volatile uint64_t *)(uintptr_t)a;}
#define MW_R(a,n) mw_read((a),(n))
#define MW_W(a,n,v) do{if((n)==1)*(volatile uint8_t *)(uintptr_t)(a)=(uint8_t)(v);else *(volatile uint32_t *)(uintptr_t)(a)=(v);}while(0)
#define MW_FP(a,v) ((void)0)
#define MW_CHILD(a,p,n,x,y,z,t,e,r) ((void)0)
#endif
static __inline uint32_t mw_trunc(MRFloat value)
{
 uint8_t raw[10];MW_FP(0x56133cu,value);mr_fp_raw80(value,raw);return (uint32_t)restore_map_trunc80(raw);
}
/* Whole00488e90. Existing C callers use five cdecl arguments; original RET14h
   cleanup is compared through the caller-final-SP adapter, not declared identical.
   ECX is not a receiver. Floating operations retain exact PC/RC raw80 values. */
void FUN_00488e90(int32_t xmin,int32_t ymin,int32_t xmax,int32_t ymax,char update)
{
 uint32_t width,height,step,tile,object,definition,value,sprite;int32_t row,x,score;MRFloat v;uint16_t cw=mr_fp_control_word();
 if(xmin<0)xmin=0;if(ymin<0)ymin=0;
 width=(uint32_t)MW_R(0xbca2d0u,4);if(xmax>=(int32_t)width)xmax=(int32_t)(width-1u);
 height=(uint32_t)MW_R(0xbca2d4u,4);if(ymax>=(int32_t)height)ymax=(int32_t)(height-1u);
 step=width-(uint32_t)xmax+(uint32_t)xmin-1u;tile=(uint32_t)MW_R(0xbca2d8u,4)+(uint32_t)ymin*228u+(uint32_t)xmin;
 MW_W(0xbc9768u,4,tile);row=ymin;
 if(row<=ymax){do{
  x=xmin;
  if(x<=xmax){do{
   object=(uint32_t)MW_R(0xd05720u+tile*4u,4);value=0;
   if(object){
    if(MW_R(object+24u,1)){
     definition=(uint32_t)MW_R(object,4);v=mr_fp_i32((int32_t)(uint32_t)MW_R(definition+8u,4));MW_FP(0x488f2fu,v);
     v=mr_fp_mul(v,mr_fp_binary64(MW_R(0x56f528u,8)),cw);MW_FP(0x488f32u,v);
     v=mr_fp_mul(v,mr_fp_binary64(MW_R(0x56f660u,8)),cw);MW_FP(0x488f87u,v);value=mw_trunc(v);
    }
   }else if(MW_R(0xaf598cu+tile*4u,4)&0x800u){
    MW_CHILD(0x481690,0,3,x,row,4,0,0,0);score=FUN_00481690(x,row,4);MW_CHILD(0x481690,1,3,x,row,4,0,0,score);v=mr_fp_i32(score);MW_FP(0x488f5au,v);
    v=mr_fp_mul(v,mr_fp_binary64(MW_R(0x56f670u,8)),cw);MW_FP(0x488f5eu,v);value=mw_trunc(v);
    sprite=(uint32_t)MW_R(0xf56c64u,4)+value;++value;MW_W(0xb7482cu+tile*4u,4,sprite);
    v=mr_fp_i32((int32_t)value);MW_FP(0x488f7du,v);v=mr_fp_mul(v,mr_fp_binary64(MW_R(0x56f668u,8)),cw);MW_FP(0x488f81u,v);
    v=mr_fp_mul(v,mr_fp_binary64(MW_R(0x56f660u,8)),cw);MW_FP(0x488f87u,v);value=mw_trunc(v);
   }
   MW_W(0xa5d4ccu+tile,1,value);x=(int32_t)((uint32_t)x+1u);++tile;MW_W(0xbc9768u,4,tile);
  }while(x<=xmax);}
  tile+=(uint32_t)MW_R(0xbca2dcu,4)+step;row=(int32_t)((uint32_t)row+1u);MW_W(0xbc9768u,4,tile);
 }while(row<=ymax);}
 if((uint8_t)update){MW_CHILD(0x477500,0,4,xmin,ymin,xmax,ymax,0,0);FUN_00477500(xmin,ymin,xmax,ymax);MW_CHILD(0x477500,1,4,xmin,ymin,xmax,ymax,0,0);}
 MW_CHILD(0x4777c0,0,4,xmin,ymin,xmax,ymax,0,0);FUN_004777c0(xmin,ymin,xmax,ymax);MW_CHILD(0x4777c0,1,4,xmin,ymin,xmax,ymax,0,0);
 MW_CHILD(0x47b590,0,4,xmin,ymin,xmax,ymax,0,0);FUN_0047b590(xmin,ymin,xmax,ymax);MW_CHILD(0x47b590,1,4,xmin,ymin,xmax,ymax,0,0);
 MW_CHILD(0x475cc0,0,5,xmin,ymin,xmax,ymax,1,0);FUN_00475cc0(xmin,ymin,xmax,ymax,1);MW_CHILD(0x475cc0,1,5,xmin,ymin,xmax,ymax,1,0);
}
