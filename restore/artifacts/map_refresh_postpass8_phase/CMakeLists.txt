cmake_minimum_required(VERSION 3.20)
project(map_refresh_postpass8 C)
get_filename_component(RESTORE "${CMAKE_CURRENT_LIST_DIR}/../.." ABSOLUTE)
foreach(kind IN ITEMS trace plain)
 add_executable(map_post8_${kind} postpass8_fixture.c unit_01099.c
  "${RESTORE}/artifacts/map_refresh_postpass2_phase/unit_01233.c" "${RESTORE}/artifacts/map_neighbors_phase/unit_01139.c"
  "${RESTORE}/src/unit_01009.c" "${RESTORE}/src/unit_01011.c" "${RESTORE}/src/unit_01097.c" "${RESTORE}/src/unit_01098.c"
  "${RESTORE}/src/unit_01137.c" "${RESTORE}/src/unit_01235.c" "${RESTORE}/src/unit_01236.c" "${RESTORE}/src/unit_01238.c")
 target_include_directories(map_post8_${kind} PRIVATE "${RESTORE}/include" "${RESTORE}/artifacts/map_refresh_postpass2_phase" "${RESTORE}/artifacts/map_neighbors_phase")
 target_compile_definitions(map_post8_${kind} PRIVATE _CRT_SECURE_NO_WARNINGS)
 target_compile_options(map_post8_${kind} PRIVATE /W4 "$<$<CONFIG:Debug>:/Od>" "$<$<CONFIG:Release>:/O2>")
 set_property(TARGET map_post8_${kind} PROPERTY MSVC_RUNTIME_LIBRARY MultiThreadedDebug)
 target_link_libraries(map_post8_${kind} PRIVATE "${RESTORE}/build/c-only-debug/Debug/restore_data.lib" advapi32)
 target_link_options(map_post8_${kind} PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED /MAP)
endforeach()
target_compile_definitions(map_post8_trace PRIVATE MAP_REFRESH_POSTPASS8_TRACE MAP_REFRESH_POSTPASS2_TRACE MAP_NEIGHBORS_TRACE)
