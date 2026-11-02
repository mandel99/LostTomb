#ifndef RESTORE_CITY_MIGRATION_H
#define RESTORE_CITY_MIGRATION_H
#define RESTORE_CITY_RECORD_SIZE 0x49d8u
/* Expand a packed old city record in its existing storage. Caller owns the
   complete output-sized buffer; there is no fixed-address dependency here. */
void restore_migrate_city_record(void *record);
#endif
