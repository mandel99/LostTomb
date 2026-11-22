#include "pharaoh/core/memory_strings.h"

#include <stdint.h>
#include <string.h>

static uint32_t load_word(const void *source)
{
    uint32_t word;
    memcpy(&word, source, sizeof(word));
    return word;
}

static void store_word(void *destination, uint32_t word)
{
    memcpy(destination, &word, sizeof(word));
}

void *pharaoh_memory_set(void *destination, int value, size_t count)
{
    unsigned char *output = destination;
    const uint32_t byte = (unsigned char)value;
    const uint32_t word = byte * 0x01010101u;
    while (count != 0u && ((uintptr_t)output & 3u) != 0u) {
        *output++ = (unsigned char)byte;
        --count;
    }
    while (count >= 4u) {
        store_word(output, word);
        output += 4;
        count -= 4;
    }
    while (count != 0u) {
        *output++ = (unsigned char)byte;
        --count;
    }
    return destination;
}

char *pharaoh_string_copy_bounded(char *destination, const char *source,
    size_t count)
{
    unsigned char *output = (unsigned char *)destination;
    const unsigned char *input = (const unsigned char *)source;
    unsigned char byte;
    while (count != 0u && ((uintptr_t)input & 3u) != 0u) {
        byte = *input++;
        *output++ = byte;
        --count;
        if (byte == 0u) goto pad;
    }
    while (count >= 4u) {
        const uint32_t word = load_word(input);
        unsigned int index;
        input += 4;
        for (index = 0; index < 4; ++index)
            if (((word >> (index * 8)) & 255u) == 0u) break;
        if (index < 4u) {
            const uint32_t mask = index != 0u ?
                (1u << (index * 8)) - 1u : 0u;
            store_word(output, word & mask);
            output += 4;
            count -= 4;
            goto pad;
        }
        store_word(output, word);
        output += 4;
        count -= 4;
    }
    while (count != 0u) {
        byte = *input++;
        *output++ = byte;
        --count;
        if (byte == 0u) break;
    }
pad:
    pharaoh_memory_set(output, 0, count);
    return destination;
}

int pharaoh_string_compare(const char *left, const char *right)
{
    const unsigned char *a = (const unsigned char *)left;
    const unsigned char *b = (const unsigned char *)right;
    for (;;) {
        const unsigned char x = *a++;
        const unsigned char y = *b++;
        if (x != y) return x < y ? -1 : 1;
        if (x == 0u) return 0;
    }
}

size_t pharaoh_string_length(const char *text)
{
    const unsigned char *end = (const unsigned char *)text;
    const unsigned char *begin = end;
    while (*end != 0u) ++end;
    return (size_t)(end - begin);
}

char *pharaoh_string_find(const char *text, const char *pattern)
{
    const unsigned char *start = (const unsigned char *)text;
    const unsigned char *needle = (const unsigned char *)pattern;

    if (*needle == 0u)
        return (char *)text;
    while (*start != 0u) {
        const unsigned char *candidate = start;
        const unsigned char *match = needle;
        while (*match != 0u && *candidate == *match) {
            ++candidate;
            ++match;
        }
        if (*match == 0u)
            return (char *)start;
        ++start;
    }
    return NULL;
}

void *pharaoh_memory_move(void *destination, const void *source, size_t count)
{
    unsigned char *output = (unsigned char *)destination;
    const unsigned char *input = (const unsigned char *)source;

    if ((uintptr_t)output > (uintptr_t)input &&
        (uintptr_t)output - (uintptr_t)input < count) {
        while (count != 0u) {
            --count;
            output[count] = input[count];
        }
    } else {
        size_t index;
        for (index = 0; index < count; ++index)
            output[index] = input[index];
    }
    return destination;
}

void pharaoh_memory_copy_forward_words64(
    const void *source, void *destination, int count)
{
    const unsigned char *input = (const unsigned char *)source;
    unsigned char *output = (unsigned char *)destination;
    while (count > 63) {
        int index;
        for (index = 0; index < 16; ++index) {
            uint32_t word;
            memcpy(&word, input + index * 4, 4);
            memcpy(output + index * 4, &word, 4);
        }
        input += 64;
        output += 64;
        count -= 64;
    }
    while (count > 0) {
        *output++ = *input++;
        --count;
    }
}
