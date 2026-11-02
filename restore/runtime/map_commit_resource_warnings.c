#include "map_commit_resource_warnings.h"
#include "pending_message.h"
#include "training_queries.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
static void warning(int32_t tool,int32_t expected,int32_t alternate,uint32_t total,
 uint32_t stock1,uint32_t stock2,int32_t first,int32_t import_resource,
 int32_t imported,int32_t export_resource,uint32_t permission,int32_t blocked,int32_t absent)
{
 uint32_t player;uint16_t state;
 if(U(0x605a3cu)==1)return;
 if(tool!=expected&&tool!=alternate)return;
 if((int32_t)U(total)>0)return;
 player=U(0xeaac28u)*0x49d8u;
 if((int32_t)U(stock1+player)>0)return;
 if(stock2&&(int32_t)U(stock2+player)>0)return;
 FUN_004e2970(first);
 if(FUN_00443b90(import_resource)){
  FUN_004e2970(imported);return;
 }
 if(FUN_00443950(export_resource)){
  player=U(0xeaac28u)*0x49d8u;state=W(permission+player);
  if(state==1||state==3)return;
  FUN_004e2970(blocked);return;
 }
 FUN_004e2970(absent);
}
void FUN_004e33a0(int32_t tool)
{warning(tool,110,110,0x78d684,0xea619c,0xea6198,27,14,34,14,0xea40b2,32,31);}
void FUN_004e3460(int32_t tool)
{warning(tool,111,111,0x78d68c,0xea61a4,0xea61a0,28,16,35,16,0xea40b6,32,31);}
void FUN_004e3520(int32_t tool)
{warning(tool,113,113,0x78d694,0xea61ac,0xea61a8,30,18,37,18,0xea40ba,32,31);}
void FUN_004e35e0(int32_t tool)
{warning(tool,114,114,0x78d678,0xea6194,0xea618c,29,11,36,11,0xea40ac,32,31);}
void FUN_004e36a0(int32_t tool)
{warning(tool,203,203,0x78d6a4,0xea61bc,0xea61b8,80,22,81,22,0xea40c2,32,31);}
void FUN_004e39d0(int32_t tool)
{warning(tool,233,233,0x78d6cc,0xea61e4,0xea61e0,261,32,262,32,0xea40d6,263,264);}
void FUN_004e3cf0(int32_t tool)
{warning(tool,194,194,0x78d650,0xea6184,0,82,1,83,9,0xea40a8,32,31);}
void FUN_004e4490(int32_t tool)
{warning(tool,177,177,0x78d69c,0xea61b0,0,143,20,144,20,0xea40be,32,31);}
void FUN_004e4e10(int32_t tool)
{warning(tool,213,222,0x78d6c4,0xea61d8,0,164,30,165,30,0xea40d2,32,31);}
