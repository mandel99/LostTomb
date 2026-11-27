#include "pharaoh/ui/city_ingame_dispatch.h"

#include "pharaoh/building/kingdom_access.h"
#include "pharaoh/core/static_objects.h"
#include "pharaoh/monuments/definitions.h"
#include "pharaoh/scenario/definition_counts.h"
#include "pharaoh/ui/window_layout_key.h"

#include <stddef.h>

static int32_t signed_bits(uint32_t value)
{
    return value <= INT32_MAX ? (int32_t)value :
        (int32_t)((int64_t)value - INT64_C(0x100000000));
}

static int32_t add(int32_t a, int32_t b)
{
    return signed_bits((uint32_t)a + (uint32_t)b);
}

static uint8_t tutorial_unlocked(const PharaohBuildMenuContext *c)
{
    return pharaoh_mission_is_unlocked(c->settings->mission_id,
        c->final_mission_id, c->tutorial_flags[13]);
}

/* Native 403F10: retain the separate category argument and live selected
   category. The religion branch reads both in the original executable. */
int32_t pharaoh_city_build_menu_next_item(PharaohBuildMenuContext *c,
    int32_t category, int32_t previous)
{
    uint32_t row = (uint32_t)previous + 1u;
    uint32_t phase = row + 1u;

    if (signed_bits(row) >= PHARAOH_BUILD_MENU_ITEMS) return 0;
    do {
        const uint32_t index = (uint32_t)category * 30u + row;
        const int32_t kind = c->types[index];
        const PharaohScenarioPlayerRuntime *player;

        if (kind <= 0) return 0;
        switch (category) {
            case 6:
                if (kind == 0x30) {
                    if (tutorial_unlocked(c)) {
                        player = &c->supply->players[*c->supply->current_player];
                        if (pharaoh_native_three_nonzero_count_004f5020(
                            player->monument_definition_ids) <
                            pharaoh_scenario_goal_text_count(c->goal_text_ids))
                            return signed_bits(row);
                    }
                } else {
                    const uint32_t other =
                        (uint32_t)*c->selected_category * 30u + row;
                    if (c->types[other] != 0x61) {
                        if (c->enabled[index] != 0) return signed_bits(row);
                    } else {
                        player = &c->supply->players[*c->supply->current_player];
                        if (player->temple_complex_enabled == 0) {
                            if (c->enabled[other] != 0) return signed_bits(row);
                        } else {
                            const PharaohBuildingTypeRecord *buildings =
                                c->monuments->demand->complex->buildings;
                            if (!pharaoh_temple_upgrade_present(buildings,
                                player->temple_complex_building_id, 1))
                                return signed_bits(row);
                            player = &c->supply->players[
                                *c->supply->current_player];
                            if (!pharaoh_temple_upgrade_present(buildings,
                                player->temple_complex_building_id, 2))
                                return signed_bits(row);
                        }
                    }
                }
                break;
            case 10:
                if (kind != 4) {
                    if (c->enabled[index] != 0) return signed_bits(row);
                } else if (tutorial_unlocked(c)) {
                    if (pharaoh_monument_guild_is_required(c->monuments, 0xb3))
                        return signed_bits(row);
                    if (pharaoh_monument_guild_is_required(c->monuments, 0xb2))
                        return signed_bits(row);
                    if (pharaoh_monument_guild_is_required(c->monuments, 0xb1))
                        return signed_bits(row);
                }
                break;
            case 0x16:
                if (tutorial_unlocked(c) &&
                    pharaoh_monument_guild_is_required(c->monuments, kind))
                    return signed_bits(row);
                break;
            case 0x1c:
                if (tutorial_unlocked(c)) {
                    player = &c->supply->players[*c->supply->current_player];
                    if (player->monument_definition_ids[row] == 0)
                        return signed_bits(row);
                }
                break;
            case 0x24:
                player = &c->supply->players[*c->supply->current_player];
                if (!pharaoh_temple_upgrade_present(
                    c->monuments->demand->complex->buildings,
                    player->temple_complex_building_id, phase))
                    return signed_bits(row);
                break;
            default:
                if (c->enabled[index] != 0) return signed_bits(row);
                break;
        }
        ++row;
        ++phase;
    } while (signed_bits(phase) < 31);
    return 0;
}

