#include "pharaoh/scenario/saved_event_upgrade.h"

static int16_t expanded_resource(int16_t value)
{
    return value > 30 ? (int16_t)((uint16_t)value + 5u) : value;
}

void pharaoh_saved_events_upgrade_resources(const int16_t *count,
    PharaohScenarioEvent *events, int32_t prices[5][2],
    const int32_t default_prices[5][2])
{
    const int32_t size = *count;
    for (int32_t i = 0; i < size; ++i) {
        PharaohScenarioEvent *const event = &events[i];
        if (!pharaoh_event_type_uses_resource(event)) continue;
        PharaohScenarioValue *const value = &event->item;
        value->value = expanded_resource(value->value);
        int16_t fixed = expanded_resource(value->fixed);
        int16_t minimum = expanded_resource(value->min);
        int16_t maximum = expanded_resource(value->max);
        int16_t last = maximum;
        if (fixed < 0) {
            if (minimum < 0) {
                if (maximum >= 0) { last = -1; fixed = maximum; }
                goto check_minimum;
            }
            fixed = minimum;
            minimum = -1;
        } else {
check_minimum:
            maximum = last;
            if (minimum >= 0) goto store;
        }
        last = maximum;
        if (maximum >= 0) { last = -1; minimum = maximum; }
store:
        value->fixed = fixed;
        value->min = minimum;
        value->max = last;
    }
    for (int32_t i = 0; i < 5; ++i) {
        const int32_t sell = default_prices[i][1];
        prices[i][0] = default_prices[i][0];
        prices[i][1] = sell;
    }
}
