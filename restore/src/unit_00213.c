#include "campaign_index.h"
#define U(address) (*(uint32_t *)(uintptr_t)(address))
#define B(address) (*(uint8_t *)(uintptr_t)(address))
void FUN_00417dd0(int8_t category)
{
    int index;
    uintptr_t record;
    U(0xe38eb0)=U(0x5d1988u+U(0xe39080)*4u);
    B(0xe38ea9)=(uint8_t)category;
    U(0xe38eb4)=0; U(0xe38eb8)=0;
    U(0xea5cc0u+U(0xeaac28)*0x49d8u)=0;
    B(0xe38eab)=(uint8_t)FUN_0041b920(RESTORE_CAMPAIGN_INDEX,(uint32_t)(int32_t)category,-1,0);
    index=(int8_t)B(0xe38eab);
    record=0x6fdd08u+(uint32_t)index*64u;
    B(0x7027a4)=0;
    B(0xe38ea8)=index>=0&&index<RESTORE_CAMPAIGN_INDEX->count&&record?B(record+12):0;
    B(0xe38eaa)=0xff;
}
