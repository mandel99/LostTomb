#ifndef RESTORE_EVENT_DESCRIPTION_H
#define RESTORE_EVENT_DESCRIPTION_H
#include "town_menu.h"
const char *restore_event_kind_label(int32_t kind);
const char *restore_event_description(const RestoreTownRecord *event);
uint32_t restore_event_has_quantity_range(const RestoreTownRecord *event);
void restore_editor_event_list_draw(void);
const char *FUN_00448f20(int32_t kind);
const char *FUN_00448f60(const RestoreTownRecord *event);
uint32_t FUN_00449620(const RestoreTownRecord *event);
void FUN_00448640(void);
#endif
