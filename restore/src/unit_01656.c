#include "text_templates.h"
#include <stddef.h>
extern int FUN_0056b240(const unsigned char *,const unsigned char *,int);
uint8_t FUN_004cb980(const RestoreTextTemplateContext *self,const char *input,char *output,int capacity)
{
    int source=0,destination=0;
    for(;;) {
        const RestoreTextTemplateEntry *entry;
        const char *replacement;
        uint32_t saved;
        int start;
        while(input[source]!='[') {
            if(destination>=capacity)return 0;
            output[destination++]=input[source];
            if(!input[source++])return 1;
        }
        start=source+1;
        while(input[source]!=']')if(!input[source++])return 0;
        entry=self->entries;
        for(;;++entry) {
            if(!entry->key)return 0;
            if(!FUN_0056b240((const unsigned char *)entry->key,(const unsigned char *)input+start,source-start))break;
        }
        saved=*(uint32_t *)0xd3b18cu;*(const RestoreTextTemplateEntry **)0xd3b18cu=entry;
        replacement=restore_text_template_callback(entry->callback);
        *(uint32_t *)0xd3b18cu=saved;
        if(replacement)while(*replacement) {
            if(destination>=capacity)return 0;
            output[destination++]=*replacement++;
        }
        ++source;
    }
}
