#include "pharaoh/ui/token_expansion.h"

int32_t pharaoh_token_expand(
    const PharaohTokenExpansionEntry *entries,
    const PharaohTokenExpansionOperations *operations,
    const char *source, char *destination, int32_t capacity)
{
    int32_t source_index = 0;
    int32_t destination_index = 0;

    for (;;) {
        char value = source[source_index];
        while (value != '[') {
            if (destination_index >= capacity) return 0;
            destination[destination_index] = value;
            if (value == '\0') return 1;
            ++source_index;
            ++destination_index;
            value = source[source_index];
        }

        const int32_t token_start = source_index + 1;
        do {
            ++source_index;
            value = source[source_index];
            if (value == '\0') return 0;
        } while (value != ']');

        const size_t token_length =
            (size_t)(source_index - token_start);
        const PharaohTokenExpansionEntry *entry = entries;
        while (entry->name != NULL &&
               operations->compare_n(operations->user_data, entry->name,
                   source + token_start, token_length) != 0) {
            ++entry;
        }
        if (entry->name == NULL) return 0;

        const char *replacement = entry->resolve(
            operations->user_data, entry);
        if (replacement != NULL) {
            while (*replacement != '\0') {
                if (destination_index >= capacity) return 0;
                destination[destination_index++] = *replacement++;
            }
        }
        ++source_index;
    }
}

void pharaoh_event_text_expand(
    const PharaohTokenExpansionEntry *entries,
    const PharaohTokenExpansionOperations *operations,
    const char *source, char *destination, int32_t capacity)
{
    destination[0] = '\0';
    (void)pharaoh_token_expand(
        entries, operations, source, destination, capacity);
}
