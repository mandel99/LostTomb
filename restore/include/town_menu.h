#ifndef RESTORE_TOWN_MENU_H
#define RESTORE_TOWN_MENU_H
#include <stdint.h>
typedef struct RestoreTownRecord {uint8_t bytes[124];} RestoreTownRecord;
typedef struct RestoreTownList {int32_t count;RestoreTownRecord records[150];uint8_t metadata[24];} RestoreTownList;
typedef char restore_town_list_size[sizeof(RestoreTownList)==18628?1:-1];
#define RESTORE_TOWNS ((RestoreTownList *)0x00785d70u)
RestoreTownRecord *restore_town_next(RestoreTownList *,const int16_t *,uint8_t);
#include "town_trade.h"
int *FUN_0044bb90(const int16_t *,char);
void FUN_0041e270(RestoreTownRecord *,int);
int FUN_0041e3b0(void);
uint8_t FUN_004493c0(const RestoreTownRecord *);
#endif
