#include "city_ui_layout.h"
#include <stdint.h>
#define U(a) (*(uint32_t *)(uintptr_t)(a))
void FUN_0051e930(void)
{
    uint32_t width=U(0x00e38db0u);
    if(width==640){U(0x00f53f64u)=478;U(0x00f53f60u)=598;return;}
    if(width==800){U(0x00f53f64u)=598;U(0x00f53f60u)=718;return;}
    if(width==1024){U(0x00f53f64u)=838;U(0x00f53f60u)=958;}
}
