#include "training_queries.h"
#include "menu_training_render.h"
/* Whole5599C0/46 instructions. Two-byte modes, full selected-city stride,
   fresh owner after each eligibility query and native partial-EAX returns. */
uint32_t FUN_005599c0(int32_t resource)
{
    uint32_t owner=TQ_U(0xeaac28u),index=owner*0x24ecu+(uint32_t)resource;
    uint32_t mode=TQ_W(0xea4096u+index*2u),result;
    if(mode==1u||mode==3u){
        result=FUN_00443950(resource);
        if(!result){
            index=TQ_U(0xeaac28u)*0x24ecu+(uint32_t)resource;
            TQ_W(0xea4096u+index*2u)=0;return 1;
        }
        owner=TQ_U(0xeaac28u);
    }
    index=owner*0x24ecu+(uint32_t)resource;
    mode=TQ_W(0xea4096u+index*2u);result=(index&0xffff0000u)|mode;
    if(mode==2u||mode==4u){
        result=FUN_00443b20(resource);
        if(!result){
            index=TQ_U(0xeaac28u)*0x24ecu+(uint32_t)resource;
            TQ_W(0xea4096u+index*2u)=0;return (index&0xffffff00u)|1u;
        }
    }
    return result&0xffffff00u;
}
