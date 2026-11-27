#include "pharaoh/simulation/farm_initialization.h"

void pharaoh_farm_max_progress_initialize(int32_t *maximum)
{
    *maximum=2000;
}

void pharaoh_farm_max_progress_construct(int32_t *maximum)
{
    pharaoh_farm_max_progress_initialize(maximum);
}
