#include "array_lifecycle.h"
void FUN_005613e3(void *end,uint32_t stride,int32_t count,RestoreArrayElement destroy)
{restore_array_unwind(end,stride,count,destroy);}
