#include "sound_file_lookup.h"
#include <string.h>
extern uint32_t FUN_0056b177(const char *);
int FUN_0055dd3f(RestoreSoundFileTable *self,const char *path,int copy)
{
    uint32_t *entries,value;int i,index=self->count;
    if(!path||index<0||index>=self->capacity||!self->entries)return -1;
    entries=(uint32_t *)(uintptr_t)self->entries;
    for(i=0;i<index;++i)if(entries[i*2]&&!strcmp((const char *)(uintptr_t)entries[i*2],path))return i;
    value=copy?FUN_0056b177(path):(uint32_t)(uintptr_t)path;
    if(copy&&!value)return -1;
    entries[index*2]=value;entries[index*2+1]=(uint32_t)copy;self->count=index+1;return index;
}
