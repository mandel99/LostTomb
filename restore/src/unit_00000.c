#include "monument_constructor.h"
extern uint32_t FUN_00562348(void);

/* Complete normal constructor00401030; reuse the existing ordered seed set. */
__declspec(noinline) void *restore_monument_construct_body_00401030(void *object,RestoreMonumentConstructorScratch *scratch)
{
  volatile uint32_t *fields=object;
  restore_monument_class_base(object,&scratch->base,0);
  restore_monument_class_clear(object,1u);
  *fields = 0x0056f438u;

  restore_monument_class_seed(object,1u,&scratch->part);

  fields[1] = 0;
    return object;
}
void *FUN_00401030(void *object)
{
  RestoreMonumentConstructorScratch scratch={0};
  return restore_monument_construct_body_00401030(object,&scratch);
}
