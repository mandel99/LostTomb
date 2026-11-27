#include "pharaoh/building/housing_warnings.h"

static void actual_warning_post(void *user, int32_t message,
    int32_t unused_value, int32_t unused_extra)
{
    (void)unused_value; (void)unused_extra;
    pharaoh_city_warning_post((PharaohCityWarningContext *)user, message);
}

static PharaohConstructionWarningState warning_inputs(
    const PharaohHousingWarningContext *c)
{
    const PharaohHousingWarningCity *city=&c->cities[*c->current_player];
    PharaohConstructionWarningState state={0};
    state.editor_active=(uint8_t)(*c->editor_active==1);
    state.mission_id=(uint8_t)*c->mission_id;
    state.scenario_mode=(int32_t)*c->scenario_mode;
    state.population=city->population;
    state.unemployment_percentage=city->unemployment_percentage;
    state.labor_available=city->labor_available;
    state.labor_needed=city->labor_needed;
    state.show_warning=actual_warning_post;
    state.user_data=c->warnings;
    return state;
}

void pharaoh_housing_warning_unemployment(const PharaohHousingWarningContext *c)
{
    PharaohConstructionWarningState state=warning_inputs(c);
    pharaoh_warning_unemployment(&state);
}

void pharaoh_housing_warning_labor_need(const PharaohHousingWarningContext *c)
{
    PharaohConstructionWarningState state=warning_inputs(c);
    pharaoh_warning_labor_need(&state);
}

void pharaoh_housing_migration_post_message(
    const PharaohHousingWarningContext *c, int32_t tool)
{
    if (*c->editor_active==1) return;
    const PharaohHousingWarningCity *city=&c->cities[*c->current_player];
    if (city->population<20) return;
    uint32_t reason=city->migration_reason;
    if (*c->scenario_mode==0 && *c->mission_id<2 &&
        (reason==0 || reason==1 || reason==3 || reason==5)) return;
    uint32_t message;
    if (city->native_2740==0) {
        if ((int32_t)(city->native_4644+city->native_4634)>3) {
            pharaoh_city_warning_post(c->warnings,216); return;
        }
        if (city->native_273c>=5) {
            if (tool==0) pharaoh_city_warning_post(c->warnings,122);
            return;
        }
        if (city->native_2728!=0 || city->native_0038<=0) {
            if (tool==0) pharaoh_city_warning_post(c->warnings,123);
            return;
        }
        if (city->native_272c>=80) {
            if (tool==0) pharaoh_city_warning_post(c->warnings,122);
            return;
        }
        message=city->migration_reason+124u;
    } else {
        if (city->native_2740!=1) return;
        if (city->native_2728!=0 || city->native_0038<=0) {
            if (tool==0) pharaoh_city_warning_post(c->warnings,130);
            return;
        }
        message=city->migration_reason+131u;
    }
    if (message!=0) pharaoh_city_warning_post(c->warnings,(int32_t)message);
}
