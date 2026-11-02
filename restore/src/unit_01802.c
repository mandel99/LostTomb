#include "city_migration.h"
void FUN_004d1ad0(void)
{
    unsigned int city;
    for(city=0;city<2;++city)
        restore_migrate_city_record((void *)(0x00ea1870u+city*RESTORE_CITY_RECORD_SIZE));
}
