#include "sound_initialization.h"
#include "media_bindings.h"
#include <string.h>
extern void *FUN_00561a1d(uint32_t);
extern void FUN_00561a2b(void *);
static void clear_arrays(RestoreSound *self)
{
    uint32_t *slots[]={&self->handles,&self->auxiliary,&self->samples,&self->cache};unsigned int i;
    for(i=0;i<4;++i)if(*slots[i]) {FUN_00561a2b((void *)(uintptr_t)*slots[i]);*slots[i]=0;}
}
int FUN_0055dea2(RestoreSound *self,int count)
{
    int i;uint32_t *handles;
    if(count<1||(uint32_t)count>0x7fffffffu/40u)return 0;
    if(self->handles) {
        RestoreSoundCache *cache=(RestoreSoundCache *)(uintptr_t)self->cache;
        FUN_0055ec7c(self);
        for(i=0;i<self->sample_count;++i) {
            FUN_0055e5d2(self,i);_AIL_release_sample_handle_4(((uint32_t *)(uintptr_t)self->handles)[i]);
            if(cache[i].data) {_AIL_mem_free_lock_4(cache[i].data);cache[i].data=0;}
        }
    }
    clear_arrays(self);
    self->handles=(uint32_t)(uintptr_t)FUN_00561a1d((uint32_t)count*4u);if(!self->handles)goto failed;
    self->auxiliary=(uint32_t)(uintptr_t)FUN_00561a1d((uint32_t)count*4u);if(!self->auxiliary)goto failed;
    self->samples=(uint32_t)(uintptr_t)FUN_00561a1d((uint32_t)count*40u);if(!self->samples)goto failed;
    self->cache=(uint32_t)(uintptr_t)FUN_00561a1d((uint32_t)count*12u);if(!self->cache)goto failed;
    self->sample_count=count;handles=(uint32_t *)(uintptr_t)self->handles;
    for(i=0;i<count;++i) {
        handles[i]=(uint32_t)_AIL_allocate_sample_handle_4(self->digital);
        if(!handles[i]) {
            int j;for(j=0;j<i;++j)_AIL_release_sample_handle_4(handles[j]);
            goto failed;
        }
    }
    return 1;
failed:
    clear_arrays(self);self->sample_count=0;return 0;
}
