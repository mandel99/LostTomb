#include "host_allocator.h"
#include "standalone_data.h"
#include <stdio.h>
#include <stdlib.h>
#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define BUFFER (*(unsigned char **)(uintptr_t)0xe92e1cu)
#define REQUIRE(x) do{if(!(x)){fprintf(stderr,"Frame buffer failure at line %d\n",__LINE__);abort();}}while(0)
extern void FUN_00414bd0(void);
static size_t requested;
static unsigned int allocations,frees,events[4],event_count;
static void *expected_free;
void FUN_0051e930(void) {event_count=0;events[event_count++]=1;}
void FUN_0053f790(int *slot,char restore) {REQUIRE(slot==(int *)(uintptr_t)0x1265170u&&restore==1);events[event_count++]=2;}
void FUN_004ddf90(void) {events[event_count++]=3;}
void FUN_004de080(void) {events[event_count++]=4;}
void *FUN_00561b68(size_t bytes) {requested=bytes;++allocations;return restore_heap_allocate(bytes);}
void FUN_00561bdc(void *p) {REQUIRE(p==expected_free);++frees;restore_heap_free(p);}
int main(void)
{
    static const uint32_t dimensions[][2]={{640,480},{800,600},{1024,768},{1280,1024},{65536,65536},{0xffffffffu,1}};
    unsigned int i,mode,iteration,cases=0;
    REQUIRE(restore_data_open());BUFFER=NULL;
    for(iteration=0;iteration<20;++iteration)for(mode=0;mode<3;++mode)for(i=0;i<6;++i) {
        uint32_t wanted=dimensions[i][0]*dimensions[i][1]*2u;
        if((int32_t)wanted<=1200000)wanted=1200000;
        U(0xe38db0)=dimensions[i][0];U(0xe38dac)=dimensions[i][1];U(0xf53f78)=mode;
        expected_free=BUFFER;FUN_00414bd0();
        REQUIRE(BUFFER&&(uintptr_t)BUFFER>0xffu&&requested==wanted);
        REQUIRE(restore_heap_size(BUFFER)>=wanted);
        BUFFER[0]=0x12;BUFFER[wanted-1]=0x34;
        REQUIRE(event_count==4&&events[0]==1&&events[1]==2&&events[2]==3&&events[3]==4);
        REQUIRE(U(0x70e464)==(mode==1?(i==0?10u:i==1?12u:i==2?16u:8u):(i==1?10u:i==2?14u:8u)));++cases;
    }
    REQUIRE(allocations==cases&&frees==cases-1);expected_free=BUFFER;FUN_00561bdc(BUFFER);BUFFER=NULL;
    printf("Frame buffer C: %u resizing scenarios, complete pointer ownership, minimum size, 32-bit arithmetic and mode dispatch verified; unrelated game callbacks controlled.\n",cases);
    restore_data_close();return 0;
}
