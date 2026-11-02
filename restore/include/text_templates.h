#ifndef RESTORE_TEXT_TEMPLATES_H
#define RESTORE_TEXT_TEMPLATES_H
#include <stdint.h>
typedef struct RestoreTextTemplateEntry {int32_t id;const char *key;uint32_t callback;} RestoreTextTemplateEntry;
typedef struct RestoreTextTemplateContext {const RestoreTextTemplateEntry *entries;} RestoreTextTemplateContext;
typedef struct RestoreMessageCatalog {uint32_t unused0;const char **entries;uint32_t unused8;int32_t count;uint32_t unused16;} RestoreMessageCatalog;
typedef struct RestoreMessageKey {int32_t id;const char *name;} RestoreMessageKey;
RestoreMessageCatalog *FUN_004cba60(RestoreMessageCatalog *,const char *,const RestoreMessageKey *);
void FUN_004cbd70(RestoreMessageCatalog *);
void FUN_004482f0(void);
typedef char restore_template_entry_size[sizeof(RestoreTextTemplateEntry)==12?1:-1];
uint8_t FUN_004cb980(const RestoreTextTemplateContext *,const char *,char *,int);
const char *FUN_004cbdb0(const RestoreMessageCatalog *,int);
const char *restore_text_template_callback(uint32_t address);
void restore_text_templates_initialize(void);
#endif
