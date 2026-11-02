#include "restore_types.h"
extern undefined1 * FUN_00564263();
extern undefined1 * FUN_00564367();
extern void FUN_00564445();

errno_t __cdecl
FUN_0056452a(double *arg,char *buffer,size_t sizeInBytes,int format,int precision,int caps)

{
  errno_t eVar1;
  
  if ((sizeInBytes == 0x65) || (sizeInBytes == 0x45)) {
    eVar1 = FUN_00564263(arg,buffer,format,precision);
  }
  else {
    if (sizeInBytes == 0x66) {
      eVar1 = FUN_00564367(arg,buffer,format);
      return eVar1;
    }
    eVar1 = (FUN_00564445(arg,buffer,format,precision), restore_unresolved_return(0x00564445u));
  }
  return eVar1;
}
