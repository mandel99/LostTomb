#include "pharaoh/editor/event_buttons.h"

enum {
    FLAG_CHAIN_ONLY = 1,
    FLAG_RECURRING = 2,
    FLAG_BY_FAVOUR = 0x10,
    FESTIVAL_GODS = 5,
    EVENT_REASONS = 7,
    ATTACK_TARGETS = 5,
    MONTHS_PER_YEAR = 12,
    ITEM_SLOTS = 3,
    TYPE_STEP = 75,
    TYPE_Y_OFFSET = 50,
    TYPE_ROWS = 15,
    SUBTYPE_X = 417,
    SUBTYPE_Y = 138,
    REASON_X = 150,
    REASON_Y = 200,
    ITEM_STEP = 50,
    ITEM_TWO_COLUMNS = 16,
    ROWS_AUTO = -1,
    DIALOG_X = 200,
    DIALOG_Y = 40,
    GROUP_MONTHS_SHORT = 25,
    GROUP_EVENT_TYPES = 156,
    GROUP_GOD_NAMES = 157,
    GROUP_EVENT_REASONS = 299,
    LINK_NONE_TYPED = 999,
    LINK_COMPLETED = 0,
    LINK_REFUSED = 1,
    LINK_TOO_LATE = 2,
    LINK_DEFEAT = 3
};

static bool has_link(const PharaohEventEditorContext *c,
    const PharaohScenarioEvent *e, int32_t link);

static bool selectable_type(int32_t type, uint8_t mummy_allowed)
{
    if (type == PHARAOH_EVENT_NONE || type == PHARAOH_EVENT_EARTHQUAKE ||
        type == PHARAOH_EVENT_REVOLT || type == PHARAOH_EVENT_CHANGE_PHARAOH)
        return false;
    return type != PHARAOH_EVENT_MUMMY || mummy_allowed != 0u;
}

static void open_list(PharaohEventEditorContext *c, int32_t x, int32_t y,
    int32_t count, int32_t group, bool keep,
    enum PharaohEditorChoiceHandler handler, int32_t rows)
{
    c->open_list(c->user_data, x, y, count, group, keep, handler, rows,
        &c->editor_value);
}

void pharaoh_editor_event_type_chosen(PharaohEventEditorContext *c)
{
    PharaohScenarioEvent *e = c->selected_event;
    int16_t type = c->editor_value;
    if (e != NULL && type >= 0 && type < PHARAOH_EVENT_TYPE_COUNT &&
        e->type != (uint8_t)type) c->set_type(c->user_data, e, type);
}

void pharaoh_editor_event_phrase_chosen(PharaohEventEditorContext *c)
{
    if (c->selected_event != NULL && c->editor_phrase < FESTIVAL_GODS)
        c->selected_event->phrase_variant = c->editor_phrase;
}

void pharaoh_editor_event_type_button(PharaohEventEditorContext *c)
{
    PharaohScenarioEvent *e = c->selected_event;
    int32_t type, count = 0;
    if (e == NULL) return;
    c->editor_value = e->type;
    for (type = 0; type < PHARAOH_EVENT_TYPE_COUNT; ++type)
        if (selectable_type(type, c->mummy_allowed)) c->select_items[count++] = type;
    open_list(c, c->display_mode * TYPE_STEP + TYPE_STEP,
        c->display_mode * TYPE_STEP - TYPE_Y_OFFSET, count, GROUP_EVENT_TYPES,
        true, PHARAOH_EDITOR_CHOOSE_TYPE, TYPE_ROWS);
    c->redraw_requested = true;
}

static int32_t first_resource(const PharaohEventEditorContext *c,
    const PharaohScenarioEvent *e)
{
    int32_t resource;
    for (resource = c->resource_min(c->user_data, e);
         resource < c->resource_max(c->user_data, e); ++resource)
        if (c->resource_in_range(c->user_data, e, resource)) return resource;
    return 0;
}

void pharaoh_editor_event_subtype_chosen(PharaohEventEditorContext *c)
{
    PharaohScenarioEvent *e = c->selected_event;
    int16_t subtype = c->editor_value;
    int16_t items[ITEM_SLOTS];
    int32_t first, i;
    if (e == NULL || subtype < 0 || subtype >= c->subtype_count(c->user_data, e) ||
        e->subtype == (uint8_t)subtype) return;
    e->subtype = (uint8_t)subtype;
    if (!c->shows_resource(c->user_data, e)) return;
    first = first_resource(c, e);
    items[0] = e->item_value; items[1] = e->item_min; items[2] = e->item_max;
    for (i = 0; i < ITEM_SLOTS; ++i)
        if (!c->resource_in_range(c->user_data, e, items[i]))
            items[i] = (int16_t)(i == 0 ? first : -1);
    e->item_value = items[0]; e->item_min = items[1]; e->item_max = items[2];
}

