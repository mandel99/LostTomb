#ifndef RESTORE_COMPRESSION_H
#define RESTORE_COMPRESSION_H
#include <stdint.h>
typedef struct RestoreCodecBuffer {uint8_t *data;uint32_t position,capacity;} RestoreCodecBuffer;
typedef struct RestoreCodecContext {
    RestoreCodecBuffer encode_input,encoded,decoded;
    uint32_t mode,crc,encoded_size,error;
} RestoreCodecContext;
typedef char restore_codec_context_size[sizeof(RestoreCodecContext)==52?1:-1];
typedef uint32_t (*RestoreCodecRead)(void *,const uint32_t *,RestoreCodecContext *);
typedef void (*RestoreCodecWrite)(const void *,const uint32_t *,RestoreCodecContext *);
uint32_t FUN_0055c130(RestoreCodecRead,RestoreCodecWrite,void *,RestoreCodecContext *,const uint32_t *,const uint32_t *);
uint32_t FUN_0055bf10(uint32_t *,uint32_t *,const void *,void *,uint32_t);
uint32_t FUN_0055bc50(uint32_t,const void *,int32_t);
uint32_t FUN_0055bdb0(void *,const uint32_t *,RestoreCodecContext *);
void FUN_0055be50(const void *,const uint32_t *,RestoreCodecContext *);
uint32_t FUN_0055c0f0(const uint8_t *,const uint32_t *,const uint32_t *);
uint32_t FUN_0055c000(uint32_t *,const void *,uint32_t,void *,uint32_t);
uint32_t FUN_0055bcf0(uint32_t,void *,uint32_t);
#endif