static void reset_drag(const PharaohCityBuildSelectionContext *c)
{
    const PharaohConstructionInputContext *input = c->construction;
    *input->drag_started = 0;
    *input->start_x = 0;
    *input->end_x = 0;
    *input->start_y = 0;
    *input->end_y = 0;
    *input->redraw = 1;
}

static void select_editor_pair(const PharaohCityBuildSelectionContext *c,
    int32_t category, int32_t tool)
{
    *c->editor_category = category;
    *c->construction->tool = tool;
}

static void resize_popup(const PharaohCityBuildSelectionContext *c)
{
    const int32_t count = pharaoh_build_menu_count_items(c->menu);
    *c->popup_count = count;
    *c->popup_y_offset = c->popup_sizes[count].y_offset;
    *c->popup_width = c->popup_sizes[count].width;
}

/* Native 403D70. Categories 1,2,3 bypass the popup and select item 1. */
void pharaoh_city_sidebar_build_click(const PharaohCityBuildSelectionContext *c)
{
    const PharaohConstructionInputContext *input = c->construction;
    if (*input->current_window == 6 && *c->menu->editor_active == 1) return;
    *input->current_window = 6;
    *c->menu->selected_category = signed_bits(*c->button_parameter);
    if (*c->button_parameter == 1 && *c->scenario_mode == 1)
        *c->menu->selected_category = 13;
    resize_popup(c);
    const int32_t category = *c->menu->selected_category;
    if (category == 2 || category == 3 || category == 1) {
        *c->button_parameter = 1;
        pharaoh_city_build_menu_set_type(c);
    }
    *input->redraw = 1;
}

/* Native 405200. Existing complete ToolSelector C is normalized without
   retaining its absolute-address macros or its native callback tokens. */
