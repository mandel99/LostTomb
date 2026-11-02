#include "file_list.h"
char *FUN_0041e010(RestoreFileList *list,int index)
{
    if(index<0||index>=list->count) return 0;
    if(*(int32_t *)0x00704db0u==0) return list->strings+list->offsets[index];
    return (char *)0x006ff60cu+((int32_t *)(list+1))[index]*50;
}
