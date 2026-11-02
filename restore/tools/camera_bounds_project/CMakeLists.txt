cmake_minimum_required(VERSION 3.20)
project(camera_bounds_verification C)
get_filename_component(RESTORE "${CMAKE_CURRENT_LIST_DIR}/../.." ABSOLUTE)
add_executable(camera_bounds_verify "${RESTORE}/tests/camera_bounds.c"
  "${RESTORE}/src/unit_00464.c" "${RESTORE}/src/unit_00465.c"
  "${RESTORE}/src/unit_01011.c" "${RESTORE}/src/unit_01151.c"
  "${RESTORE}/src/unit_01880.c" "${RESTORE}/src/unit_01881.c"
  "${RESTORE}/src/unit_01885.c" "${RESTORE}/src/unit_01886.c")
target_include_directories(camera_bounds_verify PRIVATE "${RESTORE}/include")
target_compile_options(camera_bounds_verify PRIVATE /O2 /W4)
target_compile_definitions(camera_bounds_verify PRIVATE _CRT_SECURE_NO_WARNINGS)
set_property(TARGET camera_bounds_verify PROPERTY MSVC_RUNTIME_LIBRARY MultiThreadedDebug)
target_link_libraries(camera_bounds_verify PRIVATE "${RESTORE}/build/c-only-debug/Debug/restore_data.lib" advapi32)
target_link_options(camera_bounds_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
enable_testing()
add_test(NAME camera_bounds_smoke COMMAND camera_bounds_verify)
