# Optional coordinator-owned include after restore_data is declared and the
# frozen three product mappings have been applied. No product application here.
get_filename_component(_scalar_batch_root "${CMAKE_CURRENT_LIST_DIR}/../.." ABSOLUTE)
foreach(_scalar_mapping IN ITEMS
  "unit_00123.c=290eff944d7f12f5e196bf567853f685e4be6304e95c5c6574af4725ae5f2753"
  "unit_00124.c=0bdddeb04aee5e1c78db2e92c6dc265d9cb7ce8f120d961eef786ffd8fadee9c"
  "unit_00126.c=281a902f8c9ccf785db28d2f5450fe025c0b5c3baed228b47a7974d51fef760b")
  string(REPLACE "=" ";" _scalar_parts "${_scalar_mapping}")
  list(GET _scalar_parts 0 _scalar_source)
  list(GET _scalar_parts 1 _scalar_expected)
  file(SHA256 "${_scalar_batch_root}/src/${_scalar_source}" _scalar_actual)
  if(NOT _scalar_actual STREQUAL _scalar_expected)
    message(FATAL_ERROR "Scalar batch frozen product hash mismatch: ${_scalar_source}")
  endif()
endforeach()
add_executable(restore_scalar_binding_batch_001_verify
  "${CMAKE_CURRENT_LIST_DIR}/guarded_pixel_fixture.c"
  "${_scalar_batch_root}/src/unit_00123.c"
  "${_scalar_batch_root}/src/unit_00124.c"
  "${_scalar_batch_root}/src/unit_00126.c")
target_include_directories(restore_scalar_binding_batch_001_verify PRIVATE "${_scalar_batch_root}/include")
target_compile_definitions(restore_scalar_binding_batch_001_verify PRIVATE _CRT_SECURE_NO_WARNINGS)
target_compile_options(restore_scalar_binding_batch_001_verify PRIVATE /W4 "$<$<CONFIG:Debug>:/Od>" "$<$<CONFIG:Release>:/O2>")
target_link_libraries(restore_scalar_binding_batch_001_verify PRIVATE restore_data advapi32)
target_link_options(restore_scalar_binding_batch_001_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED /MAP)
add_test(NAME standalone_scalar_binding_batch_001 COMMAND restore_scalar_binding_batch_001_verify)
set_tests_properties(standalone_scalar_binding_batch_001 PROPERTIES LABELS source-only)
unset(_scalar_batch_root)
unset(_scalar_mapping)
unset(_scalar_parts)
unset(_scalar_source)
unset(_scalar_expected)
unset(_scalar_actual)
