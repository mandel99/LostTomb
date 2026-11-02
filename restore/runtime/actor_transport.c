#include "actor_transport.h"
#include <stdlib.h>
#include <string.h>
void restore_actor_transport_raw(uint32_t target,uint32_t registers[8],uint32_t *flags)
{
 struct restore_transport_frame f;
 memcpy(f.r,registers,sizeof(f.r));f.flags=*flags;f.boundary=restore_actor_transport_dispatch;
 restore_actor_transport_dispatch(0,target,&f);
 memcpy(registers,f.r,sizeof(f.r));*flags=f.flags;
}
static uint32_t invoke(uint32_t entry,const uint32_t *args,uint32_t count)
{
 uint32_t stack[4096],sp=(uint32_t)(uintptr_t)(stack+4000),i;
 struct restore_transport_frame f={{0},2,restore_actor_transport_dispatch};
 stack[4000]=0;f.r[4]=sp;for(i=0;i<count;++i)stack[4001+i]=args[i];
 restore_actor_transport_dispatch(0,entry,&f);if(f.r[4]!=sp+4u)abort();return f.r[0];
}
int32_t FUN_00496640(int32_t actor)
{const uint32_t a[]={(uint32_t)actor};return (int32_t)invoke(0x00496640u,a,1);}

uint32_t FUN_0045acf0(int32_t param_1,uint32_t param_2,int32_t param_3,uint32_t param_4)
{const uint32_t a[]={(uint32_t)(uintptr_t)param_1,(uint32_t)(uintptr_t)param_2,(uint32_t)(uintptr_t)param_3,(uint32_t)(uintptr_t)param_4};return (uint32_t)invoke(0x45acf0u,a,4);}

void FUN_0045b880(int32_t param_1,int32_t *param_2,int32_t *param_3,int32_t *param_4,int32_t *param_5,int32_t *param_6,
                 int32_t *param_7)
{const uint32_t a[]={(uint32_t)(uintptr_t)param_1,(uint32_t)(uintptr_t)param_2,(uint32_t)(uintptr_t)param_3,(uint32_t)(uintptr_t)param_4,(uint32_t)(uintptr_t)param_5,(uint32_t)(uintptr_t)param_6,(uint32_t)(uintptr_t)param_7};(void)invoke(0x45b880u,a,7);}

int32_t FUN_0045d510(int32_t param_1,int32_t param_2)
{const uint32_t a[]={(uint32_t)(uintptr_t)param_1,(uint32_t)(uintptr_t)param_2};return (int32_t)invoke(0x45d510u,a,2);}

int32_t FUN_0045d9e0(int32_t param_1,int32_t param_2,int32_t param_3,uint32_t param_4)
{const uint32_t a[]={(uint32_t)(uintptr_t)param_1,(uint32_t)(uintptr_t)param_2,(uint32_t)(uintptr_t)param_3,(uint32_t)(uintptr_t)param_4};return (int32_t)invoke(0x45d9e0u,a,4);}

int32_t FUN_0045e360(int32_t param_1,int32_t param_2,int32_t param_3,int32_t param_4)
{const uint32_t a[]={(uint32_t)(uintptr_t)param_1,(uint32_t)(uintptr_t)param_2,(uint32_t)(uintptr_t)param_3,(uint32_t)(uintptr_t)param_4};return (int32_t)invoke(0x45e360u,a,4);}

int32_t FUN_0045eab0(uint32_t param_1,int32_t param_2,uint32_t param_3)
{const uint32_t a[]={(uint32_t)(uintptr_t)param_1,(uint32_t)(uintptr_t)param_2,(uint32_t)(uintptr_t)param_3};return (int32_t)invoke(0x45eab0u,a,3);}

int32_t FUN_0045f7b0(uint32_t param_1,uint32_t param_2,int32_t param_3,uint32_t param_4)
{const uint32_t a[]={(uint32_t)(uintptr_t)param_1,(uint32_t)(uintptr_t)param_2,(uint32_t)(uintptr_t)param_3,(uint32_t)(uintptr_t)param_4};return (int32_t)invoke(0x45f7b0u,a,4);}

