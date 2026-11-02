#include "monument_constructor.h"
extern uint32_t FUN_00562348(void);

/* Complete normal constructor00416c20; reuse the existing ordered seed set. */
__declspec(noinline) void *restore_monument_construct_body_00416c20(void *object,RestoreMonumentConstructorScratch *scratch)
{
  volatile uint32_t *fields=object;
  restore_monument_class_base(object,&scratch->base,0);
  restore_monument_class_clear(object,2u);
  *fields = 0x0056f4dcu;
  fields[0x88] = 0x005793dcu;
  fields[0x89] = 0x005793d0u;
  fields[0x8a] = 0x005793c4u;
  fields[0x8b] = 0x005793b8u;
  fields[0x8c] = 0x005793acu;
  fields[0x8d] = 0x005793a0u;
  fields[0x8e] = 0x00579394u;
  fields[0x8f] = 0x00579388u;
  fields[0x90] = 0x0057937cu;
  fields[0x91] = 0x00579370u;
  fields[0x92] = 0x00579364u;
  fields[0x93] = 0x00579358u;
  fields[0x94] = 0x0057934cu;
  fields[0x95] = 0x00579340u;
  fields[0x96] = 0x00579334u;
  fields[0x97] = 0x00579328u;
  fields[0x98] = 0x0057931cu;
  fields[0x99] = 0x00579310u;
  fields[0x9a] = 0x00579304u;
  fields[0x9b] = 0x005792f8u;

  restore_monument_class_seed(object,2u,&scratch->part);

  fields[1] = 2;
    return object;
}
void *FUN_00416c20(void *object)
{
  RestoreMonumentConstructorScratch scratch={0};
  return restore_monument_construct_body_00416c20(object,&scratch);
}
