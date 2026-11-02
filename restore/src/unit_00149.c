#include <stdint.h>

char *FUN_00410f20(char *pixels)
{
    uint32_t source=(uint32_t)(uintptr_t)pixels;
    uint32_t rows=*(volatile uint32_t *)(uintptr_t)0x00d3b538u;
    uint32_t remaining,scratch;
    while((int32_t)rows>0){
        --rows;
        remaining=(uint32_t)(int32_t)*(volatile int16_t *)(uintptr_t)0x00d3b5beu;
        scratch=0;
        while((int32_t)remaining>0){
            /* Original MOV AL preserves the high24 bits left by the
               preceding literal's doubled byte advance. Keep that quirk. */
            scratch=(scratch&0xffffff00u)|*(volatile uint8_t *)(uintptr_t)source;
            if((uint8_t)scratch==255){
                scratch=(scratch&0xffffff00u)|*(volatile uint8_t *)(uintptr_t)(source+1u);
                remaining-=scratch;source+=2u;
            }else{
                remaining-=scratch;scratch=scratch*2u+1u;source+=scratch;
            }
        }
    }
    return(char *)(uintptr_t)source;
}
