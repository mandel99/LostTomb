#ifndef PHARAOH_UI_TOKEN_EXPANSION_H
#define PHARAOH_UI_TOKEN_EXPANSION_H

#include <stddef.h>
#include <stdint.h>

typedef struct PharaohTokenExpansionEntry PharaohTokenExpansionEntry;

typedef const char *(*PharaohTokenExpansionResolve)(
    void *user_data, const PharaohTokenExpansionEntry *entry);

struct PharaohTokenExpansionEntry {
    const char *name;
    PharaohTokenExpansionResolve resolve;
};

typedef struct PharaohTokenExpansionOperations {
    void *user_data;
    int32_t (*compare_n)(void *user_data, const char *left,
        const char *right, size_t count);
} PharaohTokenExpansionOperations;

/* Complete typed translation of native 0x004CB980. Returns one after copying
   the source terminator, or zero for an unknown/unterminated token or when the
   destination capacity is exhausted. */
int32_t pharaoh_token_expand(
    const PharaohTokenExpansionEntry *entries,
    const PharaohTokenExpansionOperations *operations,
    const char *source, char *destination, int32_t capacity);

void pharaoh_event_text_expand(
    const PharaohTokenExpansionEntry *entries,
    const PharaohTokenExpansionOperations *operations,
    const char *source, char *destination, int32_t capacity);
#endif
