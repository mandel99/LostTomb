#include "compression.h"
uint32_t FUN_0055c0f0(const uint8_t *input,const uint32_t *length,const uint32_t *state)
{
    const uint32_t *table=(const uint32_t *)0x6037a0u;
    uint32_t remaining=*length,crc=*state;
    while(remaining--)crc=(crc>>8)^table[(crc^*input++)&255];
    return crc;
}
