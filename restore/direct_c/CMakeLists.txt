cmake_minimum_required(VERSION 3.25)
project(PharaohDirectC LANGUAGES C)
if(NOT MSVC OR NOT CMAKE_SIZEOF_VOID_P EQUAL 4)
 message(FATAL_ERROR "Use the Visual Studio Win32 C toolchain")
endif()
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
get_filename_component(ROOT ".." ABSOLUTE)
add_executable(pharaoh_sprite_manager_probe EXCLUDE_FROM_ALL sprite_manager_probe.c
 "${ROOT}/runtime/sprite_libraries.c" "${ROOT}/runtime/host_allocator.c"
 "${ROOT}/src/unit_02517.c" "${ROOT}/src/unit_02518.c" "${ROOT}/src/unit_02519.c" "${ROOT}/src/unit_02520.c"
 "${ROOT}/src/unit_02522.c" "${ROOT}/src/unit_02528.c" "${ROOT}/src/unit_02529.c"
 "${ROOT}/src/unit_01733.c" "${ROOT}/src/unit_01734.c" "${ROOT}/src/unit_02804.c" "${ROOT}/src/unit_02810.c")
target_include_directories(pharaoh_sprite_manager_probe PRIVATE "${ROOT}/include")
target_compile_definitions(pharaoh_sprite_manager_probe PRIVATE _CRT_SECURE_NO_WARNINGS)
target_compile_options(pharaoh_sprite_manager_probe PRIVATE /W4 /WX $<$<CONFIG:Debug>:/Od> $<$<CONFIG:Release>:/O2>)
target_link_libraries(pharaoh_sprite_manager_probe PRIVATE
 "$<IF:$<CONFIG:Debug>,${ROOT}/build/c-only-debug/Debug/restore_data.lib,${ROOT}/build/c-only/Release/restore_data.lib>" advapi32)
