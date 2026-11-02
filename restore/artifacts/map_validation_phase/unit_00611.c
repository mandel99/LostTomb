#include "map_validation.h"
/* Original returns AL only; all 27 cached return consumers test AL. */
uint8_t FUN_00442870(uint32_t kind)
{
    uint32_t mapped;
    MVE(0x442870,kind,0);
    if(kind==12u){
        if(FUN_00443a80(12))return 1;
        if(!FUN_00442870(9))return 0;
        return FUN_00442870(11);
    }
    mapped=kind;
    switch(kind){
    case 2:mapped=9;break;case 9:mapped=1;break;case 10:mapped=29;break;
    case 13:mapped=11;break;case 15:mapped=14;break;case 17:mapped=16;break;
    case 19:mapped=18;break;case 23:mapped=22;break;case 28:mapped=20;break;
    case 33:mapped=32;break;default:break;
    }
    if(FUN_00443a80(kind))return 1;
    if(kind==10u){
        if(FUN_00443a80(mapped)&&MVR(0x784d32u,2))return 1;
        if(!FUN_00443820(mapped))return 0;
        return MVR(0x784d32u,2)!=0;
    }
    if(kind==28u){
        if(FUN_00443a80(mapped)&&MVR(0x784d34u,2))return 1;
        if(!FUN_00443820(mapped))return 0;
        return MVR(0x784d34u,2)!=0;
    }
    if(kind==34u){
        if(FUN_00443a80(kind))return 1;
        if(!FUN_00442870(31))return 0;
        return FUN_00442870(13);
    }
    if(kind!=9u&&FUN_00443a80(mapped))return 1;
    return FUN_00443820(mapped)!=0;
}
