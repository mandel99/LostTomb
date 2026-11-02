cmake_minimum_required(VERSION 3.20)
project(map_refresh_postpass5 C)
get_filename_component(RESTORE "${CMAKE_CURRENT_LIST_DIR}/../.." ABSOLUTE)
foreach(kind IN ITEMS trace plain)
 add_executable(map_post5_${kind} postpass5_fixture.c unit_01107.c unit_01108.c unit_01110.c unit_02327.c
  "${RESTORE}/src/unit_01137.c" "${RESTORE}/src/unit_01237.c" "${RESTORE}/src/unit_02326.c"
  "${RESTORE}/artifacts/map_refresh_postpass2_phase/unit_01233.c"
  "${RESTORE}/artifacts/map_refresh_postpass4_phase/unit_01149.c"
  "${RESTORE}/artifacts/map_refresh_postpass4_phase/unit_01152.c"
  "${RESTORE}/artifacts/map_refresh_postpass4_phase/unit_01153.c")
 target_include_directories(map_post5_${kind} PRIVATE "${RESTORE}/include" "${RESTORE}/artifacts/map_refresh_postpass2_phase" "${RESTORE}/artifacts/map_refresh_postpass4_phase")
 target_compile_definitions(map_post5_${kind} PRIVATE _CRT_SECURE_NO_WARNINGS)
 target_compile_options(map_post5_${kind} PRIVATE /W4 "$<$<CONFIG:Debug>:/Od>" "$<$<CONFIG:Release>:/O2>")
 set_property(TARGET map_post5_${kind} PROPERTY MSVC_RUNTIME_LIBRARY MultiThreadedDebug)
 target_link_libraries(map_post5_${kind} PRIVATE "${RESTORE}/build/c-only-debug/Debug/restore_data.lib" advapi32)
 target_link_options(map_post5_${kind} PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED /MAP)
endforeach()
target_compile_definitions(map_post5_trace PRIVATE MAP_REFRESH_POSTPASS5_TRACE MAP_REFRESH_POSTPASS2_TRACE MAP_REFRESH_POSTPASS4_TRACE)
enable_testing()
add_test(NAME map_post5_trace_launch COMMAND map_post5_trace)
add_test(NAME map_post5_plain_launch COMMAND map_post5_plain)
