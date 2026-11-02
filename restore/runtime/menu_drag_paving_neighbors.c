#include "paving_neighbors.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
uint32_t FUN_00475670(int32_t tile)
{
 static const uint32_t offsets[3]={1,228,229};
 uint32_t score=0,flags,index,i;
 for(i=0;i<3u;++i){
  index=(uint32_t)tile+offsets[i];flags=U(0xaf598cu+index*4u);
  if(flags&0x40u)score++;
  if(!(B(0xb285ccu+index)&0x80u))score--;
  if(flags&4u)score--;if(flags&8u)score--;if(flags&0x100u)score--;
  if(U(0xb7482cu+index*4u))score--;
 }
 return score==3u;
}
void FUN_0047e750(void)
{
 static const int32_t offsets[8]={-228,-227,1,229,228,227,-1,-229};
 uint32_t tile=U(0xbc9768u),value=U(0xbc97c0u),next,flags,i;
 for(i=0;i<6u;++i){
  flags=U(0xaf598cu+(tile+(uint32_t)offsets[i])*4u);next=U(0xbc97c0u+(i+1u)*4u);
  U(0xbc97c0u+i*4u)=value+((flags>>14)&1u);value=next;
 }
 flags=U(0xaf598cu+(tile-1u)*4u);U(0xbc97d8u)=value+((flags>>14)&1u);
 flags=U(0xaf598cu+(tile-229u)*4u);value=U(0xbc97dcu);U(0xbc97dcu)=value+((flags>>14)&1u);
}
