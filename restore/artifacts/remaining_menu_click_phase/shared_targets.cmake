# Include from the root project only after its normal pharaoh_restored target exists.
# This is a test-only composition; the eight endpoint aliases live solely in the fixture.
foreach(kind IN ITEMS trace plain)
 if(kind STREQUAL "trace")
  set(click_target restore_remaining_menu_click_verify)
 else()
  set(click_target restore_remaining_menu_click_plain_verify)
 endif()
 add_executable(${click_target}
  "${CMAKE_CURRENT_LIST_DIR}/remaining_menu_click_fixture.c"
  "${CMAKE_SOURCE_DIR}/src/unit_00049.c" "${CMAKE_SOURCE_DIR}/src/unit_00077.c"
  "${CMAKE_SOURCE_DIR}/runtime/remaining_menu_actions.c" "${CMAKE_SOURCE_DIR}/src/unit_01628.c")
 target_include_directories(${click_target} PRIVATE "${CMAKE_SOURCE_DIR}/include")
 target_compile_definitions(${click_target} PRIVATE _CRT_SECURE_NO_WARNINGS)
 if(kind STREQUAL "trace")
  target_compile_definitions(${click_target} PRIVATE REMAINING_MENU_TRACE)
 endif()
 target_compile_options(${click_target} PRIVATE /W4 "$<$<CONFIG:Debug>:/Od>" "$<$<CONFIG:Release>:/O2>")
 # Match the root restoration target's runtime setting. Its current Debug/Release
 # configurations both use the debug static runtime.
 set_property(TARGET ${click_target} PROPERTY MSVC_RUNTIME_LIBRARY MultiThreadedDebug)
 target_link_libraries(${click_target} PRIVATE pharaoh_restored restore_data
  kernel32 user32 gdi32 shell32 ole32 oleaut32 advapi32 winmm ddraw dxguid legacy_stdio_definitions)
 target_link_options(${click_target} PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED /MAP)
 add_test(NAME ${click_target} COMMAND ${click_target})
endforeach()
