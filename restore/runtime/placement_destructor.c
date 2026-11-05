#include "placement_destructor.h"
#include "array_lifecycle.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
extern void FUN_0051f780(void);
/* Whole native51F780 is C3. Its this pointer is unobserved. */
static void destroy_empty_element(void *object){(void)object;FUN_0051f780();}

void restore_placement_destroy_004e5290(uint32_t self)
{
    U(self)=0x56f698u;
    restore_array_destroy((void *)(uintptr_t)(self+12u),52u,1,destroy_empty_element);
}

void restore_placement_destroy_00545fa0(uint32_t self)
{
    /* Keep both native DWORD vtable writes, including the intermediate identity. */
    U(self)=0x5701a0u;
    restore_placement_destroy_004e5290(self);
}

void restore_placement_destroy_00401390(uint32_t self)
{
    U(self)=0x56f438u;
    /* Native state0 covers the array only; base cleanup also runs on unwind. */
    __try{
        restore_array_destroy((void *)(uintptr_t)(self+0x4cu),52u,16,destroy_empty_element);
    }__finally{
        restore_placement_destroy_004e5290(self);
    }
}

void restore_placement_destroy_0040d780(uint32_t self)
{
    U(self)=0x56f48cu;
    /* Native state0 covers the array only; base cleanup also runs on unwind. */
    __try{
        restore_array_destroy((void *)(uintptr_t)(self+0x4cu),52u,25,destroy_empty_element);
    }__finally{
        restore_placement_destroy_004e5290(self);
    }
}

void restore_placement_destroy_00416f00(uint32_t self)
{
    U(self)=0x56f4dcu;
    /* Native state0 covers the array only; base cleanup also runs on unwind. */
    __try{
        restore_array_destroy((void *)(uintptr_t)(self+0x4cu),52u,9,destroy_empty_element);
    }__finally{
        restore_placement_destroy_004e5290(self);
    }
}

void restore_placement_destroy_004e6530(uint32_t self)
{
    U(self)=0x56f6e8u;
    /* Native state0 covers the array only; base cleanup also runs on unwind. */
    __try{
        restore_array_destroy((void *)(uintptr_t)(self+0x4cu),52u,2,destroy_empty_element);
    }__finally{
        restore_placement_destroy_004e5290(self);
    }
}

void restore_placement_destroy_00545630(uint32_t self)
{
    U(self)=0x570160u;
    /* Native state0 covers the array only; base cleanup also runs on unwind. */
    __try{
        restore_array_destroy((void *)(uintptr_t)(self+0x1e5cu),52u,167,destroy_empty_element);
    }__finally{
        restore_placement_destroy_00545fa0(self);
    }
}

void restore_placement_destroy_0054f360(uint32_t self)
{
    U(self)=0x5701e8u;
    /* Native state0 covers the array only; base cleanup also runs on unwind. */
    __try{
        restore_array_destroy((void *)(uintptr_t)(self+0x5c88u),52u,513,destroy_empty_element);
    }__finally{
        restore_placement_destroy_00545fa0(self);
    }
}

void restore_placement_destroy_00555d70(uint32_t self)
{
    U(self)=0x570228u;
    /* Native state0 covers the array only; base cleanup also runs on unwind. */
    __try{
        restore_array_destroy((void *)(uintptr_t)(self+0x4ee0u),52u,437,destroy_empty_element);
    }__finally{
        restore_placement_destroy_00545fa0(self);
    }
}

void restore_placement_destroy_00558ad0(uint32_t self)
{
    U(self)=0x570268u;
    /* Native state0 covers the array only; base cleanup also runs on unwind. */
    __try{
        restore_array_destroy((void *)(uintptr_t)(self+0x1cbcu),52u,158,destroy_empty_element);
    }__finally{
        restore_placement_destroy_00545fa0(self);
    }
}
