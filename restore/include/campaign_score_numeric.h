#ifndef RESTORE_CAMPAIGN_SCORE_NUMERIC_H
#define RESTORE_CAMPAIGN_SCORE_NUMERIC_H
#include <stdint.h>
int32_t FUN_00401540(uint32_t building,int32_t *output);
uint64_t restore_campaign_score_trunc80(const uint8_t raw80[10]);
void restore_campaign_score_raw80(uint32_t scene_offset,uint32_t bonus,uint32_t months,uint8_t raw80[10]);
int32_t restore_campaign_score_numeric(uint32_t scene_offset,uint32_t bonus,uint32_t months);
#endif
