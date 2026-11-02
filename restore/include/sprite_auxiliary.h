#ifndef RESTORE_SPRITE_AUXILIARY_H
#define RESTORE_SPRITE_AUXILIARY_H
#include <stdint.h>
#include <stddef.h>
typedef struct RestoreSpriteAuxiliary {
    uint8_t prefix[0x114];
    uint32_t child,first_list,records,free_list;
} RestoreSpriteAuxiliary;
typedef struct RestoreSpriteAuxiliaryRecord {
    uint32_t reserved0,owner,allocation,reserved12;
    int32_t index;
    uint32_t reserved20;
} RestoreSpriteAuxiliaryRecord;
typedef char restore_sprite_auxiliary_size[sizeof(RestoreSpriteAuxiliary)==0x124?1:-1];
typedef char restore_sprite_auxiliary_records[offsetof(RestoreSpriteAuxiliary,records)==0x11c?1:-1];
typedef char restore_sprite_auxiliary_record_size[sizeof(RestoreSpriteAuxiliaryRecord)==24?1:-1];
void restore_sprite_auxiliary_destroy(RestoreSpriteAuxiliary *self);
void restore_sprite_auxiliary_release_slot(int32_t *slot);
void restore_sprite_auxiliary_destroy_record(RestoreSpriteAuxiliaryRecord *self);
uint32_t restore_sprite_auxiliary_return_record(RestoreSpriteAuxiliary *self,int32_t index);
void FUN_0055d8b0(RestoreSpriteAuxiliary *self);
void FUN_0055d960(int32_t *slot);
void FUN_0055db90(RestoreSpriteAuxiliaryRecord *self);
uint32_t FUN_0055d990(RestoreSpriteAuxiliary *self,int32_t index);
#endif
