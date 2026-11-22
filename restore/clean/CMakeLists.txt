cmake_minimum_required(VERSION 3.25)

project(LostTomb LANGUAGES C RC)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_library(pharaoh_game STATIC
    game/core/x87_numeric.c
    game/monuments/base_progress.c
    game/monuments/build_requirements.c
    game/monuments/pyramid_work_queries.c
    game/monuments/pyramid_claim.c
    game/monuments/pyramid_progress.c
    game/monuments/complex_progress.c
    game/commerce/export_keep.c
    game/map/road_network.c
    game/map/road_access.c
    game/building/linked_parts.c
    game/scenario/scene_counts.c
    game/commerce/granary_supply.c
    game/commerce/granary_repair.c
    game/monuments/scenario_slots.c
    game/scenario/scene_industry.c
    game/scenario/scene_storage.c
    game/scenario/definition_counts.c
    game/map/scene_grid.c
    game/scenario/scene_record_rebuild.c
    game/scenario/scene_records.c
    game/ui/city_message_text.c
    game/commerce/empire_images.c
    game/commerce/resource_supply.c
    game/scenario/scene_economy.c
    game/scenario/scene_routing.c
    game/scenario/scene_assets.c
    game/scenario/scene_asset_bindings.c
    game/runtime/city_sound.c
    game/monuments/definitions.c
    game/assets/sprite_pixel_conversion.c
    game/assets/sprite_pixel_loading.c
    game/assets/sprite_auxiliary.c
    game/assets/sprite_libraries.c
    game/commerce/market_selection.c
    game/building/construction_warnings.c
    game/building/occupancy_query.c
    game/core/record_geometry.c
    game/core/ring_queue.c
    game/core/byte_tokenizer.c
    game/core/crt_random.c
    game/core/memory_fill.c
    game/core/memory_strings.c
    game/core/text_length.c
    game/core/percentage_adjust.c
    game/core/percentage_scale.c
    game/core/static_objects.c
    game/editor/city_detail_buttons.c
    game/editor/event_buttons.c
    game/editor/event_list.c
    game/editor/city_route_buttons.c
    game/editor/god_buttons.c
    game/editor/map_reset.c
    game/editor/modal_actions.c
    game/editor/monument_buttons.c
    game/editor/numeric_dialog.c
    game/editor/option_buttons.c
    game/editor/navigation_buttons.c
    game/editor/permission_buttons.c
    game/editor/popup_actions.c
    game/editor/route_geometry.c
    game/editor/validation.c
    game/events/event_actions.c
    game/events/event_queries.c
    game/events/gift_buttons.c
    game/military/enemy_queries.c
    game/military/enemy_ship_queries.c
    game/military/legion_orders.c
    game/military/transport_boats.c
    game/military/ship_status.c
    game/monuments/stages.c
    game/monuments/tile_removal.c
    game/monuments/startup_geometry.c
    game/monuments/mausoleum_projection.c
    game/monuments/mausoleum_projection.c
    game/population/age_counts.c
    game/population/house_services.c
    game/religion/building_types.c
    game/rendering/overlay_columns.c
    game/rendering/overlay_metrics.c
    game/rendering/primitives.c
    game/rendering/sprite_raw_blit.c
    game/runtime/frame_gate.c
    game/runtime/graphics_frame_restore.c
    game/runtime/figure_action_none.c
    game/runtime/graphics_directdraw_enable.c
    game/runtime/graphics_device_startup.c
    game/runtime/graphics_display_configuration.c
    game/runtime/graphics_screen_resize.c
    game/runtime/graphics_surfaces.c
    game/runtime/graphics_surfaces_clear.c
    game/runtime/graphics_surface_context.c
    game/runtime/graphics_refresh.c
    game/runtime/graphics_startup.c
    game/runtime/platform_measure_speed.c
    game/runtime/file_loading.c
    game/runtime/file_list.c
    game/runtime/file_size.c
    game/runtime/crt_error_translation.c
    game/runtime/current_directory.c
    game/runtime/file_directory_reset.c
    game/runtime/file_exists.c
    game/runtime/file_path_helpers.c
    game/runtime/file_media_root.c
    game/runtime/file_exists_media_fallback.c
    game/runtime/file_extension_operations.c
    game/runtime/file_read_part.c
    game/runtime/file_positioned_writing.c
    game/runtime/file_writing.c
    game/runtime/loading_text.c
    game/runtime/animation_frame_state.c
    game/runtime/animation_tick_counters.c
    game/runtime/random_generator.c
    game/runtime/simulation_tick.c
    game/runtime/pending_ui_state.c
    game/runtime/autosave_dispatch.c
    game/runtime/city_player_countdown.c
    game/runtime/mouse_update.c
    game/runtime/game_tick_gate.c
    game/runtime/application_startup.c
    game/runtime/application_settings_shutdown.c
    game/runtime/application_game_shutdown.c
    game/runtime/application_message_loop.c
    game/runtime/application_window_messages.c
    game/runtime/application_window_activation.c
    game/runtime/digital_sound_stream.c
    game/runtime/sound_stream_fade.c
    game/runtime/smack_video_shutdown.c
    game/runtime/application_startup_prepare.c
    game/runtime/application_winmain_run.c
    game/runtime/application_shutdown.c
    game/runtime/cursor_selection.c
    game/runtime/input_transient_reset.c
    game/runtime/status_log_append.c
    game/runtime/status_log_format.c
    game/runtime/status_log_timestamp.c
    game/runtime/format_unbounded.c
    game/runtime/screen_background_dispatch.c
    game/runtime/event_reason_phrases.c
    game/runtime/application_media_probe.c
    game/runtime/application_window.c
    game/runtime/application_window_startup.c
    game/runtime/application_display_startup.c
    game/runtime/model_text.c
    game/runtime/music_manager.c
    game/runtime/music_population.c
    game/runtime/sound_file_table.c
    game/runtime/sound_file_id_load.c
    game/runtime/sound_effect_files.c
    game/runtime/sound_effect_initialization.c
    game/runtime/sound_effect_request.c
    game/runtime/sound_initialization.c
    game/runtime/digital_sound_device.c
    game/runtime/game_data_loading.c
    game/runtime/text_catalog.c
    game/runtime/text_loading.c
    game/runtime/trade_recommendations.c
    game/runtime/tax_sentiment_model.c
    game/runtime/video_player.c
    game/runtime/video_blit.c
    game/runtime/smacker_window_video_start.c
    game/runtime/timed_window_transition.c
    game/runtime/startup_window_transition.c
    game/runtime/startup_foreground_slice.c
    game/runtime/startup_frame_slice.c
    game/runtime/fast_forward_note.c
    game/runtime/settings_ini.c
    game/runtime/settings_load.c
    game/runtime/building_model_loading.c
    game/runtime/figure_model_loading.c
    game/runtime/game_state_initialization.c
    game/runtime/city_view.c
    game/runtime/city_warning_clear.c
    game/map/map_ring.c
    game/runtime/text_input_state.c
    game/runtime/startup_asset_loading.c
    game/runtime/window_geometry.c
    game/runtime/settings_default_names.c
    game/scenario/campaign_start.c
    game/scenario/campaign_continue.c
    game/scenario/campaign_records.c
    game/scenario/campaign_mission_load.c
    game/scenario/saved_game_state_read.c
    game/scenario/saved_game_initialize.c
    game/scenario/mission_schema.c
    game/scenario/scenario_state_read.c
    game/simulation/flood_settings.c
    game/runtime/chunk_io.c
    game/scenario/map_file_load.c
    game/scenario/scenario_preview.c
    game/scenario/campaign_profile_load.c
    game/scenario/campaign_availability.c
    game/scenario/campaign_file.c
    game/simulation/finance_calendar.c
    game/simulation/industry_production.c
    game/simulation/industry_queries.c
    game/simulation/flood_simulation.c
    game/commerce/empire_queries.c
    game/ui/message_tokens.c
    game/ui/token_expansion.c
    game/ui/cheat_actions.c
    game/ui/economy_actions.c
    game/ui/industry_panels.c
    game/ui/label_text.c
    game/ui/main_menu.c
    game/ui/menu_button_frame.c
    game/ui/main_menu_action.c
    game/ui/campaign_menu.c
    game/ui/mission_intermezzo.c
    game/ui/victory_dialog.c
    game/ui/briefing.c
    game/ui/rich_text.c
    game/ui/custom_map_selection.c
    game/ui/family_menu_actions.c
    game/ui/family_options.c
    game/ui/load_save_actions.c
    game/ui/load_save_dialog.c
    game/ui/menu_modal_buttons.c
    game/ui/menu_tail_actions.c
    game/ui/message_actions.c
    game/ui/selection_button_actions.c
    game/ui/table_actions.c
    game/ui/map_selection_rating.c
    game/ui/map_selection_invasions.c
    game/ui/player_selection.c
    game/ui/panel_frame.c
    game/ui/front_menu_input.c
    game/ui/front_menu_action.c
    game/ui/front_menu_render.c
    game/ui/high_scores.c
    game/ui/small_button_callbacks.c
    game/ui/message_box.c
    game/ui/menu_dialog.c
    game/ui/bitmap_font.c
    game/ui/generic_button_focus.c
    game/ui/generic_button_input.c
    game/ui/button_layout.c
    game/ui/menu_button_layout_init.c
    game/ui/window_layout_key.c
    game/runtime/main_menu_open.c
    game/ui/menu_screens.c
    game/ui/title_menu_background.c
    game/ui/monument_tokens.c
    game/ui/political_actions.c
    game/ui/ship_panels.c
    game/ui/sidebar_layout.c
    game/ui/transport_cursor.c
    game/ui/terrain_panels.c
    game/ui/view_invalidation.c
)

