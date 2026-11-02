#include "training_queries.h"
uint32_t TQ_PUBLIC(FUN_004c5190)(void)
{
    uint32_t offset=TQ_U(0xeaac28)*0x49d8u,result=(TQ_S(0xea61f4u+offset)>0);
    if(TQ_S(0xea61f6u+offset)>0)result-=2u;
    if(TQ_S(0xea61f8u+offset)>0)--result;
    return result;
}
