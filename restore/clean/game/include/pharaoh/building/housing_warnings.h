#ifndef PHARAOH_BUILDING_HOUSING_WARNINGS_H
#define PHARAOH_BUILDING_HOUSING_WARNINGS_H

#include "pharaoh/building/construction_warnings.h"
#include "pharaoh/runtime/city_warning.h"
#include <stddef.h>

/* Actual EA1870 player bank, stride49D8. Unknown field semantics retain their
 * native offsets; every field below has a native DWORD load/consumer. */
typedef struct PharaohHousingWarningCity {
    uint8_t native_0000[0x24];
    int32_t population;
    uint8_t native_0028[0x38-0x28];
    int32_t native_0038;
    uint8_t native_003c[0x2728-0x3c];
    uint32_t native_2728;
    int32_t native_272c;
    uint8_t native_2730[0x273c-0x2730];
    int32_t native_273c;
    uint32_t native_2740;
    uint8_t native_2744[0x296c-0x2744];
    int32_t labor_available;
    uint8_t native_2970[0x2bfc-0x2970];
    int32_t unemployment_percentage;
    uint8_t native_2c00[0x4410-0x2c00];
    uint32_t migration_reason;
    uint8_t native_4414[0x45ac-0x4414];
    int32_t labor_needed;
    uint8_t native_45b0[0x4634-0x45b0];
    uint32_t native_4634;
    uint8_t native_4638[0x4644-0x4638];
    uint32_t native_4644;
    uint8_t native_4648[0x49d8-0x4648];
} PharaohHousingWarningCity;

_Static_assert(sizeof(PharaohHousingWarningCity)==0x49d8 &&
    offsetof(PharaohHousingWarningCity,population)==0x24 &&
    offsetof(PharaohHousingWarningCity,native_0038)==0x38 &&
    offsetof(PharaohHousingWarningCity,native_2728)==0x2728 &&
    offsetof(PharaohHousingWarningCity,native_272c)==0x272c &&
    offsetof(PharaohHousingWarningCity,native_273c)==0x273c &&
    offsetof(PharaohHousingWarningCity,native_2740)==0x2740 &&
    offsetof(PharaohHousingWarningCity,labor_available)==0x296c &&
    offsetof(PharaohHousingWarningCity,unemployment_percentage)==0x2bfc &&
    offsetof(PharaohHousingWarningCity,migration_reason)==0x4410 &&
    offsetof(PharaohHousingWarningCity,labor_needed)==0x45ac &&
    offsetof(PharaohHousingWarningCity,native_4634)==0x4634 &&
    offsetof(PharaohHousingWarningCity,native_4644)==0x4644,
    "native housing-warning bank fields");

typedef struct PharaohHousingWarningContext {
    const uint32_t *editor_active, *scenario_mode, *current_player;
    const int8_t *mission_id;
    const PharaohHousingWarningCity *cities;
    PharaohCityWarningContext *warnings;
} PharaohHousingWarningContext;

/* Full native004E2FB0, reused from actual BuildingRectCommit C. */
void pharaoh_housing_migration_post_message(
    const PharaohHousingWarningContext *, int32_t tool);
/* Input bindings for existing complete4E2D80/4E2DD0 C owners. Editor DWORD is
 * compared to1 before narrowing to their logical BYTE gate. */
void pharaoh_housing_warning_unemployment(const PharaohHousingWarningContext *);
void pharaoh_housing_warning_labor_need(const PharaohHousingWarningContext *);

#endif
