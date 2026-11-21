#ifndef PHARAOH_CORE_BYTE_TOKENIZER_H
#define PHARAOH_CORE_BYTE_TOKENIZER_H

typedef struct PharaohByteTokenizer {
    char *next;
} PharaohByteTokenizer;

/* Complete C translation of Pharaoh.exe 0x00561DB1. */
char *pharaoh_byte_tokenize(PharaohByteTokenizer *tokenizer, char *text,
    const char *delimiters);

#endif
