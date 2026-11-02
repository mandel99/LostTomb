#include "map_commit_resource_gate.h"
#include "menu_resource_neighbors.h"
#include "pending_message.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
void FUN_004e2b10(int32_t tool,int32_t x,int32_t y,uint32_t orientation)
{
 if(U(0x605a3cu)==1)return;
 switch(tool){
 case 41:case 42:case 43:case 92:case 57:case 58:case 170:case 171:
 case 200:case 201:case 202:case 56:case 10:case 40:case 44:case 45:
 case 211:case 212:case 183:case 210:case 227:case 225:case 230:case 228:
 case 222:case 207:case 208:case 213:case 229:case 234:case 236:case 235:return;
 }
 if(FUN_00480180(x,y,orientation))return;
 if((tool==72||tool==193)&&FUN_00480180(x,y,3))return;
 if(tool>=65&&tool<=69)return;
 if(tool>=140&&tool<=149&&FUN_004802c0(x,y))return;
 if((int32_t)U(0xe950d4u)>=3)return;
 FUN_004e2970(tool>=140&&tool<=149?207:11);
}
