#ifndef PHARAOH_WALKER_CROSS_COUNTRY_SETUP_H
#define PHARAOH_WALKER_CROSS_COUNTRY_SETUP_H

#include "pharaoh/walker/record_queries.h"

/* Native004B4220. Caller supplies the actual record selected by walker id. */
void pharaoh_walker_cross_country_setup(PharaohWalkerQueryRecord *walker,
    int32_t from_x,int32_t from_y,int32_t target_x,int32_t target_y,int32_t mode);

#endif
