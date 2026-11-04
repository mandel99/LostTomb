#include "access_repair_flood.h"
#include "actor_order_flood.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define I(a) ((int32_t)(uint32_t)(a))

/* Whole517C20: ordered cardinal breadth-first scan. The first signed-negative
   cost with terrain4120 ends the scan after exactly three owned tile stores.
   Both ring cursors are reloaded/published at their native load phases; the
   cached tail also survives directions which do not enqueue a neighbor. */
uint32_t restore_access_clear_first_blocker(int32_t x,int32_t y,int32_t target_x,int32_t target_y)
{
 static const uint32_t delta[4]={0xffffff1cu,1,228,0xffffffffu};
 uint32_t start,serial,target,head=0,tail=1,cell,distance,neighbor,direction,flags;
 uint8_t sprite,access;
 FUN_0040f8f0((uint32_t *)(uintptr_t)0xeed760u,0x32c40u,0);
 start=U(0xbca2d8u)+(uint32_t)y*228u+(uint32_t)x;serial=U(0xeadfd4u)+1u;
 U(0xeae004u)=start;U(0xeadfd4u)=serial;U(0xeed760u+start*4u)=1;
 start=U(0xeae004u);U(0xf203a0u)=start;U(0xeae00cu)=0;U(0xeae008u)=1;
 target=U(0xbca2d8u)+(uint32_t)target_y*228u+(uint32_t)target_x;
 do{
  cell=U(0xf203a0u+head*4u);if(cell==target)break;
  distance=U(0xeed760u+cell*4u)+1u;
  for(direction=0;direction<4u;++direction){
   neighbor=cell+delta[direction];if(U(0xeed760u+neighbor*4u))continue;
   if((int8_t)B(0xee0c50u+neighbor)>=0){
    U(0xeed760u+neighbor*4u)=distance;tail=U(0xeae008u);U(0xf203a0u+tail*4u)=neighbor;
    ++tail;U(0xeae008u)=tail;if(I(tail)>51983){tail=0;U(0xeae008u)=0;}
   }else if(U(0xaf598cu+neighbor*4u)&0x4120u){
    sprite=B(0xb67d1cu+neighbor);flags=U(0xaf598cu+neighbor*4u);access=B(0xb285ccu+neighbor);
    B(0xb67d1cu+neighbor)=(uint8_t)(sprite|64u);U(0xaf598cu+neighbor*4u)=flags&0x03052e80u;
    B(0xb285ccu+neighbor)=(uint8_t)(access&0xf0u);return 0;
   }
  }
  head=U(0xeae00cu)+1u;U(0xeae00cu)=head;if(I(head)>51983){head=0;U(0xeae00cu)=0;}
 }while(head!=tail);
 return U(0xeed760u+target*4u)!=0;
}
