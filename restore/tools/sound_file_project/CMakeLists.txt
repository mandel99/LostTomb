cmake_minimum_required(VERSION 3.25)
project(sound_file_verification C)
get_filename_component(RESTORE "${CMAKE_CURRENT_LIST_DIR}/../.." ABSOLUTE)
if(NOT MSVC OR NOT CMAKE_SIZEOF_VOID_P EQUAL 4)
 message(FATAL_ERROR "MSVC Win32 is required")
endif()
add_executable(sound_file_lookup_verify "${RESTORE}/tests/sound_file_lookup.c"
 "${RESTORE}/src/unit_02686.c" "${RESTORE}/src/unit_02725.c" "${RESTORE}/src/unit_02726.c"
 "${RESTORE}/src/unit_02848.c" "${RESTORE}/runtime/sound_file_lookup.c" "${RESTORE}/runtime/owned_buffer.c")
target_include_directories(sound_file_lookup_verify PRIVATE "${RESTORE}/include")
target_compile_options(sound_file_lookup_verify PRIVATE /W4)
target_compile_definitions(sound_file_lookup_verify PRIVATE _CRT_SECURE_NO_WARNINGS)
set_property(TARGET sound_file_lookup_verify PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
target_link_options(sound_file_lookup_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
