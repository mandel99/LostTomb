#include "sound_file_lookup.h"
#include "owned_buffer.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
extern uint32_t FUN_0056b177(const char *);
extern int FUN_005631b0(const char *,const char *);
static int32_t register_file(RestoreSoundFileTable *self,const char *path,int32_t copy)
{
    uint32_t base=(uint32_t)(uintptr_t)self,i,value;
    int32_t index=(int32_t)U(base+4u);
    if((int32_t)U(base+4u)>=(int32_t)U(base))return -1;
    if(!U(base+8u))return -1;
    for(i=0;(int32_t)i<(int32_t)U(base+4u);++i){
        if(U(U(base+8u)+i*8u)&&!FUN_005631b0((const char *)(uintptr_t)U(U(base+8u)+i*8u),path))return (int32_t)i;
    }
    value=copy?FUN_0056b177(path):(uint32_t)(uintptr_t)path;
    U(U(base+8u)+(uint32_t)index*8u)=value;
    U(U(base+8u)+(uint32_t)index*8u+4u)=(uint32_t)copy;
    U(base+4u)=U(base+4u)+1u;
    return index;
}
int32_t restore_sound_register_file(RestoreSoundFileTable *self,const char *path,int32_t copy)
{
    RestoreOwnedBuffer guard;
    (void)FUN_00560280(&guard);
    /* Keep cleanup on both normal and exceptional exits. The host compiler's
       SEH frame differs from the original frame; exception delivery/unwind
       needs separate verification from the normal-return algorithm oracle. */
    __try{return register_file(self,path,copy);}
    __finally{FUN_005602b0(&guard);}
}
