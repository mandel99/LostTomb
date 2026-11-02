#include "text_templates.h"
#include <stddef.h>
#include <string.h>
extern void *FUN_00561a1d(uint32_t);
extern void FUN_00561a2b(void *);
extern void *FUN_00561a0a(uint32_t,uint32_t);
extern uint32_t FUN_00561994(void *);
extern uint32_t FUN_0056b22e(uint32_t);
extern int32_t FUN_0056b1b6(uint32_t);
extern uint32_t FUN_00561ca1(uint8_t *,uint32_t,uint32_t,void *);
extern int FUN_00562370(const char *,const char *);
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))

/* Whole4CBA60. Native ECX is explicit. The two passes share the mutated
   source buffer, reserve every quoted string including duplicate keys, and
   stop on an unknown key. The backslash lookahead uses the matched key
   index, exactly as the native code does, rather than a modern escape rule.
   Bounds failures in malformed quoted input terminate parsing safely. */
RestoreMessageCatalog *FUN_004cba60(RestoreMessageCatalog *self,const char *filename,const RestoreMessageKey *keys)
{
    uint32_t i=0,needed=0,pass,pos,slot,look,length,total=0;
    uint8_t *source=0,byte;
    void *file;
    int32_t size,selected;
    self->count=0;self->entries=0;self->unused0=(uint32_t)(uintptr_t)keys;
    self->unused8=0;self->unused16=0;
    if(!keys)return self;
    while(keys[i].name){++self->count;++i;}
    if(!self->count)return self;
    self->entries=FUN_00561a1d((uint32_t)self->count*4u);
    if(!self->entries){self->count=0;return self;}
    for(i=0;(int32_t)i<self->count;++i)self->entries[i]=0;
    if(!filename)return self;
    file=FUN_00561a0a((uint32_t)(uintptr_t)filename,0x5763bcu);
    if(!file)return self;
    size=FUN_0056b1b6(FUN_0056b22e((uint32_t)(uintptr_t)file));
    if(size<0){(void)FUN_00561994(file);return self;}
    length=(uint32_t)size;
    source=FUN_00561a1d(length+1u);
    if(!source){(void)FUN_00561994(file);return self;}
    (void)FUN_00561ca1(source,length,1,file);source[length]=0;
    (void)FUN_00561994(file);
    for(pass=0;pass<2u;++pass){
        if(pass){self->unused8=(uint32_t)(uintptr_t)FUN_00561a1d(needed);if(!self->unused8)break;}
        needed=0;selected=-1;pos=0;slot=i;
        while(pos<=length){
            byte=source[pos];
            if(byte==59u){
                while(pos<=length&&source[pos]!=13u&&source[pos]!=10u)++pos;
                if(pos>length)break;
                byte=source[pos];
                if(pos<length&&source[pos+1u]!=byte)++pos;
            }else if(byte<=32u){++pos;}
            else if(selected<0){
                if(!pass){
                    look=pos;while(look<=length&&source[look]>32u)++look;
                    if(look<=length)source[look]=0;
                }
                slot=0;
                while((int32_t)slot<self->count){
                    keys=(const RestoreMessageKey *)(uintptr_t)self->unused0;
                    if(!FUN_00562370(keys[slot].name,(const char *)source+pos))break;
                    ++slot;
                }
                if((int32_t)slot>=self->count)goto done;
                keys=(const RestoreMessageKey *)(uintptr_t)self->unused0;
                selected=(int32_t)slot;pos+=(uint32_t)strlen(keys[slot].name);
            }else{
                if(byte!=34u)goto done;
                ++pos;
                if(pass)self->entries[selected]=(const char *)(uintptr_t)(self->unused8+needed);
                while(pos<=length&&source[pos]!=34u){
                    byte=source[pos];
                    if(byte==92u){
                        look=slot+1u;
                        while(look<=length&&source[look]<=32u&&source[look]!=13u&&source[look]!=10u)++look;
                        if(look<length&&(source[look]==13u||source[look]==10u)){
                            if(source[look+1u]!=source[look])++look;
                            pos+=look;continue;
                        }
                    }
                    if(pass){if(needed>=total)goto done;*(uint8_t *)(uintptr_t)(self->unused8+needed)=byte;}
                    ++needed;++pos;
                }
                if(pos>length)goto done;
                if(pass){if(needed>=total)goto done;*(uint8_t *)(uintptr_t)(self->unused8+needed)=0;}
                ++needed;++pos;selected=-1;
            }
        }
        if(!pass)total=needed;
    }
done:
    FUN_00561a2b(source);
    return self;
}
