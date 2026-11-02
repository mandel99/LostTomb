#include "text_templates.h"
#include "menu_building_overlay.h"
#include <stdio.h>
#include <stdlib.h>
void restore_text_templates_initialize(void)
{
    /* Native local constructor 004482d0, absent from the exported C units. */
    ((RestoreTextTemplateContext *)0x784f34u)->entries=(const RestoreTextTemplateEntry *)0x5d2ec8u;
}
const char *restore_text_template_callback(uint32_t address)
{
    /* All26 callbacks from the immutable native5D2EC8 table. Each returns the
       actual C model's pointer; null results and shared buffers stay native. */
    switch(address) {
    case 0x00447800u:
    case 0x00447840u:
    case 0x00447890u:
    case 0x00447910u:
    case 0x00447a30u:
    case 0x00447a80u:
    case 0x00447ab0u:
    case 0x00447ae0u:
    case 0x00447b10u:
    case 0x00447b50u:
    case 0x004479b0u:
    case 0x00447b80u:
    case 0x00447bb0u:
    case 0x00447c20u:
    case 0x00447c30u:
    case 0x00447d60u:
    case 0x00447d80u:
    case 0x00447de0u:
    case 0x00447e30u:
    case 0x00447f70u:
    case 0x00448000u:
    case 0x00448040u:
    case 0x00448070u:
    case 0x00448180u:
    case 0x004481b0u:
    case 0x00448270u:
        return restore_menu_overlay_text_callback(address);
    default:
        fprintf(stderr,"Unknown text-template callback %08lx.\n",(unsigned long)address);
        abort();
    }
}
