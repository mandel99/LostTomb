#include "campaign_entry.h"
#include "restore_types.h"
#include "campaign_index.h"
#include "file_list.h"
#include "host_allocator.h"
extern restore_legacy_FILE *FUN_00561a0a(const char *,const char *);
extern uint32_t FUN_00561994(restore_legacy_FILE *);
#define ENTRY_B(a) (*(volatile uint8_t *)(uintptr_t)(a))
typedef struct CampaignFilesFrame {RestoreFileList files;char pattern[260];} CampaignFilesFrame;
typedef char campaign_files_frame_size[sizeof(CampaignFilesFrame)==288?1:-1];
static void files_initialize(volatile RestoreFileList *f)
{
 f->legacy_vtable=0x56f548u;f->offsets=NULL;f->strings=NULL;
 f->count=0;f->capacity=0;f->string_capacity=0;f->used=0;
}
static void files_destroy(volatile RestoreFileList *f)
{
 char *strings=f->strings;uint32_t *offsets;
 f->legacy_vtable=0x56f548u;
 if(strings)restore_heap_free(strings);
 offsets=f->offsets;if(offsets)restore_heap_free(offsets);
}
static char *copy_end(char *out,const volatile char *in)
{char value;do{value=*in++;*out++=value;}while(value);return out-1;}
void FUN_004cb6a0(void)
{
 CampaignFilesFrame local;restore_legacy_FILE *stream;char *end;uint8_t has_file;
 ENTRY_B(0xd3b189u)=FUN_0041bab0(RESTORE_CAMPAIGN_INDEX);
 has_file=ENTRY_B(0x700998u);
 ENTRY_B(0xd3b188u)=1;ENTRY_B(0xd3b18bu)=0;ENTRY_B(0xd3b18au)=0;
 if(has_file){
  stream=FUN_00561a0a((const char *)0x700998u,(const char *)0x5763bcu);
  if(stream){(void)FUN_00561994(stream);ENTRY_B(0xd3b18bu)=1;}
 }
 files_initialize(&local.files);
 end=copy_end(local.pattern,(const char *)0x5e0494u);
 end=copy_end(end,(const char *)0xe38e74u);
 (void)copy_end(end,(const char *)0x5761c4u);
 (void)FUN_0041c740(&local.files,local.pattern,0);
 if(local.files.count>0)ENTRY_B(0xd3b18au)=1;
 ENTRY_B(0xe38ddau)=0x30;ENTRY_B(0xe38dcau)=1;
 files_destroy(&local.files);
}
