#include "compression.h"
#include <string.h>
extern void FUN_004ce6e0(const char *,int,int);
uint32_t FUN_0055bdb0(void *out,const uint32_t *requested,RestoreCodecContext *self)
{
    RestoreCodecBuffer *source;uint32_t copied;
    if(self->error==1)return 0;
    source=self->mode==1?&self->encode_input:&self->encoded;
    copied=source->position>=source->capacity?0:source->capacity-source->position;
    if(copied>*requested)copied=*requested;
    if(copied)memcpy(out,source->data+source->position,copied);
    source->position+=copied;
    if(self->mode==1)self->crc=FUN_0055c0f0(out,&copied,&self->crc);
    return copied;
}
void FUN_0055be50(const void *data,const uint32_t *length,RestoreCodecContext *self)
{
    RestoreCodecBuffer *target;
    if(self->error==1)return;
    if(self->mode==1){self->encoded_size+=*length;target=&self->encoded;}
    else target=&self->decoded;
    if(target->position>=target->capacity) {
        FUN_004ce6e0((const char *)0x603654u,0,0);self->error=1;return;
    }
    if(target->capacity-target->position<*length) {
        FUN_004ce6e0((const char *)0x603674u,0,0);self->error=1;return;
    }
    memcpy(target->data+target->position,data,*length);target->position+=*length;
    if(self->mode==2)self->crc=FUN_0055c0f0(data,length,&self->crc);
}
