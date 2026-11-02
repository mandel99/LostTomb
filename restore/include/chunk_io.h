#ifndef RESTORE_CHUNK_IO_H
#define RESTORE_CHUNK_IO_H
#include <stdint.h>
typedef struct RestoreChunkDescriptor {uint32_t codec,destination;int32_t size,count;uint32_t migration;} RestoreChunkDescriptor;
typedef struct RestoreChunkMigration {int32_t size,count,cutoff;uint32_t callback;} RestoreChunkMigration;
typedef char restore_chunk_descriptor_size[sizeof(RestoreChunkDescriptor)==20?1:-1];
uint8_t FUN_004d0e10(int,RestoreChunkDescriptor *,int);
void restore_chunk_migrate(uint32_t,void *,int);
void FUN_004d0c60(void *,int);
void FUN_004d0d40(void *,int);
#endif