void pharaoh_editor_event_subtype_button(PharaohEventEditorContext *c)
{
    PharaohScenarioEvent *e = c->selected_event;
    if (e == NULL || !c->has_subtype(c->user_data, e)) return;
    c->editor_value = e->subtype;
    open_list(c, SUBTYPE_X, SUBTYPE_Y, c->subtype_count(c->user_data, e),
        c->subtype_group(c->user_data, e), false,
        PHARAOH_EDITOR_CHOOSE_SUBTYPE, ROWS_AUTO);
    c->redraw_requested = true;
}

void pharaoh_editor_event_month_chosen(PharaohEventEditorContext *c)
{
    if (c->selected_event != NULL) c->selected_event->month = (uint8_t)c->editor_value;
}

void pharaoh_editor_event_month_button(PharaohEventEditorContext *c)
{
    PharaohScenarioEvent *e = c->selected_event;
    if (e == NULL || (e->flags & FLAG_BY_FAVOUR) != 0u) return;
    open_list(c, DIALOG_X, DIALOG_Y, MONTHS_PER_YEAR, GROUP_MONTHS_SHORT,
        false, PHARAOH_EDITOR_CHOOSE_MONTH, ROWS_AUTO);
    c->redraw_requested = true;
}

static void open_number(PharaohEventEditorContext *c,
    enum PharaohEditorNumericHandler handler)
{
    c->open_numeric_input(c->user_data, DIALOG_X, DIALOG_Y,
        &c->editor_value, handler);
}

static void range_button(PharaohEventEditorContext *c, bool is_year,
    enum PharaohEditorNumericHandler handler)
{
    PharaohScenarioEvent *e = c->selected_event;
    if (e == NULL || (is_year && (e->flags & FLAG_BY_FAVOUR) != 0u)) {
        return;
    }
    c->editor_field = (int16_t)c->button_parameter;
    open_number(c, handler);
    c->redraw_requested = true;
}

void pharaoh_editor_event_year_button(PharaohEventEditorContext *c)
{
    range_button(c, true, PHARAOH_EDITOR_NUMBER_YEAR);
}

void pharaoh_editor_event_amount_button(PharaohEventEditorContext *c)
{
    range_button(c, false, PHARAOH_EDITOR_NUMBER_AMOUNT);
}

void pharaoh_editor_event_invader_button(PharaohEventEditorContext *c)
{
    range_button(c, false, PHARAOH_EDITOR_NUMBER_INVADER);
}

void pharaoh_editor_event_route_button(PharaohEventEditorContext *c)
{
    range_button(c, false, PHARAOH_EDITOR_NUMBER_ROUTE);
}

void pharaoh_editor_event_link_button(PharaohEventEditorContext *c)
{
    PharaohScenarioEvent *e = c->selected_event;
    const int32_t link = c->button_parameter;
    if (e == NULL || link < LINK_COMPLETED || link > LINK_DEFEAT ||
        !has_link(c, e, link)) {
        return;
    }
    c->editor_field = (int16_t)link;
    c->editor_value = LINK_NONE_TYPED;
    open_number(c, PHARAOH_EDITOR_NUMBER_LINK);
    c->redraw_requested = true;
}

void pharaoh_editor_event_trigger_button(PharaohEventEditorContext *c)
{
    PharaohScenarioEvent *e = c->selected_event;
    if (e == NULL) return;
    if (e->flags == 0u) e->flags = FLAG_RECURRING;
    else if ((e->flags & FLAG_RECURRING) != 0u) e->flags = FLAG_CHAIN_ONLY;
    else if ((e->flags & FLAG_CHAIN_ONLY) != 0u)
        e->flags = e->type == PHARAOH_EVENT_INVASION ? FLAG_BY_FAVOUR : 0u;
    else e->flags = 0u;
    c->redraw_requested = true;
}

void pharaoh_editor_event_sender_button(PharaohEventEditorContext *c)
{
    if (c->selected_event == NULL) return;
    c->selected_event->sender_faction = c->selected_event->sender_faction == 0u;
    c->redraw_requested = true;
}

