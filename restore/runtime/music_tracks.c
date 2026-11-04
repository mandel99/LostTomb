#include "music_configuration.h"
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
int restore_music_add_track(RestoreMusicManager *self,const char *path,int category,int32_t delay,int32_t minimum,int32_t maximum,int ambient)
{
    uint32_t base=(uint32_t)(uintptr_t)self,row,index,destination,source=(uint32_t)(uintptr_t)path,length=0,i;
    if(category>53)return -1;
    row=base+(uint32_t)category*0x1b10u;
    index=U(row+(ambient?0x1b0cu:0x1b08u));
    if(index==(ambient?10u:50u))return -1;
    destination=row+(index+(ambient?50u:0u))*112u;
    while(B(source+length))++length;
    ++length;
    for(i=0;i<(length>>2);++i)U(destination+i*4u)=U(source+i*4u);
    for(i=length&~3u;i<length;++i)B(destination+i)=B(source+i);
    if(ambient){
        destination=row+index*112u;
        U(destination+0x1644u)=(uint32_t)delay;
        U(destination+0x1648u)=0;U(destination+0x164cu)=0;
        U(row+0x1b0cu)=U(row+0x1b0cu)+1u;
    }else{
        U(destination+100u)=(uint32_t)delay;U(destination+104u)=(uint32_t)minimum;U(destination+108u)=(uint32_t)maximum;
        U(row+0x1b08u)=U(row+0x1b08u)+1u;
    }
    return 1;
}
