#ifndef RESTORE_PREVIEW_FOOTPRINT_SEEDS_H
#define RESTORE_PREVIEW_FOOTPRINT_SEEDS_H
#include <stdint.h>
/* Initialize only x,y,size,type. Run the eight startup geometry methods after
   this call to derive each record's remaining nine DWORDs. */
void restore_preview_footprint_seeds_initialize(void);
#endif
