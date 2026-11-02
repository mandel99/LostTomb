#include "restore_types.h"
#include "file_list.h"
extern void FUN_00562069(uint32_t *,uint32_t *,uint32_t,code *);
static RestoreFileCompare active_compare;
static const char *active_strings;
static int compare_offsets(const uint32_t *left,const uint32_t *right)
{return active_compare(active_strings+*left,active_strings+*right);}
void FUN_0041ca10(RestoreFileList *list,RestoreFileCompare compare)
{
    if(list->offsets&&list->strings&&list->count>1) {
        *(RestoreFileCompare *)0x007027d8u=compare;*(char **)0x007027dcu=list->strings;
        active_compare=compare;active_strings=list->strings;
        FUN_00562069(list->offsets,(uint32_t *)(uintptr_t)list->count,4,(code *)compare_offsets);
    }
}
