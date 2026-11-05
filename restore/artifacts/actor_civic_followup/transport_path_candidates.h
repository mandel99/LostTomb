#ifndef RESTORE_TRANSPORT_PATH_CANDIDATES_H
#define RESTORE_TRANSPORT_PATH_CANDIDATES_H
#include <stdint.h>

/* Whole native producers. The results are full EAX bit patterns; zero means
   no selected target. Keep the existing canonical RAW owners for incidental
   registers, flags and caller-owned stack slots. */
uint32_t restore_transport_find_animal_land_target(int32_t x, int32_t y,
                                                 int32_t targets, int32_t count);
uint32_t restore_transport_find_marked_free_land_target(int32_t x, int32_t y,
                                                      int32_t *targets, int32_t count);
uint32_t restore_transport_find_road_target(int32_t x, int32_t y,
                                          int32_t targets, int32_t count);
uint32_t restore_transport_find_secondary_road_target(int32_t x, int32_t y,
                                                    int32_t targets, int32_t count);
uint32_t restore_transport_find_destructible_land_target(int32_t x, int32_t y,
                                                       int32_t targets, int32_t count);
#endif
