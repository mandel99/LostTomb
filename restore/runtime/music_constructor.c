#include "audio_global_lifecycle.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
RestoreMusicManager *restore_music_construct(RestoreMusicManager *self)
{
    uint32_t base=(uint32_t)(uintptr_t)self,row,i,category;
    for(i=0;i<9u;++i)U(base+0x59a50u+i*4u)=0;
    for(category=0;category<53u;++category){
        row=base+category*0x1b10u;U(row+0x1b08u)=0;
        for(i=0;i<0x690u;++i)U(row+i*4u)=0;
        for(i=0;i<50u;++i)U(row+0x1a40u+i*4u)=0;
    }
    return self;
}
