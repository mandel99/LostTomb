#ifndef PHARAOH_UI_MESSAGE_TOKENS_H
#define PHARAOH_UI_MESSAGE_TOKENS_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

enum PharaohMessagePhraseSlot {
    PHARAOH_MESSAGE_PHRASE_CITY = 3,
    PHARAOH_MESSAGE_PHRASE_AMOUNT = 4,
    PHARAOH_MESSAGE_PHRASE_ITEM = 5,
    PHARAOH_MESSAGE_PHRASE_MONTHS_TO_COMPLY = 6,
    PHARAOH_MESSAGE_PHRASE_MONTHS_LEFT = 7,
    PHARAOH_MESSAGE_PHRASE_TEXT = 8,
    PHARAOH_MESSAGE_PHRASE_REASON_CITY = 9,
    PHARAOH_MESSAGE_PHRASE_DELIVERED_AMOUNT = 10,
    PHARAOH_MESSAGE_PHRASE_REASON_AMOUNT = 12,
    PHARAOH_MESSAGE_PHRASE_REASON_ITEM = 13,
    PHARAOH_MESSAGE_PHRASE_VARIANTS = 14,
    PHARAOH_MESSAGE_PHRASE_COUNT = 15
};

enum PharaohTextGroup {
    PHARAOH_TEXT_GROUP_AMOUNTS = 8,
    PHARAOH_TEXT_GROUP_CITY_NAMES_CLASSICAL = 21,
    PHARAOH_TEXT_GROUP_RESOURCE_NAMES = 23,
    PHARAOH_TEXT_GROUP_PLAYER_RANKS = 32,
    PHARAOH_TEXT_GROUP_ENEMY_NAMES = 37,
    PHARAOH_TEXT_GROUP_PHARAOH_NAMES = 151,
    PHARAOH_TEXT_GROUP_PLAYER_INCARNATIONS = 152,
    PHARAOH_TEXT_GROUP_GOD_NAMES = 157,
    PHARAOH_TEXT_GROUP_CITY_NAMES = 195
};

enum PharaohMessageResource {
    PHARAOH_MESSAGE_RESOURCE_WEAPONS = 10,
    PHARAOH_MESSAGE_RESOURCE_STONE = 24,
    PHARAOH_MESSAGE_RESOURCE_LIMESTONE = 25,
    PHARAOH_MESSAGE_RESOURCE_GRANITE = 26,
    PHARAOH_MESSAGE_RESOURCE_CHARIOTS = 28,
    PHARAOH_MESSAGE_RESOURCE_SANDSTONE = 30,
    PHARAOH_MESSAGE_RESOURCE_MARBLE = 35,
    PHARAOH_MESSAGE_RESOURCE_DEBEN = 36
};

typedef struct PharaohTextBuffer {
    char *data;
    size_t capacity;
} PharaohTextBuffer;

typedef struct PharaohMessageRecord {
    int16_t phrases[PHARAOH_MESSAGE_PHRASE_COUNT];
} PharaohMessageRecord;

typedef const char *(*PharaohTextLookupFunction)(
    void *user_data, int32_t group, int32_t entry);
typedef void (*PharaohEventTextFormatFunction)(
    void *user_data, int32_t text_id, char *buffer, size_t capacity,
    bool use_alternate);
typedef const void *(*PharaohNextInvasionFunction)(
    void *user_data, const void *previous_event);
typedef bool (*PharaohEventRunningFunction)(
    void *user_data, const void *event);
typedef int32_t (*PharaohCityNameIdFunction)(
    void *user_data, int32_t city_index);

typedef struct PharaohMessageTokenContext {
    const PharaohMessageRecord *message;
    PharaohTextLookupFunction lookup_text;
    PharaohEventTextFormatFunction format_event_text;
    PharaohNextInvasionFunction next_invasion;
    PharaohEventRunningFunction event_is_running;
    PharaohCityNameIdFunction city_name_id;
    void *user_data;
    const char *player_name;
    const char *player_title_format;
    int32_t player_rank;
    int32_t player_title_id;
    int32_t pharaoh_name_id;
    int16_t enemy_type;
    bool classical_city_names;
    bool use_alternate;
    PharaohTextBuffer title_buffer;
    PharaohTextBuffer amount_buffer;
    PharaohTextBuffer request_amount_buffer;
    PharaohTextBuffer time_allotted_buffer;
    PharaohTextBuffer travel_time_buffer;
    PharaohTextBuffer time_until_attack_buffer;
    PharaohTextBuffer reason_phrase_buffer;
} PharaohMessageTokenContext;

/* Original addresses 0x00446610 and 0x004479B0. */
int32_t pharaoh_message_invasion_running(
    const PharaohMessageTokenContext *context);
const char *pharaoh_message_token_request_amount(
    PharaohMessageTokenContext *context);

/* Original addresses 0x00447800 through 0x00447A30. */
const char *pharaoh_message_token_player_rank(
    const PharaohMessageTokenContext *context);
const char *pharaoh_message_token_player_name(
    PharaohMessageTokenContext *context);
const char *pharaoh_message_token_city_name(
    const PharaohMessageTokenContext *context);
const char *pharaoh_message_token_amount(PharaohMessageTokenContext *context);
const char *pharaoh_message_token_resource_name(
    const PharaohMessageTokenContext *context);

/* Original addresses 0x00447A80 through 0x00447BB0. */
const char *pharaoh_message_token_time_allotted(
    PharaohMessageTokenContext *context);
const char *pharaoh_message_token_travel_time(
    PharaohMessageTokenContext *context);
const char *pharaoh_message_token_time_until_attack(
    PharaohMessageTokenContext *context);
const char *pharaoh_message_token_reason_phrase(
    PharaohMessageTokenContext *context);
const char *pharaoh_message_token_pharaoh_name(
    const PharaohMessageTokenContext *context);
const char *pharaoh_message_token_foreign_army(
    const PharaohMessageTokenContext *context);
const char *pharaoh_message_token_god(
    const PharaohMessageTokenContext *context);

#endif
