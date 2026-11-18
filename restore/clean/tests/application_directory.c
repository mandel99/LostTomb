#include "pharaoh/runtime/application_startup.h"

#include <stdio.h>
#include <string.h>

typedef struct DirectoryFixture {
    const char *path;
    uint32_t returned_length;
    uint32_t observed_capacity;
} DirectoryFixture;

static uint32_t get_current_directory(void *user_data,
                                      uint32_t buffer_capacity,
                                      char *buffer)
{
    DirectoryFixture *fixture = (DirectoryFixture *)user_data;
    fixture->observed_capacity = buffer_capacity;
    if (fixture->returned_length != 0 && fixture->path != NULL &&
        fixture->returned_length < buffer_capacity) {
        memcpy(buffer, fixture->path,
            (size_t)fixture->returned_length + 1);
    }
    return fixture->returned_length;
}

static int check_case(const char *path, uint32_t returned_length,
                      const char *expected_path, uint32_t expected_errno)
{
    DirectoryFixture fixture = { path, returned_length, 0 };
    PharaohApplicationDirectoryOperations operations = {
        &fixture, get_current_directory
    };
    PharaohApplicationDirectoryState state;
    char previous_path[PHARAOH_APPLICATION_DIRECTORY_BYTES];
    memset(&state, 0xa5, sizeof(state));
    memset(previous_path, 0xa5, sizeof(previous_path));
    state.runtime_errno = UINT32_C(0x12345678);

    pharaoh_application_capture_current_directory(&state, &operations);
    if (fixture.observed_capacity !=
            PHARAOH_APPLICATION_DIRECTORY_QUERY_BYTES ||
        state.runtime_errno != expected_errno) {
        return 0;
    }
    if (expected_path != NULL) {
        return strcmp(state.path, expected_path) == 0;
    }
    return memcmp(state.path, previous_path, sizeof(previous_path)) == 0;
}

int main(void)
{
    char maximum_fitting_path[PHARAOH_APPLICATION_DIRECTORY_BYTES];
    char too_long_path[PHARAOH_APPLICATION_DIRECTORY_BYTES + 1];
    memset(maximum_fitting_path, 'x', sizeof(maximum_fitting_path) - 1);
    maximum_fitting_path[sizeof(maximum_fitting_path) - 1] = '\0';
    memset(too_long_path, 'x', sizeof(too_long_path) - 1);
    too_long_path[sizeof(too_long_path) - 1] = '\0';

    if (!check_case("C:\\Pharaoh", 10, "C:\\Pharaoh", 0x12345678) ||
        !check_case(maximum_fitting_path, 99, maximum_fitting_path,
            0x12345678) ||
        !check_case(too_long_path, 100, NULL,
            PHARAOH_APPLICATION_DIRECTORY_RANGE_ERROR) ||
        !check_case(NULL, 0, NULL, 0x12345678) ||
        !check_case(NULL, PHARAOH_APPLICATION_DIRECTORY_QUERY_BYTES, NULL,
            0x12345678)) {
        fputs("application directory behavior mismatch\n", stderr);
        return 1;
    }
    return 0;
}
