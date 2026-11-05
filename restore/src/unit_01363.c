#include "actor_cadence_closure.h"
#include "monument_worker_motion.h"

/* 4A3B00: wrapped signed WORD counter, full EAX on every return.
   4A3B3C forwards the signed quotient; 4A3B5E forwards untouched entry EAX.
   The child adapter closes the native full-EAX result, including heat exits. */
uint32_t restore_actor_advance_cadence(int32_t actor,int32_t period,uint32_t incoming_eax)
{
 uint32_t counter=restore_monument_worker_record(actor)+MW_ACTION_CLOCK;
 int32_t value,quotient;
 if(period<=1)return restore_actor_animation_step_eax(actor,1,incoming_eax);
 MW_W(counter)=(uint16_t)(MW_W(counter)+1u);
 value=MW_H(counter);quotient=value/period;
 if(value%period)return restore_actor_animation_step_eax(actor,1,(uint32_t)quotient);
 if(value>100)MW_W(counter)=0;
 return (uint32_t)quotient;
}
