#include "music_manager.h"
#include "campaign_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern void *FUN_00561a0a(const char *,const char *);
extern char *FUN_00561e4d(char *,int,void *);
extern int FUN_00561994(void *);
void FUN_00539250(RestoreMusicManager *self)
{
    void *stream=FUN_00561a0a("music.txt","rt");
    char line[200],path[100];int category=0;int32_t delay=0,minimum=1,maximum=100;
    if(!stream)return;
    while(FUN_00561e4d(line,sizeof(line),stream)) {
        char *name,*type,*token;int ambient,written;void *test;
        if(line[0]==';'||!line[0])continue;
        name=FUN_00561db1(line," ,\t\n");if(!name)continue;
        if(!strcmp(name,"AMBIENT")) {
            if(category>=0&&category<53) {
                RestoreMusicCategory *c=&self->categories[category];
                if(c->count>=0&&c->count<50)c->ambient_after[c->count]=1;
            }
            continue;
        }
        type=FUN_00561db1(NULL," ,\t\n");if(!type)continue;
        ambient=!strcmp(type,"A");
        token=FUN_00561db1(NULL," ,\t\n");if(token)category=(int32_t)strtol(token,NULL,10);
        token=FUN_00561db1(NULL," ,\t\n");if(token)delay=(int32_t)strtol(token,NULL,10);
        token=FUN_00561db1(NULL," ,\t\n");if(token)minimum=(int32_t)strtol(token,NULL,10);
        token=FUN_00561db1(NULL," ,\t\n");if(token)maximum=(int32_t)strtol(token,NULL,10);
        written=snprintf(path,sizeof(path),"audio\\music\\%s",name);
        if(written<0||(size_t)written>=sizeof(path))continue;
        test=FUN_00561a0a(path,"rb");
        if(test)FUN_00561994(test);
        else if(*(uint8_t *)0xe38e62u) {
            written=snprintf(path,sizeof(path),"%c:\\audio\\music\\%s",*(uint8_t *)0xe38e62u,name);
            if(written<0||(size_t)written>=sizeof(path))continue;
        }
        /* Both native callers use the global manager for track insertion;
           the explicit receiver is used for AMBIENT markers. */
        FUN_005395e0(RESTORE_MUSIC,path,category,delay,minimum,maximum,ambient);
    }
    FUN_00561994(stream);
}
