/* Reviewed against the original x86 and retained REA decompilation. */
#include "core_helpers.h"

uint32_t restore_0056233e(uint32_t seed)
{
    *(volatile uint32_t *)0x604950=seed;
    return seed;
}

uint32_t restore_00562348(void)
{
    volatile uint32_t *state=(volatile uint32_t *)0x604950;
    uint32_t next=*state*214013u+2531011u;
    *state=next;
    return (next>>16)&0x7fffu;
}

char *restore_005632a0(const char *text,const char *pattern)
{
    const unsigned char *start=(const unsigned char *)text;
    const unsigned char *needle=(const unsigned char *)pattern;
    if (!*needle) return (char *)text;
    for (;*start;start++) {
        const unsigned char *a=start,*b=needle;
        while (*b && *a==*b) { a++; b++; }
        if (!*b) return (char *)start;
    }
    return NULL;
}

void *restore_00563400(void *destination,const void *source,size_t count)
{
    unsigned char *out=(unsigned char *)destination;
    const unsigned char *in=(const unsigned char *)source;
    /* Integer comparisons preserve x86 address ordering for distinct buffers. */
    if ((uintptr_t)out>(uintptr_t)in && (uintptr_t)out-(uintptr_t)in<count) {
        while (count) { count--; out[count]=in[count]; }
    } else {
        size_t i;
        for (i=0;i<count;i++) out[i]=in[i];
    }
    return destination;
}
