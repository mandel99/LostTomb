#include "map_commit_paths.h"
#include "actor_walking.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
extern void FUN_0040f8f0(uint32_t *,uint32_t,uint8_t);
uint32_t FUN_0051de70(int32_t start_x,int32_t start_y,int32_t end_x,int32_t end_y)
{
 static const int32_t directions[4]={-228,1,228,-1};
 static const int32_t nearby[8]={-228,-227,1,229,228,227,-1,-229};
 uint32_t base,start,end,head=0,tail=1,processed=0,cell,next,destination,around,flags,i,j;
 FUN_0040f8f0((uint32_t *)0xeed760u,0x32c40u,0);
 base=U(0xbca2d8u);start=base+(uint32_t)start_y*228u+(uint32_t)start_x;
 U(0xeae004u)=start;end=base+(uint32_t)end_y*228u+(uint32_t)end_x;
 if(B(0xed4140u+start)==0xff)return 0;
 U(0xeed760u+start*4u)=1;U(0xf203a0u)=U(0xeae004u);U(0xeae00cu)=0;U(0xeae008u)=1;
 for(;;){
  if(++processed>50000u)return 1;
  cell=U(0xf203a0u+head*4u);next=U(0xeed760u+cell*4u)+1u;
  for(i=0;i<4;++i){
   destination=cell+(uint32_t)directions[i];
   if(U(0xeed760u+destination*4u))continue;
   if(destination!=U(0xeae004u)&&destination!=end){
    if(B(0xed4140u+destination)==0xff)continue;
    flags=U(0xaf598cu+destination*4u);if((flags&0x10000u)||(flags&0x40u))continue;
    for(j=0;j<8;++j){
     around=destination+(uint32_t)nearby[j];flags=U(0xaf598cu+around*4u);
     if(flags&0x40u)break;
     if(around==U(0xeae004u)||around==end)goto accept;
     if(B(0xed4140u+around)==0xff||(flags&0x10000u))break;
    }
    if(j<8)continue;
   }
accept:
   U(0xeed760u+destination*4u)=next;tail=U(0xeae008u);U(0xf203a0u+tail*4u)=destination;
   ++tail;U(0xeae008u)=tail;if((int32_t)tail>51983){tail=0;U(0xeae008u)=0;}
  }
  head=U(0xeae00cu)+1u;U(0xeae00cu)=head;if((int32_t)head>51983)U(0xeae00cu)=0;
  head=U(0xeae00cu);if(head==tail)return 1;
 }
}
int32_t FUN_0051e720(int32_t route,int32_t target_x,int32_t target_y,int32_t x,int32_t y,int32_t variation,uint32_t *output)
{
 uint32_t preference=U(0xd3b374u)&3u,choice,cell,level,current,neighbor,offset,count=0,opposite=0xffffffffu,direction,index;
 int32_t best;
 cell=U(0xbca2d8u)+(uint32_t)y*228u+(uint32_t)x;U(0xeae004u)=cell;level=U(0xeed760u+cell*4u);
 if((int32_t)level<=0)return 0;
 while((int32_t)level>1){
  current=U(0xeed760u+cell*4u);choice=preference;
  if(variation==1)choice=B(0xacf85cu+cell)&3u;
  (void)FUN_0051cba0(x,y,target_x,target_y);best=-1;
  for(direction=0;direction<8;direction+=2){
   if(direction==opposite)continue;
   offset=U(0x5f80fcu+direction*4u);neighbor=U(0xeed760u+(cell+offset)*4u);
   if(neighbor&&((int32_t)neighbor<(int32_t)current||(neighbor==current&&preference==choice))){best=(int32_t)direction;current=neighbor;}
  }
  if(best==-1)return 0;
  if(best==0){y=(int32_t)((uint32_t)y-1u);cell-=228u;}
  else if(best==2){x=(int32_t)((uint32_t)x+1u);++cell;}
  else if(best==4){y=(int32_t)((uint32_t)y+1u);cell+=228u;}
  else if(best==6){x=(int32_t)((uint32_t)x-1u);--cell;}
  opposite=(uint32_t)best+4u;if((int32_t)opposite>=8)opposite-=8u;
  B(0xf533c8u+count)=(uint8_t)opposite;
  ++count;if(count>=500u)return 0;
  level=current;
 }
 if(route<1000){
  for(index=0;index<count;++index)B(0xc8ad38u+(uint32_t)route*500u+index)=B(0xf533c7u+count-index);
 }else if(output){
  for(index=0;index<count;++index){neighbor=B(0xf533c7u+count-index);*(volatile uint32_t *)(output+index)=neighbor;}
 }
 return (int32_t)count;
}
