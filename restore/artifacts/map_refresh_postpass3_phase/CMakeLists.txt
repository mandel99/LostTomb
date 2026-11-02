cmake_minimum_required(VERSION 3.20)
project(map_refresh_postpass3 C)
get_filename_component(RESTORE "${CMAKE_CURRENT_LIST_DIR}/../.." ABSOLUTE)
foreach(kind IN ITEMS trace plain)
 add_executable(map_post3_${kind} postpass3_fixture.c unit_01052.c unit_01095.c
  "${RESTORE}/src/unit_01009.c" "${RESTORE}/src/unit_01011.c" "${RESTORE}/src/unit_01097.c" "${RESTORE}/src/unit_01137.c" "${RESTORE}/src/unit_01236.c"
  "${RESTORE}/artifacts/map_neighbors_phase/unit_01140.c" "${RESTORE}/artifacts/map_refresh_postpass2_phase/unit_01233.c" "${RESTORE}/artifacts/map_refresh_postpass2_phase/unit_01234.c"
  "${RESTORE}/artifacts/map_refresh_arithmetic_phase/map_refresh_fp.c"
  "${RESTORE}/artifacts/map_refresh_phase/x87_conversion/map_x87_conversion.c")
 target_include_directories(map_post3_${kind} PRIVATE "${RESTORE}/include" "${RESTORE}/artifacts/map_neighbors_phase" "${RESTORE}/artifacts/map_refresh_postpass2_phase" "${RESTORE}/artifacts/map_refresh_arithmetic_phase" "${RESTORE}/artifacts/map_refresh_phase/x87_conversion")
 target_compile_definitions(map_post3_${kind} PRIVATE _CRT_SECURE_NO_WARNINGS)
 target_compile_options(map_post3_${kind} PRIVATE /W4 "$<$<CONFIG:Debug>:/Od>" "$<$<CONFIG:Release>:/O2>")
 set_property(TARGET map_post3_${kind} PROPERTY MSVC_RUNTIME_LIBRARY MultiThreadedDebug)
 target_link_libraries(map_post3_${kind} PRIVATE "${RESTORE}/build/c-only-debug/Debug/restore_data.lib" advapi32)
 target_link_options(map_post3_${kind} PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED /MAP)
endforeach()
target_compile_definitions(map_post3_trace PRIVATE MAP_REFRESH_POSTPASS3_TRACE MAP_REFRESH_POSTPASS2_TRACE MAP_NEIGHBORS_TRACE)
enable_testing()
add_test(NAME map_post3_trace_launch COMMAND map_post3_trace)
add_test(NAME map_post3_plain_launch COMMAND map_post3_plain)
