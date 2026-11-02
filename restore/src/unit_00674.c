#include "text_templates.h"
void FUN_00448330(int id,char *output,int capacity)
{
    const char *input;
    if(capacity>0) {
        *output=0;
        input=FUN_004cbdb0((const RestoreMessageCatalog *)0x784f20u,id);
        if(input)FUN_004cb980((const RestoreTextTemplateContext *)0x784f34u,input,output,capacity);
    }
}
