#include "file_list.h"
int32_t FUN_0041dfc0(const RestoreFileList *self,int32_t index)
{
    if(index<0||index>=self->count)return -1;
    return ((const int32_t *)(self+1))[index];
}
