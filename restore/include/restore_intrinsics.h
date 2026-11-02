/* Generated integer helpers; input and output widths are bytes. */
static __inline unsigned int CARRY4(unsigned __int64 a,unsigned __int64 b) { a &= 0xffffffffui64; b &= 0xffffffffui64; return (unsigned int)((a+b)>0xffffffffui64); }
static __inline unsigned int CONCAT11(unsigned __int64 a,unsigned __int64 b) { return (unsigned int)(((a & 0xffui64) << 8) | (b & 0xffui64)); }
static __inline unsigned int CONCAT12(unsigned __int64 a,unsigned __int64 b) { return (unsigned int)(((a & 0xffui64) << 16) | (b & 0xffffui64)); }
static __inline unsigned int CONCAT13(unsigned __int64 a,unsigned __int64 b) { return (unsigned int)(((a & 0xffui64) << 24) | (b & 0xffffffui64)); }
static __inline unsigned __int64 CONCAT14(unsigned __int64 a,unsigned __int64 b) { return (unsigned __int64)(((a & 0xffui64) << 32) | (b & 0xffffffffui64)); }
static __inline unsigned __int64 CONCAT16(unsigned __int64 a,unsigned __int64 b) { return (unsigned __int64)(((a & 0xffui64) << 48) | (b & 0xffffffffffffui64)); }
static __inline unsigned __int64 CONCAT17(unsigned __int64 a,unsigned __int64 b) { return (unsigned __int64)(((a & 0xffui64) << 56) | (b & 0xffffffffffffffui64)); }
static __inline unsigned int CONCAT21(unsigned __int64 a,unsigned __int64 b) { return (unsigned int)(((a & 0xffffui64) << 8) | (b & 0xffui64)); }
static __inline unsigned int CONCAT22(unsigned __int64 a,unsigned __int64 b) { return (unsigned int)(((a & 0xffffui64) << 16) | (b & 0xffffui64)); }
static __inline unsigned __int64 CONCAT24(unsigned __int64 a,unsigned __int64 b) { return (unsigned __int64)(((a & 0xffffui64) << 32) | (b & 0xffffffffui64)); }
static __inline unsigned __int64 CONCAT26(unsigned __int64 a,unsigned __int64 b) { return (unsigned __int64)(((a & 0xffffui64) << 48) | (b & 0xffffffffffffui64)); }
static __inline unsigned int CONCAT31(unsigned __int64 a,unsigned __int64 b) { return (unsigned int)(((a & 0xffffffui64) << 8) | (b & 0xffui64)); }
static __inline unsigned __int64 CONCAT44(unsigned __int64 a,unsigned __int64 b) { return (unsigned __int64)(((a & 0xffffffffui64) << 32) | (b & 0xffffffffui64)); }
static __inline unsigned int SBORROW2(unsigned __int64 a,unsigned __int64 b) { a &= 0xffffui64; b &= 0xffffui64; return (unsigned int)((((a^b)&(a^((a-b)&0xffffui64)))>>15)&1); }
static __inline unsigned int SBORROW4(unsigned __int64 a,unsigned __int64 b) { a &= 0xffffffffui64; b &= 0xffffffffui64; return (unsigned int)((((a^b)&(a^((a-b)&0xffffffffui64)))>>31)&1); }
static __inline unsigned int SUB41(unsigned __int64 value,unsigned int offset) { return (unsigned int)((value >> (offset*8)) & 0xffui64); }
static __inline unsigned int SUB42(unsigned __int64 value,unsigned int offset) { return (unsigned int)((value >> (offset*8)) & 0xffffui64); }
static __inline unsigned __int64 SUB87(unsigned __int64 value,unsigned int offset) { return (unsigned __int64)((value >> (offset*8)) & 0xffffffffffffffui64); }

/* Evidence-backed v7 replacements for correctly typed x87 scalar operations.
   MSVC x86 evaluates these through the CRT math entry points and retains the
   original double/long-double source widths used by the recovered expressions. */
#include <math.h>
static __inline long double restore_x87_sqrt(long double value) { return (long double)sqrt((double)value); }
static __inline long double restore_x87_fpatan(long double y,long double x) { return (long double)atan2((double)y,(double)x); }
#ifndef __builtin_isnan
#define __builtin_isnan(value) _isnan((double)(value))
#endif
