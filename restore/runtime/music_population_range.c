#include "music_configuration.h"
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
void restore_music_set_range(RestoreMusicManager *self,int32_t first,int32_t last)
{
    uint32_t base=(uint32_t)(uintptr_t)self;
    U(base+0x59a54u)=(uint32_t)first;U(base+0x59a58u)=(uint32_t)last;
}
void restore_music_select_population_range(RestoreMusicManager *self,int force)
{
    uint32_t base=(uint32_t)(uintptr_t)self,profile=U(0xeaac28u),row,track,first,last,current;
    int32_t category=(int8_t)B(0xe38ea8u),population,count;
    population=(int32_t)U(0xea1894u+profile*0x49d8u);
    row=base+(uint32_t)category*0x1b10u;current=U(base+0x59a60u);track=row+current*112u;
    if(population>(int32_t)U(track+108u)||population<(int32_t)U(track+104u)||force){
        count=(int32_t)U(row+0x1b08u);
        for(first=0;(int32_t)first<count;++first){
            track=row+first*112u;
            if(population>=(int32_t)U(track+104u)&&population<=(int32_t)U(track+108u))break;
        }
        for(last=first;(int32_t)last<count;++last)if((int32_t)U(row+last*112u+104u)>population)break;
        FUN_00539a50(self,(int32_t)first,(int32_t)(last-1u));
    }
    current=U(base+0x59a60u);first=U(base+0x59a54u);
    if((int32_t)current<(int32_t)first||(int32_t)current>(int32_t)U(base+0x59a58u))U(base+0x59a60u)=first;
    if(first!=U(base+0x59a5cu)){
        U(base+0x59a5cu)=first;
        if(!U(base+0x59a6cu))U(base+0x59a64u)=1;
    }
}
