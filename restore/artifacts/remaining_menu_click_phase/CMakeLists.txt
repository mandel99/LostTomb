cmake_minimum_required(VERSION 3.20)
project(remaining_menu_click C)
get_filename_component(RESTORE "${CMAKE_CURRENT_LIST_DIR}/../.." ABSOLUTE)
foreach(kind IN ITEMS trace plain)
 add_executable(remaining_menu_click_${kind} remaining_menu_click_fixture.c
  "${RESTORE}/src/unit_00049.c" "${RESTORE}/src/unit_00077.c"
  "${RESTORE}/runtime/remaining_menu_actions.c" "${RESTORE}/src/unit_01628.c")
 target_include_directories(remaining_menu_click_${kind} PRIVATE "${RESTORE}/include")
 target_compile_definitions(remaining_menu_click_${kind} PRIVATE _CRT_SECURE_NO_WARNINGS)
 target_compile_options(remaining_menu_click_${kind} PRIVATE /W4 "$<$<CONFIG:Debug>:/Od>" "$<$<CONFIG:Release>:/O2>")
 set_property(TARGET remaining_menu_click_${kind} PROPERTY MSVC_RUNTIME_LIBRARY MultiThreadedDebug)
 target_link_libraries(remaining_menu_click_${kind} PRIVATE
  "${RESTORE}/build/c-only-debug/Debug/pharaoh_restored.lib" "${RESTORE}/build/c-only-debug/Debug/restore_data.lib"
  kernel32 user32 gdi32 shell32 ole32 oleaut32 advapi32 winmm ddraw dxguid legacy_stdio_definitions)
 target_link_options(remaining_menu_click_${kind} PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED /MAP)
endforeach()
target_compile_definitions(remaining_menu_click_trace PRIVATE REMAINING_MENU_TRACE)
enable_testing()
add_test(NAME remaining_menu_click_trace_launch COMMAND remaining_menu_click_trace)
add_test(NAME remaining_menu_click_plain_launch COMMAND remaining_menu_click_plain)
