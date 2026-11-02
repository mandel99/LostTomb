#include "city_ui_layout.h"
#include "menu_text_glyph.h"
#include <stdint.h>
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
extern int32_t FUN_00430cc0(void);
extern void FUN_004cda40(int32_t,int32_t,int32_t,int32_t,uint32_t);
extern void FUN_004d62f0(uint32_t,uint32_t,int32_t,int32_t,uint32_t,uint32_t,int32_t,int32_t);
extern void FUN_004d6860(uint32_t,uint32_t,const char *,int32_t,int32_t,uint32_t,uint32_t,int32_t,int32_t);
extern void FUN_004d6250(uint32_t,uint32_t,int32_t,int32_t,uint32_t,uint32_t,uint32_t,int32_t,int32_t);
extern void FUN_0051f780(void);
extern void FUN_00536820(int32_t,int32_t,int32_t,int32_t);

static void label(uint32_t entry,uint32_t x,uint32_t y,uint32_t font,uint32_t color)
{
 FUN_004d62f0(6,entry,(int32_t)x,(int32_t)y,font,color,(int32_t)U(0xe38da8u),(int32_t)U(0xe38da4u));
}
static void number(uint32_t value,uint32_t suffix,uint32_t x,uint32_t y,uint32_t font,uint32_t color)
{
 FUN_004d6860(value,0x40,(const char *)(uintptr_t)suffix,(int32_t)x,(int32_t)y,font,color,
              (int32_t)U(0xe38da8u),(int32_t)U(0xe38da4u));
}
static uint32_t months_left(uint32_t final_year)
{
 uint32_t year=U(0x78d7bcu),years=0;
 if((int32_t)final_year>(int32_t)(year+1u))years=final_year-year-1u;
 return (years*3u+3u)*4u-U(0x78d7c0u);
}

/* Whole native51F790. Fonts, calendar years/month and mode flags are DWORDs;
   only tomb-placement/pause flags are BYTEs and the campaign gate is a WORD. */
void restore_city_draw_mission_status(void)
{
 uint32_t font,months,year,kind,x,y,saved_cursor,message;
 if(B(0x6de514u)){
  font=U(0xf56f84u);kind=U(0xe92dfcu)==2u?0xffffff3au:0x7f9au;
  U(0xe20778u)=0;label(6,8,47,font,0);label(6,7,46,font,kind);
 }else if(U(0xea5d40u+U(0xeaac28u)*0x49d8u)){
  font=U(0xf56f84u);kind=U(0xe92dfcu)==2u?0xffffff3au:0x7f9au;
  U(0xe20778u)=0;label(5,8,47,font,0);label(5,7,46,font,kind);
 }else if(!W(0x784b60u)){
  if(U(0x784c48u)){
   year=U(0x78d7b8u);kind=U(0xea6018u+U(0xeaac28u)*0x49d8u);
   if(kind==0u)year+=7u;else if(kind==1u)year+=2u;
   months=months_left(year);
   FUN_004cda40(1,40,230,20,0x1fe3);
   font=U(0xf56f84u);
   for(x=7;x<9;++x)for(y=45;y<49;y+=2){
    U(0xe20778u)=0;label(2,x,y,font,0);
    number(months,0x57c048u,U(0xe20778u)+x,y,font,0);
   }
   U(0xe20778u)=0;label(2,7,46,font,0xf800);
   number(months,0x57c048u,U(0xe20778u)+7u,46,font,0xf800);
  }else if(U(0x784c50u)){
   months=months_left(U(0x78d7b8u));
   FUN_004cda40(1,42,200,18,0x1fe3);
   U(0xe20778u)=0;label(3,2,46,U(0xf56f84u),0xf800);
   number(months,0x57c048u,U(0xe20778u)+2u,46,U(0xf56f84u),0xf800);
  }
 }
 U(0xe20778u)=0;FUN_0051f780();FUN_0051f780();
 if(U(0x605a0cu)==1u)
  FUN_004cc770((const uint8_t *)(uintptr_t)0x5f89c0u,(int32_t)(U(0xe20778u)+10u),61,(int32_t)U(0xf56f84u),0xffff);
 if(U(0xf53f70u)){
  number(U(0xea1880u+U(0xeaac28u)*0x49d8u),0x5f89b4u,2,80,U(0xf56f84u),0);
  number(U(0xea1880u+U(0xeaac28u)*0x49d8u),0x5f89b4u,1,81,U(0xf56f84u),0xffff);
  number(U(0xea5f18u+U(0xeaac28u)*0x49d8u),0x5f89a4u,2,100,U(0xf56f84u),0);
  number(U(0xea5f18u+U(0xeaac28u)*0x49d8u),0x5f89a4u,1,101,U(0xf56f84u),0xffff);
 }
 if(U(0x605a00u)){
  saved_cursor=U(0xe20778u);U(0xe20778u)=0;
  FUN_004d62f0(157,U(0x605a08u),0,121,U(0xf56f84u),0xffff,(int32_t)U(0xe38da8u),(int32_t)U(0xe38da4u));
  message=0;
  switch(U(0x605a00u)){
   case 1:message=0x5f8998u;break;case 2:message=0x5f898cu;break;
   case 3:message=0x5f8980u;break;case 4:message=0x5f8974u;break;
  }
  if(message)FUN_004cc770((const uint8_t *)(uintptr_t)message,(int32_t)(U(0xe20778u)+1u),121,(int32_t)U(0xf56f84u),0xffff);
  FUN_004cc770((const uint8_t *)(uintptr_t)0x5f896cu,(int32_t)U(0xe20778u),101,(int32_t)U(0xf56f84u),0xffff);
  number(U(0x605a04u),0x5f8964u,U(0xe20778u)+2u,121,U(0xf56f84u),0xffff);
  U(0xe20778u)=saved_cursor;
 }
 if(B(0xe38e60u)){
  int32_t left=(int32_t)((uint32_t)FUN_00430cc0()-448u)/2;
  FUN_00536820(left,40,28,3);
  FUN_004d6250(13,2,left,58,448,U(0xf56f8cu),0,(int32_t)U(0xe38da8u),(int32_t)U(0xe38da4u));
 }
}
void FUN_0051f790(void){restore_city_draw_mission_status();}
