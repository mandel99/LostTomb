#include "monument_constructor.h"
extern uint32_t FUN_00562348(void);

/* Complete normal constructor004e6400; reuse the existing ordered seed set. */
__declspec(noinline) void *restore_monument_construct_body_004e6400(void *object,RestoreMonumentConstructorScratch *scratch)
{
  volatile uint32_t *fields=object;
  restore_monument_class_base(object,&scratch->base,0);
  restore_monument_class_clear(object,3u);
  *fields = 0x0056f6e8u;
  fields[0x2d] = 0x005e4934u;
  fields[0x2e] = 0x005e4928u;
  fields[0x2f] = 0x005e491cu;
  fields[0x30] = 0x005e4910u;
  fields[0x31] = 0x005e4904u;
  fields[0x32] = 0x005e48f8u;
  fields[0x33] = 0x005e48ecu;
  fields[0x34] = 0x005e48e0u;
  fields[0x35] = 0x005e48d4u;
  fields[0x36] = 0x005e48c8u;

  restore_monument_class_seed(object,3u,&scratch->part);

  fields[1] = 3;
    return object;
}
void *FUN_004e6400(void *object)
{
  RestoreMonumentConstructorScratch scratch={0};
  return restore_monument_construct_body_004e6400(object,&scratch);
}