void pharaoh_city_build_menu_set_type(const PharaohCityBuildSelectionContext *c)
{
    const PharaohConstructionInputContext *input = c->construction;
    PharaohConstructionRequirements *r = c->requirements;
    int32_t category, selection, tool;

    if (*input->ferry_state == 1)
        pharaoh_construction_ferry_cancel(input->ferry_state,
            input->buildings, *input->ferry_first_building);
    if (*input->ferry_state == 2) *input->ferry_state = 0;
    category = *c->menu->selected_category;
    r->floodplain_count = 0;
    r->road = 0;
    r->wall = 0;
    r->water_10004 = 0;
    r->forest = 0;
    r->terrain_10800 = 0;
    r->water_40000 = 0;
    *c->terrain_rule = NULL;
    *c->selection_tick = (uint32_t)*c->current_tick;
    *c->variation = 0;
    *c->selected_cell = 0;
    *c->anchor_cell = 0;
    *c->selection_locked = 0;
    selection = signed_bits(*c->button_parameter);

    switch (category) {
        case 0x0e:
            *input->current_window = 1;
            *c->editor_brush_size = add(selection, -1);
            reset_drag(c);
            return;
        case 0x25:
            *input->current_window = 1;
            *c->editor_animal = (int16_t)((uint32_t)selection - 1u);
            reset_drag(c);
            return;
        case 0x1b:
            switch (selection) {
                case 1: select_editor_pair(c, 2, 0x76); break;
                case 2: select_editor_pair(c, 0x1a, 0x86); break;
                case 3: select_editor_pair(c, 0x1d, 0xc5); break;
                case 4: select_editor_pair(c, 0x1b, 8); break;
            }
            *input->current_window = 1;
            reset_drag(c);
            return;
        case 0x0f:
            *input->current_window = 1;
            *c->editor_entry_point = selection;
            if (selection == 1) select_editor_pair(c, 0x0f, 0x7f);
            else if (selection == 2) select_editor_pair(c, 0x10, 0x80);
            reset_drag(c);
            return;
        case 0x12:
            *input->current_window = 1;
            if (selection == 1) select_editor_pair(c, 0x12, 0x82);
            else if (selection == 2) select_editor_pair(c, 0x13, 0x83);
            reset_drag(c);
            return;
        case 0x13:
            *input->current_window = 1;
            if (selection == 1) select_editor_pair(c, 0x15, 0x58);
            else if (selection == 2) select_editor_pair(c, 0x16, 0x59);
            else if (selection == 3) select_editor_pair(c, 0x17, 0x5d);
            reset_drag(c);
            return;
        case 0x1a:
            if (selection <= 8) {
                select_editor_pair(c, 0x18, 0x84);
                *c->editor_special_point = selection;
            } else if (selection <= 12) {
                select_editor_pair(c, 0x19, 0x85);
                *c->editor_river_point = add(selection, -8);
            } else if (selection <= 16) {
                select_editor_pair(c, 0x1c, 0x8b);
                *c->editor_river_point = add(selection, -12);
            } else {
                select_editor_pair(c, 0x1f, 0xd6);
                *c->editor_other_point = add(selection, -16);
            }
            *input->current_window = 1;
            reset_drag(c);
            return;
        case 0x11:
            select_editor_pair(c, 0x0d, 0x7e);
            *c->editor_invasion_point = selection;
            *input->current_window = 1;
            reset_drag(c);
            return;
        case 0x10:
            *input->current_window = 1;
            if (selection == 1) select_editor_pair(c, 0x0b, 0x7b);
            else if (selection == 2) select_editor_pair(c, 0x0c, 0x7c);
            else if (selection == 3) select_editor_pair(c, 9, 0x81);
            reset_drag(c);
            return;
        case 0:
            *input->current_window = 1;
            tool = c->menu->types[(uint32_t)selection - 1u];
            *input->tool = tool;
            *c->editor_category = tool == 0xc6 ? 0x1e : tool == 0xdf ? 0x21 : 5;
            reset_drag(c);
            return;
        case 0x1c: {
            const PharaohMonumentDefinition *definition =
                &pharaoh_monument_definitions[
                    c->goal_definition_ids[(uint32_t)selection - 1u]];
            if (definition->kind == 1) {
                tool = 0xb7;
                *input->tool = tool;
                *c->obelisk_size = definition->size;
                goto ordinary_flags;
            }
            if (definition->subkind == 1) {
                const uint16_t saved = c->cities[
                    *c->menu->supply->current_player].sphinx_stage;
                *input->tool = 0xd2;
                c->cities[*c->menu->supply->current_player].sphinx_stage = 1;
                pharaoh_sphinx_load_stage_libraries(c->sprite_libraries,
                    (int16_t)c->cities[
                        *c->menu->supply->current_player].sphinx_stage,
                    c->sphinx_stage_library_names, c->sphinx_fixed_library_name);
                c->cities[*c->menu->supply->current_player].sphinx_stage = saved;
                tool = *input->tool;
                goto classify;
            }
            switch (definition->subkind) {
                case 3: tool = 0xd5; break;
                case 6: tool = 0xe1; break;
                case 14: tool = 0xe6; break;
                case 7: tool = 0xe3; break;
                case 10: tool = 0xe5; break;
                case 11: tool = 0xea; break;
                case 12: tool = 0xeb; break;
                case 13: tool = 0xec; break;
                case 5: tool = 0xe4; break;
                case 4: tool = 0xde; break;
                case 2:
                    tool = add(definition->size, 0xcf);
                    *c->pyramid_choice = selection;
                    *input->tool = tool;
                    goto classify;
                default:
                    tool = *input->tool;
                    goto classify;
            }
            *input->tool = tool;
            goto ordinary_flags;
        }
        default:
            tool = c->menu->types[(uint32_t)category * 30u +
                (uint32_t)selection - 1u];
            *input->tool = tool;
            break;
    }

classify:
    {
        int32_t submenu = -1;
        switch (tool) {
            case 2: submenu = 0x14; break;
            case 3: submenu = 0x15; break;
            case 4: submenu = 0x16; break;
            case 0x30: submenu = 0x1c; break;
            case 0x34: submenu = 0x22; break;
            case 0x5b: submenu = 0x23; break;
            case 0xae: submenu = 0x1e; break;
            case 0xaf: submenu = 0x1f; break;
            case 0xb0: submenu = 0x20; break;
            case 0x60: submenu = 0x17; break;
            case 0x61: {
                const PharaohScenarioPlayerRuntime *player =
                    &c->menu->supply->players[*c->menu->supply->current_player];
                submenu = player->temple_complex_enabled != 0 ? 0x24 : 0x18;
                break;
            }
            case 0x96: submenu = 0x1d; break;
            case 0x39: submenu = 0x19; break;
        }
        if (submenu != -1) {
            *c->menu->selected_category = submenu;
            resize_popup(c);
            *input->tool = 0;
            reset_drag(c);
            return;
        }
    }
    switch (tool) {
        case 0x66: case 0x64: case 0x67: case 0x68: case 0x65:
        case 0xe0: case 0x69: case 0xc4:
            r->terrain_10800 = 1;
            goto selected;
        case 0xc2:
            r->terrain_10800 = 0;
            goto selected;
        case 0x6a:
            *c->terrain_rule = pharaoh_construction_terrain_rule_00405b80;
            goto selected;
        case 0xdd: case 0x6b: case 0xd8: case 0xa2:
            *c->terrain_rule = pharaoh_construction_terrain_rule_00405b60;
            goto selected;
        case 0xa1: case 0xd9:
            *c->terrain_rule = pharaoh_construction_terrain_rule_00405b40;
            goto selected;
        case 0x6d:
            r->water_10004 = 1;
            goto selected;
        case 0x3a: case 0xaa: case 0xab: case 0xc8: case 0xc9: case 0xca:
            *c->variation = 1;
            goto selected;
    }

ordinary_flags:
    if (tool >= 0x8c && tool <= 0x95) *c->variation = 1;
    else switch (tool) {
        case 0x38: *c->variation = 1; break;
        case 0x3b: case 0xac: case 0xad: r->wall = 1; break;
        case 0x8a: r->road = 1; break;
        case 0x5c: case 0xb4: case 0x4d: case 0x4e: case 0x4f:
        case 0xbb: case 0xbc: case 0xbd:
            r->floodplain_count = 1;
            break;
        default:
            if (tool >= 0x41 && tool <= 0x45)
                *c->variation = (uint32_t)*c->orientation;
            else if (tool >= 0x29 && tool <= 0x2b) *c->variation = 0;
            break;
    }
selected:
    *input->current_window = 1;
    reset_drag(c);
}

