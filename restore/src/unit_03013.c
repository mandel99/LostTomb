#include "crt_environment.h"
void *FUN_00561b68(size_t bytes);

typedef int (WINAPI *RestoreWideEnvironmentConverter)(UINT,DWORD,LPCWCH,int,LPSTR,int,LPCCH,LPBOOL);
uint32_t restore_crt_convert_wide_environment(void)
{
 WCHAR **slot=RESTORE_CRT_WIDE_ENVIRONMENT_VECTOR;
 const WCHAR *current=*slot;
 RestoreWideEnvironmentConverter convert;
 if(current==0)return 0;
 /* Native caches the IAT target once for this traversal. */
 convert=WideCharToMultiByte;
 do{
  int32_t bytes=convert(1,0,current,-1,0,0,0,0);
  char *entry;
  if(bytes==0)return UINT32_MAX;
  entry=(char *)FUN_00561b68((size_t)(uint32_t)bytes);
  if(entry==0)return UINT32_MAX;
  /* Allocator may change this cell; native reloads the current wide pointer. */
  if(convert(1,0,*slot,-1,entry,bytes,0,0)==0)return UINT32_MAX;
  (void)FUN_0056aebe(entry,0);
  current=slot[1];
  ++slot;
 }while(current!=0);
 return 0;
}
uint32_t FUN_0056a896(void){return restore_crt_convert_wide_environment();}
