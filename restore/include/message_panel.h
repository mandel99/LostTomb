#ifndef RESTORE_MESSAGE_PANEL_H
#define RESTORE_MESSAGE_PANEL_H
#include "restore_types.h"
#include "sprite_libraries.h"
typedef struct RestoreMessageRecord {
    int16_t type,subtype,unknown04,x,y,columns,rows;
    int16_t picture1,picture1_x,picture1_y,picture2,picture2_x,picture2_y;
    int16_t title_x,title_y,caption_x,caption_y,unknown22,unknown24,media_x,media_y;
    uint8_t unknown2a[18];
    uint32_t media,unknown40,title,caption,body;
} RestoreMessageRecord;
typedef struct RestoreMessageEvent {
    int32_t argument0,argument1;
    int16_t year,message,sequence;
    uint8_t opened,month;
    int16_t body_template,title_template,caption_template,unknown16,quantity,resource,cost;
    int16_t unknown1e,unknown20,unknown22,unknown24;
    uint16_t buttons;
    uint8_t unknown28[8];
} RestoreMessageEvent;
typedef char restore_message_record_size[sizeof(RestoreMessageRecord)==80?1:-1];
typedef char restore_message_title_offset[offsetof(RestoreMessageRecord,title)==68?1:-1];
typedef char restore_message_event_size[sizeof(RestoreMessageEvent)==48?1:-1];
typedef char restore_message_event_buttons[offsetof(RestoreMessageEvent,buttons)==38?1:-1];
static __inline RestoreMessageRecord *restore_message_record(void)
{return (RestoreMessageRecord *)(uintptr_t)(0xe209b8u+*(uint32_t *)0xd3b124u*80u);}
static __inline RestoreMessageEvent *restore_message_event(void)
{return (RestoreMessageEvent *)(uintptr_t)(0xe95228u+*(uint32_t *)0xe951e4u*48u);}
static __inline const char *restore_message_text(uint32_t offset)
{return (const char *)(uintptr_t)(*(uint32_t *)0xe36c74u+offset);}
void FUN_004c5bf0(int32_t message,uint32_t background,uint8_t templated);
void FUN_004c5cc0(void);
void FUN_004c5ce0(void);
void FUN_004c5e60(void);
void FUN_004c6c70(void);
void FUN_004c72b0(void);
void FUN_004c7340(void);
uint8_t restore_message_sprite_dimensions(RestoreSpriteLibraries *self,int32_t image,int32_t *width,int32_t *height);
uint8_t FUN_0053e530(int32_t image,int32_t *width,int32_t *height);
#endif
