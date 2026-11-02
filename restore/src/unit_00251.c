#include "restore_types.h"
#include "campaign_profile.h"
extern restore_legacy_FILE *FUN_00561a0a(const char *,const char *);
extern uint32_t FUN_00561f5f(const void *,uint32_t,uint32_t,restore_legacy_FILE *);
extern uint32_t FUN_00561994(restore_legacy_FILE *);

uint8_t FUN_0041b8a0(const RestoreCampaignProfile *self,const char *path)
{
    restore_legacy_FILE *stream=FUN_00561a0a(path,"wb");
    uint32_t result;
    if(!stream)return 0;
    result=FUN_00561f5f(self,0x4a9c,1,stream);
    FUN_00561994(stream);
    return result==1;
}
