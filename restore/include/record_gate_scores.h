#ifndef RESTORE_RECORD_GATE_SCORES_H
#define RESTORE_RECORD_GATE_SCORES_H
#include <stdint.h>
/* Native records have a 264-byte stride; names describe proven field roles only. */
static int32_t restore_record_gate_score(int32_t index, uint16_t excluded_type,
 uint32_t gate_address, uint32_t metric_address)
{
 uint32_t offset=(uint32_t)index*264u;
 uint8_t metric;
 if (*(volatile const uint16_t *)(uintptr_t)(0x007999e8u+offset)==excluded_type) return 0;
 if (*(volatile const uint8_t *)(uintptr_t)(0x007999ddu+offset)==0) return 0;
 if (*(volatile const uint32_t *)(uintptr_t)gate_address==0) return 0;
 metric=*(volatile const uint8_t *)(uintptr_t)(metric_address+offset);
 return 10-(int32_t)(metric/10u);
}
#define RESTORE_RECORD_GATE_SCORE(Address,Excluded,Gate,Metric) \
 int32_t restore_record_gate_score_##Address(int32_t index) \
 { return restore_record_gate_score(index,Excluded,Gate,Metric); } \
 int32_t FUN_##Address(int32_t index) \
 { return restore_record_gate_score_##Address(index); }
#endif
