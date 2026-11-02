#ifndef RESTORE_MAP_FILE_LOAD_H
#define RESTORE_MAP_FILE_LOAD_H
#include <stdint.h>
#include <string.h>
void FUN_004099b0(void);
uint32_t FUN_004ce3f0(char *path);
uint32_t FUN_004ce100(char *path);
void FUN_004ce2d0(void);
void FUN_004cbea0(uint32_t source,char *destination,int32_t count);
void FUN_004cbec0(unsigned char *text);
void FUN_004ce330(char *text);
void FUN_004ce3a0(char *text);
void FUN_004ce3c0(char *text);
uint32_t FUN_004d5520(char *path);
void FUN_004de600(void);
void FUN_004ce6e0(char *text,int32_t detail,int32_t mode);
#ifdef MAP_FILE_LOAD_TRACE
uint32_t map_file_read(uint32_t address,uint32_t width);
void map_file_write(uint32_t address,uint32_t width,uint32_t value);
void map_file_enter(uint32_t address,uint32_t first,uint32_t second,uint32_t third);
#define MFR(a,n) map_file_read((uint32_t)(uintptr_t)(a),(n))
#define MFW(a,n,v) map_file_write((uint32_t)(uintptr_t)(a),(n),(v))
#define MFE(a,x,y,z) map_file_enter((a),(uint32_t)(uintptr_t)(x),(uint32_t)(uintptr_t)(y),(uint32_t)(z))
#else
static __inline uint32_t map_file_read(uint32_t a,uint32_t n)
{if(n==1)return *(volatile uint8_t *)(uintptr_t)a;return *(volatile uint32_t *)(uintptr_t)a;}
#define MFR(a,n) map_file_read((uint32_t)(uintptr_t)(a),(n))
#define MFW(a,n,v) do{if((n)==1)*(volatile uint8_t *)(uintptr_t)(a)=(uint8_t)(v);else *(volatile uint32_t *)(uintptr_t)(a)=(v);}while(0)
#define MFE(a,x,y,z) ((void)0)
#endif
#ifdef MAP_FILE_LOAD_PROOF
int32_t map_file_open(char *path,uint32_t flags);
int32_t map_file_close(int32_t descriptor);
int32_t map_file_chdir(char *path);
char *map_file_getcwd(char *destination,int32_t capacity);
#else
#include <io.h>
#include <direct.h>
static __inline int32_t map_file_open(char *path,uint32_t flags){return _open(path,(int32_t)flags);}
static __inline int32_t map_file_close(int32_t descriptor){return _close(descriptor);}
static __inline int32_t map_file_chdir(char *path){return _chdir(path);}
static __inline char *map_file_getcwd(char *destination,int32_t capacity){return _getcwd(destination,capacity);}
#endif
#endif
