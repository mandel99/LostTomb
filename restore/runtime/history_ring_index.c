#include "actor_teardown_model.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
/* Existing whole ring-index body, shared with isolated save fixtures. */
uint8_t restore_ring_004f6a10(void *self,volatile uint32_t *index)
{
 uint32_t q=(uint32_t)(uintptr_t)self,capacity=U(q+16u),value;
 if(capacity==0)return 0;
 if((int32_t)*index<0){
  do {capacity=U(q+16u);value=*index+capacity;*index=value;}while((int32_t)value<0);
 }
 capacity=U(q+16u);value=*index;
 if((int32_t)value>=(int32_t)capacity){
  do {value=*index-capacity;*index=value;capacity=U(q+16u);}while((int32_t)value>=(int32_t)capacity);
 }
 return 1;
}
