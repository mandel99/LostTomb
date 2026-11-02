#ifndef RESTORE_TOWN_TRADE_H
#define RESTORE_TOWN_TRADE_H
#include <stdint.h>
struct RestoreTownRecord;
struct RestoreTownList;
struct RestoreTownRecord *restore_town_trade_next(struct RestoreTownList *,const int16_t *);
int *FUN_0044baf0(const int16_t *);
uint32_t FUN_0044d4c0(const struct RestoreTownRecord *);
void FUN_00443330(void);
#endif
