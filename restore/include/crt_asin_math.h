#ifndef RESTORE_CRT_ASIN_MATH_H
#define RESTORE_CRT_ASIN_MATH_H
#include <stdint.h>
#if !defined(_MSC_VER) || !defined(_M_IX86)
#error The live x87 primitives and entry adapters require MSVC i386.
#endif

/* Native _exception: binary64 operands/result, NOT generated DAT DWORDs.
   arg2 is unused by the unary dispatcher and is not a logical input. */
struct restore_crt_math_exception {
    int32_t type;
    uint32_t name;
    uint64_t arg1,arg2,result;
};
/* Owned native88-byte scratch. Use byte masks for partially defined fields;
   reading an uninitialized DWORD as a C scalar would invent a logical value. */
struct restore_crt_ieee_record { uint8_t byte[88]; };

uint32_t restore_crt_classify_high(uint32_t high);
uint32_t restore_crt_normalize_math_live(uint32_t *saved_control_slot);
uint32_t restore_crt_quiet_nan_live(uint32_t classification);
uint32_t restore_crt_asin_core_live(uint32_t classification,uint32_t incoming_flags,
                                  uint64_t operand,uint32_t incoming_edx);
void restore_crt_one_arg_error_live(uint32_t type,uint32_t name,uint32_t operation,
                                   uint64_t operand,uint16_t *saved_control);
void restore_crt_math_finish_live(uint32_t saved_control,uint64_t operand,
                                 uint32_t operation,uint32_t name);
void restore_crt_math_dispatch(uint32_t operation,struct restore_crt_math_exception *record,
                               const uint16_t *saved_control);
uint32_t restore_crt_handle_masked_error(uint32_t exceptions,uint64_t *result,uint32_t control);
void restore_crt_raise_ieee(struct restore_crt_ieee_record *record,uint32_t *control,
                            uint32_t exceptions,uint32_t operation,
                            const uint64_t *operand,uint64_t *result);
void restore_crt_set_exponent_live(uint64_t value,int32_t exponent);
void restore_crt_frexp_live(uint64_t value,int32_t *exponent);
int32_t restore_crt_status_wait(void);
int32_t restore_crt_clear_status(void);
int32_t restore_crt_control_word(uint32_t value,uint32_t mask);
int32_t restore_crt_control_word_slot(uint32_t value,uint32_t mask,uint32_t incoming_ecx,
                                     uint32_t *native_mask_slot);
void restore_crt_generate_exceptions(uint32_t exceptions);

/* Only these declarations describe the physical entrypoints. The void
   numeric entries produce ST0; declaring double/float10 would round/pop it. */
void FUN_00562d00(void);
void FUN_00562d1d(void);
void FUN_00567517(void);
void FUN_00567575(void);
void FUN_0056758c(void);
void FUN_005675e8(void);
void FUN_0056760b(void); /* Tail entry: [ESP] is CW scratch, then return PC. */
void FUN_0056971f(uint32_t,struct restore_crt_math_exception *,const uint16_t *);
void FUN_00567f58(struct restore_crt_ieee_record *,uint32_t *,uint32_t,uint32_t,
                  const uint64_t *,uint64_t *);
uint32_t FUN_0056820b(uint32_t,uint64_t *,uint32_t);
void FUN_00568448(uint64_t,int32_t);
void FUN_00568471(uint64_t,int32_t *);
int32_t FUN_00568532(void);
int32_t FUN_00568540(void);
int32_t FUN_0056854f(uint32_t,uint32_t);
void FUN_00568572(uint32_t);
#endif
