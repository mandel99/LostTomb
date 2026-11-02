# Coordinator includes this frozen recipe after restore_data and applying the
# exact five unit/header mappings. Existing RESTORE_UNITS remain canonical.
get_filename_component(_map_neighbors_root "${CMAKE_CURRENT_LIST_DIR}/../.." ABSOLUTE)
foreach(_map_neighbors_target IN ITEMS restore_map_neighbors_verify restore_map_neighbors_plain_verify)
  add_executable(${_map_neighbors_target}
    "${CMAKE_CURRENT_LIST_DIR}/map_neighbors_fixture.c"
    "${_map_neighbors_root}/src/unit_01137.c"
    "${_map_neighbors_root}/src/unit_01138.c"
    "${_map_neighbors_root}/src/unit_01139.c"
    "${_map_neighbors_root}/src/unit_01140.c"
    "${_map_neighbors_root}/src/unit_01141.c"
    "${_map_neighbors_root}/src/unit_01142.c")
  target_include_directories(${_map_neighbors_target} PRIVATE "${_map_neighbors_root}/include")
  target_compile_definitions(${_map_neighbors_target} PRIVATE _CRT_SECURE_NO_WARNINGS)
  target_compile_options(${_map_neighbors_target} PRIVATE /W4 "$<$<CONFIG:Debug>:/Od>" "$<$<CONFIG:Release>:/O2>")
  target_link_libraries(${_map_neighbors_target} PRIVATE restore_data advapi32)
  target_link_options(${_map_neighbors_target} PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
endforeach()
target_compile_definitions(restore_map_neighbors_verify PRIVATE MAP_NEIGHBORS_TRACE)
add_test(NAME standalone_map_neighbors COMMAND restore_map_neighbors_verify)
add_test(NAME standalone_map_neighbors_plain COMMAND restore_map_neighbors_plain_verify)
set_tests_properties(standalone_map_neighbors standalone_map_neighbors_plain PROPERTIES LABELS source-only)
unset(_map_neighbors_root)
unset(_map_neighbors_target)
