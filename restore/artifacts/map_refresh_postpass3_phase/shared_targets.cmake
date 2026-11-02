foreach(kind IN ITEMS trace plain)
 if(kind STREQUAL "trace")
  set(post3_target restore_map_refresh_postpass3_verify)
 else()
  set(post3_target restore_map_refresh_postpass3_plain_verify)
 endif()
 add_executable(${post3_target} "${CMAKE_CURRENT_LIST_DIR}/postpass3_fixture.c"
  "${CMAKE_SOURCE_DIR}/src/unit_01052.c" "${CMAKE_SOURCE_DIR}/src/unit_01095.c"
  "${CMAKE_SOURCE_DIR}/src/unit_01009.c" "${CMAKE_SOURCE_DIR}/src/unit_01011.c" "${CMAKE_SOURCE_DIR}/src/unit_01097.c" "${CMAKE_SOURCE_DIR}/src/unit_01137.c" "${CMAKE_SOURCE_DIR}/src/unit_01236.c"
  "${CMAKE_SOURCE_DIR}/src/unit_01140.c" "${CMAKE_SOURCE_DIR}/src/unit_01233.c" "${CMAKE_SOURCE_DIR}/src/unit_01234.c"
  "${CMAKE_SOURCE_DIR}/runtime/map_refresh_fp.c" "${CMAKE_SOURCE_DIR}/runtime/map_x87_conversion.c")
 target_include_directories(${post3_target} PRIVATE "${CMAKE_SOURCE_DIR}/include")
 target_compile_definitions(${post3_target} PRIVATE _CRT_SECURE_NO_WARNINGS)
 if(kind STREQUAL "trace")
  target_compile_definitions(${post3_target} PRIVATE MAP_REFRESH_POSTPASS3_TRACE MAP_REFRESH_POSTPASS2_TRACE MAP_NEIGHBORS_TRACE)
 endif()
 target_compile_options(${post3_target} PRIVATE /W4 "$<$<CONFIG:Debug>:/Od>" "$<$<CONFIG:Release>:/O2>")
 target_link_libraries(${post3_target} PRIVATE restore_data advapi32)
 target_link_options(${post3_target} PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED /MAP)
 add_test(NAME ${post3_target} COMMAND ${post3_target})
endforeach()
