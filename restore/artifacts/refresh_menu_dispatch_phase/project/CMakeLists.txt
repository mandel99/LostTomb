cmake_minimum_required(VERSION 3.25)
project(RefreshMenuDispatch LANGUAGES C)
get_filename_component(ROOT "../../.." ABSOLUTE)
get_filename_component(PHASE ".." ABSOLUTE)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
add_library(refresh_dispatch_candidate STATIC "${PHASE}/src/unit_01867.c" "${PHASE}/runtime/refresh_menu_dispatch_boundary.c")
target_include_directories(refresh_dispatch_candidate PRIVATE "${PHASE}/include" "${ROOT}/include")
target_compile_options(refresh_dispatch_candidate PRIVATE /W4 /WX $<$<CONFIG:Debug>:/Od> $<$<CONFIG:Release>:/O2>)
add_executable(restore_refresh_menu_dispatch_verify
 "${PHASE}/tests/refresh_dispatch.c" "${PHASE}/tests/actual_mission_buttons.c" "${PHASE}/src/unit_01867.c"
 "${ROOT}/src/unit_00286.c" "${ROOT}/src/unit_00293.c" "${ROOT}/src/unit_00295.c" "${ROOT}/src/unit_00296.c" "${ROOT}/src/unit_00297.c" "${ROOT}/src/unit_02276.c")
target_include_directories(restore_refresh_menu_dispatch_verify PRIVATE "${PHASE}/include" "${ROOT}/include" "${PHASE}/tests")
target_compile_definitions(restore_refresh_menu_dispatch_verify PRIVATE _CRT_SECURE_NO_WARNINGS)
target_compile_options(restore_refresh_menu_dispatch_verify PRIVATE /W4 /WX $<$<CONFIG:Debug>:/Od> $<$<CONFIG:Release>:/O2>)
# Unchanged295 returns a constant data address as byte* from DWORD*. Native is
# MOV EAX,57CD70; RET. Keep the real body and suppress only its known C4133.
set_source_files_properties("${ROOT}/src/unit_00295.c" PROPERTIES COMPILE_OPTIONS /wd4133)
target_link_libraries(restore_refresh_menu_dispatch_verify PRIVATE "$<IF:$<CONFIG:Debug>,${ROOT}/build/c-only-debug/Debug/restore_data.lib,${ROOT}/build/c-only/Release/restore_data.lib>" advapi32)
target_link_options(restore_refresh_menu_dispatch_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
enable_testing()
add_test(NAME refresh_dispatch_smoke COMMAND restore_refresh_menu_dispatch_verify)
