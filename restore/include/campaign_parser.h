#ifndef RESTORE_CAMPAIGN_PARSER_H
#define RESTORE_CAMPAIGN_PARSER_H
#include "campaign_index.h"
/* Full object prefix used by the text parser; save-game suffix is separate. */
typedef struct RestoreCampaign {
    RestoreCampaignIndex index;
    char names[100][50];
    int32_t name_count;
    char active[65];
    uint8_t padding[3];
    uint8_t state[0x1db0];
} RestoreCampaign;
typedef char restore_campaign_names_offset[(offsetof(RestoreCampaign,names)==0x1904)?1:-1];
typedef char restore_campaign_name_count_offset[(offsetof(RestoreCampaign,name_count)==0x2c8c)?1:-1];
typedef char restore_campaign_active_offset[(offsetof(RestoreCampaign,active)==0x2c90)?1:-1];
typedef char restore_campaign_state_offset[(offsetof(RestoreCampaign,state)==0x2cd4)?1:-1];
typedef char restore_campaign_size[(sizeof(RestoreCampaign)==0x4a84)?1:-1];
#define RESTORE_CAMPAIGN ((RestoreCampaign *)0x006fdd08u)
void FUN_0041ac00(RestoreCampaignRecord *record);
RestoreCampaign *FUN_0041ac50(RestoreCampaign *self);
uint8_t FUN_0041ac90(RestoreCampaign *self,const RestoreCampaignRecord *record);
void FUN_0041ad20(uint8_t *line);
int FUN_0041ad50(int32_t *value);
int FUN_0041adc0(int16_t *value);
void FUN_0041adf0(const char *file,int line,const char *source,const char *error);
uint8_t FUN_0041ae40(RestoreCampaign *self,const char *file);
char *FUN_00561db1(char *text,const char *delimiters);
#endif