int32_t FUN_00460890(int32_t param_1,int32_t param_2,int32_t param_3)
{const uint32_t a[]={(uint32_t)(uintptr_t)param_1,(uint32_t)(uintptr_t)param_2,(uint32_t)(uintptr_t)param_3};return (int32_t)invoke(0x460890u,a,3);}

void FUN_0048ae50(int32_t param_1,uint32_t param_2)
{const uint32_t a[]={(uint32_t)(uintptr_t)param_1,(uint32_t)(uintptr_t)param_2};(void)invoke(0x48ae50u,a,2);}

void FUN_0048aef0(int32_t param_1,int32_t param_2,uint32_t param_3)
{const uint32_t a[]={(uint32_t)(uintptr_t)param_1,(uint32_t)(uintptr_t)param_2,(uint32_t)(uintptr_t)param_3};(void)invoke(0x48aef0u,a,3);}

uint32_t FUN_0048b320(uint32_t param_1)
{const uint32_t a[]={(uint32_t)(uintptr_t)param_1};return (uint32_t)invoke(0x48b320u,a,1);}

void FUN_0048b4a0(int32_t param_1)
{const uint32_t a[]={(uint32_t)(uintptr_t)param_1};(void)invoke(0x48b4a0u,a,1);}

void FUN_0048b620(void)
{(void)invoke(0x48b620u,0,0);}

void FUN_0048ce30(void)
{(void)invoke(0x48ce30u,0,0);}


int32_t FUN_00518dd0(int32_t param_1,int32_t param_2,int32_t param_3,int32_t param_4)
{const uint32_t a[]={(uint32_t)(uintptr_t)param_1,(uint32_t)(uintptr_t)param_2,(uint32_t)(uintptr_t)param_3,(uint32_t)(uintptr_t)param_4};return (int32_t)invoke(0x518dd0u,a,4);}

int32_t FUN_005190a0(int32_t param_1,int32_t param_2,int32_t *param_3,int32_t param_4)
{const uint32_t a[]={(uint32_t)(uintptr_t)param_1,(uint32_t)(uintptr_t)param_2,(uint32_t)(uintptr_t)param_3,(uint32_t)(uintptr_t)param_4};return (int32_t)invoke(0x5190a0u,a,4);}

int32_t FUN_00519e30(int32_t param_1,int32_t param_2,int32_t param_3,int32_t param_4)
{const uint32_t a[]={(uint32_t)(uintptr_t)param_1,(uint32_t)(uintptr_t)param_2,(uint32_t)(uintptr_t)param_3,(uint32_t)(uintptr_t)param_4};return (int32_t)invoke(0x519e30u,a,4);}

int32_t FUN_0051a220(int32_t param_1,int32_t param_2,int32_t param_3,int32_t param_4)
{const uint32_t a[]={(uint32_t)(uintptr_t)param_1,(uint32_t)(uintptr_t)param_2,(uint32_t)(uintptr_t)param_3,(uint32_t)(uintptr_t)param_4};return (int32_t)invoke(0x51a220u,a,4);}


int32_t FUN_0051b220(int32_t param_1,int32_t param_2,int32_t param_3,int32_t param_4)
{const uint32_t a[]={(uint32_t)(uintptr_t)param_1,(uint32_t)(uintptr_t)param_2,(uint32_t)(uintptr_t)param_3,(uint32_t)(uintptr_t)param_4};return (int32_t)invoke(0x51b220u,a,4);}

int32_t FUN_0051d6d0(int32_t param_1,int32_t param_2,int32_t param_3,int32_t param_4)
{const uint32_t a[]={(uint32_t)(uintptr_t)param_1,(uint32_t)(uintptr_t)param_2,(uint32_t)(uintptr_t)param_3,(uint32_t)(uintptr_t)param_4};return (int32_t)invoke(0x51d6d0u,a,4);}
