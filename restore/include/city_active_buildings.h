#ifndef RESTORE_CITY_ACTIVE_BUILDINGS_H
#define RESTORE_CITY_ACTIVE_BUILDINGS_H

/* Native 00462180 logical contract: ordered city memory effects and child
 * calls. No arguments or logical return value. The physical register model
 * and the existing canonical facade retain their separate ownership. */
void restore_city_update_active_buildings(void);

#endif