/* Native 403E00 uses half-open signed rectangles and fresh popup counts. */
uint32_t pharaoh_city_build_menu_handle_input(
    const PharaohCityBuildMenuInputContext *c)
{
    const PharaohCityBuildSelectionContext *selection = c->selection;
    const int32_t original = add(*c->expanded_x, -258);
    int32_t x = original;
    int32_t previous = -1;
    uint32_t row = 0;

    if (*c->sidebar_collapsed == 1) x = add(*c->collapsed_x, -258);
    if (*selection->menu->editor_active == 1) x = original;
    const int32_t y = add(*selection->popup_y_offset, 110);
    x = add(x, signed_bits(UINT32_C(161) -
        (uint32_t)pharaoh_ui_layout_label_width(*c->label_width)));
    *c->focused_button = pharaoh_generic_buttons_get_focus(x, y,
        c->rectangles, *selection->popup_count, *c->mouse_x, *c->mouse_y,
        c->focus);
    if (*selection->popup_count <= 0) return 0;
    do {
        const PharaohMenuRect *rectangle = &c->rectangles[row];
        previous = pharaoh_city_build_menu_next_item(selection->menu,
            *selection->menu->selected_category, previous);
        if (add(x, rectangle->left) <= *c->mouse_x &&
            *c->mouse_x < add(x, rectangle->right) &&
            add(y, rectangle->top) <= *c->mouse_y &&
            *c->mouse_y < add(y, rectangle->bottom) &&
            *c->left_pressed != 0) {
            const uint32_t item = (uint32_t)previous + 1u;
            *selection->button_parameter = item;
            pharaoh_city_build_menu_set_type(selection);
            return item;
        }
        ++row;
    } while (signed_bits(row) < *selection->popup_count);
    return 0;
}

