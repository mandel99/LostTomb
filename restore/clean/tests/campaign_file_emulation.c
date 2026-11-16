#include "pharaoh/scenario/campaign_file.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum {
    CAMPAIGN_OBJECT_NATIVE_ADDRESS = 0x6fdd08,
    CAMPAIGN_FIXTURE_CAPACITY = 65536,
    CAMPAIGN_ERROR_CAPACITY = 1024,
    CAMPAIGN_NATIVE_SHARE_FLAGS = 0x40
};

static PharaohCampaign campaign;
static uint8_t fixture[CAMPAIGN_FIXTURE_CAPACITY];
static size_t fixture_length;
static size_t fixture_position;
static uint32_t open_count;
static uint32_t close_count;
static uint32_t error_hashes[CAMPAIGN_ERROR_CAPACITY];
static uint32_t error_count;
static uint32_t fixture_available;
static uint32_t protocol_error;

static const uint8_t fixture_stream_identity;

static uint32_t hash_message(const char *message)
{
    uint32_t hash = UINT32_C(2166136261);
    while (*message != '\0') {
        hash = (hash ^ (uint8_t)*message++) * UINT32_C(16777619);
    }
    return hash;
}

static void *open_fixture(void *user_data, const char *filename,
                          const char *mode, uint32_t share_flags)
{
    (void)user_data;
    ++open_count;
    if (strcmp(filename, "fixture.txt") != 0 || strcmp(mode, "rb") != 0 ||
        share_flags != CAMPAIGN_NATIVE_SHARE_FLAGS) {
        protocol_error = 1;
    }
    return fixture_available != 0 ? (void *)&fixture_stream_identity : NULL;
}

static char *read_fixture_line(void *user_data, char *buffer,
                               int32_t buffer_capacity, void *stream)
{
    (void)user_data;
    if (stream != (void *)&fixture_stream_identity || buffer_capacity != 200) {
        protocol_error = 1;
        return NULL;
    }
    if (fixture_position == fixture_length) {
        return NULL;
    }

    size_t copied = 0;
    while (copied + 1 < (size_t)buffer_capacity &&
           fixture_position < fixture_length) {
        const uint8_t value = fixture[fixture_position++];
        buffer[copied++] = (char)value;
        if (value == '\n') {
            break;
        }
    }
    buffer[copied] = '\0';
    return buffer;
}

static int32_t close_fixture(void *user_data, void *stream)
{
    (void)user_data;
    if (stream != (void *)&fixture_stream_identity) {
        protocol_error = 1;
    }
    ++close_count;
    return 0;
}

static void record_message(void *user_data, const char *message,
                           int32_t first_argument, int32_t second_argument)
{
    (void)user_data;
    if (first_argument != 0 || second_argument != 0 ||
        error_count >= CAMPAIGN_ERROR_CAPACITY) {
        protocol_error = 1;
        return;
    }
    error_hashes[error_count++] = hash_message(message);
}

static const PharaohCampaignFileOperations file_operations = {
    NULL,
    open_fixture,
    read_fixture_line,
    close_fixture,
    record_message
};

static void print_hex(const void *data, size_t length)
{
    const uint8_t *bytes = (const uint8_t *)data;
    putchar('"');
    for (size_t i = 0; i < length; ++i) {
        printf("%02x", bytes[i]);
    }
    putchar('"');
}

static int parse_fixture_request(void)
{
    unsigned int seed;
    unsigned int available;
    unsigned int byte_count;
    static char encoded[CAMPAIGN_FIXTURE_CAPACITY * 2 + 1];

    while (scanf("%u %u %u", &seed, &available, &byte_count) == 3) {
        if (byte_count > sizeof(fixture) ||
            (byte_count != 0 &&
             scanf("%131072s", encoded) != 1)) {
            return 1;
        }
        if (byte_count != 0 && strlen(encoded) != (size_t)byte_count * 2) {
            return 1;
        }
        for (unsigned int i = 0; i < byte_count; ++i) {
            unsigned int value;
            if (sscanf(encoded + i * 2, "%2x", &value) != 1) {
                return 1;
            }
            fixture[i] = (uint8_t)value;
        }

        fixture_length = byte_count;
        fixture_position = 0;
        fixture_available = available;
        open_count = 0;
        close_count = 0;
        error_count = 0;
        protocol_error = 0;
        memset(&campaign, (uint8_t)seed, sizeof(campaign));
        const int32_t result = pharaoh_campaign_load(&campaign, "fixture.txt",
            &file_operations);
        if (protocol_error != 0) {
            return 1;
        }

        printf("{\"result\":%d,\"position\":%u,\"opens\":%u,"
               "\"closes\":%u,\"object\":",
            result, (unsigned int)fixture_position, open_count, close_count);
        print_hex(&campaign, sizeof(campaign));
        printf(",\"errors\":[");
        for (uint32_t i = 0; i < error_count; ++i) {
            printf("%s%u", i == 0 ? "" : ",", error_hashes[i]);
        }
        puts("]}");
        fflush(stdout);
    }
    return 0;
}

static int run_constructor_cases(void)
{
    unsigned int seed;
    puts("{\"object\":7331080,\"bytes\":19076}");
    fflush(stdout);
    while (scanf("%u", &seed) == 1) {
        memset(&campaign, (uint8_t)seed, sizeof(campaign));
        const int result =
            pharaoh_campaign_initialize(&campaign) == &campaign;
        printf("{\"result\":%d,\"object\":", result);
        print_hex(&campaign, sizeof(campaign));
        puts("}");
        fflush(stdout);
    }
    return 0;
}

int main(int argc, char **argv)
{
    if (argc == 2 && strcmp(argv[1], "--constructor-batch") == 0) {
        return run_constructor_cases();
    }
    if (argc == 2 && strcmp(argv[1], "--oracle-batch") == 0) {
        puts("{\"object\":7331080,\"bytes\":19076}");
        fflush(stdout);
        return parse_fixture_request();
    }
    return 2;
}
