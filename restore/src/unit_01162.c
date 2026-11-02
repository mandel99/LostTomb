#include "ferry_preview.h"
#include "map_commit_links.h"
/* Complete reachable native47F3E0..47F509. Ghidra's function body omits
   the switch targets and success tail; resolve the four even jump-table
   entries as C cases, never call the original code-address table. */
void restore_ferry_scan_crossing(int32_t x,int32_t y)
{
 uint32_t origin=FRY_U(0xbca2d8u)+(uint32_t)y*228u+(uint32_t)x,cell,flags,i,distance=1,orientation;
 int32_t step;
 FRY_U(0xbc96e8u)=0;FRY_U(0xbc96e4u)=0;FRY_U(0xbc96ecu)=0;FRY_U(0xbc96e0u)=0;FRY_U(0xbc96f0u)=origin;
 orientation=FUN_0047cbd0(x,y)*2u;
 switch(orientation){case 0:step=-228;break;case 2:step=1;break;case 4:step=228;break;case 6:step=-1;break;default:return;}
 FRY_U(0xbc96e0u)=orientation;FRY_U(0xbc96ecu)=(uint32_t)step;cell=origin;
 for(i=0;i<40u;++i){
  cell+=(uint32_t)step;++distance;
  /* Native terrain/obstruction probes are one cell ahead of the retained
     endpoint. A successful land probe deliberately publishes length40. */
  flags=FRY_U(0xaf598cu+(cell+(uint32_t)step)*4u);
  if((flags&1u)||FRY_B(0xadc36cu+cell+(uint32_t)step)){FRY_U(0xbc96e4u)=distance;return;}
  if(!(flags&4u)){
   FRY_U(0xbc96e4u)=distance;FRY_U(0xbc96e8u)=cell;FRY_U(0xbc96e4u)=40;return;
  }
  if(FRY_U(0xbca310u)&&(flags&0x48u)){FRY_U(0xbc96e4u)=distance;return;}
 }
 FRY_U(0xbc96e4u)=distance;
}
void FUN_0047f3e0(int32_t x,int32_t y){restore_ferry_scan_crossing(x,y);}
