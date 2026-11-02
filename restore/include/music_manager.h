#ifndef RESTORE_MUSIC_MANAGER_H
#define RESTORE_MUSIC_MANAGER_H
#include <stdint.h>
#include <stddef.h>
typedef struct RestoreMusicTrack {char path[100];int32_t delay,minimum,maximum;} RestoreMusicTrack;
typedef struct RestoreMusicCategory {RestoreMusicTrack tracks[60];int32_t ambient_after[50],count,ambient_count;} RestoreMusicCategory;
typedef struct RestoreMusicManager {
    RestoreMusicCategory categories[53];
    int32_t enabled,first,last,previous,current,phase;
    uint32_t tick;int32_t waiting,reserved;
} RestoreMusicManager;
typedef char restore_music_track_size[(sizeof(RestoreMusicTrack)==0x70)?1:-1];
typedef char restore_music_category_size[(sizeof(RestoreMusicCategory)==0x1b10)?1:-1];
typedef char restore_music_count_offset[(offsetof(RestoreMusicCategory,count)==0x1b08)?1:-1];
typedef char restore_music_state_offset[(offsetof(RestoreMusicManager,enabled)==0x59a50)?1:-1];
typedef char restore_music_size[(sizeof(RestoreMusicManager)==0x59a74)?1:-1];
#define RESTORE_MUSIC ((RestoreMusicManager *)0x00e39180u)
RestoreMusicManager *FUN_00539550(RestoreMusicManager *self);
int FUN_005395e0(RestoreMusicManager *self,const char *path,int category,int32_t delay,int32_t minimum,int32_t maximum,int ambient);
void FUN_00539250(RestoreMusicManager *self);
void FUN_00539730(RestoreMusicManager *self,int force);
void FUN_00539820(RestoreMusicManager *self);
uint32_t restore_music_milliseconds(void);
void FUN_00539a40(RestoreMusicManager *self,int32_t enabled);
void FUN_00539a50(RestoreMusicManager *self,int32_t first,int32_t last);
void restore_music_initialize_owned(void);
#endif
