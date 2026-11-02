# Prerequisites: exact P2 1233/header and map_neighbors 1139/header applied.
# Their frozen stages remain independent prerequisites, not presumed root changes.
foreach(kind IN ITEMS trace plain)
 if(kind STREQUAL "trace")
  set(post8_target restore_map_refresh_postpass8_verify)
 else()
  set(post8_target restore_map_refresh_postpass8_plain_verify)
 endif()
 add_executable(${post8_target} "${CMAKE_CURRENT_LIST_DIR}/postpass8_fixture.c" "${CMAKE_SOURCE_DIR}/src/unit_01099.c"
  "${CMAKE_SOURCE_DIR}/src/unit_01233.c" "${CMAKE_SOURCE_DIR}/src/unit_01139.c"
  "${CMAKE_SOURCE_DIR}/src/unit_01009.c" "${CMAKE_SOURCE_DIR}/src/unit_01011.c" "${CMAKE_SOURCE_DIR}/src/unit_01097.c" "${CMAKE_SOURCE_DIR}/src/unit_01098.c"
  "${CMAKE_SOURCE_DIR}/src/unit_01137.c" "${CMAKE_SOURCE_DIR}/src/unit_01235.c" "${CMAKE_SOURCE_DIR}/src/unit_01236.c" "${CMAKE_SOURCE_DIR}/src/unit_01238.c")
 target_include_directories(${post8_target} PRIVATE "${CMAKE_SOURCE_DIR}/include")
 target_compile_definitions(${post8_target} PRIVATE _CRT_SECURE_NO_WARNINGS)
 if(kind STREQUAL "trace")
  target_compile_definitions(${post8_target} PRIVATE MAP_REFRESH_POSTPASS8_TRACE MAP_REFRESH_POSTPASS2_TRACE MAP_NEIGHBORS_TRACE)
 endif()
 target_compile_options(${post8_target} PRIVATE /W4 "$<$<CONFIG:Debug>:/Od>" "$<$<CONFIG:Release>:/O2>")
 target_link_libraries(${post8_target} PRIVATE restore_data advapi32)
 target_link_options(${post8_target} PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED /MAP)
 add_test(NAME ${post8_target} COMMAND ${post8_target})
endforeach()
