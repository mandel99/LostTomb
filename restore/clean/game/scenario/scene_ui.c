#include "pharaoh/scenario/scene_ui.h"
#include "pharaoh/core/x87_numeric.h"
#include "pharaoh/core/percentage_scale.h"
#include "pharaoh/population/age_counts.h"
#include "pharaoh/religion/building_types.h"

_Static_assert(offsetof(PharaohScenarioPlayerRuntime, school_age_population)==0x2c &&
    offsetof(PharaohScenarioPlayerRuntime, academy_age_population)==0x30 &&
    offsetof(PharaohScenarioPlayerRuntime, temple_complex_enabled)==0x4478 &&
    offsetof(PharaohScenarioPlayerRuntime, temple_complex_building_id)==0x472c,
    "native loaded scene UI fields");

static int32_t percentage(uint32_t amount, int32_t population)
{
    const int32_t value = pharaoh_scale_value_to_percent((int32_t)amount, population).value;
    return value > 100 ? 100 : value;
}

/* Native00468170; count arithmetic wraps before the signed percentage call. */
void pharaoh_city_update_service_coverage(PharaohSceneUi *c)
{
    static const uint8_t types[5] = {31,30,33,32,226};
    static const uint32_t capacity[5] = {400,700,1200,5000,7500};
    PharaohScenarioPlayerRuntime *p = &c->build_menu->supply->players[
        *c->build_menu->supply->current_player];
    PharaohCityServiceCoverage *out = c->coverage;
    const uint32_t *active = c->counts->active;
    uint32_t sum = 0;
    int32_t god_count = 0;
    for (int32_t i = 0; i < 5; ++i)
        out->entertainment[i] = percentage(active[types[i]] * capacity[i], p->population);
    for (int32_t i = 0; i < 5; ++i) {
        if (c->god_status[i] == 1 || c->god_status[i] == 2) {
            const uint32_t complex = active[pharaoh_religion_building_type_for_god(
                i, PHARAOH_GOD_BUILDING_TEMPLE_COMPLEX)];
            const uint32_t temple = active[pharaoh_religion_building_type_for_god(
                i, PHARAOH_GOD_BUILDING_TEMPLE)];
            const uint32_t shrine = active[pharaoh_religion_building_type_for_god(
                i, PHARAOH_GOD_BUILDING_SHRINE)];
            const uint32_t weight = c->god_status[i] == 2 ? 1u : 2u;
            out->gods[i] = percentage(complex * 8000u + temple * (375u * weight) +
                shrine * (150u * weight), p->population);
            ++god_count;
        } else out->gods[i] = 0;
        sum += (uint32_t)out->gods[i];
    }
    /* Possible original bug: no active gods produces x87 0/0. Keep its
     * masked result and status flags, rather than substituting integer zero. */
    out->religion = pharaoh_x87_integer_ratio((int32_t)sum, god_count);
    p->school_age_population = pharaoh_population_count_school_age(c->people_at_age);
    p->academy_age_population = pharaoh_population_count_academy_age(c->people_at_age);
    out->school = percentage(active[51] * 300u, p->school_age_population);
    out->academy = percentage(active[53] * 800u, p->population);
    pharaoh_city_culture_update_health_coverage(c->health_buildings,
        PHARAOH_BUILDING_RECORD_COUNT, &out->health_76, &out->health_78,
        &out->health_6e, &out->health_75);
}

static void coverage(void *p) {pharaoh_city_update_service_coverage(p);}
static void menu(void *p) {pharaoh_build_menu_update(((PharaohSceneUi *)p)->build_menu);}
static void sidebar(void *p) {pharaoh_sidebar_update_build_buttons(((PharaohSceneUi *)p)->build_menu);}

void pharaoh_saved_game_bind_ui_steps(
    PharaohSavedGameInitializeOperations *ops, PharaohSceneUi *c)
{
    ops->step[PHARAOH_INIT_CULTURE_COVERAGE] = (PharaohSavedGameStepBinding){c, coverage};
    ops->step[PHARAOH_INIT_BUILDING_MENU] = (PharaohSavedGameStepBinding){c, menu};
    ops->step[PHARAOH_INIT_SIDEBAR] = (PharaohSavedGameStepBinding){c, sidebar};
}
