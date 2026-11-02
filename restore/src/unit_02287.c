#include "restore_types.h"
#include "campaign_profile.h"
#include "file_list.h"
#include <string.h>
extern void FUN_0041acd0(uint8_t *);
extern void FUN_004264d0(int,uint32_t,int);
extern void FUN_00516990(void);
extern char *FUN_005169c0(void);
extern char *FUN_00516a20(void);
extern void FUN_00516bb0(void);
extern restore_legacy_FILE *FUN_00561a0a(const char *,const char *);
extern uint32_t FUN_00561994(restore_legacy_FILE *);

void FUN_00516c80(void)
{
    char *name=(char *)0xe38e74u;
    char saved[32];
    restore_legacy_FILE *stream;
    int i;
    FUN_0041acd0((uint8_t *)name);
    if(!*name) {FUN_00516bb0();return;}
    stream=FUN_00561a0a(FUN_00516a20(),"rb");
    if(stream) {
        FUN_00561994(stream);
        FUN_004264d0(0x5c,0x516c10,1);
        return;
    }
    FUN_0041ae40(RESTORE_CAMPAIGN,"campaign.txt");
    FUN_0041b8a0(RESTORE_CAMPAIGN_PROFILE,FUN_00516a20());
    restore_file_create_directory(FUN_005169c0(),NULL);
    strcpy(saved,name);
    FUN_00516bb0();
    for(i=0;i<RESTORE_FAMILY_FILES->count;++i)
        if(!strcmp(FUN_0041e010(RESTORE_FAMILY_FILES,i),saved))
            *(int32_t *)0xeadfccu=i;
    strcpy(name,saved);
    FUN_00516bb0();
    FUN_00516990();
}
