#include "restore_types.h"
#include "mission_routes.h"
#include <math.h>
#define H(p,o) (*(int16_t *)((uint8_t *)(p)+(o)))
#define D(p,o) (*(int32_t *)((uint8_t *)(p)+(o)))
extern void *FUN_00561a1d(uint32_t);
extern void FUN_00561a2b(void *);

uint8_t FUN_004d4720(RestoreMissionRoute *route)
{
    uint8_t *data=route->bytes,*samples;
    int32_t estimate=0,written=0,capacity=D(data,312),i,j;
    const int32_t count=data[321];
    /* Callers validate this bound; reject malformed direct calls as well. */
    if(count>50)return 0;
    for(i=1;i<count;++i) {
        int32_t dx=H(data,8+i*6)-H(data,2+i*6);
        int32_t dy=H(data,10+i*6)-H(data,4+i*6);
        if(dx<0)dx=-dx;if(dy<0)dy=-dy;
        estimate+=(dx>dy?dx:dy)+1;
    }
    if(capacity<estimate&&capacity<estimate+50) {
        if(D(data,4)) { FUN_00561a2b((void *)(uintptr_t)D(data,4));D(data,4)=0; }
        capacity=estimate+50;
        samples=FUN_00561a1d((uint32_t)capacity*6);
        D(data,4)=(int32_t)(uintptr_t)samples;D(data,312)=capacity;
    } else samples=(uint8_t *)(uintptr_t)D(data,4);
    D(data,308)=estimate;
    for(i=1;i<count;++i) {
        const int32_t x=H(data,2+i*6),y=H(data,4+i*6);
        const int32_t dx=H(data,8+i*6)-x,dy=H(data,10+i*6)-y;
        const float length=(float)sqrt((double)dx*dx+(double)dy*dy);
        int32_t distance;
        float direction_x,direction_y;
        if(written>estimate)return 0;
        if(length==0.0f||!data[0])continue;
        direction_x=(float)((double)dx/length);
        direction_y=(float)((double)dy/length);
        for(distance=0;(float)distance<=length;distance+=data[0]) {
            /* The native estimate can undercount diagonal samples. */
            if(!samples||written>=capacity)return 0;
            H(samples,written*6)=(int16_t)(int32_t)((double)distance*direction_x+x);
            H(samples,written*6+2)=(int16_t)(int32_t)((double)distance*direction_y+y);
            samples[written*6+4]=(uint8_t)(i-1);
            ++written;
        }
    }
    D(data,308)=written;H(data,316)=H(data,318)=-1;
    if(count) {
        const int32_t first_x=H(data,8),first_y=H(data,10);
        const int32_t last_x=H(data,count*6+2),last_y=H(data,count*6+4);
        for(j=0;j<200;++j) {
            uint8_t *city=(uint8_t *)(uintptr_t)(0x77f748u+j*98u);
            int32_t sprite,x,y,width,height;
            if(!city[1]||city[0]!=1)continue;
            sprite=(uint16_t)H(city,12);
            if(sprite>=*(int32_t *)0x126517cu)sprite=0;
            x=H(city,4);y=H(city,6);
            width=*(int16_t *)(uintptr_t)(0x10d6a0cu+sprite*64u);
            height=*(int16_t *)(uintptr_t)(0x10d6a0eu+sprite*64u);
            if(x<=first_x&&first_x<=x+width&&y<=first_y&&first_y<=y+height)H(data,316)=(int16_t)j;
            if(x<=last_x&&last_x<=x+width&&y<=last_y&&last_y<=y+height)H(data,318)=(int16_t)j;
        }
    }
    return 1;
}
