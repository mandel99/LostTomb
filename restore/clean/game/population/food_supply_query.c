#include "pharaoh/population/food_supply_query.h"

#include "pharaoh/core/percentage_adjust.h"
#include "pharaoh/core/x87_numeric.h"
#include "pharaoh/simulation/industry_queries.h"

_Static_assert(sizeof(PharaohBuildingTypeRecord) == 264 &&
    sizeof(PharaohBuildingModelRecord) == 40 &&
    offsetof(PharaohBuildingModelRecord, laborers) == 20 &&
    offsetof(PharaohBuildingTypeRecord, temple_upgrade) == 0xa9 &&
    sizeof(PharaohEmpireCityRecord) == 106 &&
    offsetof(PharaohEmpireCityRecord, sells) == 42,
    "native food-supply building/model/trade views");

/* Native binary64 constants 56F520/56F5A0/56F560/56F5B0/56F588/56F5A8. */
static const double food_one = 1.0, food_zero = 0.0, food_ten = 10.0;
static const double food_produce_per_worker = 0.16;
static const double food_soil_scale = 0.010101010101010102;

static int32_t truncate80(const PharaohFloat80 *value)
{
    return (int32_t)(uint32_t)pharaoh_x87_truncate80(value);
}

static void increment_denominator(PharaohFloat80 *value)
{
    __asm {
        mov eax, value
        fld tbyte ptr [eax]
        fadd food_one
        fstp tbyte ptr [eax]
    }
}

int32_t pharaoh_food_supply_area_quality(const PharaohFoodSupplyQueryContext *c,
    int32_t x, int32_t y, int32_t columns, int32_t rows)
{
    PharaohFloat80 denominator, quotient;
    const uint32_t base = (uint32_t)*c->map_origin + (uint32_t)y * 228u +
        (uint32_t)x;
    uint32_t sum = 0;
    int32_t result;
    __asm { fld food_zero }
    __asm { fstp tbyte ptr denominator }
    *c->current_tile = base;
    if (rows > 0) for (uint32_t row = 0; row < (uint32_t)rows; ++row) {
        if (columns > 0) for (uint32_t col = 0; col < (uint32_t)columns; ++col) {
            const uint32_t offset = c->footprints[row][col].grid_offset;
            increment_denominator(&denominator);
            sum += (uint32_t)c->soil_values[base + offset] + 1u;
        }
    }
    /* Keep the denominator live under FTOL, as the native ST1 guard. */
    __asm {
        fld tbyte ptr denominator
        fild sum
        fdiv st(0), st(1)
        fstp tbyte ptr quotient
    }
    result = truncate80(&quotient);
    __asm { fstp st(0) }
    return (int32_t)((uint32_t)result + 1u);
}

static uint8_t has_active_occupant(const PharaohFoodSupplyQueryContext *c,
    int32_t building)
{
    const PharaohBuildingTypeRecord *b = &c->buildings[building];
    c->occupancy->grid_origin = (uint32_t)*c->map_origin;
    const uint8_t result = (uint8_t)pharaoh_region_has_active_occupant(
        c->occupancy, b->x, b->y, 3, 3);
    *c->current_tile = c->occupancy->last_origin;
    return result;
}

static bool query_is_farm(void *data, int32_t building)
{
    const PharaohFoodSupplyQueryContext *c = data;
    return (uint8_t)pharaoh_building_is_farm(c->buildings, building) != 0;
}

static bool query_is_on_floodplain(void *data, int32_t building)
{
    return has_active_occupant(data, building) != 0;
}

static uint8_t is_floodplain_farm(const PharaohFoodSupplyQueryContext *c,
    int32_t building)
{
    const PharaohIndustryQueryContext industry = {
        .is_farm = query_is_farm, .is_on_floodplain = query_is_on_floodplain,
        .user_data = (void *)c
    };
    return (uint8_t)pharaoh_building_is_floodplain_farm(&industry, building);
}

int32_t pharaoh_food_supply_farm_quality(const PharaohFoodSupplyQueryContext *c,
    int32_t building)
{
    const PharaohBuildingTypeRecord *b = &c->buildings[building];
    uint32_t score = (uint32_t)pharaoh_food_supply_area_quality(c,
        b->x, b->y, 3, 3);
    if (pharaoh_farm_has_water_access(c->buildings, building) &&
        (uint8_t)pharaoh_building_type_is_00488080_group(c->buildings, building))
        score += has_active_occupant(c, building) != 0 ? 20u : 40u;
    return (int32_t)score > 99 ? 99 : (int32_t)score;
}

static void capacity_value(int32_t quality, int32_t workers,
    PharaohFloat80 *extended, double *spill)
{
    __asm {
        fild quality
        fld food_produce_per_worker
        fmul food_soil_scale
        fmulp st(1), st(0)
        fimul workers
        fmul food_ten
        mov eax, spill
        fst qword ptr [eax]
        mov eax, extended
        fstp tbyte ptr [eax]
    }
}

static void capacity_reload(PharaohFloat80 *value, double spill, int doubled)
{
    __asm { fld qword ptr spill }
    if (doubled != 0) { __asm { fadd st(0), st(0) } }
    __asm { mov eax, value }
    __asm { fstp tbyte ptr [eax] }
}

