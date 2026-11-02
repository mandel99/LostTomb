#include "sound_3d.h"
#include "media_bindings.h"
#include "legacy_format.h"
extern uint32_t *FUN_005632a0(uint32_t *text,char *needle);
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))

/* Whole55F697, explicit native local prestate. Keep last match per category,
   all five opening attempts, stale EAX-support flags and callback reloads. */
__declspec(noinline) void restore_sound_select_3d_body(RestoreSound *sound,RestoreSoundProviderScratch *s)
{
    static const uint32_t needles[5]={0x604790,0x604798,0x6047a0,0x6047ac,0x6047bc};
    static const uint32_t formats[5]={0x604794,0x60479c,0x6047a8,0x6047b8,0x6047c8};
    volatile RestoreSound *self=sound;uint32_t category;
    s->receiver=(uint32_t)(uintptr_t)sound;s->iterator=0;s->unused=0;
    if(self->disabled)return;
    while(_AIL_enumerate_3D_providers_12((uintptr_t)&s->iterator,(uintptr_t)&s->enumerated,(uintptr_t)&s->name)){
        for(category=0;category<5u;++category){
            s->found=(uint32_t)(uintptr_t)FUN_005632a0((uint32_t *)(uintptr_t)s->name,(char *)(uintptr_t)needles[category]);
            if(s->found){
                s->candidates[category]=s->enumerated;
                (void)FUN_00561a36(s->names[category],(const char *)(uintptr_t)formats[category],(const char *)(uintptr_t)s->name);
            }
        }
    }
    self->provider=5;
    for(s->index=0;s->index<5;++s->index){
        s->open_result=_AIL_open_3D_provider_4(s->candidates[s->index]);
        if(!s->open_result){
            self->provider=s->index;self->device3d=s->candidates[s->index];break;
        }
    }
    if(self->provider==5)return;
    (void)_AIL_3D_provider_attribute_12(self->device3d,0x6047ccu,(uintptr_t)&s->environment);
    if(s->environment!=-1)self->eax_supported=1;
    if(!FUN_0055e14f(sound,5))return;
    for(s->index=0;s->index<self->count3d;++s->index)FUN_0055fa1c(sound,s->index);
    for(s->index=0;s->index<self->count3d;++s->index){
        U(self->cache3d+(uint32_t)s->index*12u)=UINT32_MAX;
        U(self->cache3d+(uint32_t)s->index*12u+4u)=0;
        U(self->cache3d+(uint32_t)s->index*12u+8u)=0;
    }
    if(self->eax_supported)FUN_0055fec8(sound,1);
}
void FUN_0055f697(RestoreSound *self)
{
    RestoreSoundProviderScratch scratch;
    /* Native leaves unenumerated candidates/failed attribute output
       uninitialized. Keep that prestate explicit; never invent provider IDs. */
    restore_sound_select_3d_body(self,&scratch);
}
