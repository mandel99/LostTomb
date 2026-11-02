/* Reuse the existing in-memory filesystem fixture, never its test main. */
#define main restore_reader_fixture_unused_main
#define FUN_004cea30 restore_reader_fixture_size
#define FUN_004ceb60 restore_reader_fixture_read
#include "../tests/sprite_libraries.c"
#undef main
#undef FUN_004cea30
#undef FUN_004ceb60
#include <wincrypt.h>

typedef struct {int kind,offset;char path[260];} FileEvent;
static FileEvent file_events[8];
static unsigned int file_count;
static void file_event(int kind,const char *path,int offset)
{
    REQUIRE(file_count<8&&strlen(path)<260);
    file_events[file_count].kind=kind;file_events[file_count].offset=offset;
    strcpy(file_events[file_count++].path,path);
}
int FUN_004cea30(char *path,int offset)
{file_event(0,path,offset);return restore_reader_fixture_size(path,offset);}
int FUN_004ceb60(const char *path,void *destination,int offset)
{file_event(1,path,offset);return restore_reader_fixture_read(path,destination,offset);}
static void normalized_digest(uint8_t digest[32],RestoreSpriteLibrary *library)
{
    HCRYPTPROV provider=0;HCRYPTHASH hash=0;DWORD length=32;
    RestoreSpriteLibraries *copy=restore_heap_allocate(sizeof(*copy));
    uint32_t base=(uint32_t)(uintptr_t)library->pixels;unsigned int i;
    REQUIRE(copy!=NULL);memcpy(copy,&manager,sizeof(*copy));
    for(i=0;i<25500;++i){
        uint8_t *image=copy->images[i].bytes;uint32_t address=get32(image);
        if(base&&!image[0x34]&&address>=base&&address-base<library->pixel_bytes)
            put32(image,address-base);
    }
    copy->libraries[library-manager.libraries].pixels=(uint8_t *)(uintptr_t)(base?1:0);
    REQUIRE(CryptAcquireContextA(&provider,NULL,NULL,PROV_RSA_AES,CRYPT_VERIFYCONTEXT));
    REQUIRE(CryptCreateHash(provider,CALG_SHA_256,0,0,&hash));
    REQUIRE(CryptHashData(hash,(const BYTE *)copy,sizeof(*copy),0));
    REQUIRE(CryptGetHashParam(hash,HP_HASHVAL,digest,&length,0)&&length==32);
    CryptDestroyHash(hash);CryptReleaseContext(provider,0);restore_heap_free(copy);
}
int main(void)
{
    unsigned int slot,previous,old_bytes,name_mode,append,i;
    REQUIRE(restore_data_open());
    while(scanf("%u %u %u %u %u",&slot,&previous,&old_bytes,&name_mode,&append)==5){
        const char *name=name_mode==0?NULL:name_mode==1?"fixture":name_mode==2?"FIXTURE":"other";
        RestoreSpriteLibrary *library;uint8_t result,digest[32];uint32_t cursor,base;
        REQUIRE(slot<30&&old_bytes<=16&&name_mode<=3);
        prepare(slot!=0);file_count=0;U(0xd3b5f4)=0;
        manager.loaded=slot;manager.next_image=31;manager.next_tag=41;manager.pixel_bytes=100+old_bytes;
        memset(manager.tag_limits,0,sizeof(manager.tag_limits));
        manager.tag_limits[slot]=slot?104:5;if(slot)manager.tag_limits[slot-1]=100;
        library=&manager.libraries[slot];strcpy(library->name,"fixture");
        library->image_count=previous;library->tag_count=4;library->pixel_bytes=old_bytes;
        library->first_image=11;library->first_tag=21;
        if(old_bytes){library->pixels=restore_heap_allocate(old_bytes);REQUIRE(library->pixels!=NULL);memset(library->pixels,0xa5,old_bytes);}
        result=FUN_0053dd90(&manager,name,append?-1:(int)slot);
        normalized_digest(digest,library);base=(uint32_t)(uintptr_t)library->pixels;cursor=U(0xd3b5f4);
        if(base&&cursor>=base&&cursor-base<=library->pixel_bytes)cursor-=base;
        printf("{\"result\":%u,\"manager_sha256\":",result);hex(digest,32);
        printf(",\"pixels\":");hex(library->pixels,library->pixel_bytes);
        printf(",\"cursor\":%u,\"files\":[",cursor);
        for(i=0;i<file_count;++i){if(i)putchar(',');printf("[%d,",file_events[i].kind);hex(file_events[i].path,strlen(file_events[i].path));printf(",%d]",file_events[i].offset);}
        puts("]}");fflush(stdout);restore_heap_free(library->pixels);library->pixels=NULL;
    }
    restore_data_close();return 0;
}
