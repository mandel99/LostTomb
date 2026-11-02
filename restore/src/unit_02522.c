#include "sprite_reader.h"
#include "host_allocator.h"
#include <string.h>

extern int FUN_004ceb60(const char *,void *,int);
extern void FUN_004cdf20(void *,int);
extern void FUN_004cdeb0(void *,int);
#define s_system_bmp_006018e4 (*(char (*)[11])0x006018e4u)
#define s_sg3_006018f0 (*(char (*)[5])0x006018f0u)
#define s_data_006018f8 (*(char (*)[6])0x006018f8u)

typedef struct SpriteTagNode SpriteTagNode;
typedef struct {int32_t image;SpriteTagNode *node;} SpriteTagPayload;
struct SpriteTagNode {SpriteTagNode *next,*previous;int32_t key;SpriteTagPayload *payload;};
typedef struct {SpriteTagNode *head,*tail;} SpriteTagList;
typedef char sprite_payload_size[sizeof(SpriteTagPayload)==8?1:-1];
typedef char sprite_node_size[sizeof(SpriteTagNode)==16?1:-1];
static uint32_t load32(const void *p){uint32_t v;memcpy(&v,p,4);return v;}
static int16_t load16s(const void *p){int16_t v;memcpy(&v,p,2);return v;}
static void store32(void *p,uint32_t v){memcpy(p,&v,4);}

/* Explicit receivers for the complete 5404B0/540540/540570/540590 bodies.
   These are private: the legacy generated global symbols have unrelated
   unresolved callers and must not be rebound through implicit ECX. */
static void reader_tag_append(SpriteTagList *self,SpriteTagNode *node)
{
 node->previous=self->tail;
 if(self->tail)self->tail->next=node;
 node->next=NULL;self->tail=node;if(!self->head)self->head=node;
}
static void reader_tag_prepend(SpriteTagList *self,SpriteTagNode *node)
{
 node->next=self->head;
 if(self->head)self->head->previous=node;
 node->previous=NULL;self->head=node;if(!self->tail)self->tail=node;
}
static SpriteTagNode *reader_tag_find_before(SpriteTagNode *unused,SpriteTagNode *head,int32_t key)
{(void)unused;while(head->key<key)head=head->next;return head;}
static void reader_tag_insert_before(SpriteTagNode *node,SpriteTagNode *next)
{node->next=next;node->previous=next->previous;next->previous->next=node;next->previous=node;}

uint8_t FUN_0053dff0(RestoreSpriteLibrary *self,const char *name,int index,
 RestoreSpriteImage *images,uint32_t first_image,RestoreSpriteTag *tags,uint32_t first_tag)
{
 uint8_t metadata[RESTORE_SPRITE_METADATA_CAPACITY];
 char path[260];uint32_t previous=self->image_count,skip=0,pixel_skip=0,i;
 uint8_t result;int32_t files;
 if(self->pixels){restore_heap_free(self->pixels);self->pixels=NULL;}
 self->name[0]=0;self->pixel_bytes=0;self->image_count=0;self->tag_count=0;
 strcpy(self->name,name);strcpy(path,s_data_006018f8);strcat(path,name);strcat(path,s_sg3_006018f0);
 self->first_image=first_image;self->first_tag=first_tag;
 if(FUN_004ceb60(path,metadata,0)!=0){
  SpriteTagList list={NULL,NULL};SpriteTagPayload *payload;
  if(index!=0&&strcmp((const char *)metadata+0x2a8,s_system_bmp_006018e4)==0){skip=200;pixel_skip=0x992e;}
  self->image_count=load32(metadata+16)-skip;
  memcpy(images,metadata+0x9f28+(skip<<6),(size_t)(self->image_count<<6));
  for(i=0;i<300;++i){
   int32_t image=(int32_t)((uint32_t)(int32_t)load16s(metadata+0x50+i*2)-skip);
   if(image>0){
    SpriteTagNode *node;
    payload=restore_heap_allocate(8);
    if(payload){payload->node=NULL;payload->image=image;}
    node=restore_heap_allocate(16);
    if(node){node->previous=NULL;node->next=NULL;node->payload=payload;node->key=(int32_t)i;}
    if(list.head&&list.head->key<(int32_t)i){
     if(list.tail->key<=(int32_t)i)reader_tag_append(&list,node);
     else reader_tag_insert_before(node,reader_tag_find_before(node,list.head,(int32_t)i));
    }else reader_tag_prepend(&list,node);
    payload->node=node;++self->tag_count;
   }
  }
  {SpriteTagNode *next=list.head;payload=next?next->payload:NULL;next=next?next->next:NULL;
   i=0;
   while(payload){
    SpriteTagPayload *following=next?next->payload:NULL;
    SpriteTagNode *node=payload->node;
    uint16_t tag=(uint16_t)(first_tag+i);
    next=next?next->next:NULL;
    tags[i].library=(uint32_t)index;tags[i].image=(uint32_t)payload->image+first_image-1u;
    ++i;memcpy((uint8_t *)images+((uint32_t)payload->image<<6)-0x24,&tag,2);
    if(node->previous)node->previous->next=node->next;else list.head=node->next;
    if(node->next)node->next->previous=node->previous;else list.tail=node->previous;
    node->previous=NULL;node->next=NULL;restore_heap_free(node);
    payload->node=NULL;if(payload)restore_heap_free(payload);payload=following;
   }
  }
 }
 /* 53E141 jumps here even after a failed metadata read. As in the original,
    that path has no initialized-header guarantee, rather than a return-0 rule. */
 files=(int32_t)load32(metadata+20);
 for(i=0;(int32_t)i<files;++i)strcpy(self->files[i],(const char *)metadata+0x2a8+i*200);
 self->pixel_bytes=FUN_0053f660(name,&self->pixels,0,0,0,0,pixel_skip);
 result=(uint8_t)(self->pixel_bytes!=0);
 if(result){
  volatile uint32_t *cursor=(volatile uint32_t *)(uintptr_t)0xd3b5f4u;
  *cursor=(uint32_t)(uintptr_t)self->pixels+4u;
  for(i=0;(int32_t)i<(int32_t)self->image_count;++i){
   uint8_t *image=images[i].bytes;uint32_t length;
   store32(image+12,(uint32_t)index);
   if(image[0x34]){if(!load32(image))store32(image,1);continue;}
   store32(image,*cursor);*cursor+=load32(image+4);length=load32(image+4);
   if((int32_t)length<=0)continue;
   if(image[0x32]==30){
    if(image[0x35]){
     FUN_004cdf20((void *)(uintptr_t)load32(image),(int)load32(image+8));
     FUN_004cdeb0((void *)(uintptr_t)(load32(image)+load32(image+8)),(int)(load32(image+4)-load32(image+8)));
    }else FUN_004cdf20((void *)(uintptr_t)load32(image),(int)length);
   }else{
    image[0x35]=0;
    if(image[0x33])FUN_004cdeb0((void *)(uintptr_t)load32(image),(int)length);
    else FUN_004cdf20((void *)(uintptr_t)load32(image),(int)length);
   }
  }
 }
 if((int32_t)self->image_count<=(int32_t)previous)self->image_count=previous;
 return result;
}
