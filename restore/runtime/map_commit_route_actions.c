#include "map_commit_route_actions.h"
#include "map_commit_state.h"
#include "map_commit_connect.h"
#include "map_commit_costs.h"
#include "map_commit_fields.h"
#include "route_seed.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define H(a) (*(volatile int16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
extern void FUN_0040f860(void *,void *,int32_t);
extern void FUN_004746d0(int32_t,int32_t,int32_t);
extern void FUN_00475cc0(int32_t,int32_t,int32_t,int32_t,uint32_t);
extern uint32_t FUN_0051bc60(uint32_t,int32_t,int32_t,int32_t);
extern uint32_t FUN_0051c930(uint32_t,int32_t,int32_t,int32_t,int32_t,int32_t);
static int32_t add(int32_t x,int32_t y){return (int32_t)((uint32_t)x+(uint32_t)y);}
void FUN_00474f50(int32_t preview,int32_t x1,int32_t y1,int32_t x2,int32_t y2)
{
 uint32_t origin,cell,flags;int32_t building;
 FUN_0040f860((void *)(uintptr_t)0x9de62cu,(void *)(uintptr_t)0xaf598cu,0x32c40);
 FUN_0040f860((void *)(uintptr_t)0x9d1b1cu,(void *)(uintptr_t)0xae8e7cu,0xcb10);FUN_00485830();
 origin=U(0xbca2d8u);U(0xbc971cu)=0;
 cell=origin+(uint32_t)y1*228u+(uint32_t)x1;flags=U(0xaf598cu+cell*4u);
 if(flags&8u){building=H(0xb4e6fcu+cell*2u);if(!building||H(0x7999e8u+(uint32_t)building*264u)!=138)return;}
 if(flags&0x26345237u)return;
 cell=origin+(uint32_t)y2*228u+(uint32_t)x2;flags=U(0xaf598cu+cell*4u);
 if(flags&8u)return;if(flags&0x26345237u)return;
 if(U(0x605a3cu)){if(!FUN_0051b680(0,x1,y1,0))return;}
 else if(!FUN_0051bc60(0,x1,y1,0))return;
 if(!FUN_0051c930(0,x1,y1,x2,y2,0))return;
 if(preview)return;
 FUN_00516f90(x1,y1,x2,y2,1);FUN_005173a0(x1,y1,x2,y2);
 FUN_004746d0(x1,y1,(int32_t)((uint32_t)x2-(uint32_t)x1));B(0xe38dcau)=1;
}
void FUN_00475090(int32_t preview,int32_t x1,int32_t y1,int32_t x2,int32_t y2)
{
 uint32_t origin,cell,flags;int32_t building,t;
 FUN_0040f860((void *)(uintptr_t)0x9de62cu,(void *)(uintptr_t)0xaf598cu,0x32c40);
 FUN_0040f860((void *)(uintptr_t)0x9d1b1cu,(void *)(uintptr_t)0xae8e7cu,0xcb10);FUN_00485830();
 origin=U(0xbca2d8u);U(0xbc971cu)=0;
 cell=origin+(uint32_t)y1*228u+(uint32_t)x1;flags=U(0xaf598cu+cell*4u);
 if(flags&8u){building=H(0xb4e6fcu+cell*2u);if(!building||H(0x7999e8u+(uint32_t)building*264u)!=138)return;}
 if(flags&0x26345237u)return;
 cell=origin+(uint32_t)y2*228u+(uint32_t)x2;flags=U(0xaf598cu+cell*4u);
 if(flags&8u){building=H(0xb4e6fcu+cell*2u);if(!building||H(0x7999e8u+(uint32_t)building*264u)!=138)return;}
 if(flags&0x26345237u)return;
 if(U(0x605a3cu)){if(!FUN_0051b680(0,x1,y1,1))return;}
 else if(!FUN_0051bc60(0,x1,y1,1))return;
 if(!FUN_0051c930(0,x1,y1,x2,y2,3))return;
 if(preview)return;
 if(x1>x2){t=x1;x1=x2;x2=t;}if(y1>y2){t=y1;y1=y2;y2=t;}
 FUN_00475cc0(add(x1,-1),add(y1,-1),add(x2,1),add(y2,1),1);
 FUN_00517690(x1,y1,x2,y2);FUN_00517370();FUN_005173a0(x1,y1,x2,y2);FUN_005178e0();B(0xe38dcau)=1;
}
