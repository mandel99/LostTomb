#include <stdint.h>
#include <stdbool.h>

/* Native-address local views. Indices below are in the declared C element unit. */
#define quota_weights (*(int32_t *)0x005da544u)
#define resource_groups (*(int32_t *)0x005da554u)
#define resource_capacity (*(int32_t *)0x0070573cu)
#define building_bound (*(int32_t *)0x0078d790u)
#define building_status (*(uint8_t *)0x007999dcu)
#define building_kind (*(int16_t *)0x007999e8u)
#define building_stock (*(int16_t *)0x007999f4u)
#define building_capacity (*(int16_t *)0x00799a18u)

extern void FUN_00461b20(int32_t building);
extern int32_t FUN_00475b50(int32_t building);

void FUN_00461950(int32_t requested_total)

{
  int32_t *resource;
  int32_t requested;
  bool all_quotas_empty;
  int32_t *quota_or_resource;
  int32_t group_or_building;
  int32_t capacity_or_resource;
  int32_t *group_begin;
  int32_t *group_cursor;
  int32_t *resource_cursor;
  int16_t *kind_cursor;
  int32_t building;
  int32_t quotas_and_capacities [8]; /* Native adjacent quota/capacity DWORDs. */
  
  group_or_building = 0;
  do {
    if (group_or_building == 0) {
      quotas_and_capacities[0] = (int32_t)((uint32_t)quota_weights * (uint32_t)requested_total) / 100;
    }
    else {
      quotas_and_capacities[group_or_building] = (int32_t)((uint32_t)quotas_and_capacities[group_or_building] + (uint32_t)((int32_t)((uint32_t)(&quota_weights)[group_or_building] * (uint32_t)requested_total) / 100));
    }
    quotas_and_capacities[group_or_building + 4] = 0;
    capacity_or_resource = (&resource_groups)[group_or_building * 0xc];
    if (capacity_or_resource != 0) {
      quota_or_resource = &resource_groups + group_or_building * 0xc;
      do {
        quota_or_resource = quota_or_resource + 1;
        quotas_and_capacities[group_or_building + 4] = (int32_t)((uint32_t)quotas_and_capacities[group_or_building + 4] + (uint32_t)(&resource_capacity)[capacity_or_resource]);
        capacity_or_resource = *quota_or_resource;
      } while (capacity_or_resource != 0);
    }
    if (group_or_building != 3) {
      quotas_and_capacities[group_or_building + 1] = 0;
    }
    capacity_or_resource = quotas_and_capacities[group_or_building + 4];
    requested = quotas_and_capacities[group_or_building];
    if (requested < capacity_or_resource) {
      quotas_and_capacities[group_or_building + 4] = requested;
    }
    else if ((capacity_or_resource < requested) && (group_or_building != 3)) {
      quotas_and_capacities[group_or_building + 1] = (int32_t)((uint32_t)requested - (uint32_t)capacity_or_resource);
    }
    group_or_building = group_or_building + 1;
  } while (group_or_building < 4);
  building = 0;
  if (-1 < building_bound) {
    kind_cursor = &building_kind;
    do {
      if (*((uint8_t *)kind_cursor - 16) == '\x01') {
        group_or_building = FUN_00475b50(building);
        if ((((&building_capacity)[group_or_building * 0x84] != 0) ||
            (*(char *)((int32_t)&building_status + group_or_building * 0x108 + 1) != '\0')) &&
           ((*(char *)((int32_t)&building_status + group_or_building * 0x108 + 1) == '\0' ||
            ((&building_stock)[group_or_building * 0x84] != 0)))) {
          group_begin = &resource_groups;
          all_quotas_empty = true;
          quota_or_resource = quotas_and_capacities + 4;
          group_cursor = &resource_groups;
          do {
            if (*quota_or_resource != 0) {
              all_quotas_empty = false;
              group_or_building = *group_cursor;
              resource_cursor = group_begin;
              resource = group_cursor;
              while (group_or_building != 0) {
                if ((int32_t)*kind_cursor == *resource) {
                  *quota_or_resource = (int32_t)((uint32_t)*quota_or_resource - 1u);
                  FUN_00461b20(building);
                }
                if (*quota_or_resource == 0) break;
                resource = resource_cursor + 1;
                resource_cursor = resource_cursor + 1;
                group_or_building = *resource;
                resource = resource_cursor;
              }
            }
            group_cursor = group_cursor + 0xc;
            group_begin = group_begin + 0xc;
            quota_or_resource = quota_or_resource + 1;
          } while ((int32_t)group_cursor < 0x5da614);
          if (all_quotas_empty) {
            return;
          }
        }
      }
      building = building + 1;
      kind_cursor = kind_cursor + 0x84;
    } while (building <= building_bound);
  }
  return;
}
