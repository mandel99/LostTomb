#include "pharaoh/core/byte_tokenizer.h"

#include <stdint.h>

char *pharaoh_byte_tokenize(PharaohByteTokenizer *tokenizer, char *text,
    const char *delimiters)
{
    uint8_t delimiter_set[32] = { 0 };
    uint8_t value;
    char *token;

    do {
        value = (uint8_t)*delimiters++;
        delimiter_set[value >> 3] |= (uint8_t)(1u << (value & 7u));
    } while (value != 0);

    if (text == 0)
        text = tokenizer->next;

    for (;;) {
        value = (uint8_t)*text;
        tokenizer->next = text;
        if ((delimiter_set[value >> 3] & (uint8_t)(1u << (value & 7u))) == 0 ||
            value == 0)
            break;
        ++text;
    }

    token = text;
    for (;;) {
        value = (uint8_t)*tokenizer->next;
        if (value == 0)
            break;
        if ((delimiter_set[value >> 3] &
             (uint8_t)(1u << (value & 7u))) != 0) {
            *tokenizer->next++ = '\0';
            break;
        }
        ++tokenizer->next;
    }
    return text != tokenizer->next ? text : 0;
}
