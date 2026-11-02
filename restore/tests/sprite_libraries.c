#include "sprite_libraries.h"
#include "host_allocator.h"
#include "standalone_data.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define REQUIRE(x) do{++checks;if(!(x)){fprintf(stderr,"Sprite library failure at line %d\n",__LINE__);ExitProcess(1);}}while(0)
static unsigned int checks,cases;
static RestoreSpriteLibraries manager;
static uint8_t metadata[0x9f28+204*64],pixels[0x992e+16];
static int metadata_size,pixel_size,missing,reads;
static unsigned int skip;
static void put32(void *p,uint32_t value) {memcpy(p,&value,4);}
static uint32_t get32(const void *p) {uint32_t value;memcpy(&value,p,4);return value;}
static void prepare(int special)
{
    unsigned int i;int16_t tag;uint8_t *image,*p;
    restore_sprite_libraries_initialize(&manager);skip=special?200:0;missing=0;reads=0;
    memset(metadata,0,sizeof(metadata));memset(pixels,0xa5,sizeof(pixels));
    metadata_size=0x9f28+(skip+4)*64;pixel_size=(special?0x992e:0)+16;
    put32(metadata+16,skip+4);put32(metadata+20,2);strcpy((char *)metadata+0x2a8,"system.bmp");strcpy((char *)metadata+0x2a8+200,"other.bmp");
    for(i=0;i<4;++i) {tag=(int16_t)(skip+(i==0||i==3?1:i==1?3:2));memcpy(metadata+0x50+i*2,&tag,2);}
    image=metadata+0x9f28+skip*64;
    put32(image+4,4);image[0x35]=1;
    put32(image+64+4,3);image[64+0x33]=1;
    put32(image+128+4,5);put32(image+128+8,2);image[128+0x32]=30;image[128+0x35]=1;
    image[192+0x34]=1;
    p=pixels+(special?0x992e:0);memset(p,0,4);p+=4;
    {const uint8_t data[]={0x34,0x12,0x1f,0xf8,1,0x11,0x11,0x22,0x22,1,0x33,0x33};memcpy(p,data,sizeof(data));}
    U(0xe92dfc)=2;U(0xe92e1c)=0;
}
int FUN_004cea30(char *path,int offset)
{
    int size=strstr(path,".sg3")?metadata_size:pixel_size;
    REQUIRE(!strncmp(path,"data\\",5));return missing?0:size-offset;
}
int FUN_004ceb60(const char *path,void *destination,int offset)
{
    const uint8_t *source=strstr(path,".sg3")?metadata:pixels;
    int size=strstr(path,".sg3")?metadata_size:pixel_size;
    REQUIRE(!strncmp(path,"data\\",5));if(missing)return 0;
    REQUIRE(offset>=0&&offset<=size);memcpy(destination,source+offset,(size_t)(size-offset));++reads;return 1;
}
static void reader_case(int special,unsigned int previous)
{
    RestoreSpriteImage images[8];RestoreSpriteTag tags[4];RestoreSpriteLibrary *library=&manager.libraries[0];uint32_t pointer;
    prepare(special);memset(images,0,sizeof(images));memset(tags,0,sizeof(tags));library->image_count=previous;
    REQUIRE(FUN_0053dff0(library,"fixture",special?7:0,images,11,tags,21)==1);
    REQUIRE(library->image_count==(previous>4?previous:4)&&library->tag_count==4&&library->first_image==11&&library->first_tag==21);
    REQUIRE(library->pixel_bytes==16&&library->pixels&&reads==2&&!strcmp(library->files[1],"other.bmp"));
    REQUIRE(tags[0].image==11&&tags[1].image==13&&tags[2].image==12&&tags[3].image==11);
    REQUIRE(tags[0].library==(special?7u:0u));
    REQUIRE(*(uint16_t *)(images[0].bytes+0x1c)==24&&*(uint16_t *)(images[1].bytes+0x1c)==23&&*(uint16_t *)(images[2].bytes+0x1c)==22);
    pointer=(uint32_t)(uintptr_t)library->pixels;
    REQUIRE(get32(images[0].bytes)==pointer+4&&get32(images[1].bytes)==pointer+8&&get32(images[2].bytes)==pointer+11&&get32(images[3].bytes)==1);
    REQUIRE(!images[0].bytes[0x35]&&images[2].bytes[0x35]);
    REQUIRE(library->pixels[4]==0x54&&library->pixels[5]==0x24&&library->pixels[6]==0x1f&&library->pixels[7]==0xf8);
    REQUIRE(U(0xd3b5f4)==pointer+16);
    restore_heap_free(library->pixels);library->pixels=NULL;++cases;
}
static void hex(const void *memory,size_t size) {size_t i;putchar('"');for(i=0;i<size;++i)printf("%02x",((const uint8_t *)memory)[i]);putchar('"');}
static void oracle(void)
{
    unsigned int special,previous,format,i;RestoreSpriteLibrary copy;RestoreSpriteImage images[8];RestoreSpriteTag tags[4];uint8_t result;
    puts("{\"library_bytes\":52284}");fflush(stdout);
    while(scanf("%u %u %u",&special,&previous,&format)==3) {
        prepare(special);U(0xe92dfc)=format;manager.libraries[0].image_count=previous;
        memset(images,0,sizeof(images));memset(tags,0,sizeof(tags));
        result=FUN_0053dff0(&manager.libraries[0],"fixture",special?7:0,images,11,tags,21);
        copy=manager.libraries[0];
        for(i=0;i<8;++i) {uint32_t address=get32(images[i].bytes);if(copy.pixels&&!images[i].bytes[0x34]&&address>=(uint32_t)(uintptr_t)copy.pixels)put32(images[i].bytes,address-(uint32_t)(uintptr_t)copy.pixels);}
        copy.pixels=(uint8_t *)(uintptr_t)(copy.pixels?1:0);
        printf("{\"result\":%u,\"record\":",result);hex(&copy,sizeof(copy));printf(",\"images\":");hex(images,sizeof(images));printf(",\"tags\":");hex(tags,sizeof(tags));printf(",\"pixels\":");hex(manager.libraries[0].pixels,manager.libraries[0].pixel_bytes);
        printf(",\"cursor\":%u}\n",U(0xd3b5f4)-(uint32_t)(uintptr_t)manager.libraries[0].pixels);fflush(stdout);
        restore_heap_free(manager.libraries[0].pixels);manager.libraries[0].pixels=NULL;
    }
}
int main(int argc,char **argv)
{
    unsigned int i;const char *names[30];RestoreSpriteLibrary *library;
    REQUIRE(restore_data_open());
    if(argc==2&&!strcmp(argv[1],"--oracle-batch")) {oracle();restore_data_close();return 0;}
    for(i=0;i<20;++i) {reader_case(0,i%9);reader_case(1,i%9);}
    prepare(0);manager.tag_limits[0]=5;for(i=0;i<30;++i)names[i]="fixture";
    REQUIRE(FUN_0053dd10(&manager,names)==1&&manager.loaded==1&&manager.next_image==5&&manager.next_tag==5&&manager.pixel_bytes==16);
    library=&manager.libraries[0];REQUIRE(FUN_0053dd90(&manager,"FIXTURE",0)==1&&reads==2&&manager.pixel_bytes==16);
    REQUIRE(FUN_0053dd90(&manager,NULL,0)==0);restore_heap_free(library->pixels);library->pixels=NULL;
    prepare(0);manager.loaded=30;REQUIRE(FUN_0053dd10(&manager,names)==0);FUN_0053dd40(&manager,names);
    /* Native retains the signed maximum, including mapped high-bit counts. */
    prepare(0);library=&manager.libraries[0];library->image_count=0xffffffffu;
    REQUIRE(FUN_0053dff0(library,"fixture",0,manager.images,0,manager.tags,0)==1&&library->image_count==4);
    restore_heap_free(library->pixels);library->pixels=NULL;++cases;
    /* SYSTEM exclusion depends on the name even when the file count is zero. */
    prepare(1);library=&manager.libraries[0];put32(metadata+20,0);
    REQUIRE(FUN_0053dff0(library,"fixture",7,manager.images,0,manager.tags,0)==1&&library->image_count==4&&library->pixel_bytes==16&&library->files[0][0]==0);
    restore_heap_free(library->pixels);library->pixels=NULL;++cases;
    /* A tag beyond copied records is valid with the full physical destination.
       Failed/short reads and malformed buffer overruns have no return-0 rule. */
    prepare(0);library=&manager.libraries[0];{int16_t tag=99;uint16_t before=0xa55a;memcpy(metadata+0x50,&tag,2);memcpy(manager.images[98].bytes+0x1c,&before,2);}
    REQUIRE(FUN_0053dff0(library,"fixture",0,manager.images,0,manager.tags,0)==1&&manager.tags[0].image==98&&*(uint16_t *)(manager.images[98].bytes+0x1c)==0);
    restore_heap_free(library->pixels);library->pixels=NULL;++cases;
    printf("Sprite libraries: %u checks, %u reader scenarios, object ABI, ordered tags/duplicate anchors, system-image exclusion, RGB565 pointers, signed retained counts and mapped destination extensions verified.\n",checks,cases);
    restore_data_close();return 0;
}
