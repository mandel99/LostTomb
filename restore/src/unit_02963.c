#include "crt_runtime_init.h"
uint32_t restore_crt_controlfp(uint32_t value,uint32_t mask)
{
 uint32_t native_mask=mask&0xfff7ffffu;
 return restore_crt_control87(value,native_mask,&native_mask);
}
uint32_t FUN_005687f4(uint32_t value,uint32_t mask)
{return restore_crt_controlfp(value,mask);}
