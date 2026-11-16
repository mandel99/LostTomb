#include "pharaoh/ui/message_tokens.h"

#include <stdio.h>

enum {
    LOAD_UNITS = 100,
    RANK_TEXT_FIRST = 11,
    RESOURCE_NAME_TEXT_FIRST = 54,
    FOREIGN_ARMY_TEXT_FIRST = 28,
    MESSAGE_GODS = 5
};

static bool request_amount_is_in_loads(int32_t resource)
{
    switch (resource) {
        case PHARAOH_MESSAGE_RESOURCE_STONE:
        case PHARAOH_MESSAGE_RESOURCE_LIMESTONE:
        case PHARAOH_MESSAGE_RESOURCE_GRANITE:
        case PHARAOH_MESSAGE_RESOURCE_SANDSTONE:
        case PHARAOH_MESSAGE_RESOURCE_MARBLE:
        case PHARAOH_MESSAGE_RESOURCE_CHARIOTS:
        case PHARAOH_MESSAGE_RESOURCE_WEAPONS:
        case PHARAOH_MESSAGE_RESOURCE_DEBEN:
            return false;
        default:
            return true;
    }
}

static int16_t shown_message_city(const PharaohMessageTokenContext *context)
{
    return context->message->phrases[context->use_alternate ?
        PHARAOH_MESSAGE_PHRASE_REASON_CITY : PHARAOH_MESSAGE_PHRASE_CITY];
}

static int16_t shown_message_resource(const PharaohMessageTokenContext *context)
{
    return context->message->phrases[context->use_alternate ?
        PHARAOH_MESSAGE_PHRASE_REASON_ITEM : PHARAOH_MESSAGE_PHRASE_ITEM];
}

static const char *format_number(PharaohTextBuffer buffer, int32_t value)
{
    (void) snprintf(buffer.data, buffer.capacity, "%d", value);
    return buffer.data;
}

int32_t pharaoh_message_invasion_running(
    const PharaohMessageTokenContext *context)
{
    const void *event = context->next_invasion(context->user_data, NULL);
    while (event != NULL) {
        if (context->event_is_running(context->user_data, event)) {
            return 1;
        }
        event = context->next_invasion(context->user_data, event);
    }
    return 0;
}

const char *pharaoh_message_token_request_amount(
    PharaohMessageTokenContext *context)
{
    int32_t amount =
        context->message->phrases[PHARAOH_MESSAGE_PHRASE_DELIVERED_AMOUNT];
    int32_t resource = context->message->phrases[PHARAOH_MESSAGE_PHRASE_ITEM];
    if (request_amount_is_in_loads(resource)) {
        amount *= LOAD_UNITS;
    }
    return format_number(context->request_amount_buffer, amount);
}

const char *pharaoh_message_token_player_rank(
    const PharaohMessageTokenContext *context)
{
    return context->lookup_text(context->user_data, PHARAOH_TEXT_GROUP_PLAYER_RANKS,
        context->player_rank + RANK_TEXT_FIRST);
}

const char *pharaoh_message_token_player_name(
    PharaohMessageTokenContext *context)
{
    if (context->player_title_id != 0) {
        const char *title = context->lookup_text(context->user_data,
            PHARAOH_TEXT_GROUP_PLAYER_INCARNATIONS, context->player_title_id);
        (void) snprintf(context->title_buffer.data, context->title_buffer.capacity,
            context->player_title_format, context->player_name, title);
        return context->title_buffer.data;
    }
    return context->player_name;
}

const char *pharaoh_message_token_city_name(
    const PharaohMessageTokenContext *context)
{
    int32_t group = context->classical_city_names ?
        PHARAOH_TEXT_GROUP_CITY_NAMES_CLASSICAL : PHARAOH_TEXT_GROUP_CITY_NAMES;
    int32_t name_id = context->city_name_id(
        context->user_data, shown_message_city(context));
    return context->lookup_text(context->user_data, group, name_id);
}

const char *pharaoh_message_token_amount(PharaohMessageTokenContext *context)
{
    int32_t amount = context->message->phrases[context->use_alternate ?
        PHARAOH_MESSAGE_PHRASE_REASON_AMOUNT : PHARAOH_MESSAGE_PHRASE_AMOUNT];
    if (request_amount_is_in_loads(shown_message_resource(context))) {
        amount *= LOAD_UNITS;
    }
    return format_number(context->amount_buffer, amount);
}

const char *pharaoh_message_token_resource_name(
    const PharaohMessageTokenContext *context)
{
    return context->lookup_text(context->user_data,
        PHARAOH_TEXT_GROUP_RESOURCE_NAMES,
        shown_message_resource(context) + RESOURCE_NAME_TEXT_FIRST);
}

const char *pharaoh_message_token_time_allotted(
    PharaohMessageTokenContext *context)
{
    return format_number(context->time_allotted_buffer,
        context->message->phrases[PHARAOH_MESSAGE_PHRASE_MONTHS_TO_COMPLY]);
}

const char *pharaoh_message_token_travel_time(
    PharaohMessageTokenContext *context)
{
    return format_number(context->travel_time_buffer,
        context->message->phrases[PHARAOH_MESSAGE_PHRASE_MONTHS_TO_COMPLY]);
}

const char *pharaoh_message_token_time_until_attack(
    PharaohMessageTokenContext *context)
{
    return format_number(context->time_until_attack_buffer,
        context->message->phrases[PHARAOH_MESSAGE_PHRASE_MONTHS_LEFT]);
}

const char *pharaoh_message_token_reason_phrase(
    PharaohMessageTokenContext *context)
{
    context->use_alternate = true;
    context->format_event_text(context->user_data,
        context->message->phrases[PHARAOH_MESSAGE_PHRASE_TEXT],
        context->reason_phrase_buffer.data, context->reason_phrase_buffer.capacity,
        context->use_alternate);
    context->use_alternate = false;
    return context->reason_phrase_buffer.data;
}

const char *pharaoh_message_token_pharaoh_name(
    const PharaohMessageTokenContext *context)
{
    return context->lookup_text(context->user_data,
        PHARAOH_TEXT_GROUP_PHARAOH_NAMES, context->pharaoh_name_id);
}

const char *pharaoh_message_token_foreign_army(
    const PharaohMessageTokenContext *context)
{
    return context->lookup_text(context->user_data, PHARAOH_TEXT_GROUP_ENEMY_NAMES,
        (int32_t) context->enemy_type + FOREIGN_ARMY_TEXT_FIRST);
}

const char *pharaoh_message_token_god(
    const PharaohMessageTokenContext *context)
{
    const uint8_t *variants = (const uint8_t *)
        &context->message->phrases[PHARAOH_MESSAGE_PHRASE_VARIANTS];
    uint8_t god = variants[context->use_alternate ? 1 : 0];
    if (god >= MESSAGE_GODS) {
        god = 0;
    }
    return context->lookup_text(
        context->user_data, PHARAOH_TEXT_GROUP_GOD_NAMES, god);
}
