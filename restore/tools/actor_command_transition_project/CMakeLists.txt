cmake_minimum_required(VERSION 3.20)
project(actor_command_transition_strict C)
get_filename_component(RESTORE_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../.." ABSOLUTE)
add_library(actor_command_transition_strict OBJECT
 "${RESTORE_ROOT}/runtime/actor_command.c"
 "${RESTORE_ROOT}/src/unit_02078.c"
 "${RESTORE_ROOT}/src/unit_02081.c"
 "${RESTORE_ROOT}/src/unit_02082.c")
target_include_directories(actor_command_transition_strict PRIVATE "${RESTORE_ROOT}/include")
target_compile_options(actor_command_transition_strict PRIVATE /W4 /WX /GS- /Ob0 "$<$<CONFIG:Release>:/O2>")