int32_t pharaoh_food_supply_farm_capacity(const PharaohFoodSupplyQueryContext *c,
    int32_t building, uint8_t configured)
{
    const PharaohBuildingTypeRecord *b = &c->buildings[building];
    const int32_t workers = configured != 0 ?
        c->models[b->building_type].laborers : b->num_workers;
    const int32_t quality = pharaoh_food_supply_farm_quality(c, building);
    const uint8_t upgraded = b->temple_upgrade;
    PharaohFloat80 value;
    double spill;
    int doubled = 0;
    int32_t result;
    capacity_value(quality, workers, &value, &spill);
    if (upgraded != 0) {
        /* These are actual native calls; each type is reloaded after its
         * predicate. The flag-zero path retains the extended result. */
        if ((uint8_t)pharaoh_building_is_farm(c->buildings, building) != 0 &&
            b->building_type == 102) doubled = 1;
        else if ((uint8_t)pharaoh_building_is_farm(c->buildings, building) != 0 &&
            b->building_type == 100) doubled = 1;
        capacity_reload(&value, spill, doubled);
    }
    result = truncate80(&value);
    return result < 1 ? 1 : result;
}

static int32_t weighted_total(uint32_t farms, uint32_t other)
{
    PharaohFloat80 value;
    __asm {
        fild farms
        fdiv food_ten
        fild other
        faddp st(1), st(0)
        fstp tbyte ptr value
    }
    return truncate80(&value);
}

PharaohFoodSupplyQueryResult pharaoh_city_food_supply_query(
    const PharaohFoodSupplyQueryContext *c)
{
    PharaohFoodSupplyQueryResult result = {PHARAOH_PERCENTAGE_SCALE_VALUE, 0};
    const PharaohFoodSupplyPlayer *initial = &c->players[*c->current_player];
    uint32_t ratio_first = 0, ratio_second = 0, first = 0, second = 0;
    uint32_t other = 0, town = 0, player;
    int32_t divisor, dividend, amount;

    divisor = initial->ratio_first_divisor;
    if (divisor != 0) {
        dividend = initial->ratio_first_dividend;
        if (dividend == INT32_MIN && divisor == -1) goto divide_fault;
        ratio_first = (uint32_t)(dividend / divisor) * *c->first_ratio_price;
    }
    divisor = initial->ratio_second_divisor;
    if (divisor != 0) {
        dividend = initial->ratio_second_dividend;
        if (dividend == INT32_MIN && divisor == -1) goto divide_fault;
        ratio_second = (uint32_t)(dividend / divisor) * *c->second_ratio_price;
    }
    for (int32_t id = 1; id < 4000; ++id) {
        const PharaohBuildingTypeRecord *b = &c->buildings[id];
        if (b->state != 1) continue;
        if ((uint8_t)pharaoh_building_is_farm(c->buildings, id) == 0 &&
            b->building_type != 194) continue;
        if ((uint8_t)pharaoh_building_is_farm(c->buildings, id) != 0 &&
            (uint8_t)pharaoh_building_type_is_00489880_group(c->buildings, id) == 0)
            continue;
        if (is_floodplain_farm(c, id) != 0) {
            first += (uint32_t)pharaoh_food_supply_farm_capacity(c, id, 1);
            continue;
        }
        if ((uint8_t)pharaoh_building_is_farm(c->buildings, id) != 0) {
            second += (uint32_t)pharaoh_food_supply_farm_capacity(c, id, 1);
            continue;
        }
        if (b->num_workers <= 0 || b->farm_progress <= 0) continue;
        const uint32_t price = (uint32_t)c->models[b->building_type].laborers << 4;
        other += (uint32_t)((int32_t)price / 4);
    }
    other = (uint32_t)pharaoh_adjust_by_percentage((int32_t)other, 80);
    player = *c->current_player;
    first *= 48u; second <<= 6;
    for (uint32_t i = 0; i < PHARAOH_EMPIRE_CITY_COUNT; ++i) {
        const PharaohEmpireCityRecord *city = &c->cities[i];
        if (city->in_use == 0 ||
            (city->kind != 1 && city->kind != 3 && city->kind != 5) ||
            city->is_open == 0) continue;
        for (uint32_t resource = 1; resource <= 8; ++resource) {
            const uint16_t permission = c->players[player].trade_permissions[resource];
            if (permission != 1 && permission != 3) continue;
            if (city->sells[resource] != 1) continue;
            const uint32_t price =
                (uint32_t)c->trade_amounts[city->route_id].resource[resource] * 100u;
            town += (uint32_t)((int32_t)price / 12);
        }
    }
    amount = weighted_total(first + second, town + other + ratio_second + ratio_first);
    const PharaohTempleWageBonusContext requested = {
        c->buildings, &c->players[player].temple_count,
        &c->players[player].temple_building
    };
    const uint32_t selected_player = *c->current_player;
    const PharaohTempleWageBonusContext selected = {
        c->buildings, &c->players[selected_player].temple_count,
        &c->players[selected_player].temple_building
    };
    const PharaohFoodRateContext rate = {&requested, &selected};
    divisor = pharaoh_city_food_consumption_rate(&rate); /* Always20 or25. */
    result.value = (int32_t)((uint32_t)amount * 100u) / divisor;
    return result;

divide_fault:
    result.status = PHARAOH_PERCENTAGE_SCALE_SIGNED_DIVIDE_OVERFLOW;
    return result;
}
