#include "campaign_parser.h"
#include <stdio.h>
#include <string.h>
extern void *FUN_00561a0a(const char *,const char *);
extern char *FUN_00561e4d(char *,int,void *);
extern int FUN_00561994(void *);
static const char *const sections[]={
    "[MISSION_NAMES]","[PREDYNASTIC_PERIOD]","[ARCHAIC_PERIOD]","[OLD_KINGDOM]",
    "[MIDDLE_KINGDOM]","[NEW_KINGDOM]","[VALLEY_OF_THE_KINGS]","[RAMSES_II]",
    "[ANCIENT_CONQUERORS]","[CLEOPATRAS_ALEXANDRIA]","mission","choicescreen","choice"
};
static int32_t get32(const uint8_t *p) {int32_t value;memcpy(&value,p,4);return value;}
static void put32(uint8_t *p,int32_t value) {memcpy(p,&value,4);}
static int number(uint8_t *p) {int32_t value;int result=FUN_0041ad50(&value);if(result==1)put32(p,value);return result;}
static void next_record(RestoreCampaignRecord *record)
{
    uint8_t *p=record->bytes;
    put32(p+0x38,0);put32(p+0xc,-1);put32(p+8,-1);put32(p+0x10,-1);p[1]=0;p[7]=0;
}
static int same(const char *a,const char *b)
{
    while(*a&&*b) {
        unsigned int x=(uint8_t)*a++,y=(uint8_t)*b++;
        if(x>='A'&&x<='Z')x+='a'-'A';if(y>='A'&&y<='Z')y+='a'-'A';
        if(x!=y)return 0;
    }
    return *a==*b;
}
uint8_t FUN_0041ae40(RestoreCampaign *self,const char *file)
{
    RestoreCampaignRecord record={{0}};
    char line[200],source[200]={0};void *stream;int line_number=0,names=0,ok=1,i;
    FUN_0041ac00(&record);
    stream=FUN_00561a0a(file,"rb");if(!stream)return 0;
    /* The native parser resets and later validates the global record array,
       even when its outer object receiver is explicit. Both real callers bind it. */
    for(i=0;i<100;++i)FUN_0041ac00(&RESTORE_CAMPAIGN_INDEX->records[i]);
    self->index.count=0;self->name_count=0;memset(self->active,0,sizeof(self->active));
    while(FUN_00561e4d(line,sizeof(line),stream)) {
        char *token;const char *error=NULL;int section,value,status,j;size_t length;
        ++line_number;strcpy(source,line);FUN_0041ad20((uint8_t *)line);
        if(line[0]==';'||!line[0])continue;
        token=FUN_00561db1(line,",=");if(!token)continue;
        for(section=0;section<13&&!same(token,sections[section]);++section) {}
        switch(section) {
        case 0:names=1;break;
        case 1:case 2:case 3:case 4:case 5:case 6:case 7:case 8:case 9:
            names=0;
            if(get32(record.bytes+0xc)>=0) {
                if(!FUN_0041ac90(self,&record)) {error=(const char *)0x7027d4u;break;}
                next_record(&record);
            }
            record.bytes[0]=(uint8_t)(section-1);break;
        case 10:
            if(names) {error="bad mission name";break;}
            if(get32(record.bytes+0xc)>=0) {
                if(!FUN_0041ac90(self,&record)) {error="failed to add mission link";break;}
                next_record(&record);
            }
            if(number(record.bytes+0xc)!=1||get32(record.bytes+0xc)<0||get32(record.bytes+0xc)>=self->name_count) {error="bad mission id";break;}
            if(number(record.bytes+8)!=1) {error="bad intro id";break;}
            if(number(record.bytes+0x10)!=1) {error="bad transition text id";break;}
            status=FUN_0041ad50(&value);
            if(status==2) {error="bad path_id";break;}
            if(!status)break;
            if(value<0||value>255) {error="bad path_id";break;}
            record.bytes[1]=(uint8_t)value;record.bytes[7]=0;
            /* Native compatibility: each merge-loop probe consumes one token
               before the numeric helper consumes the following token. */
            for(j=0;j<5&&FUN_00561db1(NULL,",=");++j) {
                status=FUN_0041ad50(&value);if(!status)break;
                if(status==2||value<0||value>255) {error="bad merge id";break;}
                record.bytes[2+record.bytes[7]++]=(uint8_t)value;
            }
            break;
        case 11:
            if(get32(record.bytes+0xc)<0)put32(record.bytes+0xc,127);
            if(names) {error="bad mission name";break;}
            if(number(record.bytes+0x1c)!=1) {error="bad background id";break;}
            if(number(record.bytes+0x18)!=1)error="bad text id";
            break;
        case 12:
            if(names) {error="bad mission name";break;}
            j=get32(record.bytes+0x38);if(j>=2) {error="too many choices";break;}
            status=FUN_0041ad50(&value);
            if(status!=1||value<0||value>255) {error="bad path_id";break;}
            record.bytes[0x2a+j*12]=(uint8_t)value;
            if(number(record.bytes+0x20+j*12)!=1) {error="bad x value";break;}
            if(number(record.bytes+0x24+j*12)!=1) {error="bad y value";break;}
            {int16_t text;if(FUN_0041adc0(&text)!=1) {error="bad text id";break;}memcpy(record.bytes+0x28+j*12,&text,2);}
            put32(record.bytes+0x38,j+1);break;
        default:
            if(!names) {error="unrecognized line";break;}
            length=strlen(source);while(length&&(uint8_t)source[length-1]<=32)source[--length]=0;
            if(length>=50) {error="mission name too long";break;}
            if(length<2)break;
            if(self->name_count>=100) {error="max missions exceeded";break;}
            strcpy(self->names[self->name_count++],source);
            break;
        }
        if(error) {FUN_0041adf0(file,line_number,source,error);ok=0;}
    }
    if(get32(record.bytes+0xc)>=0&&!FUN_0041ac90(self,&record)) {
        FUN_0041adf0(file,line_number,source,"failed to add mission link");ok=0;
    }
    for(i=0;i<self->index.count;++i) {
        const uint8_t *r=RESTORE_CAMPAIGN_INDEX->records[i].bytes;
        int count=get32(r+0x38),choice;
        if(count<=1)continue;
        for(choice=0;choice<count;++choice)if(FUN_0041b920(RESTORE_CAMPAIGN_INDEX,r[0],i,choice)<0) {
            snprintf(line,sizeof(line),"%s mission %d: choice %d not found",sections[r[0]+1],get32(r+0xc),r[0x2a+choice*12]);
            FUN_0041adf0(file,line_number,(const char *)0x7027d4u,line);ok=0;
        }
    }
    memset(self->state,0,sizeof(self->state));FUN_00561994(stream);return (uint8_t)ok;
}
