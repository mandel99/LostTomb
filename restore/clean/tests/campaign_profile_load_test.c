#include "pharaoh/scenario/campaign_profile_load.h"

#ifdef NDEBUG
#undef NDEBUG
#endif
#include <assert.h>
#include <stdint.h>
#include <string.h>

enum {
    PROFILE_BYTES = 0x4a9c,
    PROFILE_SHORT_BYTES = 0x4a6c,
    PROFILE_PATH_OFFSET = 0x2c90,
    PROFILE_RECORDS_OFFSET = 0x2cd4,
    PROFILE_RECORD_SIZE = 0x4c,
    PROFILE_USED_OFFSET = 0x48,
    PROFILE_SUFFIX_OFFSET = 0x4a84
};

typedef struct FakeFile {
    const uint8_t *bytes;
    size_t size;
    size_t position;
} FakeFile;

static FakeFile fake_file;

static PharaohReadOnlyFile *open_file(void *user_data, const char *path)
{
    (void)user_data;
    return strcmp(path, "save\\Alice.dat") == 0 ?
        (PharaohReadOnlyFile *)&fake_file : NULL;
}

static size_t read_file(void *user_data, void *destination, size_t element_size,
    size_t element_count, PharaohReadOnlyFile *file)
{
    FakeFile *const input = (FakeFile *)file;
    const size_t requested = element_size * element_count;
    size_t available = input->size - input->position;
    size_t copied = requested < available ? requested : available;
    (void)user_data;
    memcpy(destination, input->bytes + input->position, copied);
    input->position += copied;
    return copied == requested ? element_count : 0;
}

static int32_t seek_file(void *user_data, PharaohReadOnlyFile *file,
    int32_t offset, int32_t origin)
{
    FakeFile *const input = (FakeFile *)file;
    (void)user_data;
    if (origin != 0 || offset != 0) return -1;
    input->position = 0;
    return 0;
}

static void close_file(void *user_data, PharaohReadOnlyFile *file)
{
    (void)user_data;
    (void)file;
}

static void prepare_profile(uint8_t bytes[PROFILE_BYTES])
{
    memset(bytes, 0, PROFILE_BYTES);
    memcpy(bytes + PROFILE_PATH_OFFSET, "save\\Alice", 11);
    bytes[PROFILE_RECORDS_OFFSET + PROFILE_USED_OFFSET] = 1;
    bytes[PROFILE_RECORDS_OFFSET + PROFILE_RECORD_SIZE + PROFILE_USED_OFFSET] = 2;
    memset(bytes + PROFILE_SUFFIX_OFFSET, 0x5a,
        PROFILE_BYTES - PROFILE_SUFFIX_OFFSET);
}

int main(void)
{
    static const PharaohFileReadOperations file_operations = {
        open_file, NULL, read_file, seek_file, close_file
    };
    const PharaohFileReadContext files = { &file_operations, NULL };
    uint8_t bytes[PROFILE_BYTES];
    uint16_t suffix[PHARAOH_CAMPAIGN_PROFILE_SUFFIX_WORDS];
    PharaohCampaign campaign = { 0 };

    prepare_profile(bytes);
    pharaoh_campaign_initialize(&campaign);
    campaign.mission_name_count = 2;
    fake_file = (FakeFile){ bytes, sizeof(bytes), 0 };
    assert(pharaoh_campaign_profile_load(&files, &campaign, suffix,
        "save\\Alice.dat") == 1);
    assert(strcmp(campaign.saved_game_path, "save\\Alice") == 0);
    assert(campaign.index.records[0].link.played == 1);
    assert(campaign.index.records[1].link.played == 1);
    assert(suffix[0] == UINT16_C(0x5a5a));

    memset(suffix, 0xff, sizeof(suffix));
    pharaoh_campaign_initialize(&campaign);
    campaign.mission_name_count = 2;
    fake_file = (FakeFile){ bytes, PROFILE_SHORT_BYTES, 0 };
    assert(pharaoh_campaign_profile_load(&files, &campaign, suffix,
        "save\\Alice.dat") == 1);
    assert(campaign.index.records[0].link.played == 1);
    assert(campaign.index.records[1].link.played == 1);
    assert(suffix[0] == 0);
    return 0;
}
