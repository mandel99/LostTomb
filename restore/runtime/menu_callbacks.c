#include "menu_input.h"
#include "family_menu.h"
#include "menu_dialog.h"
#include "campaign_menu.h"
#include "campaign_actions.h"
#include "mission_menu.h"
#include "load_save_menu.h"
#include "high_scores_menu.h"
#include "message_panel_input.h"
#include "tool_selector.h"
#include "dropdown_actions.h"
#include "dropdown_system_actions.h"
#include "dropdown_start_actions.h"
#include "settings_callbacks.h"
#include "remaining_menu_actions.h"
#include "selection_popup.h"
#include "editor_validation.h"
#include "display_confirmation.h"
#include "replay_mission.h"
#include "menu_keyboard_modal.h"
#include "menu_table_actions.h"
#include "menu_training_render.h"
#include "command_dialog_actions.h"
#include "basic_command_actions.h"
#include "command_migration.h"
#include "god_command_actions.h"
#include "god_remaining_commands.h"
#include "actor_command.h"
#include "invasion_commands.h"
#include "numeric_input.h"
#include "numeric_completion.h"
#include "selection_button_actions.h"
#include "detail_button_actions.h"
#include "menu_modal_buttons.h"
#include "unit_button_actions.h"
#include "menu_group_selection.h"
#include "unit_keyboard_actions.h"
#include "menu_modal_buttons2.h"
#include "small_button_actions2.h"
#include "editor_option_buttons.h"
#include "editor_option_buttons2.h"
#include "sidebar_command.h"
#include "sidebar_message_actions.h"
#include "sidebar_undo.h"
#include "editor_modal_actions3.h"
#include "editor_short_actions.h"
#include "editor_city_route_actions.h"
#include "editor_permissions_actions.h"
#include "editor_city_detail_actions.h"
#include "editor_event_list_actions.h"
#include "diplomacy_input_actions.h"
#include "editor_popup_actions.h"
#include "actor_panel_actions.h"
#include "actor_panel_native_context.h"
#include "editor_event_detail_actions.h"
#include "late_menu_actions.h"
#include "remaining_resource_actions.h"
#include "remaining_editor_list_actions.h"
#include "remaining_selection_actions.h"
#include "construction_confirm_actions.h"
#include "menu_economy_actions.h"
#include "menu_remaining_actions.h"
#include "menu_message_actions.h"
#include "menu_labor_actions.h"
#include "figure_menu_actions.h"
#include "storage_order_actions.h"
#include "menu_rotation_actions.h"
#include "menu_advisor_actions.h"
#include "menu_event_actions.h"
#include <stdio.h>
#include <stdlib.h>
extern void FUN_00402bb0(void);
extern void FUN_0041f650(void);
/* 00418350 is another unexported callback; its shared 0051f780 call is a ret. */
static void back_to_mission(void)
{
    if(*(uint8_t *)0xe38ddau==0x35&&*(int8_t *)0xe38eaau>=0) {
        *(uint8_t *)0xe38ddau=0x34;
        *(uint8_t *)0xe38eabu=*(uint8_t *)0xe38eaau;*(uint8_t *)0xe38eaau=0xff;
    }
    *(uint8_t *)0xe38dcau=1;
}
void restore_menu_callback(uint32_t address)
{
    if(restore_menu_event_action(address))return;
    if(restore_menu_advisor_action(address))return;
    if(restore_menu_rotation_action(address))return;
    if(restore_figure_menu_action(address))return;
    if(restore_storage_order_action(address))return;
    if(restore_menu_labor_action(address))return;
    if(restore_menu_remaining_action(address))return;
    if(restore_menu_message_action(address))return;
    if(restore_menu_economy_action(address))return;
    if(restore_construction_confirm_action(address))return;
    if(restore_remaining_resource_action(address))return;
    if(restore_remaining_editor_list_action(address))return;
    if(restore_remaining_selection_action(address))return;
    if(restore_late_menu_action(address))return;
    if(restore_editor_event_detail_action(address))return;
    if(restore_editor_event_detail_completion_action(address))return;
    if(restore_actor_panel_native_action(address))return;
    if(restore_actor_panel_action(address))return;
    if(restore_editor_popup_action(address))return;
    if(restore_diplomacy_input_action(address))return;
    if(restore_editor_event_list_action(address))return;
    if(restore_editor_city_detail_action(address))return;
    if(restore_editor_permissions_action(address))return;
    if(restore_editor_permissions_completion_action(address))return;
    if(restore_editor_city_route_action(address))return;
    if(restore_editor_short_action(address))return;
    if(restore_editor_modal3_action(address))return;
    if(restore_sidebar_undo_action(address))return;
    if(restore_sidebar_message_action(address))return;
    if(restore_sidebar_command_action(address))return;
    if(restore_unit_keyboard_action(address))return;
    if(restore_menu_modal_button2_action(address))return;
    if(restore_small_button_action2(address))return;
    if(restore_editor_option_action(address))return;
    if(restore_editor_option2_action(address))return;
    if(restore_menu_modal_button_action(address))return;
    if(restore_detail_button_action(address))return;
    if(restore_unit_button_action(address))return;
    if(restore_selection_button_action(address))return;
    if(address==0x406720u){FUN_00406720();return;}
    if(restore_numeric_input_action(address))return;
    if(restore_numeric_completion_action(address))return;
    if(restore_invasion_command_action(address))return;
    if(restore_actor_command_action(address))return;
    if(restore_god_remaining_command_action(address))return;
    if(restore_god_command_action(address))return;
    if(restore_migration_command_action(address))return;
    if(restore_basic_command_action(address))return;
    if(restore_command_dialog_action(address))return;
    if(restore_training_render_action(address))return;
    if(restore_dropdown_action(address))return;
    if(restore_dropdown_system_action(address))return;
    if(restore_dropdown_start_action(address))return;
    if(restore_remaining_menu_action(address))return;
    if(restore_selection_popup_action(address))return;
    if(address==0x00409830u){FUN_00409830();return;}
    if(restore_display_confirmation_action(address))return;
    if(address==0x00418510u){FUN_00418510();return;}
    if(restore_menu_modal_action(address))return;
    if(address==0x004ea7f0u){FUN_004ea7f0();return;}
    if(address==0x004ea830u){FUN_004ea830();return;}
    if(address==0x004ea840u){FUN_004ea840();return;}
    if(address==0x00402c10u){FUN_00402c10();return;}
    if(address==0x00402c40u){FUN_00402c40();return;}
    if(address==0x00402cc0u){FUN_00402cc0();return;}
    if(address==0x00402f70u){FUN_00402f70();return;}
    if(address==0x00403060u){FUN_00403060();return;}
    if(address==0x00403130u){FUN_00403130();return;}
    if(address==0x004031a0u){FUN_004031a0();return;}
    if(address==0x00403210u){FUN_00403210();return;}
    if(address==0x00403270u){FUN_00403270();return;}
    if(address==0x004032f0u){FUN_004032f0();return;}
    if(address==0x00403320u){FUN_00403320();return;}
    if(address==0x00403390u){FUN_00403390();return;}
    if(address==0x004ea760u){FUN_004ea760();return;}
    if(address==0x00405200u){FUN_00405200();return;}
    if(address==0x00402450u){FUN_00402450();return;}
    if(address==0x00402540u){FUN_00402540();return;}
    if(address==0x00402630u){FUN_00402630();return;}
    if(address==0x00402720u){FUN_00402720();return;}
    if(address==0x00402980u){FUN_00402980();return;}
    if(address==0x00402a80u){FUN_00402a80();return;}
    if(address==0x00417ff0u) {FUN_00417ff0();return;}
    if(address==0x00402bb0u) {FUN_00402bb0();return;}
    if(address==0x00418220u) {FUN_00418220();return;}
    if(address==0x00418640u) {FUN_00418640();return;}
    if(address==0x00418350u) {back_to_mission();return;}
    if(address==0x00426590u) {FUN_00426590();return;}
    if(address==0x004265b0u) {FUN_004265b0();return;}
    if(address==0x00516890u) {FUN_00516890();return;}
    if(address==0x00516ab0u) {FUN_00516ab0();return;}
    if(address==0x00516990u) {FUN_00516990();return;}
    if(address==0x00516c10u) {FUN_00516c10();return;}
    if(address==0x0040c890u) {restore_family_open_name_choices();return;}
    if(address==0x0040c810u) {restore_family_close_name_choices();return;}
    if(address==0x0040c830u) {restore_family_choose_name();return;}
    if(address==0x00516c60u) {FUN_00516c60();return;}
    if(address==0x00516dd0u) {FUN_00516dd0();return;}
    if(address==0x004cb800u) {FUN_004cb800();return;}
    if(address==0x004cb860u) {FUN_004cb860();return;}
    if(address==0x004cb910u) {FUN_004cb910();return;}
    if(address==0x004cb940u) {FUN_004cb940();return;}
    if(address==0x004cb970u) {FUN_004cb970();return;}
    if(address==0x0041bb10u) {FUN_0041bb10();return;}
    if(address==0x0041bb70u) {FUN_0041bb70();return;}
    if(address==0x0041c310u) {FUN_0041c310();return;}
    if(address==0x0041c350u) {FUN_0041c350();return;}
    if(address==0x0041f650u) {FUN_0041f650();return;}
    if(address==0x0041f460u) {FUN_0041f460();return;}
    if(address==0x0041f670u) {FUN_0041f670();return;}
    if(address==0x0041e0e0u) {FUN_0041e0e0();return;}
    if(address==0x0041f5c0u) {FUN_0041f5c0();return;}
    if(address==0x00402e50u) {FUN_00402e50();return;}
    if(address==0x004c5400u) {FUN_004c5400();return;}
    if(address==0x004c7e90u) {FUN_004c7e90();return;}
    if(address==0x004c7ee0u) {FUN_004c7ee0();return;}
    if(address==0x004c7f30u) {FUN_004c7f30();return;}
    if(address==0x0051f780u) return; /* Original shared callback is a bare ret. */
    fprintf(stderr,"Unreconstructed menu callback %08lx in mode %u.\n",(unsigned long)address,
            *(uint8_t *)0x00e38ddau);
    abort();
}
