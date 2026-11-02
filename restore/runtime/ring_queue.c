/* Original 50-slot object, including untouched words 0 and 4 and exact alias order. */
#include "ring_queue.h"
#include <string.h>

static uint32_t load(const void *memory,unsigned int index)
{
    uint32_t value;
    memcpy(&value,(const unsigned char *)memory+index*4,4);
    return value;
}

static void store(void *memory,unsigned int index,uint32_t value)
{
    memcpy((unsigned char *)memory+index*4,&value,4);
}

uint32_t restore_004f6520(void *queue)
{
    unsigned int i;
    store(queue,1,0);
    store(queue,2,0);
    store(queue,3,50);
    for (i=0;i<50;i++) store(queue,5+i,0);
    return 1;
}

uint32_t restore_004f6560(void *queue)
{
    store(queue,2,0);
    store(queue,1,0);
    return 0;
}

uint32_t restore_004f6570(const void *queue,uint32_t value)
{
    uint32_t index=load(queue,1),tail=load(queue,2);
    while (index!=tail) {
        if (load(queue,5+index)==value) return 1;
        index++;
        if ((int32_t)index>=(int32_t)load(queue,3)) index=0;
    }
    return 0;
}

uint32_t restore_004f65b0(void *queue,const void *value)
{
    uint32_t tail=load(queue,2),capacity=load(queue,3),next=tail+1;
    if ((int32_t)next>=(int32_t)capacity) next=0;
    if (next==load(queue,1)) return 0;
    store(queue,5+tail,load(value,0));
    store(queue,2,next);
    return 1;
}

uint32_t restore_004f65f0(const void *queue,void *cursor)
{
    uint32_t tail=load(queue,2),index=load(cursor,0),value=0;
    if (index!=tail) {
        value=load(queue,5+index);
        index++;
        store(cursor,0,index);
        /* Cursor may alias a queue word, including capacity. Reread after writing. */
        if ((int32_t)index>=(int32_t)load(queue,3)) store(cursor,0,0);
    }
    return value;
}

uint32_t restore_004f6630(const void *queue)
{
    uint32_t difference=load(queue,2)-load(queue,1);
    return difference&0x80000000u ? load(queue,3)+difference : difference;
}
