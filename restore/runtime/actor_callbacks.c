#include "actor_update.h"
#include "actor_callbacks_human.h"
#include "actor_callbacks_small.h"
#include "actor_callbacks_chain.h"
#include "actor_callback_496140.h"
#include "population_updates.h"
#include "actor_laborer.h"
#include "actor_civic.h"
#include "actor_constable.h"
#include "actor_transport.h"
#include <stdio.h>
#include <stdlib.h>
static void empty_callback(void) {}
RestoreActorCallback restore_actor_callback_resolve(uint32_t address)
{
    switch(address) {
    case 0x004ac9f0u:return FUN_004ac9f0;
    case 0x004aceb0u:return FUN_004aceb0;
    case 0x004adf60u:return FUN_004adf60;
    case 0x004aee10u:return FUN_004aee10;
    case 0x004af140u:return FUN_004af140;
    case 0x004af4c0u:return FUN_004af4c0;
    case 0x004af4e0u:return FUN_004af4e0;
    case 0x004af7c0u:return FUN_004af7c0;
    case 0x004af970u:return FUN_004af970;
    case 0x004afc40u:return FUN_004afc40;
    case 0x004afeb0u:return FUN_004afeb0;
    case 0x004b0ec0u:return FUN_004b0ec0;
    case 0x004a1ef0u:return FUN_004a1ef0;
    case 0x004a2430u:return FUN_004a2430;
    case 0x004a3070u:return FUN_004a3070;
    case 0x004a3b70u:return FUN_004a3b70;
    case 0x004a4fd0u:return FUN_004a4fd0;
    case 0x004a7030u:return FUN_004a7030;
    case 0x0049b750u:return FUN_0049b750;
    case 0x0049c750u:return FUN_0049c750;
    case 0x0049d0f0u:return FUN_0049d0f0;
    case 0x0049dc50u:return FUN_0049dc50;
    case 0x0049e8b0u:return FUN_0049e8b0;
    case 0x0049f710u:return FUN_0049f710;
    case 0x004a04d0u:return FUN_004a04d0;
    case 0x004a1130u:return FUN_004a1130;
    case 0x00494f70u:return FUN_00494f70;
    case 0x00497010u:return FUN_00497010;
    case 0x00497450u:return FUN_00497450;
    case 0x0049a150u:return FUN_0049a150;
    case 0x0049a5a0u:return FUN_0049a5a0;
    case 0x0049a880u:return FUN_0049a880;
    case 0x0049ac00u:return FUN_0049ac00;
    case 0x0049aeb0u:return FUN_0049aeb0;
    case 0x0049b280u:return FUN_0049b280;
    case 0x0049b610u:return FUN_0049b610;
    case 0x00491a90u:return FUN_00491a90;
    case 0x004923a0u:return FUN_004923a0;
    case 0x00492580u:return FUN_00492580;
    case 0x00492770u:return FUN_00492770;
    case 0x004930b0u:return FUN_004930b0;
    case 0x004937a0u:return FUN_004937a0;
    case 0x0048e9e0u:return FUN_0048e9e0;
    case 0x00490b50u:return FUN_00490b50;
    case 0x00490e00u:return FUN_00490e00;
    case 0x00491420u:return FUN_00491420;
    case 0x00494800u:return FUN_00494800;
    case 0x00494b30u:return FUN_00494b30;
    case 0x00497dc0u:return FUN_00497dc0;
    case 0x00497fc0u:return FUN_00497fc0;
    case 0x00498160u:return FUN_00498160;
    case 0x00498370u:return FUN_00498370;
    case 0x004984a0u:return FUN_004984a0;
    case 0x00498590u:return FUN_00498590;
    case 0x004986b0u:return FUN_004986b0;
    case 0x00498750u:return FUN_00498750;
    case 0x004987f0u:return FUN_004987f0;
    case 0x004988b0u:return FUN_004988b0;
    case 0x00498a10u:return FUN_00498a10;
    case 0x00498ad0u:return FUN_00498ad0;
    case 0x00498c60u:return FUN_00498c60;
    case 0x00498df0u:return FUN_00498df0;
    case 0x00499010u:return FUN_00499010;
    case 0x0051f780u:return empty_callback;
    case 0x0049bd40u:return FUN_0049bd40;
    case 0x00489ca0u:return restore_actor_00489ca0;
    case 0x0048a260u:return restore_actor_0048a260;
    case 0x0049b390u:return FUN_0049b390;
    case 0x00496f20u:return FUN_00496f20;
    case 0x00493ec0u:return FUN_00493ec0;
    case 0x00494010u:return FUN_00494010;
    case 0x004a2e90u:return FUN_004a2e90;
    case 0x0049b400u:return FUN_0049b400;
    case 0x00496140u:return restore_actor_00496140;
    case 0x004b03b0u:return FUN_004b03b0;
    case 0x004a98b0u:return FUN_004a98b0;
    case 0x004a7600u:return FUN_004a7600;
    case 0x004a7ea0u:return FUN_004a7ea0;
    case 0x004a8800u:return FUN_004a8800;
    case 0x004b0760u:return FUN_004b0760;
    case 0x004ab980u:return FUN_004ab980;
    case 0x004af2c0u:return FUN_004af2c0;
    case 0x00494200u:return FUN_00494200;
    case 0x004942f0u:return FUN_004942f0;
    case 0x004943e0u:return FUN_004943e0;
    case 0x004944d0u:return FUN_004944d0;
    case 0x004945c0u:return FUN_004945c0;
    case 0x004946b0u:return FUN_004946b0;
    case 0x004abd50u:return FUN_004abd50;
    case 0x0048bff0u:return FUN_0048bff0;
    case 0x0048c6a0u:return FUN_0048c6a0;
    case 0x004ac450u:return FUN_004ac450;
    case 0x004941b0u:return FUN_004941b0;
    case 0x004947a0u:return FUN_004947a0;
    case 0x0049b2a0u:return FUN_0049b2a0;
    case 0x0048c200u:return FUN_0048c200;
    case 0x004abe40u:return FUN_004abe40;
    case 0x0048ded0u:return FUN_0048ded0;
    case 0x0048a720u:return FUN_0048a720;
    case 0x0048c2b0u:return FUN_0048c2b0;
    case 0x0048b620u:return FUN_0048b620;
    case 0x0048ce30u:return FUN_0048ce30;
    default:return NULL;
    }
}
void restore_actor_callback(uint32_t address)
{
    RestoreActorCallback callback=restore_actor_callback_resolve(address);
    if(callback) {callback();return;}
    fprintf(stderr,"Unreconstructed actor callback %08lx for actor %lu.\n",
        (unsigned long)address,(unsigned long)*(uint32_t *)0xd056f8u);
    abort();
}
