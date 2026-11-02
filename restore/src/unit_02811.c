#include <stdint.h>

extern uint32_t FUN_00565f66(uint32_t character,uint32_t mask);

/* Native 561C0B reaches the classification table through a DWORD pointer. */
static uint32_t classify(uint32_t character,uint32_t mask)
{
    if (*(int32_t *)0x00604e64u > 1)
        return FUN_00565f66(character,mask);
    return (*(uint8_t **)(uintptr_t)0x00604c58u)[character*2u]&mask;
}

int32_t FUN_00561c0b(const uint8_t *text)
{
    uint32_t character,sign,value=0;
    while(classify(*text,8u)) ++text;
    sign=character=*text++;
    if(character=='-' || character=='+') character=*text++;
    /* Native accumulation and negation wrap modulo 2^32. */
    while(classify(character,4u)) {
        value=value*10u+character-0x30u;
        character=*text++;
    }
    return (int32_t)(sign=='-' ? 0u-value : value);
}
