#ifndef RESTORE_SOUND_INITIALIZATION_H
#define RESTORE_SOUND_INITIALIZATION_H
#include <stdint.h>
#include <stddef.h>
typedef struct RestoreSoundFileTable {int32_t capacity,count;uint32_t entries;} RestoreSoundFileTable;
typedef struct RestoreSoundSample {int32_t values[10];} RestoreSoundSample;
typedef struct RestoreSoundCache {int32_t file;uint32_t data;int32_t state;} RestoreSoundCache;
typedef struct RestoreSoundFormat {uint16_t tag,channels;uint32_t rate,bytes_per_second;uint16_t block,bits;} RestoreSoundFormat;
typedef struct RestoreSound {
    RestoreSoundFileTable files;
    uint32_t providers[26][3];
    uint32_t enabled;int32_t selected;
    int32_t volumes[4];uint32_t reserved15c[2];
    int32_t provider;uint32_t reserved168;float factors[3];
    uint32_t digital;RestoreSoundFormat format;
    int32_t rate,bits,channels,sample_count;
    uint32_t handles,auxiliary,reserved1a4,samples,cache;
    int32_t settings[10];uint32_t streams[3];
    int32_t count3d;uint32_t device3d,reserved1ec,handles3d,samples3d,cache3d,reserved1fc,disabled;
} RestoreSound;
typedef char restore_sound_size[(sizeof(RestoreSound)==0x204)?1:-1];
typedef char restore_sound_digital[(offsetof(RestoreSound,digital)==0x178)?1:-1];
typedef char restore_sound_format[(offsetof(RestoreSound,format)==0x17c)?1:-1];
typedef char restore_sound_rate[(offsetof(RestoreSound,rate)==0x18c)?1:-1];
typedef char restore_sound_samples[(offsetof(RestoreSound,samples)==0x1a8)?1:-1];
typedef char restore_sound_disabled[(offsetof(RestoreSound,disabled)==0x200)?1:-1];
#define RESTORE_SOUND ((RestoreSound *)0x00e92bf8u)
RestoreSound *FUN_0055e38b(RestoreSound *self);
int FUN_0055e6c4(RestoreSound *self);
int FUN_0055dea2(RestoreSound *self,int count);
void FUN_0055e5d2(RestoreSound *self,int channel);
void FUN_0055ec7c(RestoreSound *self);
void FUN_0055ecf3(uintptr_t self);
int FUN_0055edd0(uintptr_t self,const int32_t *request,int channel);
int FUN_0055f02d(uintptr_t self);
int FUN_0055f07f(uintptr_t self,const int32_t *request);
int FUN_0055f0e8(RestoreSound *self,const int32_t *request);
int FUN_0055dc63(RestoreSoundFileTable *self,int count);
void restore_sound_initialize_owned(void);
int restore_sound_version(uint32_t *platform,uint32_t *major);
void restore_sound_debug(const char *message);
#endif