target_link_options(pharaoh_sprite_manager_probe PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
# Existing cleanup compares the same pointer bits through signed/unsigned DWORDs.
set_source_files_properties("${ROOT}/src/unit_02529.c" PROPERTIES COMPILE_OPTIONS /wd4389)
add_library(pharaoh_menu_instructions STATIC menu_input_controller.c menu_input_boundary.c menu_004f96e0.c
 menu_inline_capacity.c "${ROOT}/runtime/menu_route_capacity.c"
 "${ROOT}/runtime/menu_list_getter.c")
target_include_directories(pharaoh_menu_instructions PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}" "${ROOT}/include")
target_compile_definitions(pharaoh_menu_instructions PRIVATE _CRT_SECURE_NO_WARNINGS)
target_compile_options(pharaoh_menu_instructions PRIVATE /W4 /WX $<$<CONFIG:Debug>:/Od> $<$<CONFIG:Release>:/O2>)
add_library(pharaoh_menu_large_instructions STATIC EXCLUDE_FROM_ALL menu_004f7e50.c menu_004f96e0.c)
target_compile_options(pharaoh_menu_large_instructions PRIVATE /W4 /WX $<$<CONFIG:Debug>:/Od> $<$<CONFIG:Release>:/O2>)
if(EXISTS "${CMAKE_CURRENT_BINARY_DIR}/instruction-patterns/shr.c" AND EXISTS "${CMAKE_CURRENT_BINARY_DIR}/instruction-patterns/rep.c")
 add_library(pharaoh_instruction_patterns_verify SHARED EXCLUDE_FROM_ALL
  verify_large_004f7e50.c verify_large_004f96e0.c
  "${CMAKE_CURRENT_BINARY_DIR}/instruction-patterns/shr.c"
  "${CMAKE_CURRENT_BINARY_DIR}/instruction-patterns/rep.c")
 target_compile_options(pharaoh_instruction_patterns_verify PRIVATE /W4 /WX $<$<CONFIG:Debug>:/Od> $<$<CONFIG:Release>:/O2>)
 target_link_options(pharaoh_instruction_patterns_verify PRIVATE /BASE:0x10000000 /DYNAMICBASE:NO /FIXED /OPT:NOICF
  /EXPORT:restore_C_004f7e50 /EXPORT:restore_C_004f96e0
  /EXPORT:restore_test_rom_004f7e50 /EXPORT:restore_test_rom_004f96e0
  /EXPORT:restore_test_shr /EXPORT:restore_test_rep)
endif()
if(EXISTS "${CMAKE_CURRENT_BINARY_DIR}/instruction-patterns/idiv.c" AND EXISTS "${CMAKE_CURRENT_BINARY_DIR}/instruction-patterns/imul64.c")
 add_library(pharaoh_integer_wide_verify SHARED EXCLUDE_FROM_ALL
  "${CMAKE_CURRENT_BINARY_DIR}/instruction-patterns/idiv.c"
  "${CMAKE_CURRENT_BINARY_DIR}/instruction-patterns/imul64.c")
 target_compile_options(pharaoh_integer_wide_verify PRIVATE /W4 /WX $<$<CONFIG:Debug>:/Od> $<$<CONFIG:Release>:/O2>)
 target_link_options(pharaoh_integer_wide_verify PRIVATE /BASE:0x10000000 /DYNAMICBASE:NO /FIXED
  /EXPORT:restore_test_idiv /EXPORT:restore_test_imul64 /EXPORT:abort)
 if(EXISTS "${CMAKE_CURRENT_BINARY_DIR}/instruction-patterns/setge.c" AND EXISTS "${CMAKE_CURRENT_BINARY_DIR}/instruction-patterns/setl.c")
  target_sources(pharaoh_integer_wide_verify PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/instruction-patterns/setge.c" "${CMAKE_CURRENT_BINARY_DIR}/instruction-patterns/setl.c")
  target_link_options(pharaoh_integer_wide_verify PRIVATE /EXPORT:restore_test_setge /EXPORT:restore_test_setl)
 endif()
endif()
add_executable(pharaoh_menu_smoke EXCLUDE_FROM_ALL menu_smoke.c)
target_include_directories(pharaoh_menu_smoke PRIVATE "${ROOT}/include")
target_link_libraries(pharaoh_menu_smoke PRIVATE
 "$<IF:$<CONFIG:Debug>,${ROOT}/build/c-only-debug/Debug/pharaoh_restored.lib,${ROOT}/build/c-only/Release/pharaoh_restored.lib>"
 "$<IF:$<CONFIG:Debug>,${ROOT}/build/c-only-debug/Debug/restore_data.lib,${ROOT}/build/c-only/Release/restore_data.lib>"
 kernel32 user32 gdi32 shell32 ole32 oleaut32 advapi32 winmm ddraw dxguid legacy_stdio_definitions)
target_link_options(pharaoh_menu_smoke PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_executable(pharaoh_route_capacity_search EXCLUDE_FROM_ALL search_route_capacity.c)
target_include_directories(pharaoh_route_capacity_search PRIVATE "${ROOT}/include")
target_compile_options(pharaoh_route_capacity_search PRIVATE /W4 /WX /O2)
target_link_libraries(pharaoh_route_capacity_search PRIVATE
 "$<IF:$<CONFIG:Debug>,${ROOT}/build/c-only-debug/Debug/pharaoh_restored.lib,${ROOT}/build/c-only/Release/pharaoh_restored.lib>"
 "$<IF:$<CONFIG:Debug>,${ROOT}/build/c-only-debug/Debug/restore_data.lib,${ROOT}/build/c-only/Release/restore_data.lib>"
 kernel32 user32 gdi32 shell32 ole32 oleaut32 advapi32 winmm ddraw dxguid legacy_stdio_definitions)
target_link_options(pharaoh_route_capacity_search PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED /MAP)

add_library(pharaoh_menu_tool_return_verify SHARED EXCLUDE_FROM_ALL
 "${ROOT}/src/unit_00043.c" "${ROOT}/src/unit_01248.c")
target_include_directories(pharaoh_menu_tool_return_verify PRIVATE "${ROOT}/include")
target_compile_options(pharaoh_menu_tool_return_verify PRIVATE /W4 /WX $<$<CONFIG:Debug>:/Od> $<$<CONFIG:Release>:/O2>)
target_link_options(pharaoh_menu_tool_return_verify PRIVATE /BASE:0x10000000 /DYNAMICBASE:NO /FIXED /OPT:NOICF
 /EXPORT:FUN_00405ba0 /EXPORT:FUN_00486060)

add_library(pharaoh_campaign_score_numeric_verify SHARED EXCLUDE_FROM_ALL "${ROOT}/runtime/campaign_score_numeric.c")
target_include_directories(pharaoh_campaign_score_numeric_verify PRIVATE "${ROOT}/include")
target_compile_options(pharaoh_campaign_score_numeric_verify PRIVATE /W4 /WX $<$<CONFIG:Debug>:/Od> $<$<CONFIG:Release>:/O2>)
target_link_options(pharaoh_campaign_score_numeric_verify PRIVATE /BASE:0x10000000 /DYNAMICBASE:NO /FIXED
 /EXPORT:FUN_00401540 /EXPORT:restore_campaign_score_trunc80 /EXPORT:restore_campaign_score_raw80 /EXPORT:restore_campaign_score_numeric)

add_library(pharaoh_menu_list_scroll_verify SHARED EXCLUDE_FROM_ALL
 "${ROOT}/src/unit_00096.c" "${ROOT}/src/unit_01730.c" "${ROOT}/src/unit_01731.c"
 "${ROOT}/src/unit_01732.c" "${ROOT}/src/unit_02242.c" "${ROOT}/src/unit_02246.c")
target_sources(pharaoh_menu_list_scroll_verify PRIVATE "${ROOT}/src/unit_00678.c" "${ROOT}/src/unit_02638.c")
target_include_directories(pharaoh_menu_list_scroll_verify PRIVATE "${ROOT}/include")
target_compile_options(pharaoh_menu_list_scroll_verify PRIVATE /W4 /WX $<$<CONFIG:Debug>:/Od> $<$<CONFIG:Release>:/O2>)
target_link_options(pharaoh_menu_list_scroll_verify PRIVATE /BASE:0x10000000 /DYNAMICBASE:NO /FIXED /OPT:NOICF
 /EXPORT:FUN_0040d1b0 /EXPORT:FUN_004483f0 /EXPORT:FUN_0055bb90
 /EXPORT:FUN_004cde40 /EXPORT:FUN_004cde60 /EXPORT:FUN_004cde80 /EXPORT:FUN_005013a0 /EXPORT:FUN_00502780)

add_library(pharaoh_message_open_verify SHARED EXCLUDE_FROM_ALL
 "${ROOT}/src/unit_00062.c" "${ROOT}/src/unit_01619.c" "${ROOT}/src/unit_01631.c")
target_include_directories(pharaoh_message_open_verify PRIVATE "${ROOT}/include")
target_compile_options(pharaoh_message_open_verify PRIVATE /W4 /WX $<$<CONFIG:Debug>:/Od> $<$<CONFIG:Release>:/O2>)
target_link_options(pharaoh_message_open_verify PRIVATE /BASE:0x10000000 /DYNAMICBASE:NO /FIXED /OPT:NOICF
 /EXPORT:FUN_004072c0 /EXPORT:FUN_004c5bf0 /EXPORT:FUN_004c8040)

add_library(pharaoh_menu_group_selection_verify SHARED EXCLUDE_FROM_ALL
 "${ROOT}/runtime/menu_group_selection.c" "${ROOT}/src/unit_01130.c")
target_include_directories(pharaoh_menu_group_selection_verify PRIVATE "${ROOT}/include")
target_compile_options(pharaoh_menu_group_selection_verify PRIVATE /W4 /WX $<$<CONFIG:Debug>:/Od> $<$<CONFIG:Release>:/O2>)
target_link_options(pharaoh_menu_group_selection_verify PRIVATE /BASE:0x10000000 /DYNAMICBASE:NO /FIXED /OPT:NOICF
 /EXPORT:FUN_00406720 /EXPORT:FUN_004b7b50 /EXPORT:FUN_0047d350)

add_library(pharaoh_monument_progress_verify SHARED EXCLUDE_FROM_ALL
 "${ROOT}/runtime/monument_progress.c" "${ROOT}/runtime/campaign_score_numeric.c")
target_include_directories(pharaoh_monument_progress_verify PRIVATE "${ROOT}/include")
target_compile_options(pharaoh_monument_progress_verify PRIVATE /W4 /WX $<$<CONFIG:Debug>:/Od> $<$<CONFIG:Release>:/O2>)
target_link_options(pharaoh_monument_progress_verify PRIVATE /BASE:0x10000000 /DYNAMICBASE:NO /FIXED
 /EXPORT:restore_monument_worker_active /EXPORT:restore_monument_progress_percent /EXPORT:restore_monument_progress
 /EXPORT:restore_monument_progress_00545740 /EXPORT:restore_monument_progress_00558be0
 /EXPORT:restore_monument_progress_00555e80 /EXPORT:restore_monument_progress_0054f470
 /EXPORT:restore_campaign_score_trunc80 /EXPORT:restore_monument_progress_invalid)

add_library(pharaoh_monument_base_queries_verify SHARED EXCLUDE_FROM_ALL
 "${ROOT}/runtime/monument_base_queries.c" "${ROOT}/runtime/campaign_score_numeric.c" "${ROOT}/runtime/monument_progress.c")
target_include_directories(pharaoh_monument_base_queries_verify PRIVATE "${ROOT}/include")
target_compile_options(pharaoh_monument_base_queries_verify PRIVATE /W4 /WX $<$<CONFIG:Debug>:/Od> $<$<CONFIG:Release>:/O2>)
target_link_options(pharaoh_monument_base_queries_verify PRIVATE /BASE:0x10000000 /DYNAMICBASE:NO /FIXED
 /EXPORT:restore_monument_base_initialize /EXPORT:restore_monument_base_worker
 /EXPORT:restore_monument_base_0040d940 /EXPORT:restore_monument_base_00417050 /EXPORT:restore_monument_base_004e6710
 /EXPORT:restore_campaign_score_trunc80 /EXPORT:restore_monument_progress_invalid)

add_library(pharaoh_menu_tail_ui_verify SHARED EXCLUDE_FROM_ALL
 "${ROOT}/runtime/menu_tail_ui.c" "${ROOT}/artifacts/menu_tail_ui_phase/test_boundaries.c"
 "${ROOT}/src/unit_01836.c" "${ROOT}/src/unit_01845.c")
target_include_directories(pharaoh_menu_tail_ui_verify PRIVATE "${ROOT}/include")
target_compile_options(pharaoh_menu_tail_ui_verify PRIVATE /W4 /WX $<$<CONFIG:Debug>:/Od> $<$<CONFIG:Release>:/O2>)
target_link_options(pharaoh_menu_tail_ui_verify PRIVATE /BASE:0x10000000 /DYNAMICBASE:NO /FIXED /OPT:NOICF
 /EXPORT:FUN_00515c60 /EXPORT:FUN_00516020 /EXPORT:FUN_004d63d0
 /EXPORT:FUN_00425bc0 /EXPORT:FUN_00425990 /EXPORT:FUN_004d5d90 /EXPORT:FUN_004d6090 /EXPORT:FUN_00536c30
 /EXPORT:FUN_004d5d70 /EXPORT:FUN_004d5fe0)

add_library(pharaoh_menu_nearest_rect_verify SHARED EXCLUDE_FROM_ALL
 "${ROOT}/runtime/nearest_00407870.c" "${ROOT}/runtime/nearest_00407a10.c" "${ROOT}/runtime/nearest_rect.c")
target_include_directories(pharaoh_menu_nearest_rect_verify PRIVATE "${ROOT}/include")
target_compile_options(pharaoh_menu_nearest_rect_verify PRIVATE /W4 /WX $<$<CONFIG:Debug>:/Od> $<$<CONFIG:Release>:/O2>)
target_link_options(pharaoh_menu_nearest_rect_verify PRIVATE /BASE:0x10000000 /DYNAMICBASE:NO /FIXED
 /EXPORT:FUN_00407870 /EXPORT:FUN_00407a10)

add_library(pharaoh_monument_complex_queries_verify SHARED EXCLUDE_FROM_ALL "${ROOT}/runtime/monument_complex_queries.c")
target_include_directories(pharaoh_monument_complex_queries_verify PRIVATE "${ROOT}/include")
target_compile_options(pharaoh_monument_complex_queries_verify PRIVATE /W4 /WX $<$<CONFIG:Debug>:/Od> $<$<CONFIG:Release>:/O2>)
target_link_options(pharaoh_monument_complex_queries_verify PRIVATE /BASE:0x10000000 /DYNAMICBASE:NO /FIXED
 /EXPORT:FUN_004df040 /EXPORT:FUN_00542ad0 /EXPORT:FUN_004e0530 /EXPORT:FUN_00542110 /EXPORT:FUN_004204a0)

add_library(pharaoh_three_way_button_verify SHARED EXCLUDE_FROM_ALL
 "${ROOT}/runtime/three_way_button.c" "${ROOT}/artifacts/menu_three_way_button_phase/test_boundaries.c"
 "${ROOT}/src/unit_00404.c" "${ROOT}/src/unit_00405.c" "${ROOT}/src/unit_00406.c" "${ROOT}/src/unit_01130.c")
target_include_directories(pharaoh_three_way_button_verify PRIVATE "${ROOT}/include")
target_compile_options(pharaoh_three_way_button_verify PRIVATE /W4 /WX $<$<CONFIG:Debug>:/Od> $<$<CONFIG:Release>:/O2>)
target_link_options(pharaoh_three_way_button_verify PRIVATE /BASE:0x10000000 /DYNAMICBASE:NO /FIXED /OPT:NOICF
 /EXPORT:FUN_00425840 /EXPORT:FUN_00425900 /EXPORT:FUN_00425990 /EXPORT:FUN_0053e590 /EXPORT:restore_menu_callback)

add_library(pharaoh_list_panel_input_verify SHARED EXCLUDE_FROM_ALL
 "${ROOT}/runtime/list_panel_input.c" "${ROOT}/artifacts/menu_list_panel_input_phase/test_boundaries.c"
 "${ROOT}/src/unit_00404.c" "${ROOT}/src/unit_00405.c" "${ROOT}/src/unit_00406.c" "${ROOT}/src/unit_00407.c"
 "${ROOT}/src/unit_00416.c" "${ROOT}/src/unit_00417.c" "${ROOT}/src/unit_01130.c"
 "${ROOT}/src/unit_01730.c" "${ROOT}/src/unit_01731.c")
target_include_directories(pharaoh_list_panel_input_verify PRIVATE "${ROOT}/include")
target_compile_options(pharaoh_list_panel_input_verify PRIVATE /W4 /WX $<$<CONFIG:Debug>:/Od> $<$<CONFIG:Release>:/O2>)
target_link_options(pharaoh_list_panel_input_verify PRIVATE /BASE:0x10000000 /DYNAMICBASE:NO /FIXED /OPT:NOICF
 /EXPORT:FUN_004e1e10 /EXPORT:FUN_004e2090 /EXPORT:restore_menu_callback
 /EXPORT:FUN_00425990 /EXPORT:FUN_00425bc0 /EXPORT:FUN_00426450 /EXPORT:FUN_004261d0
 /EXPORT:FUN_004cde40 /EXPORT:FUN_004cde60)

foreach(_order_variant IN ITEMS trace plain)
 set(_order_target pharaoh_order_input_${_order_variant}_verify)
 add_executable(${_order_target} EXCLUDE_FROM_ALL "${ROOT}/artifacts/order_input_phase/order_test.c"
  "${ROOT}/src/unit_00080.c" "${ROOT}/src/unit_00081.c" "${ROOT}/src/unit_00082.c" "${ROOT}/src/unit_00083.c"
  "${ROOT}/src/unit_01455.c" "${ROOT}/src/unit_01456.c" "${ROOT}/src/unit_01457.c")
 target_include_directories(${_order_target} PRIVATE "${ROOT}/include")
 target_compile_definitions(${_order_target} PRIVATE _CRT_SECURE_NO_WARNINGS)
 if(_order_variant STREQUAL "trace")
  target_compile_definitions(${_order_target} PRIVATE ORDER_INPUT_TRACE)
 endif()
 target_compile_options(${_order_target} PRIVATE /W4 $<$<CONFIG:Debug>:/Od> $<$<CONFIG:Release>:/O2>)
 target_link_libraries(${_order_target} PRIVATE
  "$<IF:$<CONFIG:Debug>,${ROOT}/build/c-only-debug/Debug/restore_data.lib,${ROOT}/build/c-only/Release/restore_data.lib>" advapi32)
 target_link_options(${_order_target} PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED /MAP)
endforeach()



# The canonical editor functions compose actual C helpers from the main library.
foreach(_editor_kind IN ITEMS brush hover)
 foreach(_editor_variant IN ITEMS trace plain)
  set(_editor_target pharaoh_editor_${_editor_kind}_${_editor_variant}_verify)
  if(_editor_kind STREQUAL "brush")
   set(_editor_fixture "${ROOT}/artifacts/editor_brush_phase/brush_test.c")
  else()
   set(_editor_fixture "${ROOT}/artifacts/editor_hover_phase/hover_test.c")
  endif()
  add_executable(${_editor_target} EXCLUDE_FROM_ALL ${_editor_fixture}
   "${ROOT}/src/unit_02085.c" "${ROOT}/src/unit_02086.c" "${ROOT}/src/unit_02089.c" "${ROOT}/src/unit_01241.c"
   "${ROOT}/src/unit_00047.c" "${ROOT}/src/unit_02088.c" "${ROOT}/src/unit_02087.c")
  if(_editor_kind STREQUAL "hover")
   target_sources(${_editor_target} PRIVATE "${ROOT}/src/unit_00048.c")
  endif()
  target_include_directories(${_editor_target} PRIVATE "${ROOT}/include")
  target_compile_definitions(${_editor_target} PRIVATE _CRT_SECURE_NO_WARNINGS)
  if(_editor_variant STREQUAL "trace")
   target_compile_definitions(${_editor_target} PRIVATE EDITOR_BRUSH_TRACE)
  endif()
  target_compile_options(${_editor_target} PRIVATE /W4 $<$<CONFIG:Debug>:/Od> $<$<CONFIG:Release>:/O2>)
  target_link_libraries(${_editor_target} PRIVATE
   "$<IF:$<CONFIG:Debug>,${ROOT}/build/c-only-debug/Debug/pharaoh_restored.lib,${ROOT}/build/c-only/Release/pharaoh_restored.lib>"
   "$<IF:$<CONFIG:Debug>,${ROOT}/build/c-only-debug/Debug/restore_data.lib,${ROOT}/build/c-only/Release/restore_data.lib>"
   kernel32 user32 gdi32 shell32 ole32 oleaut32 advapi32 winmm ddraw dxguid legacy_stdio_definitions)
  target_link_options(${_editor_target} PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED /MAP)
 endforeach()
endforeach()

add_executable(pharaoh_editor_ring_verify EXCLUDE_FROM_ALL
 "${ROOT}/artifacts/editor_ring_phase/ring_test.c" "${ROOT}/src/unit_02087.c")
target_include_directories(pharaoh_editor_ring_verify PRIVATE "${ROOT}/include")
target_compile_definitions(pharaoh_editor_ring_verify PRIVATE _CRT_SECURE_NO_WARNINGS)
target_compile_options(pharaoh_editor_ring_verify PRIVATE /W4 /WX $<$<CONFIG:Debug>:/Od> $<$<CONFIG:Release>:/O2>)
target_link_libraries(pharaoh_editor_ring_verify PRIVATE
 "$<IF:$<CONFIG:Debug>,${ROOT}/build/c-only-debug/Debug/restore_data.lib,${ROOT}/build/c-only/Release/restore_data.lib>" advapi32)
target_link_options(pharaoh_editor_ring_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)

# Small native-oracle check; built only when explicitly requested.
add_library(pharaoh_menu_records_verify SHARED EXCLUDE_FROM_ALL
 "${ROOT}/src/unit_00050.c" "${ROOT}/src/unit_00052.c"
 "${ROOT}/src/unit_00604.c" "${ROOT}/src/unit_00638.c" "${ROOT}/src/unit_00686.c"
 "${ROOT}/src/unit_00238.c" "${ROOT}/src/unit_00239.c" "${ROOT}/src/unit_00252.c"
 "${ROOT}/src/unit_02061.c" "${ROOT}/src/unit_02062.c"
 "${ROOT}/src/unit_01928.c"
 "${ROOT}/src/unit_00229.c" menu_input_test_boundaries.c
 "${ROOT}/runtime/menu_table_actions.c"
 "${ROOT}/artifacts/menu_table_actions_phase/test_boundaries.c"
 "${ROOT}/runtime/menu_list_getter.c" "${ROOT}/runtime/display_confirmation_clock.c")
target_include_directories(pharaoh_menu_records_verify PRIVATE "${ROOT}/include")
target_compile_options(pharaoh_menu_records_verify PRIVATE /W4 /WX $<$<CONFIG:Debug>:/Od> $<$<CONFIG:Release>:/O2>)
target_link_options(pharaoh_menu_records_verify PRIVATE /BASE:0x10000000 /DYNAMICBASE:NO /FIXED
 /EXPORT:FUN_00406190 /EXPORT:FUN_00406250
 /EXPORT:FUN_00442500 /EXPORT:FUN_00444920 /EXPORT:FUN_00448cf0
 /EXPORT:FUN_004e2ab0
 /EXPORT:FUN_00418fe0 /EXPORT:FUN_00425bc0 /EXPORT:FUN_00425990 /EXPORT:FUN_004c7db0
 /EXPORT:FUN_0041aa20 /EXPORT:FUN_0041aa60 /EXPORT:FUN_0041b8f0 /EXPORT:restore_menu_list_can_open
 /EXPORT:FUN_004ea880 /EXPORT:FUN_004ea8d0
 /EXPORT:FUN_004ea7f0 /EXPORT:FUN_004ea830 /EXPORT:FUN_004ea840
 /EXPORT:FUN_004cb6a0 /EXPORT:FUN_00418510 /OPT:NOICF)
target_link_libraries(pharaoh_menu_records_verify PRIVATE winmm)

# Actual campaign/file-list C; only external I/O and heap are finite test entries.
add_library(pharaoh_campaign_entry_verify SHARED EXCLUDE_FROM_ALL
 "${ROOT}/src/unit_01654.c" "${ROOT}/artifacts/campaign_entry_phase/test_boundaries.c"
 "${ROOT}/src/unit_00253.c" "${ROOT}/src/unit_00256.c" "${ROOT}/src/unit_00258.c"
 "${ROOT}/src/unit_00269.c" "${ROOT}/src/unit_00270.c")
target_include_directories(pharaoh_campaign_entry_verify PRIVATE "${ROOT}/include")
target_compile_options(pharaoh_campaign_entry_verify PRIVATE /W4 /WX $<$<CONFIG:Debug>:/Od> $<$<CONFIG:Release>:/O2>)
set_source_files_properties("${ROOT}/src/unit_00270.c" PROPERTIES COMPILE_OPTIONS /wd4146)
target_link_options(pharaoh_campaign_entry_verify PRIVATE /BASE:0x10000000 /DYNAMICBASE:NO /FIXED /OPT:NOICF
 /EXPORT:FUN_004cb6a0 /EXPORT:FUN_0041c740 /EXPORT:FUN_00561a0a /EXPORT:FUN_00561994
 /EXPORT:FUN_00561a1d /EXPORT:FUN_00561a2b /EXPORT:restore_heap_free
 /EXPORT:restore_file_find_first /EXPORT:restore_file_find_next /EXPORT:restore_file_find_close)

add_library(pharaoh_campaign_actions_verify SHARED EXCLUDE_FROM_ALL
 "${ROOT}/src/unit_00220.c" "${ROOT}/src/unit_00223.c" "${ROOT}/src/unit_00225.c" "${ROOT}/src/unit_02494.c"
 "${ROOT}/artifacts/campaign_actions_phase/test_boundaries.c"
 "${ROOT}/src/unit_00253.c" "${ROOT}/src/unit_00222.c" "${ROOT}/src/unit_00238.c"
 "${ROOT}/src/unit_00251.c" "${ROOT}/src/unit_02283.c" "${ROOT}/src/unit_00311.c"
 "${ROOT}/src/unit_02708.c" "${ROOT}/src/unit_02709.c" "${ROOT}/src/unit_02710.c" "${ROOT}/src/unit_02711.c"
 "${ROOT}/runtime/sound_streams.c")
target_include_directories(pharaoh_campaign_actions_verify PRIVATE "${ROOT}/include")
target_compile_definitions(pharaoh_campaign_actions_verify PRIVATE _CRT_SECURE_NO_WARNINGS)
target_compile_options(pharaoh_campaign_actions_verify PRIVATE /W4 /WX $<$<CONFIG:Debug>:/Od> $<$<CONFIG:Release>:/O2>)
target_link_options(pharaoh_campaign_actions_verify PRIVATE /BASE:0x10000000 /DYNAMICBASE:NO /FIXED /OPT:NOICF
 /EXPORT:FUN_00418220 /EXPORT:FUN_00418640 /EXPORT:FUN_00418140 /EXPORT:FUN_0053b690
 /EXPORT:restore_campaign_selection_failure /EXPORT:FUN_004188f0 /EXPORT:FUN_00516c80 /EXPORT:FUN_004cb6a0
 /EXPORT:FUN_0041f810 /EXPORT:FUN_0041f670 /EXPORT:FUN_00413580 /EXPORT:restore_display_confirmation_clock
 /EXPORT:FUN_00561a0a /EXPORT:FUN_00561994 /EXPORT:FUN_00561f5f
 /EXPORT:_AIL_close_stream_4 /EXPORT:_AIL_stream_status_4 /EXPORT:_AIL_stream_volume_4 /EXPORT:_AIL_set_stream_volume_8)

add_library(pharaoh_campaign_summary_verify SHARED EXCLUDE_FROM_ALL
 "${ROOT}/src/unit_00226.c" "${ROOT}/artifacts/campaign_summary_phase/test_boundaries.c"
 "${ROOT}/src/unit_01672.c" "${ROOT}/runtime/text_selection.c"
 "${ROOT}/src/unit_02283.c" "${ROOT}/src/unit_00251.c")
target_include_directories(pharaoh_campaign_summary_verify PRIVATE "${ROOT}/include")
target_compile_definitions(pharaoh_campaign_summary_verify PRIVATE _CRT_SECURE_NO_WARNINGS)
target_compile_options(pharaoh_campaign_summary_verify PRIVATE /W4 /WX $<$<CONFIG:Debug>:/Od> $<$<CONFIG:Release>:/O2>)
target_link_options(pharaoh_campaign_summary_verify PRIVATE /BASE:0x10000000 /DYNAMICBASE:NO /FIXED /OPT:NOICF
 /EXPORT:FUN_004188f0 /EXPORT:FUN_004cc110 /EXPORT:FUN_00516a20 /EXPORT:FUN_0041b8a0
 /EXPORT:FUN_00561a0a /EXPORT:FUN_00561994 /EXPORT:FUN_00561f5f)

add_library(pharaoh_campaign_map_input_verify SHARED EXCLUDE_FROM_ALL
 "${ROOT}/src/unit_00230.c" "${ROOT}/src/unit_01770.c"
 "${ROOT}/artifacts/campaign_map_input_phase/test_failure.c" "${ROOT}/src/unit_01130.c")
target_include_directories(pharaoh_campaign_map_input_verify PRIVATE "${ROOT}/include")
target_compile_options(pharaoh_campaign_map_input_verify PRIVATE /W4 /WX $<$<CONFIG:Debug>:/Od> $<$<CONFIG:Release>:/O2>)
target_link_options(pharaoh_campaign_map_input_verify PRIVATE /BASE:0x10000000 /DYNAMICBASE:NO /FIXED /OPT:NOICF
 /EXPORT:FUN_00419090 /EXPORT:FUN_004cf620 /EXPORT:FUN_0047d350
 /EXPORT:restore_campaign_map_fanfare /EXPORT:restore_campaign_selection_failure)

add_library(pharaoh_high_score_submission_verify SHARED EXCLUDE_FROM_ALL
 "${ROOT}/runtime/high_score_submission.c" "${ROOT}/artifacts/high_score_submission_phase/test_boundaries.c"
 "${ROOT}/src/unit_01614.c" "${ROOT}/src/unit_02283.c" "${ROOT}/src/unit_00251.c" "${ROOT}/src/unit_02826.c")
set_source_files_properties("${ROOT}/src/unit_02826.c" PROPERTIES COMPILE_OPTIONS /wd4244)
target_include_directories(pharaoh_high_score_submission_verify PRIVATE "${ROOT}/include")
target_compile_definitions(pharaoh_high_score_submission_verify PRIVATE _CRT_SECURE_NO_WARNINGS)
target_compile_options(pharaoh_high_score_submission_verify PRIVATE /W4 /WX $<$<CONFIG:Debug>:/Od> $<$<CONFIG:Release>:/O2>)
target_link_options(pharaoh_high_score_submission_verify PRIVATE /BASE:0x10000000 /DYNAMICBASE:NO /FIXED /OPT:NOICF
 /EXPORT:FUN_004c5a40 /EXPORT:FUN_004c5970 /EXPORT:FUN_0041c600 /EXPORT:restore_high_scores_write /EXPORT:FUN_004c58d0
 /EXPORT:restore_high_scores_read /EXPORT:FUN_004c5880 /EXPORT:FUN_00516a20 /EXPORT:FUN_0041b8a0 /EXPORT:FUN_00562370
 /EXPORT:FUN_0041c380 /EXPORT:FUN_004192b0 /EXPORT:FUN_00561a0a /EXPORT:FUN_00561994 /EXPORT:FUN_00561f5f
 /EXPORT:FUN_00561ca1 /EXPORT:FUN_00566682)

# Optional replays compile canonical production sources; fixtures supply only test boundaries.
function(restore_native_dll target)
 target_include_directories(${target} PRIVATE "${ROOT}/include")
 target_compile_options(${target} PRIVATE /W4 /WX $<$<CONFIG:Debug>:/Od> $<$<CONFIG:Release>:/O2>)
 target_link_options(${target} PRIVATE /BASE:0x10000000 /DYNAMICBASE:NO /FIXED /OPT:NOICF ${ARGN})
endfunction()
add_library(pharaoh_pending_message_verify SHARED EXCLUDE_FROM_ALL
 "${ROOT}/runtime/message_controller.c" "${ROOT}/runtime/message_strings.c" "${ROOT}/runtime/action_tail.c"
 "${ROOT}/artifacts/menu_pending_message_phase/test_boundaries.c"
 "${ROOT}/src/unit_00416.c" "${ROOT}/src/unit_00417.c" "${ROOT}/src/unit_01686.c" "${ROOT}/src/unit_01672.c"
 "${ROOT}/runtime/text_selection.c" "${ROOT}/src/unit_00119.c" "${ROOT}/src/unit_01663.c"
 "${ROOT}/src/unit_02823.c" "${ROOT}/runtime/actor_order_action_calls.c")
restore_native_dll(pharaoh_pending_message_verify /EXPORT:FUN_0041d310 /EXPORT:FUN_004e2970
 /EXPORT:FUN_004cbf30 /EXPORT:FUN_004cc000 /EXPORT:FUN_0041d050 /EXPORT:FUN_0041d010 /EXPORT:FUN_0041d3f0
 /EXPORT:restore_menu_callback /EXPORT:FUN_00426450 /EXPORT:FUN_004261d0 /EXPORT:FUN_004cc500 /EXPORT:FUN_004cc110
 /EXPORT:FUN_0040f8f0 /EXPORT:FUN_004cbea0 /EXPORT:FUN_00562240)
add_library(pharaoh_record_place_verify SHARED EXCLUDE_FROM_ALL
 "${ROOT}/runtime/record_place.c" "${ROOT}/runtime/record_rebuild.c" "${ROOT}/runtime/record_queries.c"
 "${ROOT}/src/unit_00418.c" "${ROOT}/src/unit_00606.c" "${ROOT}/src/unit_00608.c"
 "${ROOT}/src/unit_00615.c" "${ROOT}/src/unit_00119.c")
restore_native_dll(pharaoh_record_place_verify /EXPORT:FUN_00407620 /EXPORT:FUN_00444720 /EXPORT:FUN_00443040
 /EXPORT:FUN_00444950 /EXPORT:FUN_00444a00 /EXPORT:FUN_004264d0 /EXPORT:FUN_004426c0 /EXPORT:FUN_004425c0
 /EXPORT:FUN_00443000 /EXPORT:FUN_0040f8f0)
add_library(pharaoh_building_kind_verify SHARED EXCLUDE_FROM_ALL "${ROOT}/runtime/menu_building_kind.c" "${ROOT}/src/unit_01275.c")
restore_native_dll(pharaoh_building_kind_verify /EXPORT:FUN_004f8960 /EXPORT:FUN_00488030 /EXPORT:FUN_00487f80)
add_library(pharaoh_route_seed_verify SHARED EXCLUDE_FROM_ALL "${ROOT}/runtime/menu_route_seed.c"
 "${ROOT}/runtime/menu_route_queries.c" "${ROOT}/runtime/menu_route_flood.c"
 "${ROOT}/src/unit_00118.c" "${ROOT}/src/unit_00119.c" "${ROOT}/src/unit_01143.c")
restore_native_dll(pharaoh_route_seed_verify /EXPORT:FUN_00405c20 /EXPORT:FUN_00484f60
 /EXPORT:FUN_0051b680 /EXPORT:FUN_0051c790 /EXPORT:FUN_0051c390 /EXPORT:FUN_0051c510 /EXPORT:FUN_0051c630
 /EXPORT:FUN_0047b8e0 /EXPORT:FUN_0047e0d0 /EXPORT:FUN_0040f860 /EXPORT:FUN_0040f8f0 /EXPORT:FUN_0047e090)
add_library(pharaoh_building_options_verify SHARED EXCLUDE_FROM_ALL "${ROOT}/runtime/menu_building_options.c"
 "${ROOT}/src/unit_00718.c" "${ROOT}/src/unit_00741.c" "${ROOT}/src/unit_01275.c"
 "${ROOT}/runtime/menu_building_kind.c" "${ROOT}/runtime/pyramid_summary.c"
 "${ROOT}/runtime/monument_base_queries.c" "${ROOT}/runtime/monument_progress.c"
 "${ROOT}/runtime/monument_complex_queries.c" "${ROOT}/runtime/campaign_score_numeric.c")
restore_native_dll(pharaoh_building_options_verify /EXPORT:FUN_00515130 /EXPORT:FUN_005150a0 /EXPORT:FUN_004f5050
 /EXPORT:FUN_00488030 /EXPORT:FUN_00487f80 /EXPORT:FUN_004f1310 /EXPORT:FUN_00542ad0 /EXPORT:FUN_004df040
 /EXPORT:FUN_00401540 /EXPORT:restore_monument_base_0040d940 /EXPORT:restore_monument_base_00417050
 /EXPORT:restore_monument_base_004e6710 /EXPORT:restore_monument_progress_00545740 /EXPORT:restore_monument_progress_00558be0
 /EXPORT:restore_monument_progress_00555e80 /EXPORT:restore_monument_progress_0054f470 /EXPORT:restore_campaign_score_trunc80
 /EXPORT:restore_monument_base_initialize /EXPORT:restore_pyramid_summary_failure /EXPORT:restore_monument_progress_invalid /EXPORT:abort)
add_library(pharaoh_resource_availability_verify SHARED EXCLUDE_FROM_ALL "${ROOT}/runtime/resource_availability.c"
 "${ROOT}/src/unit_00625.c" "${ROOT}/src/unit_00626.c" "${ROOT}/src/unit_00631.c" "${ROOT}/src/unit_00632.c"
 "${ROOT}/src/unit_00669.c" "${ROOT}/src/unit_01603.c" "${ROOT}/src/unit_01605.c")
restore_native_dll(pharaoh_resource_availability_verify /EXPORT:FUN_00559480 /EXPORT:FUN_005596d0
 /EXPORT:restore_avail_C_00559480 /EXPORT:restore_avail_C_005596d0
 /EXPORT:restore_resource_availability_dispatch /EXPORT:restore_resource_availability_failure /EXPORT:abort)
add_library(pharaoh_resource_neighbors_verify SHARED EXCLUDE_FROM_ALL "${ROOT}/runtime/menu_resource_neighbors.c")
restore_native_dll(pharaoh_resource_neighbors_verify /EXPORT:FUN_00480180 /EXPORT:FUN_004802c0
 /EXPORT:FUN_00480460 /EXPORT:FUN_004806a0 /EXPORT:restore_resource_C_00480180 /EXPORT:restore_resource_C_004802c0
 /EXPORT:restore_resource_C_00480460 /EXPORT:restore_resource_C_004806a0
 /EXPORT:restore_resource_neighbors_dispatch /EXPORT:restore_resource_neighbors_failure /EXPORT:abort)
foreach(_actions_variant IN ITEMS trace plain)
 set(_actions_target pharaoh_order_actions_${_actions_variant}_verify)
 add_executable(${_actions_target} EXCLUDE_FROM_ALL "${ROOT}/artifacts/order_actions_finish_phase/actions_test.c"
  "${ROOT}/artifacts/order_actions_finish_phase/query_calls.c"
  "${ROOT}/src/unit_01458.c" "${ROOT}/src/unit_01468.c" "${ROOT}/src/unit_01469.c"
  "${ROOT}/src/unit_01470.c" "${ROOT}/src/unit_00088.c" "${ROOT}/src/unit_00089.c")
 target_include_directories(${_actions_target} PRIVATE "${ROOT}/include")
 target_compile_definitions(${_actions_target} PRIVATE _CRT_SECURE_NO_WARNINGS
  ORDER_FLOOD_SOURCE="${ROOT}/src/unit_02296.c" ORDER_PATH_SOURCE="${ROOT}/src/unit_02300.c")
 if(_actions_variant STREQUAL "trace")
  target_compile_definitions(${_actions_target} PRIVATE ORDER_ACTIONS_TRACE)
 endif()
 target_compile_options(${_actions_target} PRIVATE /W4 $<$<CONFIG:Debug>:/Od> $<$<CONFIG:Release>:/O2>)
 target_link_libraries(${_actions_target} PRIVATE
  "$<IF:$<CONFIG:Debug>,${ROOT}/build/c-only-debug/Debug/pharaoh_restored.lib,${ROOT}/build/c-only/Release/pharaoh_restored.lib>"
  "$<IF:$<CONFIG:Debug>,${ROOT}/build/c-only-debug/Debug/restore_data.lib,${ROOT}/build/c-only/Release/restore_data.lib>"
  kernel32 user32 gdi32 shell32 ole32 oleaut32 advapi32 winmm ddraw dxguid legacy_stdio_definitions)
 target_link_options(${_actions_target} PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED /MAP)
endforeach()
add_library(pharaoh_minimap_input_verify SHARED EXCLUDE_FROM_ALL
 "${ROOT}/src/unit_00051.c" "${ROOT}/src/unit_00582.c" "${ROOT}/src/unit_00576.c"
 "${ROOT}/src/unit_00458.c" "${ROOT}/src/unit_00467.c" "${ROOT}/src/unit_01886.c"
 "${ROOT}/src/unit_01880.c" "${ROOT}/src/unit_01881.c" "${ROOT}/src/unit_00464.c" "${ROOT}/src/unit_00465.c")
restore_native_dll(pharaoh_minimap_input_verify /EXPORT:FUN_004061e0 /EXPORT:FUN_0043ef10 /EXPORT:FUN_0043e670)
add_library(pharaoh_pyramid_summary_verify SHARED EXCLUDE_FROM_ALL "${ROOT}/runtime/pyramid_summary.c")
restore_native_dll(pharaoh_pyramid_summary_verify
 /EXPORT:FUN_004f1310 /EXPORT:restore_pyramid_summary_dispatch /EXPORT:restore_pyramid_summary_failure
 /EXPORT:restore_pyramid_C_004f1310 /EXPORT:restore_pyramid_C_004eea10 /EXPORT:restore_pyramid_C_004ef100
 /EXPORT:restore_pyramid_C_004ef450 /EXPORT:restore_pyramid_C_004f26a0 /EXPORT:restore_pyramid_C_004efd90 /EXPORT:abort)
add_library(pharaoh_campaign_score_verify SHARED EXCLUDE_FROM_ALL "${ROOT}/runtime/campaign_score.c"
 "${ROOT}/runtime/campaign_score_numeric.c" "${ROOT}/runtime/monument_progress.c"
 "${ROOT}/runtime/monument_base_queries.c" "${ROOT}/runtime/monument_complex_queries.c" "${ROOT}/runtime/pyramid_summary.c")
restore_native_dll(pharaoh_campaign_score_verify /EXPORT:FUN_0041c380 /EXPORT:FUN_004f1310 /EXPORT:FUN_00542ad0
 /EXPORT:FUN_004df040 /EXPORT:FUN_00401540 /EXPORT:restore_monument_base_initialize
 /EXPORT:restore_monument_base_0040d940 /EXPORT:restore_monument_base_00417050 /EXPORT:restore_monument_base_004e6710
 /EXPORT:restore_monument_progress_00545740 /EXPORT:restore_monument_progress_00558be0
 /EXPORT:restore_monument_progress_00555e80 /EXPORT:restore_monument_progress_0054f470
 /EXPORT:restore_campaign_score_trunc80 /EXPORT:restore_pyramid_summary_failure /EXPORT:restore_monument_progress_invalid /EXPORT:abort)
add_library(pharaoh_prepare_integer_verify SHARED EXCLUDE_FROM_ALL "${ROOT}/runtime/menu_prepare_integer.c")
restore_native_dll(pharaoh_prepare_integer_verify /EXPORT:FUN_00454190 /EXPORT:FUN_0045d570 /EXPORT:FUN_00475b50
 /EXPORT:FUN_004b7590 /EXPORT:FUN_0050f0f0 /EXPORT:restore_prepare_C_00454190 /EXPORT:restore_prepare_C_0045d570
 /EXPORT:restore_prepare_C_00475b50 /EXPORT:restore_prepare_C_004b7590 /EXPORT:restore_prepare_C_0050f0f0
 /EXPORT:restore_prepare_integer_dispatch /EXPORT:restore_prepare_integer_failure /EXPORT:abort)
add_library(pharaoh_monument_search_verify SHARED EXCLUDE_FROM_ALL "${ROOT}/runtime/menu_monument_search.c"
 "${ROOT}/artifacts/menu_monument_search_phase/test_link_guards.c"
 "${ROOT}/src/unit_02085.c" "${ROOT}/src/unit_02089.c" "${ROOT}/src/unit_01011.c" "${ROOT}/src/unit_01012.c")
restore_native_dll(pharaoh_monument_search_verify /EXPORT:FUN_004f5d90 /EXPORT:FUN_004f6000
 /EXPORT:FUN_004ecea0 /EXPORT:FUN_004ed360 /EXPORT:FUN_0046f9a0 /EXPORT:FUN_0046f930 /EXPORT:abort)
add_library(pharaoh_video_open_verify SHARED EXCLUDE_FROM_ALL "${ROOT}/runtime/video_open.c" "${ROOT}/runtime/video_frame.c"
 "${ROOT}/artifacts/menu_video_open_phase/test_boundaries.c"
 "${ROOT}/src/unit_01663.c" "${ROOT}/src/unit_01664.c" "${ROOT}/src/unit_01740.c" "${ROOT}/src/unit_01741.c"
 "${ROOT}/src/unit_01743.c" "${ROOT}/src/unit_01748.c" "${ROOT}/src/unit_01777.c" "${ROOT}/src/unit_01786.c"
 "${ROOT}/src/unit_02351.c" "${ROOT}/src/unit_00434.c" "${ROOT}/src/unit_02472.c" "${ROOT}/runtime/media_bindings.c")
target_compile_definitions(pharaoh_video_open_verify PRIVATE MAP_FILE_LOAD_PROOF)
restore_native_dll(pharaoh_video_open_verify /EXPORT:FUN_00538dc0 /EXPORT:FUN_00538e30 /EXPORT:FUN_00538fa0
 /EXPORT:restore_media_resolve /EXPORT:map_file_chdir /EXPORT:map_file_getcwd
 /EXPORT:FUN_00568eab /EXPORT:FUN_005647eb /EXPORT:FUN_004ce6e0
 /EXPORT:FUN_004cbea0 /EXPORT:FUN_004ce100 /EXPORT:FUN_004ce2d0 /EXPORT:FUN_004ce450
 /EXPORT:FUN_004cff20 /EXPORT:FUN_004d0280 /EXPORT:FUN_0051f780 /EXPORT:FUN_00427510 /EXPORT:FUN_00539220)
foreach(_branch_variant IN ITEMS trace plain)
 set(_branch_target pharaoh_order_branch_${_branch_variant}_verify)
 add_executable(${_branch_target} EXCLUDE_FROM_ALL "${ROOT}/artifacts/order_branch_phase/branch_test.c"
  "${ROOT}/src/unit_00084.c" "${ROOT}/src/unit_01459.c" "${ROOT}/src/unit_00080.c" "${ROOT}/src/unit_00081.c"
  "${ROOT}/src/unit_00082.c" "${ROOT}/src/unit_00083.c" "${ROOT}/src/unit_01456.c" "${ROOT}/src/unit_01457.c"
  "${ROOT}/src/unit_01455.c" "${ROOT}/src/unit_01427.c" "${ROOT}/src/unit_00087.c" "${ROOT}/src/unit_02335.c"
  "${ROOT}/src/unit_00119.c" "${ROOT}/runtime/actor_order_los_numeric.c" "${ROOT}/runtime/menu_full_input_numeric.c")
 target_include_directories(${_branch_target} PRIVATE "${ROOT}/include")
 target_compile_definitions(${_branch_target} PRIVATE _CRT_SECURE_NO_WARNINGS)
 if(_branch_variant STREQUAL "trace")
  target_compile_definitions(${_branch_target} PRIVATE ORDER_BRANCH_TRACE)
 endif()
 target_compile_options(${_branch_target} PRIVATE /W4 $<$<CONFIG:Debug>:/Od> $<$<CONFIG:Release>:/O2>)
 target_link_libraries(${_branch_target} PRIVATE
  "$<IF:$<CONFIG:Debug>,${ROOT}/build/c-only-debug/Debug/restore_data.lib,${ROOT}/build/c-only/Release/restore_data.lib>" advapi32)
 target_link_options(${_branch_target} PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED /MAP)
endforeach()

# Canonical drag/build helpers: optional native-oracle targets.
add_library(pharaoh_drag_leaves_verify SHARED EXCLUDE_FROM_ALL "${ROOT}/runtime/menu_drag_leaves.c")
restore_native_dll(pharaoh_drag_leaves_verify /EXPORT:FUN_00477960 /EXPORT:FUN_004779f0 /EXPORT:FUN_00477a40 /EXPORT:FUN_00477ab0 /EXPORT:FUN_00477b10 /EXPORT:FUN_00477b70 /EXPORT:FUN_00477bd0 /EXPORT:FUN_00477c20 /EXPORT:FUN_00477c80 /EXPORT:FUN_00477ce0 /EXPORT:FUN_00477d40 /EXPORT:FUN_004793e0 /EXPORT:FUN_00479460 /EXPORT:FUN_0047ed60 /EXPORT:FUN_0040f920 /EXPORT:FUN_0046fde0)
add_library(pharaoh_drag_footprint_verify SHARED EXCLUDE_FROM_ALL "${ROOT}/runtime/menu_drag_footprint.c")
restore_native_dll(pharaoh_drag_footprint_verify /EXPORT:FUN_0047cc70 /EXPORT:FUN_0047d060 /EXPORT:FUN_0047d360 /EXPORT:FUN_0047d4d0)
add_library(pharaoh_drag_region_gate_verify SHARED EXCLUDE_FROM_ALL "${ROOT}/runtime/menu_drag_region_gate.c" "${ROOT}/src/unit_01011.c" "${ROOT}/src/unit_01012.c")
restore_native_dll(pharaoh_drag_region_gate_verify /EXPORT:FUN_0047d530 /EXPORT:FUN_0046f9a0 /EXPORT:FUN_0046f930)
add_library(pharaoh_drag_neighbor_verify SHARED EXCLUDE_FROM_ALL "${ROOT}/runtime/menu_drag_neighbors.c")
restore_native_dll(pharaoh_drag_neighbor_verify /EXPORT:FUN_00477690 /EXPORT:FUN_00477eb0 /EXPORT:FUN_004781a0 /EXPORT:FUN_0047cae0)
add_library(pharaoh_drag_refresh_tile_verify SHARED EXCLUDE_FROM_ALL "${ROOT}/runtime/menu_drag_refresh_tile.c" "${ROOT}/src/unit_01011.c")
restore_native_dll(pharaoh_drag_refresh_tile_verify /EXPORT:FUN_00477430 /EXPORT:FUN_00479ac0 /EXPORT:FUN_0046f930)
add_library(pharaoh_drag_ground_verify SHARED EXCLUDE_FROM_ALL "${ROOT}/runtime/menu_drag_ground.c"
 "${ROOT}/src/unit_01198.c" "${ROOT}/src/unit_01097.c" "${ROOT}/src/unit_01011.c" "${ROOT}/runtime/menu_drag_neighbors.c")
restore_native_dll(pharaoh_drag_ground_verify /EXPORT:FUN_00475bf0 /EXPORT:FUN_00479770 /EXPORT:FUN_00486600 /EXPORT:FUN_00481490
 /EXPORT:FUN_00481aa0 /EXPORT:FUN_00477690 /EXPORT:FUN_0047a1c0 /EXPORT:FUN_0046f930)
add_library(pharaoh_drag_water_refresh_verify SHARED EXCLUDE_FROM_ALL "${ROOT}/runtime/menu_drag_water_refresh.c" "${ROOT}/runtime/menu_drag_ground.c"
 "${ROOT}/src/unit_01198.c" "${ROOT}/src/unit_01097.c" "${ROOT}/src/unit_01011.c" "${ROOT}/runtime/menu_drag_neighbors.c" "${ROOT}/src/unit_01233.c")
restore_native_dll(pharaoh_drag_water_refresh_verify /EXPORT:FUN_004776f0 /EXPORT:FUN_0047ab10 /EXPORT:FUN_0047db00 /EXPORT:FUN_00484f00
 /EXPORT:FUN_00484be0 /EXPORT:FUN_00486600 /EXPORT:FUN_00481490 /EXPORT:FUN_00481aa0 /EXPORT:FUN_00477690 /EXPORT:FUN_0047a1c0 /EXPORT:FUN_0046f930)
add_library(pharaoh_actor_message_verify SHARED EXCLUDE_FROM_ALL "${ROOT}/runtime/menu_actor_message.c"
 "${ROOT}/src/unit_00720.c" "${ROOT}/src/unit_01602.c" "${ROOT}/src/unit_01606.c"
 "${ROOT}/src/unit_01603.c" "${ROOT}/src/unit_01605.c" "${ROOT}/src/unit_01731.c"
 "${ROOT}/runtime/menu_building_kind.c" "${ROOT}/src/unit_01275.c")
restore_native_dll(pharaoh_actor_message_verify /EXPORT:FUN_0053c100 /EXPORT:FUN_00495b60 /EXPORT:FUN_0053c060
 /EXPORT:restore_actor_message_dispatch /EXPORT:restore_actor_message_failure /EXPORT:abort)
foreach(_actor_address IN ITEMS 004434e0 004435f0 0044d6d0 0044d700 00459240 0045b230 0045cf50 0045d180 0045d510 004893c0 00489470 004894a0 00495b60 004bf9d0 0053c060 0053c100 0053d3a0)
 target_link_options(pharaoh_actor_message_verify PRIVATE "/EXPORT:restore_actor_message_C_${_actor_address}")
endforeach()
add_library(pharaoh_drag_road_refresh_verify SHARED EXCLUDE_FROM_ALL "${ROOT}/runtime/menu_drag_road_refresh.c" "${ROOT}/src/unit_01011.c" "${ROOT}/src/unit_01141.c" "${ROOT}/src/unit_01233.c")
restore_native_dll(pharaoh_drag_road_refresh_verify /EXPORT:FUN_004770f0 /EXPORT:FUN_004798e0 /EXPORT:FUN_00484f20 /EXPORT:FUN_0046f930 /EXPORT:FUN_0047dee0 /EXPORT:FUN_00484be0)
add_library(pharaoh_drag_road_variant_verify SHARED EXCLUDE_FROM_ALL "${ROOT}/runtime/menu_drag_road_variant.c" "${ROOT}/src/unit_01138.c")
restore_native_dll(pharaoh_drag_road_variant_verify /EXPORT:FUN_00475fd0 /EXPORT:FUN_0047dbc0)
add_library(pharaoh_drag_building_state_verify SHARED EXCLUDE_FROM_ALL "${ROOT}/runtime/menu_drag_building_state.c" "${ROOT}/src/unit_02326.c")
set_source_files_properties("${ROOT}/src/unit_02326.c" PROPERTIES COMPILE_OPTIONS /wd4100)
restore_native_dll(pharaoh_drag_building_state_verify /EXPORT:FUN_004f3d70 /EXPORT:FUN_004f60d0 /EXPORT:FUN_0051c5f0)
add_library(pharaoh_drag_queue_publish_verify SHARED EXCLUDE_FROM_ALL "${ROOT}/runtime/menu_drag_queue_publish.c"
 "${ROOT}/src/unit_02085.c" "${ROOT}/src/unit_02089.c" "${ROOT}/src/unit_01011.c" "${ROOT}/src/unit_01012.c")
restore_native_dll(pharaoh_drag_queue_publish_verify /EXPORT:FUN_004f6130 /EXPORT:FUN_004ecea0 /EXPORT:FUN_004ed360 /EXPORT:FUN_0046f9a0 /EXPORT:FUN_0046f930)
target_link_libraries(pharaoh_drag_queue_publish_verify PRIVATE
 "$<IF:$<CONFIG:Debug>,${ROOT}/build/c-only-debug/Debug/pharaoh_restored.lib,${ROOT}/build/c-only/Release/pharaoh_restored.lib>"
 "$<IF:$<CONFIG:Debug>,${ROOT}/build/c-only-debug/Debug/restore_data.lib,${ROOT}/build/c-only/Release/restore_data.lib>"
 kernel32 user32 gdi32 shell32 ole32 oleaut32 advapi32 winmm ddraw dxguid legacy_stdio_definitions)
file(SHA256 "${ROOT}/artifacts/mission_unlock_phase/CMakeLists.txt" unlock_recipe_sha)
if(NOT unlock_recipe_sha STREQUAL "d9952579b67378a79788608fa31c2611e594d461578162c2f2b2b3f3aafea2fb")
 message(FATAL_ERROR "Frozen unlock recipe drift")
endif()
file(READ "${ROOT}/artifacts/mission_unlock_phase/CMakeLists.txt" unlock_recipe)
string(REGEX MATCHALL "/EXPORT:[A-Za-z_0-9]+" unlock_exports "${unlock_recipe}")
add_library(pharaoh_mission_unlock_verify SHARED EXCLUDE_FROM_ALL "${ROOT}/runtime/mission_unlock.c" "${ROOT}/runtime/tool_availability.c")
restore_native_dll(pharaoh_mission_unlock_verify ${unlock_exports} /MAP)
target_link_libraries(pharaoh_mission_unlock_verify PRIVATE
 "$<IF:$<CONFIG:Debug>,${ROOT}/build/c-only-debug/Debug/pharaoh_restored.lib,${ROOT}/build/c-only/Release/pharaoh_restored.lib>"
 "$<IF:$<CONFIG:Debug>,${ROOT}/build/c-only-debug/Debug/restore_data.lib,${ROOT}/build/c-only/Release/restore_data.lib>"
 kernel32 user32 gdi32 shell32 ole32 oleaut32 advapi32 winmm ddraw dxguid legacy_stdio_definitions)
add_library(pharaoh_drag_canal_refresh_verify SHARED EXCLUDE_FROM_ALL "${ROOT}/runtime/menu_drag_canal_refresh.c"
 "${ROOT}/src/unit_01152.c" "${ROOT}/src/unit_01153.c" "${ROOT}/src/unit_01233.c" "${ROOT}/src/unit_01110.c" "${ROOT}/src/unit_02327.c")
restore_native_dll(pharaoh_drag_canal_refresh_verify /EXPORT:FUN_0047bd50 /EXPORT:FUN_0047be30 /EXPORT:FUN_0047e870 /EXPORT:FUN_0047eb60 /EXPORT:FUN_00484be0 /EXPORT:FUN_0047b920 /EXPORT:FUN_0051c620)
add_library(pharaoh_drag_building_graph_verify SHARED EXCLUDE_FROM_ALL "${ROOT}/runtime/menu_drag_building_graph.c")
restore_native_dll(pharaoh_drag_building_graph_verify /EXPORT:FUN_004f3f70 /EXPORT:FUN_004f3d70 /EXPORT:FUN_00475bf0 /EXPORT:FUN_004767d0 /EXPORT:FUN_00477500 /EXPORT:FUN_004f6130)
add_library(pharaoh_drag_road_place_verify SHARED EXCLUDE_FROM_ALL "${ROOT}/runtime/menu_drag_road_place.c")
restore_native_dll(pharaoh_drag_road_place_verify /EXPORT:FUN_0047bf50 /EXPORT:FUN_004746d0 /EXPORT:FUN_004767d0 /EXPORT:FUN_004f6130)
foreach(_drag_archive_target IN ITEMS pharaoh_drag_building_graph_verify pharaoh_drag_road_place_verify)
 target_link_libraries(${_drag_archive_target} PRIVATE
  "$<IF:$<CONFIG:Debug>,${ROOT}/build/c-only-debug/Debug/pharaoh_restored.lib,${ROOT}/build/c-only/Release/pharaoh_restored.lib>"
  "$<IF:$<CONFIG:Debug>,${ROOT}/build/c-only-debug/Debug/restore_data.lib,${ROOT}/build/c-only/Release/restore_data.lib>"
  kernel32 user32 gdi32 shell32 ole32 oleaut32 advapi32 winmm ddraw dxguid legacy_stdio_definitions)
endforeach()

# Complete preparation parent with genuine shared boundary and main C graph.
add_library(pharaoh_prepare_control_graph_verify SHARED EXCLUDE_FROM_ALL menu_004f7e50.c)
target_include_directories(pharaoh_prepare_control_graph_verify PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
restore_native_dll(pharaoh_prepare_control_graph_verify /EXPORT:restore_C_004f7e50 /EXPORT:restore_menu_input_boundary_dispatch)
target_link_libraries(pharaoh_prepare_control_graph_verify PRIVATE
 "$<IF:$<CONFIG:Debug>,${ROOT}/build/c-only-debug/Debug/pharaoh_restored.lib,${ROOT}/build/c-only/Release/pharaoh_restored.lib>"
 "$<IF:$<CONFIG:Debug>,${ROOT}/build/c-only-debug/Debug/restore_data.lib,${ROOT}/build/c-only/Release/restore_data.lib>"
 kernel32 user32 gdi32 shell32 ole32 oleaut32 advapi32 winmm ddraw dxguid legacy_stdio_definitions)
# Use the actual main-library raw parent; compile only its typed facade here.
add_library(pharaoh_prepare_control_entry_verify SHARED EXCLUDE_FROM_ALL "${ROOT}/runtime/menu_prepare_control.c")
target_include_directories(pharaoh_prepare_control_entry_verify PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
restore_native_dll(pharaoh_prepare_control_entry_verify /EXPORT:FUN_004f7e50 /EXPORT:restore_C_004f7e50 /EXPORT:restore_menu_input_boundary_dispatch)
target_link_libraries(pharaoh_prepare_control_entry_verify PRIVATE
 "$<IF:$<CONFIG:Debug>,${ROOT}/build/c-only-debug/Debug/pharaoh_restored.lib,${ROOT}/build/c-only/Release/pharaoh_restored.lib>"
 "$<IF:$<CONFIG:Debug>,${ROOT}/build/c-only-debug/Debug/restore_data.lib,${ROOT}/build/c-only/Release/restore_data.lib>"
 kernel32 user32 gdi32 shell32 ole32 oleaut32 advapi32 winmm ddraw dxguid legacy_stdio_definitions)

add_library(pharaoh_drag_terrain_graph_verify SHARED EXCLUDE_FROM_ALL "${ROOT}/runtime/menu_drag_terrain_graph.c" "${ROOT}/runtime/menu_drag_building_graph.c")
restore_native_dll(pharaoh_drag_terrain_graph_verify /EXPORT:FUN_004767d0 /EXPORT:FUN_004f3f70 /EXPORT:FUN_004f3d70 /EXPORT:FUN_004f60d0 /EXPORT:FUN_004f6130 /EXPORT:FUN_0047c140 /EXPORT:FUN_0047c880 /EXPORT:FUN_0047db00 /EXPORT:FUN_00484f40 /EXPORT:FUN_00484be0 /EXPORT:FUN_0051c5f0 /EXPORT:FUN_0046f9a0 /EXPORT:FUN_0046f930 /EXPORT:FUN_004ecea0 /EXPORT:FUN_004ed360)
target_link_libraries(pharaoh_drag_terrain_graph_verify PRIVATE
 "$<IF:$<CONFIG:Debug>,${ROOT}/build/c-only-debug/Debug/pharaoh_restored.lib,${ROOT}/build/c-only/Release/pharaoh_restored.lib>"
 "$<IF:$<CONFIG:Debug>,${ROOT}/build/c-only-debug/Debug/restore_data.lib,${ROOT}/build/c-only/Release/restore_data.lib>"
 kernel32 user32 gdi32 shell32 ole32 oleaut32 advapi32 winmm ddraw dxguid legacy_stdio_definitions)

add_library(pharaoh_drag_road_pack_verify SHARED EXCLUDE_FROM_ALL "${ROOT}/runtime/menu_drag_road_pack.c" "${ROOT}/runtime/menu_drag_terrain_graph.c")
restore_native_dll(pharaoh_drag_road_pack_verify /EXPORT:FUN_00475dc0 /EXPORT:FUN_00475fd0 /EXPORT:FUN_0047cae0 /EXPORT:FUN_0047bf50 /EXPORT:FUN_004746d0 /EXPORT:FUN_004767d0 /EXPORT:FUN_004f6130 /EXPORT:FUN_0047dbc0)
target_link_libraries(pharaoh_drag_road_pack_verify PRIVATE
 "$<IF:$<CONFIG:Debug>,${ROOT}/build/c-only-debug/Debug/pharaoh_restored.lib,${ROOT}/build/c-only/Release/pharaoh_restored.lib>"
 "$<IF:$<CONFIG:Debug>,${ROOT}/build/c-only-debug/Debug/restore_data.lib,${ROOT}/build/c-only/Release/restore_data.lib>"
 kernel32 user32 gdi32 shell32 ole32 oleaut32 advapi32 winmm ddraw dxguid legacy_stdio_definitions)

add_library(pharaoh_drag_refresh_dispatch_verify SHARED EXCLUDE_FROM_ALL "${ROOT}/runtime/menu_drag_refresh_dispatch.c" "${ROOT}/runtime/menu_drag_terrain_graph.c" "${ROOT}/runtime/menu_drag_road_pack.c")
restore_native_dll(pharaoh_drag_refresh_dispatch_verify /EXPORT:FUN_00477dd0 /EXPORT:FUN_00475cc0 /EXPORT:FUN_004877f0 /EXPORT:FUN_004776f0 /EXPORT:FUN_004770f0 /EXPORT:FUN_00475dc0 /EXPORT:FUN_00477430 /EXPORT:FUN_004767d0 /EXPORT:FUN_004777c0 /EXPORT:FUN_0047b590 /EXPORT:FUN_0047bd50 /EXPORT:FUN_00477500 /EXPORT:FUN_004f6130)
target_link_libraries(pharaoh_drag_refresh_dispatch_verify PRIVATE
 "$<IF:$<CONFIG:Debug>,${ROOT}/build/c-only-debug/Debug/pharaoh_restored.lib,${ROOT}/build/c-only/Release/pharaoh_restored.lib>"
 "$<IF:$<CONFIG:Debug>,${ROOT}/build/c-only-debug/Debug/restore_data.lib,${ROOT}/build/c-only/Release/restore_data.lib>"
 kernel32 user32 gdi32 shell32 ole32 oleaut32 advapi32 winmm ddraw dxguid legacy_stdio_definitions)

add_library(pharaoh_drag_flood_edges_verify SHARED EXCLUDE_FROM_ALL "${ROOT}/runtime/menu_drag_flood_edges.c" "${ROOT}/runtime/menu_drag_refresh_dispatch.c" "${ROOT}/runtime/menu_drag_terrain_graph.c" "${ROOT}/runtime/menu_drag_road_pack.c")
restore_native_dll(pharaoh_drag_flood_edges_verify /EXPORT:FUN_00477f70 /EXPORT:FUN_00478090 /EXPORT:FUN_00478270 /EXPORT:FUN_00477eb0 /EXPORT:FUN_004781a0 /EXPORT:FUN_00477690 /EXPORT:FUN_00477dd0 /EXPORT:FUN_00475cc0 /EXPORT:FUN_004877f0 /EXPORT:FUN_004776f0 /EXPORT:FUN_004770f0 /EXPORT:FUN_00475dc0 /EXPORT:FUN_00477430 /EXPORT:FUN_004767d0 /EXPORT:FUN_004777c0 /EXPORT:FUN_0047b590 /EXPORT:FUN_0047bd50 /EXPORT:FUN_00477500 /EXPORT:FUN_004f6130 /EXPORT:FUN_00479e70 /EXPORT:FUN_0046f930 /EXPORT:FUN_0047db00 /EXPORT:FUN_00484f40 /EXPORT:FUN_00484be0)
target_link_libraries(pharaoh_drag_flood_edges_verify PRIVATE
 "$<IF:$<CONFIG:Debug>,${ROOT}/build/c-only-debug/Debug/pharaoh_restored.lib,${ROOT}/build/c-only/Release/pharaoh_restored.lib>"
 "$<IF:$<CONFIG:Debug>,${ROOT}/build/c-only-debug/Debug/restore_data.lib,${ROOT}/build/c-only/Release/restore_data.lib>"
 kernel32 user32 gdi32 shell32 ole32 oleaut32 advapi32 winmm ddraw dxguid legacy_stdio_definitions)

add_library(pharaoh_map_commit_fields_verify SHARED EXCLUDE_FROM_ALL "${ROOT}/runtime/map_commit_fields.c" "${ROOT}/src/unit_00119.c" "${ROOT}/src/unit_01151.c" "${ROOT}/src/unit_01011.c")
restore_native_dll(pharaoh_map_commit_fields_verify /EXPORT:FUN_00517690 /EXPORT:FUN_005178b0 /EXPORT:FUN_00479670 /EXPORT:FUN_0040f8f0 /EXPORT:FUN_0047e830 /EXPORT:FUN_0046f930)

add_library(pharaoh_map_commit_costs_verify SHARED EXCLUDE_FROM_ALL "${ROOT}/runtime/map_commit_costs.c" "${ROOT}/src/unit_00119.c")
restore_native_dll(pharaoh_map_commit_costs_verify /EXPORT:FUN_005173a0 /EXPORT:FUN_00517660 /EXPORT:FUN_005178e0 /EXPORT:FUN_0040f8f0)
set_source_files_properties("${ROOT}/src/unit_01151.c" PROPERTIES COMPILE_OPTIONS /wd4244)

add_library(pharaoh_map_commit_refresh_verify SHARED EXCLUDE_FROM_ALL "${ROOT}/runtime/map_commit_refresh.c" "${ROOT}/runtime/map_commit_fields.c" "${ROOT}/runtime/menu_drag_road_pack.c" "${ROOT}/runtime/menu_drag_terrain_graph.c")
restore_native_dll(pharaoh_map_commit_refresh_verify /EXPORT:FUN_004795e0 /EXPORT:FUN_00479670 /EXPORT:FUN_0051f780 /EXPORT:FUN_00475dc0 /EXPORT:FUN_004767d0 /EXPORT:FUN_00477500 /EXPORT:FUN_004777c0)
target_link_libraries(pharaoh_map_commit_refresh_verify PRIVATE
 "$<IF:$<CONFIG:Debug>,${ROOT}/build/c-only-debug/Debug/pharaoh_restored.lib,${ROOT}/build/c-only/Release/pharaoh_restored.lib>"
 "$<IF:$<CONFIG:Debug>,${ROOT}/build/c-only-debug/Debug/restore_data.lib,${ROOT}/build/c-only/Release/restore_data.lib>"
 kernel32 user32 gdi32 shell32 ole32 oleaut32 advapi32 winmm ddraw dxguid legacy_stdio_definitions)

add_library(pharaoh_map_commit_state_verify SHARED EXCLUDE_FROM_ALL "${ROOT}/runtime/map_commit_state.c")
restore_native_dll(pharaoh_map_commit_state_verify /EXPORT:FUN_00485200 /EXPORT:FUN_00485830 /EXPORT:FUN_004e9680)

add_library(pharaoh_map_commit_selection_verify SHARED EXCLUDE_FROM_ALL "${ROOT}/runtime/map_commit_selection.c" "${ROOT}/src/unit_00428.c")
restore_native_dll(pharaoh_map_commit_selection_verify /EXPORT:FUN_004f44e0 /EXPORT:FUN_0053d930 /EXPORT:FUN_004f4b70 /EXPORT:FUN_004f61e0 /EXPORT:FUN_00426b30)

# Keep each reviewed export contract while compiling current canonical bodies.
function(restore_reviewed_exports target recipe expected)
 file(SHA256 "${ROOT}/${recipe}" _sha)
 if(NOT _sha STREQUAL "${expected}")
  message(FATAL_ERROR "Reviewed verification recipe changed: ${recipe}")
 endif()
 file(READ "${ROOT}/${recipe}" _recipe)
 string(REGEX MATCHALL "/EXPORT:[A-Za-z_0-9]+" _exports "${_recipe}")
 foreach(_alias IN LISTS ARGN)
  if(NOT _alias MATCHES "^([A-Za-z_0-9]+)=([A-Za-z_0-9]+)$")
   message(FATAL_ERROR "Invalid reviewed export alias: ${_alias}")
  endif()
  set(_public "${CMAKE_MATCH_1}")
  set(_private "${CMAKE_MATCH_2}")
  list(FIND _exports "/EXPORT:${_public}" _index)
  if(_index LESS 0)
   message(FATAL_ERROR "Alias is absent from reviewed export contract: ${_public}")
  endif()
  list(REMOVE_AT _exports ${_index})
  list(INSERT _exports ${_index} "/EXPORT:${_public}=${_private}")
 endforeach()
 restore_native_dll(${target} ${_exports})
endfunction()

add_library(pharaoh_drag_refresh_wrappers_verify SHARED EXCLUDE_FROM_ALL
 "${ROOT}/runtime/menu_drag_refresh_wrappers.c" "${ROOT}/runtime/menu_drag_paving_pack.c"
 "${ROOT}/runtime/menu_drag_terrain_graph.c" "${ROOT}/runtime/menu_drag_road_pack.c")
restore_reviewed_exports(pharaoh_drag_refresh_wrappers_verify artifacts/menu_drag_refresh_wrappers_phase/CMakeLists.txt 2f57bf4c5ec2655ceaa2a9be352789bdc4f8c75ac4f496319a44ae487897ab8a)

add_library(pharaoh_drag_growth_verify SHARED EXCLUDE_FROM_ALL
 "${ROOT}/runtime/menu_drag_neighbor_growth.c" "${ROOT}/runtime/menu_drag_growth_numeric.c"
 "${ROOT}/runtime/map_refresh_fp.c" "${ROOT}/runtime/map_x87_conversion.c"
 "${ROOT}/src/unit_01260.c" "${ROOT}/src/unit_01261.c" "${ROOT}/src/unit_01262.c" "${ROOT}/src/unit_01263.c" "${ROOT}/src/unit_01238.c")
restore_reviewed_exports(pharaoh_drag_growth_verify artifacts/menu_drag_growth_phase/CMakeLists.txt 77b5c3d917d54d0b766835bc35213af26007c8aa7dc9eb7d7f0d7b6edf56755c)

add_library(pharaoh_drag_growth_update_verify SHARED EXCLUDE_FROM_ALL
 "${ROOT}/runtime/menu_drag_growth_update.c" "${ROOT}/runtime/menu_drag_neighbor_growth.c" "${ROOT}/runtime/menu_drag_growth_numeric.c"
 "${ROOT}/runtime/map_refresh_fp.c" "${ROOT}/runtime/map_x87_conversion.c"
 "${ROOT}/src/unit_01256.c" "${ROOT}/src/unit_01233.c" "${ROOT}/src/unit_01260.c" "${ROOT}/src/unit_01261.c" "${ROOT}/src/unit_01262.c" "${ROOT}/src/unit_01263.c" "${ROOT}/src/unit_01238.c")
restore_reviewed_exports(pharaoh_drag_growth_update_verify artifacts/menu_drag_growth_update_phase/CMakeLists.txt 8c2b6a219d3eab51faac574d45337fbbf30f813c7ab090d00274e17b7b363bac)

add_library(pharaoh_map_commit_warnings_verify SHARED EXCLUDE_FROM_ALL "${ROOT}/runtime/map_commit_warnings.c")
restore_reviewed_exports(pharaoh_map_commit_warnings_verify artifacts/map_commit_warnings_phase/CMakeLists.txt 5adbbb470d7ddbb65de9b0434457348891c09e3b2a0db4a688798d126f906fa5)

add_library(pharaoh_map_commit_proximity_verify SHARED EXCLUDE_FROM_ALL "${ROOT}/runtime/map_commit_proximity.c" "${ROOT}/src/unit_02332.c")
restore_reviewed_exports(pharaoh_map_commit_proximity_verify artifacts/map_commit_proximity_phase/CMakeLists.txt df32e0c707e4c5db668302ae1cf9446c6ec5856816b5e5d87b901c8161bc7d6b)
add_library(pharaoh_map_commit_resource_warnings_verify SHARED EXCLUDE_FROM_ALL "${ROOT}/runtime/map_commit_resource_warnings.c")
restore_reviewed_exports(pharaoh_map_commit_resource_warnings_verify artifacts/map_commit_resource_warnings_phase/CMakeLists.txt 467e599f8ffc4d846441f9f08255ecdfb04599e351f2ae409b38355f7ee1f73c)
add_library(pharaoh_map_commit_resource_gate_verify SHARED EXCLUDE_FROM_ALL "${ROOT}/runtime/map_commit_resource_gate.c")
restore_reviewed_exports(pharaoh_map_commit_resource_gate_verify artifacts/map_commit_resource_gate_phase/CMakeLists.txt d403683863ea8f9e7885f10d6249bbe2cb46a2593a77fbcb916f5d888e8281e1)
add_library(pharaoh_map_commit_town_warning_verify SHARED EXCLUDE_FROM_ALL "${ROOT}/runtime/map_commit_town_warning.c")
restore_reviewed_exports(pharaoh_map_commit_town_warning_verify artifacts/map_commit_town_warning_phase/CMakeLists.txt 0e34b2a23e2edf9ff63881742ff7250bb7206cf4bea5a2119a5769542f0fd726)

set(_reviewed_ring_aliases FUN_004f6560=restore_ring_004f6560 FUN_004f65f0=restore_ring_004f65f0 FUN_004f6620=restore_ring_004f6620 FUN_004f6630=restore_ring_004f6630)
add_library(pharaoh_map_commit_links_verify SHARED EXCLUDE_FROM_ALL "${ROOT}/runtime/map_commit_links.c")
restore_reviewed_exports(pharaoh_map_commit_links_verify artifacts/map_commit_links_phase/CMakeLists.txt fb0008c791accf079344a5c38af7a04da271ad4d1da7838e7539cf96e12e344c ${_reviewed_ring_aliases})
add_library(pharaoh_map_commit_paths_verify SHARED EXCLUDE_FROM_ALL "${ROOT}/runtime/map_commit_paths.c")
restore_reviewed_exports(pharaoh_map_commit_paths_verify artifacts/map_commit_paths_phase/CMakeLists.txt 8867b67a70fb500048ae45deee29a93e885eec4d3ed8c414be74ca58c147d201)
add_library(pharaoh_drag_paving_neighbors_verify SHARED EXCLUDE_FROM_ALL
 "${ROOT}/runtime/menu_drag_paving_neighbors.c" "${ROOT}/src/unit_01142.c")
restore_reviewed_exports(pharaoh_drag_paving_neighbors_verify artifacts/menu_drag_paving_neighbors_phase/CMakeLists.txt f2a0e5647f7bb0ba46ff1e5a8c6bc3d882f453d78c67db98bef96a609f7697c0)

add_library(pharaoh_map_commit_connect_verify SHARED EXCLUDE_FROM_ALL "${ROOT}/runtime/map_commit_connect.c" "${ROOT}/runtime/map_commit_paths.c" "${ROOT}/runtime/map_commit_links.c")
restore_reviewed_exports(pharaoh_map_commit_connect_verify artifacts/map_commit_connect_phase/CMakeLists.txt 4364c8bc0f3cf15acf232c27b90d53a19ebdb66e1c0991d702f3fe66ccc290a7 ${_reviewed_ring_aliases})

set(_canonical_surface_sources "${ROOT}/runtime/menu_drag_surface_passes.c"
 "${ROOT}/runtime/menu_drag_paving_pass_model.c" "${ROOT}/runtime/menu_drag_vegetation_pass_model.c" "${ROOT}/runtime/menu_drag_rail_pass_model.c")
set(_canonical_gate_sources "${ROOT}/runtime/menu_drag_map_gates.c"
 "${ROOT}/runtime/menu_drag_gate_query_model.c" "${ROOT}/runtime/menu_drag_gate_publish_model.c")
set(_canonical_canal_region_sources "${ROOT}/runtime/menu_drag_canal_region.c" "${ROOT}/runtime/menu_drag_canal_region_model.c")
add_library(pharaoh_drag_surface_passes_verify SHARED EXCLUDE_FROM_ALL ${_canonical_surface_sources})
restore_reviewed_exports(pharaoh_drag_surface_passes_verify artifacts/menu_drag_surface_passes_phase/CMakeLists.txt 858f0b1e56f54cebabebc5937a0d1617351038b100350d4348e451cd0cd9e323)
add_library(pharaoh_drag_map_gates_verify SHARED EXCLUDE_FROM_ALL ${_canonical_gate_sources})
restore_reviewed_exports(pharaoh_drag_map_gates_verify artifacts/menu_drag_map_gates_phase/CMakeLists.txt c4f9f943954e6c529524b6fa9519859c821b2e446028250722d8f2bab9794c7d)
add_library(pharaoh_drag_canal_region_verify SHARED EXCLUDE_FROM_ALL ${_canonical_canal_region_sources})
restore_reviewed_exports(pharaoh_drag_canal_region_verify artifacts/menu_drag_canal_region_phase/CMakeLists.txt 8a891644a0be1770628ff29017f2d09f8c0fc2c143fa07ad721351b4793570d9)
add_library(pharaoh_drag_redraw_dispatch_verify SHARED EXCLUDE_FROM_ALL
 "${ROOT}/runtime/menu_drag_redraw_dispatch.c" "${ROOT}/runtime/menu_drag_redraw_dispatch_model.c"
 ${_canonical_surface_sources} ${_canonical_gate_sources} ${_canonical_canal_region_sources})
restore_reviewed_exports(pharaoh_drag_redraw_dispatch_verify artifacts/menu_drag_redraw_dispatch_phase/CMakeLists.txt 2027e2ba0319b6e91349f3d7e892d9273ae5c06772f38414bb539e3cb7eba101)
add_library(pharaoh_drag_preview_geometry_verify SHARED EXCLUDE_FROM_ALL "${ROOT}/runtime/menu_drag_preview_geometry.c")
restore_reviewed_exports(pharaoh_drag_preview_geometry_verify artifacts/menu_drag_preview_geometry_phase/CMakeLists.txt cdbe6e39851466ffabdff579c53a08dea291db11624a202df73af40fc43a8a21)
add_library(pharaoh_drag_route_query_verify SHARED EXCLUDE_FROM_ALL
 "${ROOT}/runtime/menu_drag_route_query.c" "${ROOT}/runtime/menu_drag_route_query_model.c")
restore_reviewed_exports(pharaoh_drag_route_query_verify artifacts/menu_drag_route_query_phase/CMakeLists.txt 67840fd1208344a3bed6cbf835324ee38194829e7282e6f49cc05d9fdc6ebebf)

add_library(pharaoh_drag_actor_graph_verify SHARED EXCLUDE_FROM_ALL
 "${ROOT}/runtime/menu_drag_actor_teardown.c" "${ROOT}/runtime/menu_drag_actor_teardown_model.c"
 "${ROOT}/runtime/menu_drag_actor_profile.c" "${ROOT}/runtime/menu_drag_actor_profile_model.c"
 "${ROOT}/runtime/menu_drag_actor_create.c" "${ROOT}/runtime/menu_drag_actor_spawn.c" "${ROOT}/runtime/menu_drag_actor_move.c"
 "${ROOT}/runtime/menu_drag_building_cleanup.c" "${ROOT}/runtime/menu_drag_group_clear.c"
 "${ROOT}/runtime/menu_drag_actor_walk_link.c" "${ROOT}/runtime/menu_drag_actor_walk_unlink.c"
 "${ROOT}/runtime/actor_walking_facades.c")
restore_reviewed_exports(pharaoh_drag_actor_graph_verify artifacts/menu_drag_actor_teardown_phase/CMakeLists.txt d37d0f5304cf7981812128b46226a11a3ff1e439e5564c1e9537444143777643)
restore_reviewed_exports(pharaoh_drag_actor_graph_verify artifacts/menu_drag_actor_profile_phase/CMakeLists.txt 8b7eb8ce5170b0e9dab948ff6df969e9b65701f69dc9d022dff981f10958a9fd)
restore_reviewed_exports(pharaoh_drag_actor_graph_verify artifacts/menu_drag_actor_create_phase/CMakeLists.txt e0c40fdc04342a14784ab2bc0d6143323dbfc0df27657dc0fd642887c1162ace)
restore_reviewed_exports(pharaoh_drag_actor_graph_verify artifacts/menu_drag_actor_spawn_phase/CMakeLists.txt 4339fbfcde38d09a978a1f7d9a089083b665b2b596a6841df2e05bce282b2a7c)
restore_reviewed_exports(pharaoh_drag_actor_graph_verify artifacts/menu_drag_actor_move_phase/CMakeLists.txt d62ce78cc97dcc27649d56377c742af03b553863aa2af0db694e6f577813da88)
restore_reviewed_exports(pharaoh_drag_actor_graph_verify artifacts/menu_drag_building_cleanup_phase/CMakeLists.txt 82d465d236d006327b70e02bf165d5045d68af1edbe5d4a9ce368bf54fcf0709)
restore_reviewed_exports(pharaoh_drag_actor_graph_verify artifacts/menu_drag_group_clear_phase/CMakeLists.txt 80528a2a5bdd7a9d1f9a98353deea6b0d2b60276f0ef40e91d8484759d59eee2)

add_library(pharaoh_menu_commit_graph_verify SHARED EXCLUDE_FROM_ALL
 "${ROOT}/runtime/map_commit_population.c"
 "${ROOT}/runtime/menu_drag_monument_unlink.c" "${ROOT}/runtime/menu_drag_monument_clear.c" "${ROOT}/runtime/menu_drag_monument_clear_model.c"
 "${ROOT}/runtime/menu_drag_building_release.c" "${ROOT}/runtime/menu_drag_building_release_model.c"
 "${ROOT}/runtime/menu_drag_commit.c" "${ROOT}/runtime/menu_drag_commit_model.c"
 "${ROOT}/runtime/menu_drag_paint_region.c" "${ROOT}/runtime/menu_drag_paving_region_model.c" "${ROOT}/runtime/menu_drag_vegetation_region_model.c"
 "${ROOT}/runtime/menu_drag_route_footprint.c" "${ROOT}/runtime/city_footprint_validation.c")
restore_reviewed_exports(pharaoh_menu_commit_graph_verify artifacts/map_commit_population_phase/CMakeLists.txt fcd78f48da9d7210de7ae64f99778b35429e388d15cac731b3b486e3641a79f7)
restore_reviewed_exports(pharaoh_menu_commit_graph_verify artifacts/menu_drag_monument_unlink_phase/CMakeLists.txt d87be5daecb475b2e4e5873c082873a55486e324c0278de909094ea05c02d2fe)
restore_reviewed_exports(pharaoh_menu_commit_graph_verify artifacts/menu_drag_monument_clear_phase/CMakeLists.txt 0561c4e190b34e0a097beb582c0fe7a63d92d9d05446fb43936460dbf5030142)
restore_reviewed_exports(pharaoh_menu_commit_graph_verify artifacts/menu_drag_building_release_phase/CMakeLists.txt 80a3f9d0f0fde97c05c322c1b76924ac7049248884cc717b8227d1fe420b3545)
restore_reviewed_exports(pharaoh_menu_commit_graph_verify artifacts/menu_drag_commit_phase/CMakeLists.txt d849845fa447944d848fc862c3e835e51a071fbdf1eedcb196cd92198b6243c4)
restore_reviewed_exports(pharaoh_menu_commit_graph_verify artifacts/menu_drag_paint_region_phase/CMakeLists.txt cde133cc29aec5fec7a08caa48bade8d27ff0f9771de44a47036e6e7d7730464)
restore_reviewed_exports(pharaoh_menu_commit_graph_verify artifacts/menu_drag_route_footprint_phase/CMakeLists.txt 903f16965c922b089d6cd5842592b29b61d607e78e0763fb689b1045800c0bf9)
restore_reviewed_exports(pharaoh_menu_commit_graph_verify artifacts/city_footprint_validation_phase/CMakeLists.txt f3d722ec2a8548f1ed845064fa820e2f28b0b7965b028873908c7c0fcd5c66d0)

add_library(pharaoh_menu_route_build_graph_verify SHARED EXCLUDE_FROM_ALL
 "${ROOT}/runtime/menu_drag_land_route.c"
 "${ROOT}/runtime/menu_drag_land_route_model.c"
 "${ROOT}/runtime/menu_drag_route_walk.c"
 "${ROOT}/runtime/menu_drag_route_walk_model.c"
 "${ROOT}/runtime/menu_drag_canal_paint_model.c"
 "${ROOT}/runtime/menu_drag_road_paint_model.c"
 "${ROOT}/runtime/menu_drag_route_paint.c"
 "${ROOT}/runtime/menu_drag_connection_plan.c"
 "${ROOT}/runtime/menu_drag_connection_plan_model.c"
 "${ROOT}/runtime/map_commit_placement.c"
 "${ROOT}/runtime/map_commit_route_actions.c"
 "${ROOT}/runtime/building_reservation.c"
 "${ROOT}/runtime/building_neighbor_query.c"
 "${ROOT}/runtime/building_nearest_search.c"
 "${ROOT}/runtime/menu_drag_world_offset.c"
 "${ROOT}/runtime/menu_drag_world_offset_model.c"
 "${ROOT}/runtime/construction_tiles.c"
 "${ROOT}/runtime/bridge_place.c"
 "${ROOT}/runtime/building_quality_update.c"
 "${ROOT}/runtime/displaced_population.c"
 "${ROOT}/runtime/menu_drag_construction_scan.c"
 "${ROOT}/runtime/menu_drag_empty_slots_model.c"
 "${ROOT}/runtime/menu_drag_footprint_mask_model.c"
 "${ROOT}/runtime/menu_drag_occupancy_model.c"
 "${ROOT}/runtime/menu_drag_road_scan_model.c"
 "${ROOT}/runtime/menu_drag_construction_parent.c"
 "${ROOT}/runtime/menu_drag_placement_model.c"
 "${ROOT}/runtime/menu_drag_road_check_model.c"
 "${ROOT}/runtime/construction_lines.c"
 "${ROOT}/runtime/construction_refresh.c")
restore_reviewed_exports(pharaoh_menu_route_build_graph_verify artifacts/menu_drag_land_route_phase/CMakeLists.txt b0a8b05a5539def77caaa6db50807f4fa6874d9a0240fab3d045ece3cbab3c7d)
restore_reviewed_exports(pharaoh_menu_route_build_graph_verify artifacts/menu_drag_route_walk_phase/CMakeLists.txt 250674c3b0c3935540236703b92407279d8918c57a8cb38ef9449d601931338c)
restore_reviewed_exports(pharaoh_menu_route_build_graph_verify artifacts/menu_drag_route_paint_phase/CMakeLists.txt 1dd07fccc93b484b998fec9c194726db2d622c2f2971b61f41cffa20a715fa01)
restore_reviewed_exports(pharaoh_menu_route_build_graph_verify artifacts/menu_drag_connection_plan_phase/CMakeLists.txt 9133daf7ea6f0297162f50b7d3539c407d5ffeda0c1c36f54fc6b68b7ebc7889)
restore_reviewed_exports(pharaoh_menu_route_build_graph_verify artifacts/map_commit_placement_phase/CMakeLists.txt 1f7dd6e8fe0a938868d4e1395f3a268fa240231fa4a9382e76db00b2c1798978 ${_reviewed_ring_aliases})
restore_reviewed_exports(pharaoh_menu_route_build_graph_verify artifacts/map_commit_route_actions_phase/CMakeLists.txt 1a269eeee9350702735782f0dd275c0852e97dcf2fe9cbd05b4fbc0df74e70c7 ${_reviewed_ring_aliases})
restore_reviewed_exports(pharaoh_menu_route_build_graph_verify artifacts/building_reservation_phase/CMakeLists.txt bd4252701571be16042f436a5f2f00dc03624da16d76186e2dd49dfee743488d)
restore_reviewed_exports(pharaoh_menu_route_build_graph_verify artifacts/building_neighbor_query_phase/CMakeLists.txt 44e0dd75371eeb801c58a9a40511c79b63f1de85046fed3fb840492e01cecc09)
restore_reviewed_exports(pharaoh_menu_route_build_graph_verify artifacts/building_nearest_search_phase/CMakeLists.txt 0cd664dce99672262cf17eec4613a6cee93af60d0e9a6e7169b2569d2922f156)
restore_reviewed_exports(pharaoh_menu_route_build_graph_verify artifacts/menu_drag_world_offset_phase/CMakeLists.txt a9e91c79220d8946b91d54989c6bf9a84e7b4735d5c5ab394cb4949e8af3eafd)
restore_reviewed_exports(pharaoh_menu_route_build_graph_verify artifacts/map_commit_construction_tiles_phase/CMakeLists.txt b2498eb16d93bca7979680ca48db91c230ee709321fa78749002c7ad8cdd2c73)
restore_reviewed_exports(pharaoh_menu_route_build_graph_verify artifacts/map_commit_bridge_place_phase/CMakeLists.txt 6f7a41cf35520aa0fd46847f86b21f0773f7e1f7b0315eba304994f182ddf48a)
restore_reviewed_exports(pharaoh_menu_route_build_graph_verify artifacts/building_quality_update_phase/CMakeLists.txt 9fab80e6f6b95359b473009e60539a524fdaab84b8f075ab2e86b387a6dd0935)
restore_reviewed_exports(pharaoh_menu_route_build_graph_verify artifacts/map_commit_displaced_population_phase/CMakeLists.txt c8b246ad84805eb496584e1d2c4bfe56ab3e158e2547f2faff0807ec46f16e7c)
restore_reviewed_exports(pharaoh_menu_route_build_graph_verify artifacts/menu_drag_construction_scan_phase/CMakeLists.txt 3c55fa01111671a079610d66ae57b9e17f18bc6ddb999bb53ec4bfff34a62151)
restore_reviewed_exports(pharaoh_menu_route_build_graph_verify artifacts/menu_drag_construction_parent_phase/CMakeLists.txt 6303d52b624d572c1c741b5194af1f49c49679a27ee5ff0fbf5997f85e95f3f5)
restore_reviewed_exports(pharaoh_menu_route_build_graph_verify artifacts/map_commit_construction_lines_phase/CMakeLists.txt c87228ff3bd5a4a305f22d59b1e20dfbf699618753c0f681b20a57a5d225fbfb)
restore_reviewed_exports(pharaoh_menu_route_build_graph_verify artifacts/map_commit_construction_refresh_phase/CMakeLists.txt 7e8a7e91bbe588872aa42079526cf6044e4e64432020d7b5798eb9d8ef4889f2)

add_executable(pharaoh_building_quality_numeric_probe EXCLUDE_FROM_ALL
 "${ROOT}/artifacts/building_quality_update_phase/numeric_probe.c"
 "${ROOT}/runtime/building_quality_update.c" "${ROOT}/runtime/building_nearest_search.c" "${ROOT}/runtime/menu_full_input_numeric.c")
set_target_properties(pharaoh_building_quality_numeric_probe PROPERTIES OUTPUT_NAME building_quality_numeric_probe)
target_include_directories(pharaoh_building_quality_numeric_probe PRIVATE "${ROOT}/include")
target_compile_definitions(pharaoh_building_quality_numeric_probe PRIVATE _CRT_SECURE_NO_WARNINGS)
target_compile_options(pharaoh_building_quality_numeric_probe PRIVATE /W4 /WX /GS- /Ob0 $<$<CONFIG:Debug>:/Od> $<$<CONFIG:Release>:/O2>)

# Only the unchanged genuine RET provider is compiled locally. All reconstructed
# bodies below must resolve from the actual production archive.
add_library(pharaoh_drag_promoted_main_verify SHARED EXCLUDE_FROM_ALL "${ROOT}/src/unit_02351.c")
restore_native_dll(pharaoh_drag_promoted_main_verify)
foreach(_checked_graph IN ITEMS pharaoh_drag_actor_graph_verify pharaoh_drag_surface_passes_verify pharaoh_drag_map_gates_verify
 pharaoh_drag_canal_region_verify pharaoh_drag_redraw_dispatch_verify pharaoh_drag_preview_geometry_verify pharaoh_drag_route_query_verify pharaoh_menu_commit_graph_verify pharaoh_menu_route_build_graph_verify)
 get_target_property(_checked_exports ${_checked_graph} LINK_OPTIONS)
 list(FILTER _checked_exports INCLUDE REGEX "^/EXPORT:")
 target_link_options(pharaoh_drag_promoted_main_verify PRIVATE ${_checked_exports})
endforeach()

foreach(_map_graph IN ITEMS pharaoh_map_commit_resource_warnings_verify pharaoh_map_commit_resource_gate_verify pharaoh_map_commit_town_warning_verify pharaoh_map_commit_links_verify pharaoh_map_commit_paths_verify pharaoh_map_commit_connect_verify
 pharaoh_drag_actor_graph_verify pharaoh_drag_promoted_main_verify pharaoh_menu_commit_graph_verify pharaoh_menu_route_build_graph_verify
 pharaoh_drag_surface_passes_verify pharaoh_drag_map_gates_verify pharaoh_drag_canal_region_verify pharaoh_drag_redraw_dispatch_verify pharaoh_drag_preview_geometry_verify pharaoh_drag_route_query_verify)
 target_link_libraries(${_map_graph} PRIVATE
  "$<IF:$<CONFIG:Debug>,${ROOT}/build/c-only-debug/Debug/pharaoh_restored.lib,${ROOT}/build/c-only/Release/pharaoh_restored.lib>"
  "$<IF:$<CONFIG:Debug>,${ROOT}/build/c-only-debug/Debug/restore_data.lib,${ROOT}/build/c-only/Release/restore_data.lib>"
  kernel32 user32 gdi32 shell32 ole32 oleaut32 advapi32 winmm ddraw dxguid legacy_stdio_definitions)
endforeach()

add_executable(pharaoh_sprite_warning_probe EXCLUDE_FROM_ALL "${ROOT}/artifacts/sprite_warning_phase/probe.c" "${ROOT}/src/unit_02520.c"
 "${ROOT}/src/unit_02528.c" "${ROOT}/src/unit_02529.c" "${ROOT}/src/unit_01733.c" "${ROOT}/src/unit_01734.c")
target_include_directories(pharaoh_sprite_warning_probe PRIVATE "${ROOT}/include")
target_compile_definitions(pharaoh_sprite_warning_probe PRIVATE _CRT_SECURE_NO_WARNINGS READER_SOURCE="${ROOT}/src/unit_02522.c")
target_compile_options(pharaoh_sprite_warning_probe PRIVATE /W4 /WX $<$<CONFIG:Debug>:/Od> $<$<CONFIG:Release>:/O2>)
target_link_libraries(pharaoh_sprite_warning_probe PRIVATE
 "$<IF:$<CONFIG:Debug>,${ROOT}/build/c-only-debug/Debug/restore_data.lib,${ROOT}/build/c-only/Release/restore_data.lib>" advapi32 user32)
target_link_options(pharaoh_sprite_warning_probe PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED /STACK:4194304)

foreach(_canonical_graph IN ITEMS pharaoh_drag_refresh_wrappers_verify pharaoh_map_commit_warnings_verify)
 target_link_libraries(${_canonical_graph} PRIVATE
  "$<IF:$<CONFIG:Debug>,${ROOT}/build/c-only-debug/Debug/pharaoh_restored.lib,${ROOT}/build/c-only/Release/pharaoh_restored.lib>"
  "$<IF:$<CONFIG:Debug>,${ROOT}/build/c-only-debug/Debug/restore_data.lib,${ROOT}/build/c-only/Release/restore_data.lib>"
  kernel32 user32 gdi32 shell32 ole32 oleaut32 advapi32 winmm ddraw dxguid legacy_stdio_definitions)
endforeach()

add_library(pharaoh_drag_clear_helpers_verify SHARED EXCLUDE_FROM_ALL "${ROOT}/runtime/menu_drag_clear_helpers.c" "${ROOT}/src/unit_01011.c")
restore_native_dll(pharaoh_drag_clear_helpers_verify /EXPORT:FUN_0047c7b0 /EXPORT:FUN_00541ab0 /EXPORT:FUN_0047a120 /EXPORT:FUN_00477890 /EXPORT:FUN_0046f930)

add_library(pharaoh_drag_clear_footprint_verify SHARED EXCLUDE_FROM_ALL "${ROOT}/runtime/menu_drag_clear_footprint.c" "${ROOT}/runtime/menu_drag_clear_helpers.c" "${ROOT}/runtime/menu_drag_terrain_graph.c")
restore_native_dll(pharaoh_drag_clear_footprint_verify /EXPORT:FUN_0047c480 /EXPORT:FUN_0047c7b0 /EXPORT:FUN_00488030 /EXPORT:FUN_00487f80 /EXPORT:FUN_00541ab0 /EXPORT:FUN_0047a220 /EXPORT:FUN_0046f930 /EXPORT:FUN_004767d0 /EXPORT:FUN_004746d0 /EXPORT:FUN_00477500 /EXPORT:FUN_004777c0 /EXPORT:FUN_00475cc0 /EXPORT:FUN_004776f0 /EXPORT:FUN_00477890 /EXPORT:FUN_0047a120 /EXPORT:FUN_004f6130 /EXPORT:FUN_00479e70)
target_link_libraries(pharaoh_drag_clear_footprint_verify PRIVATE
 "$<IF:$<CONFIG:Debug>,${ROOT}/build/c-only-debug/Debug/pharaoh_restored.lib,${ROOT}/build/c-only/Release/pharaoh_restored.lib>"
 "$<IF:$<CONFIG:Debug>,${ROOT}/build/c-only-debug/Debug/restore_data.lib,${ROOT}/build/c-only/Release/restore_data.lib>"
 kernel32 user32 gdi32 shell32 ole32 oleaut32 advapi32 winmm ddraw dxguid legacy_stdio_definitions)
