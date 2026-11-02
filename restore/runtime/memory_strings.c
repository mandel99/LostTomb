/* Reviewed C derivation of 00562240, 005631b0, 00563240, 00563380.
   Original bytes, ABI and retained REA links: metadata/memory-strings-evidence.json. */
#include "memory_strings.h"
#include <string.h>

static uint32_t load_word(const void *source)
{
    uint32_t word;
    memcpy(&word,source,4);
    return word;
}

static void store_word(void *destination,uint32_t word)
{
    memcpy(destination,&word,4);
}

void *restore_00563240(void *destination,int value,size_t count)
{
    unsigned char *output=destination;
    uint32_t byte=(unsigned char)value;
    uint32_t word=byte*0x01010101u;
    while (count && ((uintptr_t)output&3u)) {
        *output++=(unsigned char)byte;
        count--;
    }
    while (count>=4) {
        store_word(output,word);
        output+=4;
        count-=4;
    }
    while (count) {
        *output++=(unsigned char)byte;
        count--;
    }
    return destination;
}

char *restore_00562240(char *destination,const char *source,size_t count)
{
    unsigned char *output=(unsigned char *)destination;
    const unsigned char *input=(const unsigned char *)source;
    unsigned char byte;
    /* Match the original source-alignment prefix before four-byte transfers.
       Each complete word is loaded before it is stored, preserving overlap. */
    while (count && ((uintptr_t)input&3u)) {
        byte=*input++;
        *output++=byte;
        count--;
        if (!byte) goto pad;
    }
    while (count>=4) {
        uint32_t word=load_word(input);
        unsigned int index;
        input+=4;
        for (index=0;index<4;index++) {
            if (!((word>>(index*8))&255u)) break;
        }
        if (index<4) {
            uint32_t mask=index ? (1u<<(index*8))-1u : 0;
            store_word(output,word&mask);
            output+=4;
            count-=4;
            goto pad;
        }
        store_word(output,word);
        output+=4;
        count-=4;
    }
    while (count) {
        byte=*input++;
        *output++=byte;
        count--;
        if (!byte) break;
    }
pad:
    restore_00563240(output,0,count);
    return destination;
}

int restore_005631b0(const char *left,const char *right)
{
    const unsigned char *a=(const unsigned char *)left;
    const unsigned char *b=(const unsigned char *)right;
    for (;;) {
        unsigned char x=*a++,y=*b++;
        if (x!=y) return x<y ? -1 : 1;
        if (!x) return 0;
    }
}

size_t restore_00563380(const char *text)
{
    const unsigned char *end=(const unsigned char *)text;
    const unsigned char *begin=end;
    while (*end) end++;
    return (size_t)(end-begin);
}
