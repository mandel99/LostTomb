#include "monument_unlink.h"
#include "building_cleanup.h"
#include "sprite_libraries.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
int32_t FUN_004f52b0(int32_t slot)
{
 uint32_t selector=(uint32_t)(int32_t)(int16_t)W(0x784de0u+(uint32_t)slot*2u),record=0x5d19c8u+selector*16u;
 if(W(record)!=2)return 0;
 if(W(record+2u)!=2)return 0;
 return 207+(W(record+4u)!=0);
}
void FUN_004e5950(int32_t slot)
{
 int32_t kind=FUN_004f52b0(slot);uint32_t name;
 if(kind==207)name=U(0x5e4588u);
 else if(kind==208)name=U(0x5e4598u);
 else return;
 (void)FUN_0053dd90(RESTORE_SPRITE_LIBRARIES,(const char *)(uintptr_t)name,(int32_t)((uint32_t)slot+5u));
}
void FUN_004f4ae0(int32_t building)
{
 uint32_t offset=(uint32_t)building*264u,player,slot,base;uint16_t kind;
 if(W(0x799a10u+offset)!=0)return;
 player=U(0xeaac28u);base=0xea5facu+player*18904u;
 for(slot=0;slot<3u;++slot)if(U(base+slot*4u)==(uint32_t)building)break;
 if(slot==3u)return;
 U(0xea5fa0u+player*18904u+slot*4u)=0;
 kind=W(0x7999e8u+offset);
 if(kind==207||kind==208)FUN_004e5950((int32_t)slot);
 FUN_004f4a80((uint32_t)building,(int32_t)slot);
}

void FUN_004e5830(int32_t building)
{FUN_004f4ae0(building);}