void pharaoh_editor_event_months_button(PharaohEventEditorContext *c)
{
    if (c->selected_event == NULL ||
        !pharaoh_event_has_months(c->selected_event)) {
        return;
    }
    open_number(c, PHARAOH_EDITOR_NUMBER_MONTHS);
}

void pharaoh_editor_event_phrase_button(PharaohEventEditorContext *c)
{
    PharaohScenarioEvent *e = c->selected_event;
    if (e == NULL) {
        return;
    }
    if (pharaoh_editor_event_has_phrase_number(e)) {
        open_number(c, PHARAOH_EDITOR_NUMBER_PHRASE);
        return;
    }
    if (pharaoh_editor_event_has_phrase_list(e)) {
        c->open_phrase_list(c->user_data, DIALOG_X, DIALOG_Y,
            FESTIVAL_GODS, GROUP_GOD_NAMES, &c->editor_phrase,
            PHARAOH_EDITOR_CHOOSE_PHRASE);
    }
}

void pharaoh_editor_event_attack_target_button(PharaohEventEditorContext *c)
{
    PharaohScenarioEvent *e = c->selected_event;
    if (e == NULL || e->type != PHARAOH_EVENT_INVASION) return;
    if (++e->attack_target >= ATTACK_TARGETS) e->attack_target = 0;
    c->redraw_requested = true;
}

static bool has_link(const PharaohEventEditorContext *c,
    const PharaohScenarioEvent *e, int32_t link)
{
    if (e == NULL) return false;
    if (link == LINK_COMPLETED) return c->has_chain(c->user_data, e) != 0;
    if (link == LINK_DEFEAT) return c->has_followup(c->user_data, e) != 0;
    return c->type_chains(c->user_data, e) != 0;
}

static void reason_chosen(PharaohEventEditorContext *c, int32_t link)
{
    if (c->selected_event != NULL)
        c->selected_event->reason[link] = (uint8_t)c->editor_value;
}

void pharaoh_editor_event_reason_completed_chosen(PharaohEventEditorContext *c)
{ reason_chosen(c, LINK_COMPLETED); }
void pharaoh_editor_event_reason_refused_chosen(PharaohEventEditorContext *c)
{ reason_chosen(c, LINK_REFUSED); }
void pharaoh_editor_event_reason_too_late_chosen(PharaohEventEditorContext *c)
{ reason_chosen(c, LINK_TOO_LATE); }
void pharaoh_editor_event_reason_defeat_chosen(PharaohEventEditorContext *c)
{ reason_chosen(c, LINK_DEFEAT); }

static void reason_button(PharaohEventEditorContext *c, int32_t link,
    enum PharaohEditorChoiceHandler handler)
{
    if (!has_link(c, c->selected_event, link)) return;
    open_list(c, REASON_X, REASON_Y, EVENT_REASONS, GROUP_EVENT_REASONS,
        false, handler, ROWS_AUTO);
}

void pharaoh_editor_event_reason_completed_button(PharaohEventEditorContext *c)
{ reason_button(c, LINK_COMPLETED, PHARAOH_EDITOR_CHOOSE_REASON_COMPLETED); }
void pharaoh_editor_event_reason_refused_button(PharaohEventEditorContext *c)
{ reason_button(c, LINK_REFUSED, PHARAOH_EDITOR_CHOOSE_REASON_REFUSED); }
void pharaoh_editor_event_reason_too_late_button(PharaohEventEditorContext *c)
{ reason_button(c, LINK_TOO_LATE, PHARAOH_EDITOR_CHOOSE_REASON_TOO_LATE); }
void pharaoh_editor_event_reason_defeat_button(PharaohEventEditorContext *c)
{ reason_button(c, LINK_DEFEAT, PHARAOH_EDITOR_CHOOSE_REASON_DEFEAT); }

void pharaoh_editor_event_item_chosen(PharaohEventEditorContext *c)
{
    PharaohScenarioEvent *e = c->selected_event;
    int16_t items[ITEM_SLOTS], field;
    if (e == NULL) return;
    if (c->resource_min(c->user_data, e) > 0 &&
        c->editor_value < c->resource_min(c->user_data, e)) c->editor_value = -1;
    field = c->editor_field;
    if (field < 0 || field >= ITEM_SLOTS) return;
    items[0] = e->item_value; items[1] = e->item_min; items[2] = e->item_max;
    items[field] = c->editor_value;
    e->item_value = items[0]; e->item_min = items[1]; e->item_max = items[2];
}

