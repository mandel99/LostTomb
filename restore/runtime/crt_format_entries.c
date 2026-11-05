#include "crt_format.h"
/* Native cdecl entries retain their stack arguments and real pointer result. */
char *FUN_0056452a(const double *v,char *b,int32_t f,int32_t p,int32_t c){return restore_crt_format_float(v,b,f,p,c);}
char *FUN_00564263(const double *v,char *b,int32_t p,int32_t c){return restore_crt_format_scientific(v,b,p,c);}
char *FUN_00564367(const double *v,char *b,int32_t p){return restore_crt_format_fixed(v,b,p);}
char *FUN_00564445(const double *v,char *b,int32_t p,int32_t c){return restore_crt_format_general(v,b,p,c);}
char *FUN_005644e0(const double *v,char *b,int32_t p,int32_t c){return restore_crt_general_scientific(v,b,p,c);}
char *FUN_00564507(const double *v,char *b,int32_t p){return restore_crt_general_fixed(v,b,p);}
RestoreCrtDecimal *FUN_00568d88(uint64_t v){return restore_crt_binary64_decimal(v);}
uint32_t FUN_0056a47e(uint32_t l,uint32_t h,uint32_t e,int32_t n,uint32_t f,RestoreCrtDecimalDigits *d){return restore_crt_raw80_decimal(l,h,e,n,f,d);}
void FUN_0056a904(RestoreCrt96 *a,const RestoreCrt96 *b){restore_crt_multiply_raw96(a,b);}
void FUN_0056ab24(RestoreCrt96 *v,int32_t e,int32_t p){restore_crt_scale_raw96(v,e,p);}
