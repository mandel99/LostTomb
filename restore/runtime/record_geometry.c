/* Original addresses: 004e5140, 004e5190, 004e52b0.
   See metadata/geometry-evidence.json for exact byte ranges and REA provenance. */
#include "record_geometry.h"
#include <string.h>

static uint32_t load_word(const void *memory,unsigned int index)
{
    uint32_t value;
    memcpy(&value,(const unsigned char *)memory+index*4,sizeof(value));
    return value;
}

static void store_word(void *memory,unsigned int index,uint32_t value)
{
    memcpy((unsigned char *)memory+index*4,&value,sizeof(value));
}

void *restore_004e5140(void *destination,const void *source)
{
    unsigned int i;
    /* Match the observed load/store order, including overlapping records.
       A bulk memcpy or memmove would change the original overlap behavior. */
    for (i=0;i<5;i++) store_word(destination,i,load_word(source,i));
    for (i=0;i<4;i++) {
        store_word(destination,5+i,load_word(source,5+i));
        store_word(destination,9+i,load_word(source,9+i));
    }
    return destination;
}

uint32_t restore_004e5190(void *record,uint32_t scale,uint32_t step)
{
    uint32_t x=load_word(record,0),y=load_word(record,1);
    uint32_t one_plus=step+1u;
    uint32_t difference=x-y,sum=x+y;
    uint32_t a=difference*one_plus,b=sum*scale,c=sum*one_plus;
    uint32_t result=(y-x)*one_plus;
    /* Unsigned operations preserve x86 wraparound without signed C overflow. */
    store_word(record,4,x+y*228u);
    store_word(record,5,a);
    store_word(record,9,b);
    store_word(record,6,c);
    store_word(record,12,difference*scale);
    store_word(record,8,0u-c);
    store_word(record,10,(y-x)*scale);
    store_word(record,7,result);
    store_word(record,11,0u-b);
    /* The original leaves this value in EAX, although its inferred type is void. */
    return result;
}

void restore_004e52b0(void *records,int count,const volatile uint32_t *scale,const volatile uint32_t *step)
{
    unsigned char *record=records;
    while (count>0) {
        uint32_t current_step=*step;
        uint32_t current_scale=*scale;
        restore_004e5190(record,current_scale,current_step);
        record+=52;
        count--;
    }
}
