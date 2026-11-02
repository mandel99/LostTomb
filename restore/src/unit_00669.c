#include "training_queries.h"
bool TQ_PUBLIC(FUN_00447790)(int record,int resource)
{
    uint32_t category=TQ_B(0x77de04u+(uint32_t)record*106u);
    uint32_t fee=TQ_U(0x77646cu+(category*36u+(uint32_t)resource)*4u);
    return FUN_004c5240((int)fee)>0;
}
