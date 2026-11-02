# Coordinator integration: include this file after restore_data is declared.
# The five product units remain canonical members of RESTORE_UNITS. The test
# executables compile them separately and never link pharaoh_restored (which
# would introduce duplicate native entries). Tracing is target-local only.
get_filename_component(_restore_map_cells_root "${CMAKE_CURRENT_LIST_DIR}/.." ABSOLUTE)
foreach(_map_cells_target IN ITEMS restore_map_cells_verify restore_map_cells_plain_verify)
  add_executable(${_map_cells_target}
    "${_restore_map_cells_root}/tests/map_cells_phase.c"
    "${_restore_map_cells_root}/src/unit_01256.c"
    "${_restore_map_cells_root}/src/unit_01260.c"
    "${_restore_map_cells_root}/src/unit_01261.c"
    "${_restore_map_cells_root}/src/unit_01262.c"
    "${_restore_map_cells_root}/src/unit_01263.c")
  target_include_directories(${_map_cells_target} PRIVATE "${_restore_map_cells_root}/include")
  target_compile_definitions(${_map_cells_target} PRIVATE _CRT_SECURE_NO_WARNINGS)
  target_compile_options(${_map_cells_target} PRIVATE /W4 "$<$<CONFIG:Debug>:/Od>" "$<$<CONFIG:Release>:/O2>")
  target_link_libraries(${_map_cells_target} PRIVATE restore_data advapi32)
  target_link_options(${_map_cells_target} PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
endforeach()
target_compile_definitions(restore_map_cells_verify PRIVATE MAP_CELLS_TRACE)
add_test(NAME standalone_map_cells COMMAND restore_map_cells_verify)
add_test(NAME standalone_map_cells_plain COMMAND restore_map_cells_plain_verify)
set_tests_properties(standalone_map_cells standalone_map_cells_plain PROPERTIES LABELS source-only)
unset(_restore_map_cells_root)
unset(_map_cells_target)
