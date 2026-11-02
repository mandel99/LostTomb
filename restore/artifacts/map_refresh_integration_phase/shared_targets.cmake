# Include once at root after pharaoh_restored and restore_data exist, after exact
# integration-manifest.json source application. Does not create alternate units.
include_guard(GLOBAL)
if(NOT TARGET pharaoh_restored OR NOT TARGET restore_data)
 message(FATAL_ERROR "Map integration recipe requires pharaoh_restored and restore_data")
endif()
if(NOT CMAKE_MSVC_RUNTIME_LIBRARY STREQUAL "MultiThreaded$<$<CONFIG:Debug>:Debug>")
 message(FATAL_ERROR "Map integration expects root DebugMTd/ReleaseMT")
endif()
target_sources(pharaoh_restored PRIVATE
 "${CMAKE_SOURCE_DIR}/runtime/map_refresh_fp.c"
 "${CMAKE_SOURCE_DIR}/runtime/map_x87_conversion.c")
get_filename_component(_mi_artifacts "${CMAKE_CURRENT_LIST_DIR}/.." ABSOLUTE)
include("${_mi_artifacts}/map_neighbors_phase/shared_targets.cmake")
include("${_mi_artifacts}/map_refresh_phase/shared_targets.cmake")
include("${_mi_artifacts}/map_refresh_arithmetic_phase/shared_targets.cmake")
foreach(_mi_pass RANGE 1 8)
 include("${_mi_artifacts}/map_refresh_postpass${_mi_pass}_phase/shared_targets.cmake")
endforeach()

# Frozen numeric target already exists in the arithmetic recipe; add map evidence.
target_link_options(restore_map_refresh_numeric_verify PRIVATE /MAP)
add_executable(restore_map_refresh_x87_conversion_verify
 "${_mi_artifacts}/map_refresh_phase/x87_conversion/x87_conversion_fixture.c"
 "${CMAKE_SOURCE_DIR}/runtime/map_x87_conversion.c")
target_include_directories(restore_map_refresh_x87_conversion_verify PRIVATE "${CMAKE_SOURCE_DIR}/include")
target_compile_definitions(restore_map_refresh_x87_conversion_verify PRIVATE _CRT_SECURE_NO_WARNINGS)
target_compile_options(restore_map_refresh_x87_conversion_verify PRIVATE /W4 "$<$<CONFIG:Debug>:/Od>" "$<$<CONFIG:Release>:/O2>")
target_link_options(restore_map_refresh_x87_conversion_verify PRIVATE /MAP)
add_test(NAME restore_map_refresh_x87_conversion_verify COMMAND restore_map_refresh_x87_conversion_verify)

add_executable(restore_map_refresh_postpass8_leaf_verify
 "${_mi_artifacts}/map_refresh_postpass8_phase/leaf_preflight/fixture.c"
 "${CMAKE_SOURCE_DIR}/src/unit_01235.c" "${CMAKE_SOURCE_DIR}/src/unit_01238.c" "${CMAKE_SOURCE_DIR}/src/unit_01098.c")
target_include_directories(restore_map_refresh_postpass8_leaf_verify PRIVATE "${CMAKE_SOURCE_DIR}/include")
target_compile_definitions(restore_map_refresh_postpass8_leaf_verify PRIVATE _CRT_SECURE_NO_WARNINGS)
target_compile_options(restore_map_refresh_postpass8_leaf_verify PRIVATE /W4 "$<$<CONFIG:Debug>:/Od>" "$<$<CONFIG:Release>:/O2>")
target_link_libraries(restore_map_refresh_postpass8_leaf_verify PRIVATE restore_data advapi32)
target_link_options(restore_map_refresh_postpass8_leaf_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED /MAP)
add_test(NAME restore_map_refresh_postpass8_leaf_verify COMMAND restore_map_refresh_postpass8_leaf_verify)
unset(_mi_artifacts)
unset(_mi_pass)