void pharaoh_city_sidebar_collapse(const PharaohCitySidebarActionContext *c)
{
    *c->slide_step = 0;
    *c->current_window = 2;
    pharaoh_city_view_set_viewport_without_sidebar(c->view);
    pharaoh_city_view_check_camera_boundaries(c->view);
    (void)pharaoh_native_return_zero_0047d350();
    pharaoh_sound_effect_request_play(c->sound, c->sound_device, 6);
}

void pharaoh_city_overlay_menu_open(const PharaohCitySidebarActionContext *c)
{
    *c->current_window = 8;
    *c->popup_count = 20;
    *c->popup_y_offset = 40;
    *c->popup_width = 12;
    *c->native_7056e0 = 0;
    *c->overlay_submenu_open = NULL;
    *c->overlay_submenu_count = 0;
    *c->native_7056d4 = 0;
    *c->overlay_submenu_index = 40;
    *c->native_7056cc = 12;
    *c->redraw = 1;
}

void pharaoh_city_button_message_help(const PharaohCityMessageQueueContext *c,
    const uint32_t *secondary_parameter, uint8_t *redraw)
{
    pharaoh_city_message_dialog_show(c, signed_bits(*secondary_parameter), 0);
    *redraw = 1;
}

void pharaoh_city_refresh_tomb_overlay(const PharaohCityOverlayRefreshContext *c)
{
    for (int32_t building = 1; building <= *c->highest_building_id;
        building = add(building, 1)) {
        int32_t receiver;
        switch (c->buildings[building].building_type) {
            case 0xe5: receiver = 0; break;
            case 0xea: receiver = 1; break;
            case 0xeb: receiver = 2; break;
            case 0xec: receiver = 3; break;
            default: continue;
        }
        pharaoh_royal_tomb_update_map(c->tombs[receiver], building);
    }
}

void pharaoh_city_hotkey_toggle_overlay(const PharaohCityHotkeyContext *c,
    int32_t overlay)
{
    const uint8_t window = *c->current_window;
    if (window == 30 || window == 73 || window == 31 || window == 32)
        *c->current_window = 1;
    else if (window == 18 || window == 35) {
        if (*c->message_return_window != 1) return;
    } else if (window != 1) return;

    if (*c->overlay == overlay) {
        *c->previous_overlay = overlay;
        *c->overlay = 0;
    } else {
        *c->previous_overlay = 0;
        *c->overlay = overlay;
    }
    *c->redraw = 1;
    pharaoh_city_refresh_tomb_overlay(c->overlay_refresh);
}

PharaohPercentageScaleStatus pharaoh_city_hotkey_show_advisor(
    const PharaohCityHotkeyContext *c, uint32_t advisor)
{
    const uint8_t window = *c->current_window;
    if (window == 30 || window == 73 || window == 31 || window == 32)
        *c->current_window = 1;
    else if (window == 7) {
        if (*c->advisors->active_advisor == advisor) {
            *c->current_window = 1;
            *c->redraw = 1;
            c->advisors->gift_notification_banks[
                *c->advisors->initialize->current_player * UINT32_C(0x49D8)] = 0;
            return PHARAOH_PERCENTAGE_SCALE_VALUE;
        }
    } else if (window != 1) return PHARAOH_PERCENTAGE_SCALE_VALUE;
    *c->button_parameter = advisor;
    return pharaoh_advisors_show_checked(c->advisors);
}

void pharaoh_city_hotkey_go_to_bookmark(const PharaohCityHotkeyContext *c,
    int32_t bookmark)
{
    const uint8_t window = *c->current_window;
    if (window == 30 || window == 73) *c->current_window = 1;
    else if (window != 1) return;
    if (bookmark >= 0 && bookmark < 5 && c->bookmarks[bookmark * 2] >= 0) {
        pharaoh_city_view_focus_tile(c->view, c->bookmarks[bookmark * 2],
            c->focus_x, c->focus_y);
        *c->redraw = 1;
    }
}

