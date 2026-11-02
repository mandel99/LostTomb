#include "advisor_shell.h"
#include "kingdom_advisor_render.h"
#include "military_advisor_render.h"
#include "fleet_advisor_render.h"
#include "service_advisor_render.h"
#include "religion_advisor_render.h"
#include "financial_advisor_render.h"
#include "monument_advisor_render.h"
#include "chief_advisor_render.h"
#include "population_advisor_render.h"
#include "ratings_advisor_render.h"
#include <stdint.h>
extern void FUN_00521080(void);
extern void FUN_00559e10(void);
/* Whole520EA0. Read the full selection after the shared shell returns. */
void FUN_00520ea0(void)
{
    FUN_00520f40();
    switch(*(volatile uint32_t *)(uintptr_t)0x605a44u){
    case 1:FUN_00521080();break;
    case 2:FUN_00521b50();break;
    case 3:FUN_00522e10();break;
    case 4:FUN_00523ea0();break;
    case 5:FUN_004f6af0();break;
    case 6:FUN_00559e10();break;
    case 7:FUN_00525cb0();break;
    case 8:FUN_00527bf0();break;
    case 9:FUN_00528360();break;
    case 10:FUN_00528950();break;
    case 11:FUN_005298e0();break;
    case 12:FUN_0052a250();break;
    case 13:FUN_0052c570();break;
    case 14:FUN_0052b520();break;
    default:break;
    }
}
