cmake_minimum_required(VERSION 3.25)

project(PharaohClean LANGUAGES C RC)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_library(pharaoh_game STATIC
    game/assets/sprite_pixel_conversion.c
    game/assets/sprite_pixel_loading.c
    game/assets/sprite_libraries.c
    game/commerce/market_selection.c
    game/core/record_geometry.c
    game/core/ring_queue.c
    game/core/static_objects.c
    game/editor/event_buttons.c
    game/events/event_actions.c
    game/events/event_queries.c
    game/events/gift_buttons.c
    game/military/enemy_queries.c
    game/military/enemy_ship_queries.c
    game/military/transport_boats.c
    game/military/ship_status.c
    game/monuments/stages.c
    game/monuments/tile_removal.c
    game/population/age_counts.c
    game/population/house_services.c
    game/religion/building_types.c
    game/rendering/overlay_columns.c
    game/rendering/overlay_metrics.c
    game/rendering/primitives.c
    game/runtime/frame_gate.c
    game/runtime/figure_action_none.c
    game/runtime/graphics_directdraw_enable.c
    game/runtime/graphics_device_startup.c
    game/runtime/graphics_refresh.c
    game/runtime/graphics_startup.c
    game/runtime/platform_measure_speed.c
    game/runtime/file_loading.c
    game/runtime/application_startup.c
    game/runtime/application_display_startup.c
    game/runtime/game_data_loading.c
    game/runtime/text_catalog.c
    game/runtime/text_loading.c
    game/runtime/trade_recommendations.c
    game/runtime/video_player.c
    game/runtime/settings_ini.c
    game/runtime/settings_load.c
    game/runtime/startup_asset_loading.c
    game/runtime/window_geometry.c
    game/runtime/settings_default_names.c
    game/scenario/campaign_start.c
    game/scenario/campaign_availability.c
    game/simulation/finance_calendar.c
    game/simulation/industry_production.c
    game/simulation/industry_queries.c
    game/ui/message_tokens.c
    game/ui/industry_panels.c
    game/ui/label_text.c
    game/ui/main_menu.c
    game/ui/menu_screens.c
    game/ui/monument_tokens.c
    game/ui/political_actions.c
    game/ui/ship_panels.c
    game/ui/sidebar_layout.c
    game/ui/terrain_panels.c
    game/ui/view_invalidation.c
)

add_executable(pharaoh_sprite_real_data_verify
    tests/sprite_library_real_data.c
)
target_link_libraries(pharaoh_sprite_real_data_verify PRIVATE pharaoh_game)
target_link_options(pharaoh_sprite_real_data_verify PRIVATE /STACK:2097152)

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
    set(pharaoh_bootstrap_sources game/platform/windows_bootstrap.c)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/resources/Application.ico" AND
       EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/resources/cursors/default.cur")
        list(APPEND pharaoh_bootstrap_sources resources/PharaohClean.rc)
    endif()
    add_executable(pharaoh_clean_bootstrap WIN32 ${pharaoh_bootstrap_sources})
    target_include_directories(pharaoh_clean_bootstrap PRIVATE resources)
    target_link_libraries(pharaoh_clean_bootstrap PRIVATE pharaoh_game
        user32 gdi32 shell32 ole32 winmm)
    if(MSVC)
        target_compile_options(pharaoh_clean_bootstrap PRIVATE /W4 /WX)
    endif()
    set_target_properties(pharaoh_clean_bootstrap PROPERTIES
        OUTPUT_NAME "pharaoh_clean_bootstrap"
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
endif()