void pharaoh_editor_event_item_button(PharaohEventEditorContext *c)
{
    PharaohScenarioEvent *e = c->selected_event;
    int32_t count, rows, parameter;
    if (e == NULL) return;
    count = c->resource_list(c->user_data, e, c->select_items);
    rows = count >= ITEM_TWO_COLUMNS ? (count + 1) / 2 : count;
    parameter = c->button_parameter;
    c->editor_field = (int16_t)parameter;
    if (count > 1) {
        int32_t at = c->display_mode * ITEM_STEP;
        open_list(c, at, at, count, c->resource_group(c->user_data, e), true,
            PHARAOH_EDITOR_CHOOSE_ITEM, rows);
    } else if (count == 1) {
        c->editor_value = (int16_t)(parameter == 0 ? c->select_items[0] : 0);
        pharaoh_editor_event_item_chosen(c);
    }
}

static void range_pack(PharaohScenarioValue *range, int16_t first,
    int16_t second, int16_t third)
{
    if (first < 0) {
        if (second >= 0) {
            first = second;
            second = -1;
        } else if (third >= 0) {
            first = third;
            third = -1;
        }
    }
    if (second < 0 && third >= 0) {
        second = third;
        third = -1;
    }
    range->fixed = first;
    range->min = second;
    range->max = third;
}

static void range_store(PharaohScenarioValue *range, int16_t fixed,
    int16_t minimum, int16_t maximum)
{
    range->fixed = fixed;
    range->min = minimum;
    range->max = maximum;
}

static void edit_range(PharaohEventEditorContext *c,
    PharaohScenarioValue *range)
{
    int16_t low;
    int16_t high;
    const int16_t value = c->editor_value;
    if (range->fixed < 0) {
        low = range->min;
        high = range->max;
    } else if (range->min < 0) {
        low = range->fixed;
        high = range->fixed;
    } else {
        low = -1;
        high = -1;
    }
    if (c->editor_field == 1) {
        if (low > value) low = value;
        if (low < 0 || low == value) {
            range_store(range, value, -1, -1);
        } else if (value < 0) {
            range_pack(range, low, -1, -1);
        } else if (low > value) {
            range_store(range, -1, value, low);
        } else {
            range_store(range, -1, low, value);
        }
    } else if (c->editor_field == 0) {
        if (high < value) high = value;
        if (value < 0 || high == value) {
            range_store(range, high, -1, -1);
        } else if (high < 0) {
            range_store(range, value, -1, -1);
        } else if (value > high) {
            range_store(range, -1, high, value);
        } else {
            range_store(range, -1, value, high);
        }
    }
}

void pharaoh_editor_event_year_completed(PharaohEventEditorContext *c)
{
    if (c->selected_event != NULL) edit_range(c, &c->selected_event->year);
}

void pharaoh_editor_event_amount_completed(PharaohEventEditorContext *c)
{
    if (c->selected_event != NULL) edit_range(c, &c->selected_event->amount);
}

void pharaoh_editor_event_invader_completed(PharaohEventEditorContext *c)
{
    if (c->selected_event != NULL) edit_range(c, &c->selected_event->invasion);
}

void pharaoh_editor_event_route_completed(PharaohEventEditorContext *c)
{
    if (c->selected_event != NULL) edit_range(c, &c->selected_event->route);
}

void pharaoh_editor_event_link_completed(PharaohEventEditorContext *c)
{
    PharaohScenarioEvent *e = c->selected_event;
    if (e == NULL) return;
    switch (c->editor_field) {
        case LINK_COMPLETED: e->on_completed = c->editor_value; break;
        case LINK_REFUSED: e->on_refusal = c->editor_value; break;
        case LINK_TOO_LATE: e->on_too_late = c->editor_value; break;
        case LINK_DEFEAT: e->on_defeat = c->editor_value; break;
        default: break;
    }
}

void pharaoh_editor_event_months_completed(PharaohEventEditorContext *c)
{
    if (c->selected_event != NULL) {
        c->selected_event->months_initial = c->editor_value;
    }
}

void pharaoh_editor_event_phrase_completed(PharaohEventEditorContext *c)
{
    if (c->selected_event != NULL) {
        c->selected_event->attack_target =
            (int16_t)((c->selected_event->attack_target & 0xff00) |
                (uint16_t)(uint8_t)c->editor_value);
    }
}
