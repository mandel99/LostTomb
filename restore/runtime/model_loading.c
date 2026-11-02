#include <windows.h>
#include <stdint.h>
#include "legacy_format.h"

extern void FUN_0040f8f0(void *destination,uint32_t size,uint8_t value);
extern uint32_t FUN_004ceb60(const char *path,void *destination,int32_t mode);
extern uint32_t FUN_004ced50(const char *path);
extern int32_t FUN_004cbdd0(const char *text,const char *needle,int32_t length);
extern int32_t FUN_004cbe30(const char *text,int32_t character,int32_t length);
extern int32_t FUN_004cc0e0(const uint8_t *text);
extern int32_t FUN_004cc070(const uint8_t *text);
extern void FUN_004ce6e0(const char *message,int32_t detail,int32_t number);

#define U(a) (*(volatile uint32_t *)(uintptr_t)(a))
#define MODEL_TEXT(a) ((const char *)(uintptr_t)(a))

typedef struct {
    uint32_t marker,begin,end,columns;
} ModelSection;
typedef struct {
    uint32_t path,by_difficulty,count_marker,count;
    uint32_t missing,mismatch,title,loaded;
    ModelSection sections[2];
} ModelFile;

static const char *model_path(const ModelFile *file)
{
    return MODEL_TEXT(file->by_difficulty ? U(file->path+U(0xe39080u)*4u) : file->path);
}

static uint32_t section_start(uint32_t marker,uint32_t length)
{
    uint32_t cursor=U(0xe92e1cu);
    if(marker)
        cursor+=(uint32_t)FUN_004cbdd0(MODEL_TEXT(cursor),MODEL_TEXT(marker),(int32_t)length);
    return cursor;
}

/* The four native loaders share a count pass followed by bounded table fills.
 * Keep each helper call and final comma scan; do not infer rows from IDs or
 * replace the native count-mismatch continuation with a different policy. */
static uint32_t load_model(const ModelFile *file)
{
    uint32_t cursor,length,count=0,remaining=299,section;
    if(!U(0xe92e1cu)) {
        FUN_004ce6e0(MODEL_TEXT(0x601ba8u),0,0);
        return 0;
    }
    FUN_0040f8f0((void *)(uintptr_t)U(0xe92e1cu),100000u,0);
    if(!FUN_004ceb60(model_path(file),(void *)(uintptr_t)U(0xe92e1cu),0)) {
        FUN_004ce6e0(MODEL_TEXT(file->missing),0,0);
        return 0;
    }
    /* Native code reloads difficulty after the file-read boundary. */
    length=FUN_004ced50(model_path(file));
    cursor=section_start(file->count_marker,length);
    do {
        uint32_t advance=(uint32_t)FUN_004cbe30(MODEL_TEXT(cursor),'{',(int32_t)length);
        if(!advance) break;
        ++count;
        cursor+=advance;
    } while(--remaining);
    if(count!=file->count) {
        /* Original warning text can reach 105 bytes after formatting. Keep
           the full native message without overflowing the C prompt buffer. */
        char error[100],question[128];
        FUN_00561a36(error,MODEL_TEXT(file->mismatch),count+1u,file->count);
        FUN_00561a36(question,MODEL_TEXT(0x5da204u),error);
        if(MessageBoxA((HWND)(uintptr_t)U(0xe39140u),question,MODEL_TEXT(file->title),4)==7) {
            FUN_004ce6e0(error,0,0);
            return 0;
        }
    }
    for(section=0;section<2u&&file->sections[section].columns;++section) {
        const ModelSection *table=&file->sections[section];
        uint32_t destination=table->begin;
        cursor=section_start(table->marker,length);
        remaining=299;
        do {
            uint32_t column;
            if(destination>=table->end) break;
            cursor+=(uint32_t)FUN_004cbe30(MODEL_TEXT(cursor),'{',(int32_t)length);
            for(column=0;column<table->columns;++column) {
                cursor+=(uint32_t)FUN_004cc0e0((const uint8_t *)(uintptr_t)cursor);
                U(destination)=(uint32_t)FUN_004cc070((const uint8_t *)(uintptr_t)cursor);
                cursor+=(uint32_t)FUN_004cbe30(MODEL_TEXT(cursor),',',(int32_t)length);
                destination+=4u;
            }
        } while(--remaining);
    }
    FUN_004ce6e0(MODEL_TEXT(file->loaded),0,0);
    return 1;
}

/* 540C90..540EC7: 237 building rows and 20 house rows. */
uint32_t FUN_00540c90(void)
{
    static const ModelFile file={0x601908u,1,0x601b78u,257,
        0x601b88u,0x601b38u,0x601b14u,0x601af4u,
        {{0x601b78u,0x705ea4u,0x7083acu,10},
         {0x601b08u,0x70cdccu,0x70d4acu,22}}};
    return load_model(&file);
}

/* 540ED0..541152: enemy rows end exactly where figure rows begin. */
uint32_t FUN_00540ed0(void)
{
    static const ModelFile file={0x60191cu,1,0x601c54u,170,
        0x601c60u,0x601c14u,0x601bf4u,0x601bc8u,
        {{0x601c54u,0x7090ccu,0x70a8dcu,14},
         {0x601be8u,0x7083acu,0x7090ccu,14}}};
    return load_model(&file);
}

/* 541160..541302: 26 rows, 11 sentiment values per tax rate. */
uint32_t FUN_00541160(void)
{
    static const ModelFile file={0x601930u,1,0,26,
        0x601c60u,0x601cc8u,0x601ca0u,0x601c80u,
        {{0,0xea1384u,0xea17fcu,11},{0,0,0,0}}};
    return load_model(&file);
}

/* 541310..5414A2: four population bands with six trade amounts. */
uint32_t FUN_00541310(void)
{
    static const ModelFile file={0x601dc0u,0,0,4,
        0x601d9cu,0x601d58u,0x601d34u,0x601d10u,
        {{0,0x7758a0u,0x775900u,6},{0,0,0,0}}};
    return load_model(&file);
}
