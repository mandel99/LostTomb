#include "map_commit_warnings.h"
#include "pending_message.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
extern uint32_t FUN_00485100(int32_t record);
/* The complete canonical message provider consumes only native argument1;
 * the two original zero arguments are ignored. It is never replaced here. */
void FUN_004e2d30(int32_t tool)
{
 uint32_t player;
 if(U(0x605a3cu)==1)return;if(tool==92)return;if(U(0xe94e20u))return;
 if((int32_t)U(0x705eb8u+(uint32_t)tool*40u)<=0)return;
 player=U(0xeaac28u);if((int32_t)U(0xea4474u+player*0x49d8u)<10)return;
 FUN_004e2970(17);
}
void FUN_004e3100(int32_t tool)
{
 if(U(0x605a3cu)==1)return;if(U(0xe94e20u))return;
 if(tool==71&&(int32_t)U(0x705854u)<=0)FUN_004e2970(19);
}
void FUN_004e3140(int32_t tool)
{
 if(U(0x605a3cu)==1)return;if(U(0xe94e20u))return;
 if(tool==31&&(int32_t)U(0x7057ccu)<=0)FUN_004e2970(44);
}
void FUN_004e3180(int32_t tool)
{
 if(U(0x605a3cu)==1)return;if(U(0xe94e20u))return;
 if(tool==30&&(int32_t)U(0x7057c4u)<=0)FUN_004e2970(45);
}
void FUN_004e31c0(int32_t tool)
{
 if(U(0x605a3cu)==1)return;if(U(0xe94e20u))return;
 if(tool==33&&(int32_t)U(0x7057c8u)<=0)FUN_004e2970(46);
}
void FUN_004e3200(int32_t tool)
{
 if(U(0x605a3cu)==1)return;if(U(0xe94e20u))return;
 if((tool==40||tool==44||tool==45)&&(int32_t)U(0x7058b8u)<=0)FUN_004e2970(42);
}
void FUN_004e3240(int32_t tool)
{
 uint32_t player;
 if(U(0x605a3cu)==1)return;if(U(0xe94e20u))return;if(tool!=95)return;
 player=U(0xeaac28u);if((int32_t)U(0xea6188u+player*0x49d8u)<=0)FUN_004e2970(43);
}
uint32_t FUN_00480770(int32_t x,int32_t y,int32_t layout)
{
 uint32_t cell=U(0xbca2d8u)+(uint32_t)y*228u+(uint32_t)x,table=0x5cc9e0u+(uint32_t)layout*96u,index,offset;
 for(index=0;index<24;++index){
  offset=U(table+index*4u);if(!offset)return 0;
  if(U(0xaf598cu+(cell+offset)*4u)&0x4000u)return 1;
 }
 return 0;
}
void FUN_004e3280(int32_t tool,int32_t x,int32_t y,int32_t layout)
{
 if(U(0x605a3cu)==1)return;if(U(0xe94e20u))return;
 if((tool==59||tool==172||tool==173)&&!FUN_00480770(x,y,layout))FUN_004e2970(41);
}
void FUN_004e32e0(int32_t tool,int32_t x,int32_t y)
{
 uint32_t cell;
 if(U(0x605a3cu)==1)return;if(U(0xe94e20u))return;if(tool!=92)return;
 cell=U(0xbca2d8u)+(uint32_t)y*228u+(uint32_t)x;
 if(!(B(0xaf598cu+cell*4u)&0x80u))FUN_004e2970(38);
}
void FUN_00486020(void)
{
 uint32_t mode=U(0xbca310u);int32_t record;
 if(!mode){U(0xbca310u)=1;FUN_004e2970(67);}
 else{U(0xbca310u)=0;if(mode==3){record=(int32_t)U(0xbc9800u);(void)FUN_00485100(record);}}
}
