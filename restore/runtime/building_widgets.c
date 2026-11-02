#include "building_widgets.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
extern uint32_t FUN_00562348(void);

uint32_t restore_widget_initialize_0041fca0(void *self,uint16_t style)
{
 uint32_t object=(uint32_t)(uintptr_t)self,type,table,count,vector,slot=0,p,options=0,index,value,alternate,image;
 W(object+16u)=style;type=U(object);
 if((int32_t)type<0||(int32_t)type>=253)return type;
 U(object+4u)=UINT32_MAX;table=0x5be9b8u+type*40u;count=U(table+28u);
 if((int32_t)count<=0)return count;
 vector=U(table+24u);p=vector;
 while((int32_t)U(p)>=0){++slot;p+=40u;if((int32_t)slot>=(int32_t)count)return count;}
 p=vector+slot*40u+8u;
 while((int32_t)U(p)>=0){++options;p+=8u;if(options>=4u)break;}
 if(!options)return p;
 value=FUN_00562348();index=(uint32_t)((int32_t)value%(int32_t)options)+slot*5u;
 vector=U(table+24u);value=U(vector+index*8u+8u);U(object+4u)=value;
 vector=U(table+24u);alternate=B(vector+index*8u+12u);B(object+21u)=(uint8_t)alternate;
 if((int32_t)value<0){U(object+4u)=UINT32_MAX;return value;}
 if((int32_t)value>=(int32_t)U(table+20u)){U(object+4u)=UINT32_MAX;return value;}
 if(alternate){image=U(table+16u);U(object+8u)=U(image+value*48u+12u);return image;}
 U(object+8u)=0;return value;
}
void FUN_0041fca0(void *self,uint16_t style)
{(void)restore_widget_initialize_0041fca0(self,style);}

static void refresh(uint32_t first,uint32_t end,uint32_t configuration)
{
 uint32_t object,value;
 do{
  object=U(first);W(object+18u)=0;object=U(first);
  (void)restore_widget_initialize_0041fca0((void *)(uintptr_t)object,3);
  object=U(first);value=U(configuration);first+=16u;configuration+=16u;
  U(object+12u)=value;
 }while(first<end);
}
void FUN_0041ff90(void){refresh(0x5c2750u,0x5c28b0u,0x5c2754u);}
void FUN_0041ffc0(void){refresh(0x596088u,0x596158u,0x59608cu);}
void FUN_004206a0(void){refresh(0x59edf0u,0x59efa0u,0x59edf4u);}
void FUN_004209e0(void){refresh(0x5a8148u,0x5a84c8u,0x5a814cu);}
void FUN_00420d10(void){refresh(0x5ad218u,0x5ad3c8u,0x5ad21cu);}
void FUN_00421010(void){refresh(0x5b0060u,0x5b0150u,0x5b0064u);}
void FUN_004212f0(void)
{
 refresh(0x5be510u,0x5be540u,0x5be514u);
 refresh(0x5be540u,0x5be570u,0x5be544u);
 refresh(0x5be570u,0x5be5a0u,0x5be574u);
 refresh(0x5be750u,0x5be780u,0x5be754u);
 /* Original4213AF deliberately uses5BE574, not this group's+4 address. */
 refresh(0x5be780u,0x5be7b0u,0x5be574u);
 refresh(0x5be5a0u,0x5be5d0u,0x5be5a4u);
 refresh(0x5be5d0u,0x5be600u,0x5be5d4u);
 refresh(0x5be600u,0x5be630u,0x5be604u);
 refresh(0x5be630u,0x5be660u,0x5be634u);
 refresh(0x5be7b0u,0x5be7e0u,0x5be7b4u);
 refresh(0x5be7e0u,0x5be810u,0x5be7e4u);
 refresh(0x5be660u,0x5be690u,0x5be664u);
 refresh(0x5be690u,0x5be6c0u,0x5be694u);
 refresh(0x5be8a0u,0x5be8d0u,0x5be8a4u);
 refresh(0x5be810u,0x5be840u,0x5be814u);
 refresh(0x5be8d0u,0x5be900u,0x5be8d4u);
 refresh(0x5be900u,0x5be930u,0x5be904u);
 refresh(0x5be6c0u,0x5be6f0u,0x5be6c4u);
 refresh(0x5be6f0u,0x5be720u,0x5be6f4u);
 refresh(0x5be720u,0x5be750u,0x5be724u);
 refresh(0x5be840u,0x5be870u,0x5be844u);
 refresh(0x5be870u,0x5be8a0u,0x5be874u);
 refresh(0x5be930u,0x5be960u,0x5be934u);
 refresh(0x5be960u,0x5be990u,0x5be964u);
 refresh(0x5be990u,0x5be9c0u,0x5be994u);
}

void FUN_004e5b90(int32_t building)
{
 uint32_t kind,index=0,offset=0,table,object,value;
 for(;;){
  kind=W(0x7999e8u+(uint32_t)building*264u);
  if((int32_t)index>=(kind==207u?10:18))return;
  table=kind==207u?0x586b08u:0x58fe48u;
  object=U(table+offset+8u);W(object+18u)=0;object=U(table+offset+8u);
  (void)restore_widget_initialize_0041fca0((void *)(uintptr_t)object,3);
  object=U(table+offset+8u);value=U(table+offset+12u);++index;offset+=16u;
  U(object+12u)=value;
 }
}