static PharaohCityIngameResult input_result(PharaohCityIngameDisposition kind)
{
    const PharaohCityIngameResult result = {kind, PHARAOH_PERCENTAGE_SCALE_VALUE};
    return result;
}

static PharaohCityIngameResult arithmetic_result(PharaohPercentageScaleStatus status)
{
    const PharaohCityIngameResult result = {
        status == PHARAOH_PERCENTAGE_SCALE_VALUE ? PHARAOH_CITY_INPUT_HANDLED :
            PHARAOH_CITY_INPUT_ARITHMETIC_FAULT,
        status
    };
    return result;
}

PharaohCityIngameResult pharaoh_city_construction_drag(
    const PharaohCityConstructionDragContext *c)
{
    const PharaohConstructionInputContext *input = c->input;
    if (*input->drag_started == 0 || *c->selected_cell == 0)
        return input_result(PHARAOH_CITY_INPUT_HANDLED);
    *input->byte_605798 = 1;
    int32_t x, y;
    if (*input->input_locked == 0) {
        y = *c->map_y;
        *c->fallback_cell = *c->selected_cell;
        x = *c->map_x;
        *input->start_y = y;
        *input->start_x = x;
    } else {
        x = *input->start_x;
        y = *input->start_y;
    }
    const int32_t tool = *input->tool;
    if (*c->editor_active != 0 && tool != 5 && tool != 88 && tool != 89 &&
        tool != 93) return input_result(PHARAOH_CITY_INPUT_EDITOR_PAINT_REQUIRED);
    return arithmetic_result(pharaoh_construction_preview(c->preview,
        *input->end_x, *input->end_y, x, y, tool));
}

static void prepare_image_input(const PharaohCityIngameDispatchContext *c)
{
    PharaohImageButtonInputState *input = c->image_input;
    input->cursor_x = *c->frame->mouse_x;
    input->cursor_y = *c->frame->mouse_y;
    input->left_pressed = *c->frame->left_pressed;
    input->right_pressed = *c->frame->right_released;
    input->left_down = *c->frame->left_down;
}

static int32_t image_mouse(const PharaohCityIngameDispatchContext *c,
    const PharaohCityInputImageTable *table, int32_t x, int32_t y, int32_t count)
{
    prepare_image_input(c);
    return pharaoh_image_buttons_handle_mouse(x, y, table->records, count,
        *table->trailing_trigger, c->image_input);
}

static void image_tooltip(const PharaohCityIngameDispatchContext *c,
    const PharaohCityInputImageTable *table, int32_t x, int32_t y,
    int32_t count, int32_t first)
{
    pharaoh_image_buttons_set_tooltip(x, y, table->records, count, first,
        *c->frame->mouse_x, *c->frame->mouse_y, c->tooltip);
}

static int32_t problem_mouse(const PharaohCityIngameDispatchContext *c,
    int32_t x, int32_t y)
{
    c->generic_input->origin_x = x;
    c->generic_input->origin_y = y;
    /* The generic input owner also carries double-click/release/hold slots;
       they are the full live native state supplied by the host. */
    return pharaoh_generic_buttons_handle_mouse(c->generic_input,
        c->buttons->problem, 1);
}

static void sidebar_hover(const PharaohCityIngameDispatchContext *c)
{
    const PharaohMinimapClickContext *click = c->minimap;
    const PharaohMinimap *minimap = click->minimap;
    /* hovered is shared by the hover writer and the minimap click reader. */
    pharaoh_build_sidebar_hover_update(*c->frame->mouse_x, *c->frame->mouse_y,
        minimap->left, minimap->top, minimap->right, minimap->bottom,
        c->minimap_hover);
}

static uint32_t minimap_mouse(const PharaohCityIngameDispatchContext *c)
{
    PharaohMinimapClickContext click = *c->minimap;
    click.mouse_x = c->frame->mouse_x;
    click.mouse_y = c->frame->mouse_y;
    click.hovered = c->minimap_hover;
    /* Native 4061E0 reads LEFT PRESS, despite the older member name. */
    click.left_down = c->frame->left_pressed;
    click.right_went_down = c->frame->right_pressed;
    return pharaoh_minimap_sidebar_click(&click);
}

