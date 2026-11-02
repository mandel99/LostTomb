#include "sprite_warning.h"
#include <windows.h>
#include <string.h>
#define s_Graphic_file_out_of_sync_00601858 (*(char (*)[25])0x00601858u)
#define s_The_file___00601874 (*(char (*)[11])0x00601874u)
#define s___contains_00601880 (*(char (*)[12])0x00601880u)
#define s_tags__whereas_the_program_expect_0060188c (*(char (*)[37])0x0060188cu)
#define s_tags__This_could_very_well_lead_t_006018b4 (*(char (*)[45])0x006018b4u)
static void warning_text(char **out,const char *s)
{while(*s)*(*out)++=*s++;}
static void warning_int32(char **out,uint32_t bits)
{
 char reverse[10];unsigned n=0;
 if(bits&0x80000000u){*(*out)++='-';bits=0u-bits;}
 do{reverse[n++]=(char)('0'+bits%10u);bits/=10u;}while(bits);
 while(n)*(*out)++=reverse[--n];
}
uint8_t FUN_0053dd90(RestoreSpriteLibraries *self,const char *name,int index)
{
 RestoreSpriteLibrary *library;uint32_t first_image=0,first_tag=0,expected;
 uint8_t result=1;int fresh,cached=0;
 if(!name)return 0;
 if(index==-1)index=(int32_t)self->loaded;
 if((uint32_t)index>=30)return 0;
 library=&self->libraries[index];fresh=library->image_count==0;
 if(fresh){first_image=self->next_image;first_tag=self->next_tag;}
 else{
  cached=_stricmp(name,library->name)==0;self->pixel_bytes-=library->pixel_bytes;
  if(!cached){first_image=library->first_image;first_tag=library->first_tag;}
 }
 if(!cached){
  result=FUN_0053dff0(library,name,index,&self->images[first_image],first_image,&self->tags[first_tag],first_tag);
  if(result&&fresh){self->next_image+=library->image_count;self->next_tag+=library->tag_count;++self->loaded;}
 }
 self->pixel_bytes+=library->pixel_bytes;
 expected=self->tag_limits[index]-1u-(index?self->tag_limits[index-1]-1u:0u);
 if(expected!=library->tag_count){
  char message[RESTORE_SPRITE_WARNING_CAPACITY],*out=message;
  warning_text(&out,s_The_file___00601874);warning_text(&out,name);
  warning_text(&out,s___contains_00601880);warning_int32(&out,library->tag_count);
  warning_text(&out,s_tags__whereas_the_program_expect_0060188c);warning_int32(&out,expected);
  warning_text(&out,s_tags__This_could_very_well_lead_t_006018b4);*out=0;
  /* 53DF82..53DF8A: HWND is a full DWORD, MB_OK/type=0. Native restores
     its saved AL after stream destruction; the MessageBox result is unused. */
  (void)MessageBoxA((HWND)(uintptr_t)*(uint32_t *)(uintptr_t)0xe39140u,message,s_Graphic_file_out_of_sync_00601858,0);
 }
 return result;
}