add_executable(pharaoh_sprite_real_data_verify
    tests/sprite_library_real_data.c
)
target_link_libraries(pharaoh_sprite_real_data_verify PRIVATE pharaoh_game)
target_link_options(pharaoh_sprite_real_data_verify PRIVATE /STACK:2097152)

add_executable(pharaoh_campaign_file_emulation
    tests/campaign_file_emulation.c
)
target_link_libraries(pharaoh_campaign_file_emulation PRIVATE pharaoh_game)
target_link_options(pharaoh_campaign_file_emulation PRIVATE /STACK:2097152)

add_executable(pharaoh_application_directory_verify
    tests/application_directory.c
)
target_link_libraries(pharaoh_application_directory_verify PRIVATE pharaoh_game)

target_include_directories(pharaoh_game
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/game/include"
)

if(MSVC)
    target_compile_options(pharaoh_game PRIVATE /W4 /WX)
else()
    target_compile_options(pharaoh_game PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

if(WIN32)
    set(pharaoh_bootstrap_sources
        game/platform/windows_bootstrap.c
        game/platform/windows_application_window.c
        game/platform/windows_graphics.c
        game/platform/windows_startup.c
        game/platform/windows_startup_files.c
        game/platform/windows_startup_host.c
        game/platform/windows_startup_dispatch.c
        game/platform/windows_sound.c
        game/platform/windows_bink.c
        game/platform/windows_smacker.c
        game/platform/windows_video_io.c
        game/platform/windows_frame_gate.c
        game/platform/windows_status_log.c
    )
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/resources/Application.ico" AND
       EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/resources/cursors/default.cur")
        list(APPEND pharaoh_bootstrap_sources resources/PharaohClean.rc)
    endif()
    add_executable(pharaoh_clean_bootstrap WIN32 ${pharaoh_bootstrap_sources})
    target_include_directories(pharaoh_clean_bootstrap PRIVATE resources)
    target_link_libraries(pharaoh_clean_bootstrap PRIVATE pharaoh_game
        user32 gdi32 shell32 ole32 winmm ddraw)
    if(MSVC)
        target_compile_options(pharaoh_clean_bootstrap PRIVATE /W4 /WX)
    endif()
    set_target_properties(pharaoh_clean_bootstrap PROPERTIES
        OUTPUT_NAME "LostTomb"
    )
endif()

include(CTest)
if(BUILD_TESTING)
    add_executable(pharaoh_window_geometry_test tests/window_geometry_test.c game/runtime/window_geometry.c)
    target_include_directories(pharaoh_window_geometry_test PRIVATE game/include)
    if(MSVC)
        target_compile_options(pharaoh_window_geometry_test PRIVATE /W4 /WX)
    endif()
    add_test(NAME pharaoh_window_geometry_test COMMAND pharaoh_window_geometry_test)
    add_executable(pharaoh_menu_screens_test tests/menu_screens_test.c game/ui/menu_screens.c)
    target_include_directories(pharaoh_menu_screens_test PRIVATE game/include)
    if(MSVC)
        target_compile_options(pharaoh_menu_screens_test PRIVATE /W4 /WX)
    endif()
    add_test(NAME pharaoh_menu_screens_test COMMAND pharaoh_menu_screens_test)
    add_executable(pharaoh_clean_tests tests/clean_game_test.c)
    target_link_libraries(pharaoh_clean_tests PRIVATE pharaoh_game)
    if(MSVC)
        target_compile_options(pharaoh_clean_tests PRIVATE /W4 /WX)
    else()
        target_compile_options(pharaoh_clean_tests PRIVATE -Wall -Wextra -Wpedantic -Werror)
    endif()
    add_test(NAME pharaoh_clean_tests COMMAND pharaoh_clean_tests)

    add_executable(pharaoh_startup_asset_loading_test
        tests/startup_asset_loading_test.c
    )
    target_link_libraries(pharaoh_startup_asset_loading_test PRIVATE pharaoh_game)
    if(MSVC)
        target_compile_options(pharaoh_startup_asset_loading_test PRIVATE /W4 /WX)
    else()
        target_compile_options(pharaoh_startup_asset_loading_test PRIVATE
            -Wall -Wextra -Wpedantic -Werror)
    endif()
    add_test(NAME pharaoh_startup_asset_loading_test
        COMMAND pharaoh_startup_asset_loading_test)

    add_executable(pharaoh_application_media_probe_test
        tests/application_media_probe_test.c
    )
    target_link_libraries(pharaoh_application_media_probe_test PRIVATE pharaoh_game)
    if(MSVC)
        target_compile_options(pharaoh_application_media_probe_test PRIVATE /W4 /WX)
    else()
        target_compile_options(pharaoh_application_media_probe_test PRIVATE
            -Wall -Wextra -Wpedantic -Werror)
    endif()
    add_test(NAME pharaoh_application_media_probe_test
        COMMAND pharaoh_application_media_probe_test)

    add_executable(pharaoh_application_window_test
        tests/application_window_test.c
    )
    target_link_libraries(pharaoh_application_window_test PRIVATE pharaoh_game)
    if(MSVC)
        target_compile_options(pharaoh_application_window_test PRIVATE /W4 /WX)
    else()
        target_compile_options(pharaoh_application_window_test PRIVATE
            -Wall -Wextra -Wpedantic -Werror)
    endif()
    add_test(NAME pharaoh_application_window_test
        COMMAND pharaoh_application_window_test)

    add_executable(pharaoh_application_window_startup_test
        tests/application_window_startup_test.c
    )
    target_link_libraries(pharaoh_application_window_startup_test PRIVATE pharaoh_game)
    if(MSVC)
        target_compile_options(pharaoh_application_window_startup_test PRIVATE /W4 /WX)
    else()
        target_compile_options(pharaoh_application_window_startup_test PRIVATE
            -Wall -Wextra -Wpedantic -Werror)
    endif()
    add_test(NAME pharaoh_application_window_startup_test
        COMMAND pharaoh_application_window_startup_test)

    add_executable(pharaoh_event_reason_phrases_test
        tests/event_reason_phrases_test.c
    )
    target_link_libraries(pharaoh_event_reason_phrases_test PRIVATE pharaoh_game)
    if(MSVC)
        target_compile_options(pharaoh_event_reason_phrases_test PRIVATE /W4 /WX)
    else()
        target_compile_options(pharaoh_event_reason_phrases_test PRIVATE
            -Wall -Wextra -Wpedantic -Werror)
    endif()
    add_test(NAME pharaoh_event_reason_phrases_test
        COMMAND pharaoh_event_reason_phrases_test)

    add_executable(pharaoh_main_menu_open_test
        tests/main_menu_open_test.c
    )
    target_link_libraries(pharaoh_main_menu_open_test PRIVATE pharaoh_game)
    if(MSVC)
        target_compile_options(pharaoh_main_menu_open_test PRIVATE /W4 /WX)
    else()
        target_compile_options(pharaoh_main_menu_open_test PRIVATE
            -Wall -Wextra -Wpedantic -Werror)
    endif()
    add_test(NAME pharaoh_main_menu_open_test
        COMMAND pharaoh_main_menu_open_test)

    add_executable(pharaoh_campaign_profile_load_test
        tests/campaign_profile_load_test.c
    )
    target_link_libraries(pharaoh_campaign_profile_load_test PRIVATE pharaoh_game)
    if(MSVC)
        target_compile_options(pharaoh_campaign_profile_load_test PRIVATE /W4 /WX)
    else()
        target_compile_options(pharaoh_campaign_profile_load_test PRIVATE
            -Wall -Wextra -Wpedantic -Werror)
    endif()
    add_test(NAME pharaoh_campaign_profile_load_test
        COMMAND pharaoh_campaign_profile_load_test)

    add_executable(pharaoh_campaign_mission_selection_test
        tests/campaign_mission_selection_test.c)
    target_link_libraries(pharaoh_campaign_mission_selection_test
        PRIVATE pharaoh_game)
    if(MSVC)
        target_compile_options(pharaoh_campaign_mission_selection_test
            PRIVATE /W4 /WX)
    else()
        target_compile_options(pharaoh_campaign_mission_selection_test
            PRIVATE -Wall -Wextra -Wpedantic -Werror)
    endif()
    add_test(NAME pharaoh_campaign_mission_selection_test
        COMMAND pharaoh_campaign_mission_selection_test)

    add_executable(pharaoh_startup_geometry_test
        tests/startup_geometry_test.c)
    target_link_libraries(pharaoh_startup_geometry_test PRIVATE pharaoh_game)
    if(MSVC)
        target_compile_options(pharaoh_startup_geometry_test PRIVATE /W4 /WX)
    else()
        target_compile_options(pharaoh_startup_geometry_test PRIVATE
            -Wall -Wextra -Wpedantic -Werror)
    endif()
    add_test(NAME pharaoh_startup_geometry_test
        COMMAND pharaoh_startup_geometry_test)

    add_executable(pharaoh_sound_file_table_test
        tests/sound_file_table_test.c)
    target_link_libraries(pharaoh_sound_file_table_test PRIVATE pharaoh_game)
    if(MSVC)
        target_compile_options(pharaoh_sound_file_table_test PRIVATE /W4 /WX)
    else()
        target_compile_options(pharaoh_sound_file_table_test PRIVATE
            -Wall -Wextra -Wpedantic -Werror)
    endif()
    add_test(NAME pharaoh_sound_file_table_test
        COMMAND pharaoh_sound_file_table_test)

    add_executable(pharaoh_music_population_test
        tests/music_population_test.c)
    target_link_libraries(pharaoh_music_population_test PRIVATE pharaoh_game)
    if(MSVC)
        target_compile_options(pharaoh_music_population_test PRIVATE /W4 /WX)
    else()
        target_compile_options(pharaoh_music_population_test PRIVATE
            -Wall -Wextra -Wpedantic -Werror)
    endif()
    add_test(NAME pharaoh_music_population_test
        COMMAND pharaoh_music_population_test)

    add_executable(pharaoh_high_scores_test
        tests/high_scores_test.c)
    target_link_libraries(pharaoh_high_scores_test PRIVATE pharaoh_game)
    if(MSVC)
        target_compile_options(pharaoh_high_scores_test PRIVATE /W4 /WX)
    else()
        target_compile_options(pharaoh_high_scores_test PRIVATE
            -Wall -Wextra -Wpedantic -Werror)
    endif()
    add_test(NAME pharaoh_high_scores_test
        COMMAND pharaoh_high_scores_test)

    add_executable(pharaoh_message_box_test tests/message_box_test.c)
    target_link_libraries(pharaoh_message_box_test PRIVATE pharaoh_game)
    if(MSVC)
        target_compile_options(pharaoh_message_box_test PRIVATE /W4 /WX)
    else()
        target_compile_options(pharaoh_message_box_test PRIVATE
            -Wall -Wextra -Wpedantic -Werror)
    endif()
    add_test(NAME pharaoh_message_box_test COMMAND pharaoh_message_box_test)

    add_executable(pharaoh_menu_button_frame_test
        tests/menu_button_frame_test.c)
    target_link_libraries(pharaoh_menu_button_frame_test PRIVATE pharaoh_game)
    if(MSVC)
        target_compile_options(pharaoh_menu_button_frame_test PRIVATE /W4 /WX)
    else()
        target_compile_options(pharaoh_menu_button_frame_test PRIVATE
            -Wall -Wextra -Wpedantic -Werror)
    endif()
    add_test(NAME pharaoh_menu_button_frame_test
        COMMAND pharaoh_menu_button_frame_test)

    add_executable(pharaoh_front_menu_exit_test tests/front_menu_exit_test.c)
    target_link_libraries(pharaoh_front_menu_exit_test PRIVATE pharaoh_game)
    if(MSVC)
        target_compile_options(pharaoh_front_menu_exit_test PRIVATE /W4 /WX)
    else()
        target_compile_options(pharaoh_front_menu_exit_test PRIVATE
            -Wall -Wextra -Wpedantic -Werror)
    endif()
    add_test(NAME pharaoh_front_menu_exit_test COMMAND pharaoh_front_menu_exit_test)

    add_executable(pharaoh_front_menu_action_test
        tests/front_menu_action_test.c
    )
    target_link_libraries(pharaoh_front_menu_action_test PRIVATE pharaoh_game)
    if(MSVC)
        target_compile_options(pharaoh_front_menu_action_test PRIVATE /W4 /WX)
    else()
        target_compile_options(pharaoh_front_menu_action_test PRIVATE
            -Wall -Wextra -Wpedantic -Werror)
    endif()
    add_test(NAME pharaoh_front_menu_action_test
        COMMAND pharaoh_front_menu_action_test)
endif()
