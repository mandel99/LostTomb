#include <windows.h>
#include <stdio.h>
#include "resource_manifest.h"
int main(void)
{
    size_t i,j;
    HMODULE module=GetModuleHandleW(0);
    for(i=0;i<sizeof(resources)/sizeof(resources[0]);++i) {
        HRSRC resource=FindResourceExW(module,resources[i].type,resources[i].name,resources[i].language);
        HGLOBAL loaded;
        const unsigned char *data;
        unsigned int hash=0x811c9dc5u;
        if(!resource||SizeofResource(module,resource)!=resources[i].bytes) return 1;
        loaded=LoadResource(module,resource);
        if(!loaded||(data=(const unsigned char *)LockResource(loaded))==0) return 2;
        for(j=0;j<resources[i].bytes;++j) hash=(hash^data[j])*0x1000193u;
        if(hash!=resources[i].fnv1a) return 3;
    }
    printf("%zu embedded Win32 resources: identities, languages, sizes and payload checksums verified.\n",sizeof(resources)/sizeof(resources[0]));
    return 0;
}
