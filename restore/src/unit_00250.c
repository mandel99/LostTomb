#include "restore_types.h"
#include "campaign_profile.h"
#include <string.h>
extern restore_legacy_FILE *FUN_00561a0a(const char *,const char *);
extern uint32_t FUN_00561ca1(void *,uint32_t,uint32_t,restore_legacy_FILE *);
extern int FUN_00561ea4(restore_legacy_FILE *,int,int);
extern uint32_t FUN_00561994(restore_legacy_FILE *);

uint8_t FUN_0041b680(RestoreCampaignProfile *self,const char *path)
{
    RestoreCampaignProfile temporary={0};
    restore_legacy_FILE *stream;
    uint32_t result;
    int i;
    size_t length;
    FUN_0041ac50(&temporary.campaign);
    memset(self->suffix,0,sizeof(self->suffix));
    stream=FUN_00561a0a(path,"rb");
    if(!stream)return 0;
    result=FUN_00561ca1(&temporary,0x4a9c,1,stream);
    if(!result) {
        FUN_00561ea4(stream,0,0);
        /* The 0x4a6c fallback retains any tail from the first partial read.
           Zero initialization defines bytes that neither read supplied. */
        result=FUN_00561ca1(&temporary,0x4a6c,1,stream);
    } else memcpy(self->suffix,temporary.suffix,sizeof(self->suffix));
    FUN_00561994(stream);
    if(result!=1)return 0;
    /* Reject malformed profiles before the native unbounded string/count loops. */
    if(!memchr(temporary.campaign.active,0,sizeof(temporary.campaign.active))||
       self->campaign.name_count<0||self->campaign.name_count>100)return 0;
    length=strlen(temporary.campaign.active)+1;
    memcpy(self->campaign.active,temporary.campaign.active,length);
    memcpy(self->campaign.state,temporary.campaign.state,sizeof(self->campaign.state));
    for(i=0;i<self->campaign.name_count;++i) {
        int32_t completed;
        memcpy(&completed,(uint8_t *)self+0x2d1c+i*0x4c,4);
        self->campaign.index.records[i].bytes[0x3c]=(completed!=0);
    }
    return 1;
}
