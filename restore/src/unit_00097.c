#include "monument_constructor.h"
extern uint32_t FUN_00562348(void);

/* Complete normal constructor0040d280; reuse the existing ordered seed set. */
__declspec(noinline) void *restore_monument_construct_body_0040d280(void *object,RestoreMonumentConstructorScratch *scratch)
{
  volatile uint32_t *fields=object;
  restore_monument_class_base(object,&scratch->base,0);
  restore_monument_class_clear(object,0u);
  *fields = 0x0056f48cu;

  restore_monument_class_seed(object,0u,&scratch->part);

  fields[1] = 2;
    return object;
}
void *FUN_0040d280(void *object)
{
  RestoreMonumentConstructorScratch scratch={0};
  return restore_monument_construct_body_0040d280(object,&scratch);
}
