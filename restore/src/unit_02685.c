#include "sound_initialization.h"
#include <string.h>
extern void *FUN_00561a1d(uint32_t);
extern void FUN_00561a2b(void *);
extern void FUN_00561bdc(void *);
int FUN_0055dc63(RestoreSoundFileTable *self,int count)
{
    uint32_t *block;
    if(count<1||(uint32_t)count>(0xffffffe0u-4u)/8u)return 0;
    if(self->entries) {
        uint32_t *entries=(uint32_t *)(uintptr_t)self->entries;uint32_t n=entries[-1],i;
        for(i=n;i>0;--i)if(entries[(i-1)*2+1]&&entries[(i-1)*2]) {
            FUN_00561bdc((void *)(uintptr_t)entries[(i-1)*2]);entries[(i-1)*2]=0;
        }
        FUN_00561a2b(entries-1);self->entries=0;
    }
    block=FUN_00561a1d((uint32_t)count*8u+4u);
    if(!block)return 0;
    *block=(uint32_t)count;memset(block+1,0,(size_t)count*8);
    self->entries=(uint32_t)(uintptr_t)(block+1);self->capacity=count;self->count=0;return 1;
}
