# Root-only integration after exact staged mappings are approved/applied.
get_filename_component(_remaining_root "${CMAKE_CURRENT_LIST_DIR}/../.." ABSOLUTE)
foreach(_remaining_target IN ITEMS restore_remaining_menu_actions_verify restore_remaining_menu_actions_plain_verify)
 add_executable(${_remaining_target} "${CMAKE_CURRENT_LIST_DIR}/remaining_menu_fixture.c"
  "${_remaining_root}/src/unit_00049.c" "${_remaining_root}/src/unit_00077.c"
  "${_remaining_root}/runtime/remaining_menu_actions.c" "${_remaining_root}/src/unit_01628.c")
 target_include_directories(${_remaining_target} PRIVATE "${_remaining_root}/include")
 target_compile_definitions(${_remaining_target} PRIVATE _CRT_SECURE_NO_WARNINGS)
 target_compile_options(${_remaining_target} PRIVATE /W4 "$<$<CONFIG:Debug>:/Od>" "$<$<CONFIG:Release>:/O2>")
 target_link_libraries(${_remaining_target} PRIVATE restore_data advapi32)
 target_link_options(${_remaining_target} PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
endforeach()
target_compile_definitions(restore_remaining_menu_actions_verify PRIVATE REMAINING_MENU_TRACE)
add_test(NAME standalone_remaining_menu_actions COMMAND restore_remaining_menu_actions_verify)
add_test(NAME standalone_remaining_menu_actions_plain COMMAND restore_remaining_menu_actions_plain_verify)
set_tests_properties(standalone_remaining_menu_actions standalone_remaining_menu_actions_plain PROPERTIES LABELS source-only)
# Add runtime/remaining_menu_actions.c to pharaoh_restored and route its two
# tokens through restore_remaining_menu_action in menu_callbacks.c. Those are
# coordinator-owned changes; this recipe deliberately does not apply them.
unset(_remaining_root)
unset(_remaining_target)
