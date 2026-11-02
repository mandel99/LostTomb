/* Fail closed when the decompiler consumes a return it could not recover. */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
unsigned int restore_unresolved_return(unsigned int address)
{
    fprintf(stderr, "Unresolved return register at original function 0x%08x\n", address);
    abort();
}
int restore_interrupt3(void)
{
    __debugbreak();
    abort();
}

unsigned int restore_pointer32_checked(const void *value,unsigned int origin)
{
    uintptr_t pointer=(uintptr_t)value;
    if (pointer>0xffffffffu) {
        fprintf(stderr,"Pointer wider than recovered x86 value after 0x%08x: %p\n",origin,value);
        abort();
    }
    return (unsigned int)pointer;
}
