#include "campaign_parser.h"
#include <stdio.h>
extern void FUN_004ce6e0(const char *,int,int);
void FUN_0041adf0(const char *file,int line,const char *source,const char *error)
{
    char message[300];
    snprintf(message,sizeof(message),"ERR: %s line %d: %s %s",file,line,source,error);
    FUN_004ce6e0(message,0,0);
}
