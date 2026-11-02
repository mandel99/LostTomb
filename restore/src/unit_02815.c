#include "campaign_parser.h"
#include <string.h>
#define CURSOR (*(char **)0x012663c8u)
char *FUN_00561db1(char *text,const char *delimiters)
{
    char *first;
    if(!text)text=CURSOR;
    if(!text)return NULL;
    while(*text&&strchr(delimiters,(uint8_t)*text))++text;
    first=text;
    while(*text&&!strchr(delimiters,(uint8_t)*text))++text;
    if(*text)*text++=0;
    CURSOR=text;
    return text==first?NULL:first;
}
