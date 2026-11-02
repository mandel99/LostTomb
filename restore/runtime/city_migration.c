#include "city_migration.h"
#include <string.h>
#include <stddef.h>
typedef struct {unsigned int destination; int source; unsigned int bytes;} Field;
static const Field fields[]={
#include "city_migration_layout.h"
};
void restore_migrate_city_record(void *record)
{
    unsigned char expanded[RESTORE_CITY_RECORD_SIZE];
    size_t i;
    for(i=0;i<sizeof(fields)/sizeof(fields[0]);++i) {
        const Field *field=&fields[i];
        if(field->source<0) memset(expanded+field->destination,0,field->bytes);
        else memcpy(expanded+field->destination,(const unsigned char *)record+field->source,field->bytes);
    }
    memcpy(record,expanded,sizeof(expanded));
}
