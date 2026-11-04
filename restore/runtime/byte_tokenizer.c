#include "byte_tokenizer.h"
#include <stdint.h>
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
char *restore_tokenize_byte_set(char *text,const char *delimiters)
{
    uint8_t set[32]={0},value;
    uint32_t p=(uint32_t)(uintptr_t)text,d=(uint32_t)(uintptr_t)delimiters,first;
    do{value=B(d);set[value>>3]|=(uint8_t)(1u<<(value&7u));++d;}while(value);
    if(!p)p=U(0x12663c8u);
    for(;;){value=B(p);if(!(set[value>>3]&(1u<<(value&7u)))||!value)break;++p;}
    first=p;
    for(;;){
        value=B(p);if(!value)break;
        if(set[value>>3]&(1u<<(value&7u))){B(p)=0;++p;break;}
        ++p;
    }
    U(0x12663c8u)=p;
    return first==p?0:(char *)(uintptr_t)first;
}
