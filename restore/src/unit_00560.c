#include "actor_predicate.h"

/* Original 004387d0, including the two jump tables at 00438d20/00438dec.
   All offsets are byte offsets. Volatile accesses preserve reads around the
   deep calls, including the selected-profile reload before the final query. */
static uint32_t actor_read(uint32_t address, uint32_t width)
{
#ifdef RESTORE_ACTOR_PREDICATE_TRACE_READS
    return restore_actor_predicate_trace_read(address, width);
#else
    if (width == 1) return *(volatile uint8_t *)(uintptr_t)address;
    if (width == 2) return *(volatile uint16_t *)(uintptr_t)address;
    return *(volatile uint32_t *)(uintptr_t)address;
#endif
}

static uint32_t selected_profile(void)
{
    return actor_read(0x00eaac28u, 4) * 0x49d8u;
}

uint8_t FUN_004387d0(int32_t actor_id)
{
    uint32_t mode = actor_read(0x00e94e0cu, 4);
    uint32_t actor = (uint32_t)actor_id * 0x184u;
    uint32_t profile, selected, active, first_category, category, desired;
    int32_t building, building_type;
    uint8_t type, subtype;

    /* Native mode 51 returns true without reading the actor. The default
       branch also does not dereference an actor outside recognized modes. */
    if (mode == 51) return 1;
    switch (mode) {
    case 4: case 8: case 9: case 10: case 11: case 12: case 13:
    case 14: case 15: case 16: case 17: case 18: case 19: case 20:
    case 21: case 22: case 23: case 24: case 25: case 26: case 29:
    case 30: case 31: case 43: case 45: case 46: case 48: case 49:
    case 50:
        type = (uint8_t)actor_read(0x00bcd602u + actor, 1);
        break;
    default:
        return 0;
    }

    switch (mode) {
    case 4: return type == 27;
    case 8: return type == 10;
    case 9: return type == 8;
    case 10:
        switch (type) {
        case 88: case 89: case 22: case 23: case 24:
        case 11: case 12: case 13: case 14:
            return 1;
        case 27:
            building = (int16_t)actor_read(0x00bcd652u + actor, 2);
            profile = selected_profile();
            active = actor_read(0x00ea5ce8u + profile, 2);
            /* The native MOV reads selected even when active is zero. */
            selected = actor_read(0x00ea5f9cu + profile, 4);
            if (!active) return 0;
            first_category = FUN_004c5120((int32_t)selected);
            category = FUN_004c5120(building);
            if (category != first_category) return 0;
            category = FUN_004c5120((int32_t)selected);
            if (category == 1) desired = 1;
            else if (category == 3) desired = 2;
            else return 0;
            /* Use the captured selected ID for the third classification,
               then reload the current profile for the final query. */
            profile = selected_profile();
            selected = actor_read(0x00ea5f9cu + profile, 4);
            return FUN_004c52d0((int32_t)selected, desired) != 0;
        default: return 0;
        }
    case 11: return (type >= 15 && type <= 18) || type == 105;
    case 12: return type == 15;
    case 13: return type == 16;
    case 14: return type == 17;
    case 15: return type == 18;
    case 16: return type == 105;
    case 17: return type == 30 || type == 29;
    case 18: case 20: return type == 29;
    case 19: return type == 30;
    case 21: return type == 31;
    case 22: return type == 32;
    case 23: case 48: return type == 33;
    case 24: return type == 34;
    case 25: return type == 7;
    case 26:
        if (type == 26 || type == 39 || type == 66 || type == 25) return 1;
        if (type != 4) return 0;
        subtype = (uint8_t)actor_read(0x00bcd603u + actor, 1);
        return subtype >= 1 && subtype <= 8;
    case 29: return type == 5;
    case 30: return type == 41 || type == 64;
    case 31:
        if (type == 4) {
            if (actor_read(0x00bcd630u + actor, 1) != 8) return 0;
            return actor_read(0x00bcd65eu + actor, 1) != 0;
        }
        if (type == 5) {
            building = (int16_t)actor_read(0x00bcd652u + actor, 2);
            return (actor_read(0x00799ad8u + (uint32_t)building * 0x108u, 4)
                    & 0x1000u) != 0;
        }
        return type == 106 || type == 109 || type == 107;
    case 43: return type == 32 || type == 98;
    case 45: return type == 87;
    case 46: return type == 1 || type == 2;
    case 49:
        switch (type) {
        case 11: case 12: case 13: case 14:
        case 41: case 42: case 43: case 44: case 45: case 46:
        case 47: case 48: case 49: case 50: case 51: case 52:
        case 53: case 54: case 55: case 56: case 57: case 58:
        case 59: case 60: case 61: case 62: case 77: case 78:
        case 88: case 92: case 93: case 99: case 100: case 101:
            return 1;
        case 27:
            building = (int16_t)actor_read(0x00bcd652u + actor, 2);
            building_type = (int16_t)actor_read(
                0x007999e8u + (uint32_t)building * 0x108u, 2);
            if (building_type != 63 && building_type != 68) return 0;
            profile = selected_profile();
            if (!actor_read(0x00ea5ce8u + profile, 2)) return 0;
            selected = actor_read(0x00ea5f9cu + profile, 4);
            return FUN_004c52d0((int32_t)selected, 2) != 0;
        default: return 0;
        }
    case 50: return type == 89;
    default: return 0;
    }
}
