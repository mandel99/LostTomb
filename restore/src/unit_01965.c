#include "startup_geometry.h"
void FUN_004e5190(RestoreGeometryRecord *record)
{
    uint32_t *p=record->value,x=p[0],y=p[1],a=*(uint32_t *)0x70e44cu,b=*(uint32_t *)0x70e45cu+1u;
    p[4]=x+y*228u;p[5]=(x-y)*b;p[6]=(x+y)*b;p[7]=(y-x)*b;p[8]=0u-p[6];
    p[9]=(x+y)*a;p[10]=(y-x)*a;p[11]=0u-p[9];p[12]=(x-y)*a;
}
