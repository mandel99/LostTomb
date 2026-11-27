#include "pharaoh/ui/city_message_publish.h"
#include "pharaoh/ui/window_state_actions.h"
#include "pharaoh/core/static_objects.h"
#include <stddef.h>
#include <string.h>

_Static_assert(sizeof(PharaohMenuMessageRecord)==48 &&
    offsetof(PharaohMenuMessageRecord,kind)==8 &&
    offsetof(PharaohMenuMessageRecord,message)==10 &&
    offsetof(PharaohMenuMessageRecord,sequence)==12 &&
    offsetof(PharaohMenuMessageRecord,read)==14 &&
    offsetof(PharaohMenuMessageRecord,source)==15 &&
    offsetof(PharaohMenuMessageRecord,template_id)==16 &&
    offsetof(PharaohMenuMessageRecord,native_26)==38 &&
    sizeof(PharaohCityMessageMediaLayout)==80 &&
    offsetof(PharaohCityMessageMediaLayout,subtype)==2 &&
    offsetof(PharaohCityMessageMediaLayout,media_offset)==60,
    "native message publication layouts");

void pharaoh_city_message_post(const PharaohCityMessagePublishContext *c,
    int32_t message,uint32_t argument0,uint32_t argument1,
    const int16_t *parameters,uint8_t force_popup)
{
    const PharaohCityMessageQueueContext *const q=c->ui;
    *q->popup=force_popup!=0?1:
        pharaoh_city_message_should_popup(c->policy,message,parameters);
    const uint8_t window=*c->window_id;
    if(window==30 || window==73 || window==31 || window==32)
        *c->window_id=1;
    int32_t index=pharaoh_city_message_new_id(q->messages);
    if(index==-1) return;
    pharaoh_sound_effect_request(c->sound,c->sound_operations,9);
    if(*q->popup==0) *c->message_button_alert=1;
    PharaohMenuMessageRecord *event=&q->messages[index];
    int16_t year;
    memcpy(&year,c->current_year,sizeof(year));
    const uint32_t used=*c->messages_used;
    event->message=(int16_t)message;
    event->read=0;
    event->kind=year;
    event->source=*(const uint8_t *)c->current_month;
    event->first=argument0;
    event->second=argument1;
    const uint32_t sequence=*c->next_sequence;
    event->sequence=(int16_t)sequence;
    *c->messages_used=used+1;
    *c->newest_index=index;
    *c->next_sequence=sequence+1;
    uint8_t templated=0;
    uint8_t *const packet=(uint8_t *)event+16;
    if(parameters==NULL){
        const int16_t missing=-1;
        event->template_id=missing;
        memcpy(packet+2,&missing,2);
        memcpy(packet+4,&missing,2);
    }else{
        const uint8_t *const source=(const uint8_t *)parameters;
        /* Native forward REP MOVSD(7), MOVSW; overlap order is observable. */
        for(int32_t i=0;i<7;++i){
            uint32_t value;
            memcpy(&value,source+i*4,4);
            memcpy(packet+i*4,&value,4);
        }
        uint16_t last;
        memcpy(&last,source+28,2);
        memcpy(packet+28,&last,2);
        templated=*parameters>=0;
    }
    if(*c->window_id==1){
        pharaoh_city_message_set_category((uint32_t)message,q->category);
        index=*c->newest_index;
    }
    const PharaohCityMessageMediaLayout *const layout=&q->layouts[message];
    if(layout->subtype==1 || layout->subtype==7){
        *c->special_message_present=1;
        *c->special_message_flag=1;
    }
    if(*q->popup!=0){
        if(*c->window_id==1){
            event=&q->messages[index];
            *q->shown_index=index;
            *q->category=5;
            event->read=1;
            const uint32_t month=event->source;
            *q->year=event->kind;
            const uint32_t first=event->first;
            *q->month=month;
            const uint32_t second=event->second;
            *q->modal_reset=0;
            *q->first=first;
            *q->second=second;
            *q->current_message=message;
            pharaoh_tooltip_invalidate(*q->current_tick,q->tooltip_id,
                q->tooltip_tick);
            const char *const filename=*q->strings+layout->media_offset;
            if(pharaoh_file_exists_with_media_fallback(q->files,filename)==0)
                (void)pharaoh_native_return_zero_0047d350();
            else *q->show_video=1;
            pharaoh_city_message_dialog_show(q,*q->current_message,templated);
        }else{
            for(int32_t i=0;i<20;++i) if(q->queue[i]==0){
                q->queue[i]=q->messages[index].sequence;
                break;
            }
        }
    }else if(*c->posted_once==1){
        /* Both native7/6 argument branches call this genuine zero leaf. */
        (void)pharaoh_native_return_zero_0047d350();
    }
    *c->posted_once=1;
    *q->popup=0;
    *q->redraw=1;
}
