#include "dropdown_menu.h"
extern void restore_menu_callback(uint32_t token);
void restore_dropdown_callback(uint32_t token)
{
    switch(token){
    case 0x403410u:FUN_00403410();return;
    case 0x403490u:FUN_00403490();return;
    case 0x403500u:FUN_00403500();return;
    case 0x403540u:FUN_00403540();return;
    case 0x40c7d0u:FUN_0040c7d0();return;
    default:restore_menu_callback(token);return;
    }
}