static void dispatcher_prefix(const PharaohCityIngameDispatchContext *c)
{
    *c->keyboard_field = 0;
    *c->image_focus = 0;
    *c->arrow_focus = 0;
    *c->menu_focus = 0;
    *c->ui_mouse_x = *c->frame->mouse_x;
    *c->ui_mouse_y = *c->frame->mouse_y;
    if (*c->mouse_drag_state == 2) {
        *c->mouse_drag_state = 0;
        *c->redraw = 1;
    }
}

PharaohCityIngameResult pharaoh_city_ingame_dispatch(
    const PharaohCityIngameDispatchContext *c)
{
    const uint8_t window = *c->current_window;
    if (window != 1 && window != 3 && window != 6 && window != 18)
        return input_result(PHARAOH_CITY_INPUT_OTHER_WINDOW);
    dispatcher_prefix(c);
    prepare_image_input(c);

    if (window == 3) {
        const PharaohMenuBarFocusRecord *headers = *c->editor_active == 1 ?
            c->editor_headers : c->game_headers;
        if (pharaoh_menu_dropdown_handle_input(c->dropdown, headers, 4) == 0 &&
            *c->frame->right_released != 0) pharaoh_menu_bar_close(c->menu_bar);
        return input_result(PHARAOH_CITY_INPUT_HANDLED);
    }
    if (window == 18) {
        if (*c->editor_active != 1 && *c->sidebar_collapsed == 0)
            (void)problem_mouse(c, *c->expanded_x, 24);
        return input_result(PHARAOH_CITY_INPUT_HANDLED);
    }
    if (window == 6) {
        if (*c->frame->right_released != 0) {
            *c->current_window = 1;
            *c->redraw = 1;
            return input_result(PHARAOH_CITY_INPUT_HANDLED);
        }
        if (pharaoh_city_build_menu_handle_input(c->build_menu) != 0)
            return input_result(PHARAOH_CITY_INPUT_HANDLED);
        if (*c->editor_active == 1) {
            const uint8_t preview = (uint8_t)(*c->editor_map_preview != 0);
            if (image_mouse(c, preview ? &c->buttons->editor_preview :
                &c->buttons->editor_full, *c->expanded_x, 30, preview ? 3 : 15)) {
                *c->ui_redraw = 1;
                return input_result(PHARAOH_CITY_INPUT_HANDLED);
            }
        }
        if (*c->sidebar_collapsed == 0) {
            if (problem_mouse(c, *c->expanded_x, 30))
                return input_result(PHARAOH_CITY_INPUT_HANDLED);
            if (image_mouse(c, &c->buttons->expanded_build,
                *c->expanded_x, 30, 16)) *c->ui_redraw = 1;
        } else if (*c->sidebar_collapsed == 1) {
            if (image_mouse(c, &c->buttons->collapsed_build,
                *c->collapsed_x, 24, 12)) *c->ui_redraw = 1;
        }
        return input_result(PHARAOH_CITY_INPUT_HANDLED);
    }

    *c->city_transient = 0;
    const PharaohMenuBarFocusRecord *headers = *c->editor_active == 1 ?
        c->editor_headers : c->game_headers;
    *c->menu_focus = pharaoh_menu_bar_focus(headers, 4,
        *c->frame->mouse_x, *c->frame->mouse_y);
    if (pharaoh_menu_bar_select(c->menu_bar, headers))
        return input_result(PHARAOH_CITY_INPUT_HANDLED);
    if (*c->editor_active == 0)
        pharaoh_tutorial_update_mission_messages(c->tutorial);
    pharaoh_camera_scroll_apply(c->scroll,
        pharaoh_camera_scroll_direction(c->scroll_input));
    if (*c->editor_active == 1)
        return input_result(PHARAOH_CITY_INPUT_EDITOR_DISPATCH_REQUIRED);

    if (*c->sidebar_collapsed == 0) {
        sidebar_hover(c);
        if (minimap_mouse(c) != 0)
            return input_result(PHARAOH_CITY_INPUT_HANDLED);
        if (pharaoh_city_rotate_button_input(c->rotate) != 0)
            return input_result(PHARAOH_CITY_INPUT_HANDLED);
        pharaoh_top_menu_hover_index(*c->frame->mouse_x, *c->frame->mouse_y,
            c->top_info_left_edges, c->top_info_focus);
        if (*c->top_info_focus != 0 && *c->frame->right_released != 0) {
            pharaoh_city_top_menu_help(c->top_info_focus, c->messages, c->redraw);
            return input_result(PHARAOH_CITY_INPUT_HANDLED);
        }
        image_tooltip(c, &c->buttons->expanded_extra, *c->expanded_x, 30, 2, 10);
        if (image_mouse(c, &c->buttons->expanded_extra, *c->expanded_x, 30, 2)) {
            *c->ui_redraw = 1;
            return input_result(PHARAOH_CITY_INPUT_HANDLED);
        }
        image_tooltip(c, &c->buttons->expanded_build, *c->expanded_x, 30, 16, 30);
        if (problem_mouse(c, *c->expanded_x, 30))
            return input_result(PHARAOH_CITY_INPUT_HANDLED);
        if (image_mouse(c, &c->buttons->expanded_build, *c->expanded_x, 30, 16)) {
            *c->ui_redraw = 1;
            return input_result(PHARAOH_CITY_INPUT_HANDLED);
        }
        image_tooltip(c, &c->buttons->expanded_top, *c->expanded_x, 30, 2, 50);
        if (image_mouse(c, &c->buttons->expanded_top, *c->expanded_x, 30, 2)) {
            *c->ui_redraw = 1;
            return input_result(PHARAOH_CITY_INPUT_HANDLED);
        }
    } else if (*c->sidebar_collapsed == 1) {
        if (pharaoh_city_rotate_button_input(c->rotate) != 0)
            return input_result(PHARAOH_CITY_INPUT_HANDLED);
        pharaoh_top_menu_hover_index(*c->frame->mouse_x, *c->frame->mouse_y,
            c->top_info_left_edges, c->top_info_focus);
        if (*c->top_info_focus != 0 && *c->frame->right_released != 0) {
            pharaoh_city_top_menu_help(c->top_info_focus, c->messages, c->redraw);
            return input_result(PHARAOH_CITY_INPUT_HANDLED);
        }
        image_tooltip(c, &c->buttons->collapsed_expand, *c->collapsed_x, 24, 1, 10);
        if (image_mouse(c, &c->buttons->collapsed_expand, *c->collapsed_x, 24, 1)) {
            *c->ui_redraw = 1;
            return input_result(PHARAOH_CITY_INPUT_HANDLED);
        }
        image_tooltip(c, &c->buttons->collapsed_build, *c->collapsed_x, 24, 12, 30);
        if (image_mouse(c, &c->buttons->collapsed_build, *c->collapsed_x, 24, 12)) {
            *c->ui_redraw = 1;
            return input_result(PHARAOH_CITY_INPUT_HANDLED);
        }
    }

    pharaoh_city_view_mouse_select(c->map);
    *c->construction_place_ok = 0;
    if (*c->selection->selected_grid == 0)
        return input_result(PHARAOH_CITY_INPUT_HANDLED);
    if (*c->frame->left_pressed != 0) {
        if (pharaoh_city_select_legion(c->selection) != 0 ||
            pharaoh_city_select_transport_live(c->selection) != 0 ||
            pharaoh_city_select_warship_live(c->selection) != 0)
            return input_result(PHARAOH_CITY_INPUT_HANDLED);
        pharaoh_construction_begin(c->construction);
        return input_result(PHARAOH_CITY_INPUT_HANDLED);
    }
    if (*c->frame->left_down != 0) return pharaoh_city_construction_drag(c->drag);
    if (*c->frame->left_released != 0)
        return arithmetic_result(pharaoh_construction_finish(c->finish));
    if (*c->frame->right_released == 0)
        return input_result(PHARAOH_CITY_INPUT_HANDLED);
    if (pharaoh_construction_cancel_live(c->construction) != 0)
        return input_result(PHARAOH_CITY_INPUT_HANDLED);
    return input_result(PHARAOH_CITY_INPUT_OPEN_BUILDING_INFO);
}
