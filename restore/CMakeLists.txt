cmake_minimum_required(VERSION 3.25)
project(PharaohRestore LANGUAGES C)
set_source_files_properties(runtime/message_category.c runtime/actor_message.c src/unit_02503.c src/unit_02506.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX;/O2;/GS-;/Ob0")
set_source_files_properties(runtime/labor_allocation.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX;/O2;/GS-;/Ob0")
set_source_files_properties(runtime/migration_flow.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX;/O2;/GS-;/Ob0")
set_source_files_properties(runtime/population_demography.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX;/O2;/GS-;/Ob0")
set_source_files_properties(runtime/housing_capacity.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX;/O2;/GS-;/Ob0")
set_source_files_properties(runtime/warehouse_stock.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX;/O2;/GS-;/Ob0")
set_source_files_properties(runtime/city_supply.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX;/O2;/GS-;/Ob0")
set_source_files_properties(runtime/industry_logistics.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX;/O2;/GS-;/Ob0")
set_source_files_properties(runtime/granary.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX;/O2;/GS-;/Ob0")
set_source_files_properties(runtime/storage_transactions.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX;/O2;/GS-;/Ob0")
set_source_files_properties(runtime/storage_trade.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX;/O2;/GS-;/Ob0")
set_source_files_properties(runtime/storage_delivery.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX;/O2;/GS-;/Ob0")
set_source_files_properties(runtime/menu_prepare_integer.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX;/O2;/GS-;/Ob0")
set_source_files_properties(runtime/menu_drag_actor_teardown.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX;/O2;/GS-;/Ob0")
set_source_files_properties(runtime/menu_drag_building_release.c runtime/menu_drag_monument_clear.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX;/O2;/GS-;/Ob0")
set_source_files_properties(runtime/construction_controller.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX;/O2;/GS-;/Ob0")
set_source_files_properties(runtime/menu_drag_preview_parent.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX;/O2;/GS-;/Ob0")
set_source_files_properties(runtime/menu_drag_construction_family.c runtime/menu_drag_world_offset.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX;/O2;/GS-;/Ob0")
set_source_files_properties(runtime/menu_drag_construction_parent.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX;/O2;/GS-;/Ob0")
set_source_files_properties(runtime/menu_drag_construction_scan.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX;/O2;/GS-;/Ob0")
set_source_files_properties(runtime/resource_availability.c src/unit_02629.c src/unit_02630.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX;/O2;/GS-;/Ob0")
set_source_files_properties(runtime/storage_settings.c runtime/gatehouse_display.c src/unit_00967.c src/unit_01574.c src/unit_01609.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX;/O2;/GS-;/Ob0")
set_source_files_properties(runtime/formation_control.c src/unit_01021.c src/unit_01471.c src/unit_01490.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX;/O2;/GS-;/Ob0")
set_source_files_properties(runtime/temple_complex.c src/unit_01583.c src/unit_01585.c src/unit_01586.c src/unit_01590.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX;/O2;/GS-;/Ob0")
set_source_files_properties(runtime/housing_cycle.c src/unit_00798.c src/unit_00804.c src/unit_00970.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX;/O2;/GS-;/Ob0")
set_source_files_properties(runtime/house_split.c src/unit_00978.c src/unit_00979.c src/unit_00980.c src/unit_00981.c src/unit_00982.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX;/O2;/GS-;/Ob0")
set_source_files_properties(runtime/house_layout.c src/unit_00969.c src/unit_00971.c src/unit_00972.c src/unit_00973.c src/unit_00974.c src/unit_00975.c src/unit_00976.c src/unit_00977.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX;/O2;/GS-;/Ob0")
set_source_files_properties(runtime/house_levels.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX;/O2;/GS-;/Ob0")
set_source_files_properties(runtime/house_evolution.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX;/O2;/GS-;/Ob0")
set_source_files_properties(runtime/monument_complex_images.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX;/O2;/GS-;/Ob0")
set_source_files_properties(runtime/monument_complex_positions.c src/unit_02569.c src/unit_02623.c src/unit_02610.c src/unit_02597.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX;/O2;/GS-;/Ob0")
set_source_files_properties(runtime/monument_complex_create.c src/unit_02572.c src/unit_02600.c src/unit_02613.c src/unit_02627.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX;/O2;/GS-;/Ob0")
set_source_files_properties(runtime/sun_temple.c src/unit_02546.c src/unit_02547.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX;/O2;/GS-;/Ob0")
set_source_files_properties(runtime/monument_images.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX;/O2;/GS-;/Ob0")
set_source_files_properties(runtime/monument_map.c src/unit_00009.c src/unit_00107.c src/unit_02001.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX;/O2;/GS-;/Ob0")
set_source_files_properties(runtime/monument_entrance.c src/unit_00010.c src/unit_00108.c src/unit_00202.c src/unit_02002.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX;/O2;/GS-;/Ob0")
set_source_files_properties(runtime/monument_create.c src/unit_00008.c src/unit_00106.c src/unit_00201.c src/unit_02000.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX;/O2;/GS-;/Ob0")
set_source_files_properties(runtime/monument_base_queries.c src/unit_00105.c src/unit_00109.c src/unit_00200.c src/unit_00203.c src/unit_01999.c src/unit_02003.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX;/O2;/GS-;/Ob0")
set_source_files_properties(runtime/event_schedule.c src/unit_00721.c src/unit_00731.c src/unit_00732.c src/unit_00737.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX;/O2;/GS-;/Ob0")
set_source_files_properties(runtime/event_parameters.c src/unit_00681.c src/unit_00685.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX;/O2;/GS-;/Ob0")
set_source_files_properties(runtime/event_packet.c runtime/event_message_kind.c runtime/flood_event.c src/unit_00641.c src/unit_00712.c src/unit_00727.c src/unit_00747.c src/unit_00767.c src/unit_00768.c src/unit_00779.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX;/O2;/GS-;/Ob0")
set_source_files_properties(runtime/flood_settings.c runtime/flood_quality.c runtime/flood_quality_effects.c src/unit_01542.c src/unit_01543.c src/unit_01544.c src/unit_01545.c src/unit_01546.c src/unit_01547.c src/unit_01548.c src/unit_01549.c src/unit_01567.c src/unit_01568.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX;/O2;/GS-;/Ob0")
set_source_files_properties(runtime/town_record_queries.c src/unit_00684.c src/unit_00708.c src/unit_00709.c src/unit_00710.c src/unit_00719.c src/unit_00744.c src/unit_00745.c src/unit_00746.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX;/O2;/GS-;/Ob0")
set_source_files_properties(runtime/sprite_frame_clock.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX;/O2;/GS-;/Ob0")
set_source_files_properties(src/unit_03100.c src/unit_03101.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX;/O2;/GS-;/Ob0")
set_source_files_properties(runtime/file_list_lifecycle.c src/unit_00018.c src/unit_00019.c src/unit_00266.c src/unit_00267.c src/unit_00268.c src/unit_00288.c src/unit_00289.c src/unit_00290.c src/unit_00291.c src/unit_00292.c src/unit_02274.c src/unit_02275.c src/unit_03091.c src/unit_03099.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX;/O2;/GS-;/Ob0")
set_source_files_properties(runtime/actor_ring_lifecycle.c src/unit_02183.c src/unit_02184.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX;/O2;/GS-;/Ob0")
set_source_files_properties(runtime/ring_queue.c src/unit_02185.c src/unit_02186.c src/unit_02187.c src/unit_02188.c src/unit_02189.c src/unit_02190.c src/unit_02191.c src/unit_02192.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX;/O2;/GS-;/Ob0")
set_source_files_properties(src/unit_02680.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX;/O2;/GS-;/Ob0")
set_source_files_properties(src/unit_02667.c src/unit_02672.c src/unit_02673.c src/unit_02674.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX;/O2;/GS-;/Ob0")
set_source_files_properties(src/unit_02660.c src/unit_02661.c src/unit_02662.c src/unit_02665.c src/unit_02666.c src/unit_02668.c src/unit_02669.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX;/O2;/GS-;/Ob0")
set_source_files_properties(src/unit_01297.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX")
set_source_files_properties(src/unit_01913.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX")
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
add_executable(restore_debug EXCLUDE_FROM_ALL tools/debug_child.c)
target_link_libraries(restore_debug PRIVATE dbghelp)
if(NOT MSVC OR NOT CMAKE_SIZEOF_VOID_P EQUAL 4)
  message(FATAL_ERROR "This generated x86 compilation model requires MSVC and a Win32 configuration.")
endif()
include(src/units.cmake)
# Complete native model loaders share explicit byte-addressed table fills.
foreach(_model_unit IN ITEMS 02541 02542 02543 02544)
 list(REMOVE_ITEM RESTORE_UNITS "${CMAKE_CURRENT_SOURCE_DIR}/src/unit_${_model_unit}.c")
endforeach()
unset(_model_unit)
# Canonical actor relocation/query entries reuse their complete reviewed models.
foreach(_actor_grid_unit IN ITEMS 00664 01560 01555)
 list(REMOVE_ITEM RESTORE_UNITS "${CMAKE_CURRENT_SOURCE_DIR}/src/unit_${_actor_grid_unit}.c")
endforeach()
unset(_actor_grid_unit)
# Canonical cell targeting, movement phases and combat state machines.
foreach(_actor_target_unit IN ITEMS 01367 01368 01369 01371 01372 01373 01374 01375 01376 01377 01378 01379 01380 01381 01382 01383 01384 01385 01388 01389 01390 01391 01392)
 list(REMOVE_ITEM RESTORE_UNITS "${CMAKE_CURRENT_SOURCE_DIR}/src/unit_${_actor_target_unit}.c")
endforeach()
unset(_actor_target_unit)
# The complete stock-overlay model replaces the legacy scaled-pointer body.
list(REMOVE_ITEM RESTORE_UNITS "${CMAKE_CURRENT_SOURCE_DIR}/src/unit_00546.c")
# Complete bazaar source selection and buyer retry/return transitions.
foreach(_bazaar_target_unit IN ITEMS 00823 01326 01327 01328 01329 01331 01332 01333 01334)
 list(REMOVE_ITEM RESTORE_UNITS "${CMAKE_CURRENT_SOURCE_DIR}/src/unit_${_bazaar_target_unit}.c")
endforeach()
unset(_bazaar_target_unit)
# Both complete routing occupancy predicates use the reviewed typed provider.
list(REMOVE_ITEM RESTORE_UNITS "${CMAKE_CURRENT_SOURCE_DIR}/src/unit_02317.c")
# Canonical disease selectors and initiation replace the legacy public bodies.
list(REMOVE_ITEM RESTORE_UNITS "${CMAKE_CURRENT_SOURCE_DIR}/src/unit_00448.c")
list(REMOVE_ITEM RESTORE_UNITS "${CMAKE_CURRENT_SOURCE_DIR}/src/unit_00449.c")
list(REMOVE_ITEM RESTORE_UNITS "${CMAKE_CURRENT_SOURCE_DIR}/src/unit_00450.c")
# Whole trader generation replaces the legacy mixed-width country/actor writes.
list(REMOVE_ITEM RESTORE_UNITS "${CMAKE_CURRENT_SOURCE_DIR}/src/unit_00649.c")
# Canonical debt and ledger updates use complete typed C providers.
foreach(_finance_unit IN ITEMS 00658 00853 00872 00873 00874 00875 00876 00995)
 list(REMOVE_ITEM RESTORE_UNITS "${CMAKE_CURRENT_SOURCE_DIR}/src/unit_${_finance_unit}.c")
endforeach()
unset(_finance_unit)
# Whole detail-overlay producer replaces the legacy truncated-field renderer.
list(REMOVE_ITEM RESTORE_UNITS "${CMAKE_CURRENT_SOURCE_DIR}/src/unit_00564.c")
foreach(_terrain_overlay_unit IN ITEMS 00471 00478 00479 00480 00514 00515)
 list(REMOVE_ITEM RESTORE_UNITS "${CMAKE_CURRENT_SOURCE_DIR}/src/unit_${_terrain_overlay_unit}.c")
endforeach()
unset(_terrain_overlay_unit)
# Canonical metric/column entries share their complete reviewed C bodies.
foreach(_overlay_metric_unit IN ITEMS 00475 00477 00516 00517 00518 00519 00520 00521 00522 00523 00524 00525 00526 00527 00528 00529 00530 00531 00532 00533 00534 00535 00536 00537 00538 00539 00540 00542 00543 00544 00545)
 list(REMOVE_ITEM RESTORE_UNITS "${CMAKE_CURRENT_SOURCE_DIR}/src/unit_${_overlay_metric_unit}.c")
endforeach()
unset(_overlay_metric_unit)
foreach(_special_building_unit IN ITEMS 00017 01579 02105 02558)
 list(REMOVE_ITEM RESTORE_UNITS "${CMAKE_CURRENT_SOURCE_DIR}/src/unit_${_special_building_unit}.c")
endforeach()
unset(_special_building_unit)
list(REMOVE_ITEM RESTORE_UNITS "${CMAKE_CURRENT_SOURCE_DIR}/src/unit_02379.c")
foreach(_military_render_unit IN ITEMS 02374 02375)
 list(REMOVE_ITEM RESTORE_UNITS "${CMAKE_CURRENT_SOURCE_DIR}/src/unit_${_military_render_unit}.c")
endforeach()
unset(_military_render_unit)
foreach(_fleet_render_unit IN ITEMS 02376 02377 02378)
 list(REMOVE_ITEM RESTORE_UNITS "${CMAKE_CURRENT_SOURCE_DIR}/src/unit_${_fleet_render_unit}.c")
endforeach()
unset(_fleet_render_unit)
foreach(_population_render_unit IN ITEMS 02387 02388 02389 02390 02391)
 list(REMOVE_ITEM RESTORE_UNITS "${CMAKE_CURRENT_SOURCE_DIR}/src/unit_${_population_render_unit}.c")
endforeach()
unset(_population_render_unit)
foreach(_ratings_render_unit IN ITEMS 02208 02209 02210 02211 02212)
 list(REMOVE_ITEM RESTORE_UNITS "${CMAKE_CURRENT_SOURCE_DIR}/src/unit_${_ratings_render_unit}.c")
endforeach()
unset(_ratings_render_unit)
foreach(_service_render_unit IN ITEMS 02393 02394 02395)
 list(REMOVE_ITEM RESTORE_UNITS "${CMAKE_CURRENT_SOURCE_DIR}/src/unit_${_service_render_unit}.c")
endforeach()
unset(_service_render_unit)
foreach(_religion_unit IN ITEMS 01604 02054 02056 02396 02397 02398 02399 02400)
 list(REMOVE_ITEM RESTORE_UNITS "${CMAKE_CURRENT_SOURCE_DIR}/src/unit_${_religion_unit}.c")
endforeach()
unset(_religion_unit)
list(REMOVE_ITEM RESTORE_UNITS "${CMAKE_CURRENT_SOURCE_DIR}/src/unit_02401.c")
list(REMOVE_ITEM RESTORE_UNITS "${CMAKE_CURRENT_SOURCE_DIR}/src/unit_02402.c")
foreach(_labor_render_unit IN ITEMS 01592 02368 02369 02370 02371)
 list(REMOVE_ITEM RESTORE_UNITS "${CMAKE_CURRENT_SOURCE_DIR}/src/unit_${_labor_render_unit}.c")
endforeach()
unset(_labor_render_unit)
foreach(_image_animation_unit IN ITEMS 00551 00552 00553 00554 00555 00556 00557 00558 00559)
 list(REMOVE_ITEM RESTORE_UNITS "${CMAKE_CURRENT_SOURCE_DIR}/src/unit_${_image_animation_unit}.c")
endforeach()
unset(_image_animation_unit)
# Ring member facades replace legacy functions with undefined ECX receivers.
foreach(_ring_unit IN ITEMS 00787 01365 02193 02194 02195 02196 02197 02198 02201 02202 02203 02204 02205 02206 02207)
 list(REMOVE_ITEM RESTORE_UNITS "${CMAKE_CURRENT_SOURCE_DIR}/src/unit_${_ring_unit}.c")
endforeach()
unset(_ring_unit)
# Typed score and group-selection implementations replace these entries.
set(_restore_compiled_units ${RESTORE_UNITS})
foreach(_score_unit IN ITEMS 00668 01608 01943 01973 01975 02156 02162 01584 00964 00965 01074 01031 01034 01123 01160 01897 01033 01041 01103 01136 01204 01112 01893 01254 01255 01573 01253 00646 00961 01366 01438 01446 01447 01448 01465 01480 01494 01519 02154 01451 01453 01032 01150 01019 01463 01506 01944 01945 01946 01947 01948 01950 01952 01957 01960 01929 01933 01125 01177 01359 01360 02341 02342 00583 02288 02289 01075 01076 01077 01078 01079 01080 01081 01082 01083 01084 01085 01088 01235 01106 01113 01046 01258 01259 01264 01178 01247 01930 01935 01936 01937 01938 01939 01940 01941 01942 02150 02157 02181 02510 01120 01243 01244 02049 01096 01121 02548 01053 02214 01089 02294 02293 02292 02291 02290 01090 01071 01069 01068 01065 01039 01043 01026 01116 02145 01115 01114 02502 01325 01236 01137 01100 01051 01191 01250 01091 01037 02489 00723 00212 00210 00209 00208 00041 00039 00035 02180 02179 02144 01040 01237 01092 01044 00007 00044 00053 00064 00065 00066 00120 00264 00265 00272 00273 00276 00278 00332 00402 00403 00616 00636 00639 00642 00803 00900 01017 01035 01047 01050 01054 01055 01056 01057 01058 01059 01060 01061 01062 01063 01064 01066 01070 01086 01087 01093 01109 01124 01126 01128 01131 01132 01133 01144 01158 01173 01174 01175 01176 01239 01276 01476 01487 01615 01617 01618 01667 01668 01854 01898 01905 01923 01924 01927 02118 02161 02174 02177 02215 02258 02267 02268 02270 02271 02321 02323 02325 02328 02329 02467 02468 02469 02553 02556)
 list(REMOVE_ITEM _restore_compiled_units "${CMAKE_CURRENT_SOURCE_DIR}/src/unit_${_score_unit}.c")
endforeach()
unset(_score_unit)
# Complete reviewed construction and route bodies replace their legacy entries.
 foreach(_construction_unit IN ITEMS 01336 01337 01338 01339 01340 01341 01342 01343 00928 00824 01363 02080 02107 00662 01709 01850 02403 02404 00902 01008 02178 02012 02013 02014 02041 02042 00840 00841 02029 02030 00671 00221 00228 00231 00232 00234 00235 00240 02444 01866 01867 02269 02272 02273 02217 02218 02240 02241 02243 02244 02245 02248 02249 02250 02251 02266 01290 01281 01292 01364 00121 02216 02221 02223 02224 02225 02226 02227 02228 02229 02230 02231 02232 02233 02234 02235 02236 02237 02238 02239 02253 02254 02256 02257 02259 02260 02261 02262 02263 02264 02265 02531 00045 00584 00585 00586 00589 00590 00591 00592 00593 00594 00595 00596 00597 00598 00843 01018 01023 01025 01027 01028 01029 01030 01101 01102 01104 01105 01111 01159 01164 01165 01187 01240 01242 02040 02043 02152 02175 02322 02330
 00828 00857 00886 01163 01577 01591 01190 00856 00887 00885 02549 02559
 01119 01127 01129 01274 01270 01271 01272
 00043 00983 00984 01002 01201 02153 02158
 01949 01951 01953 01954 01955 01956 01958 01959 01961 01962
 00316 00322 00323 00338 00345 00352 00359 00366 01982 01900 02155
 01020 00046 01022 02247 02367 02440 02450 01848
 00127 00640 01713 01714 01715 01716 01718 01719 01720 01721 01722 01723 01727 02414 02415 02416 02417 00233
 02435 00092 01717 01856 02447 02633 02634 02635 02636 01832
 00312 00313 01985 00324 00325 00340 00354 00361 00368 00347
 01249 01257 00607 01891
 01516 01517 01518 01445 01503 01830 01528 02151 02509 02511
 02213 02165 01000 00541
 01890 00645 01016 02458 00277
 02164 02142 00950 00951 00952 01169 01170
 01925 02452 01355 01422 01522 01523 01347 01348 01351 01441 01442 01452
 01349 01350 01413 01412 01525 01449 01486 00932 00936 00650 00895 00648 00897 00647 02309 00898
 00858 01180 01181 01182 01183 01184 01185 01316 01317 01318 01323 01324 01421 02182
 00652 00653 00654 00655 00657 01179 01186 01321 01322 01344 01346 01424 01460 01461
 02069 02070 02071 02073 02074 02077 02083 02084 02824 01398 00660
 01386 01395 01396 01397 01399 01400 01401 01402 01403 01404 01405 01406 01407 01408 01409 01410 01411 01535 01536 01537
 00797 00800 00806 00807 00808 00809 00813 00827 00829 00832 00833 00844 00845 00847 00849 00850 00851 00877 00878 00901 00905 00909 00939 00942 00947 00949 00954 01147 02297
 00859 00860 00933 00934 00935 01582 02023 02046 02047 02355 00122 02219 02220 02222 01921 01922 00115 00116 01168
 01479 01492 01483 01485 01491 01502 01504 01505 01488 01489 01507 01508 01509 01510 01511 01512 01513 01514 02075 02067 02068
 00028 00029 00030 00031 00032 00404 00405 00406 01916 02171 02481 00236 00676 00716 00717 00227 01918 01919
 02539 02540 01892 02018 02021 00282 00958 01464 01493 00791 00955 01199 01729
 00794 00795 00796 01562 00852 00854 00861 00862 00863 00864 00865 00866 00867 00868 00869 00870 00871
 02025 02032 02033 00453 00452 00454 00455 00842 02035 00446 00447 02037 02044 02028 02057
 02015 02016 02017 01571 01566 01529
 00929 01282 00815 00816 00927 00919 00920 00921 00922 01569 00892 00890 00893 00931 00818 00930 00915 00917 00925 00916 01005 01118 01478 01474 01473 00819 00821 00820 00822 00817 01293 01477 01597
 01387 01423 01303 01426 02336 02320 02339 02340 02298 02299 02301 02302 02306 02307 02308 02310 02312 02314 02318 02316 01431 01433 01434 01432 00943 02337 01393
 01231 01232 01220 01221 01226 01211 01223 01224 01225 01227 01210 00451 01229 01230 01222 01213 01214 01209 01212 01228 01215 01216 01217 01218 01219 01146 01515 01420 00891 01148 00904 01145
 00327 00329 00341 00342 00370 01418 01419 01436 01437 01439 01440 02094 02095 02557 01320 01443 01636 01637 01638 01639 01645 01646 01647 01648 01649 01650 01651
 01314 01315 02303 02492 01313 00948 00837 02039
 00881 00889 00899 00903 00907 00911 00912 00914 01308 01309 01310 01311 01425 02304 02305 02311 02313 02315 02319 02338)
 list(REMOVE_ITEM _restore_compiled_units "${CMAKE_CURRENT_SOURCE_DIR}/src/unit_${_construction_unit}.c")
endforeach()
unset(_construction_unit)
# Typed irrigation controller, intake, propagation and terrain-area implementations.
foreach(_irrigation_unit IN ITEMS 00317 00326 00328 00330 00348 00349 00350 00355 00356 00362 00363 00792 00793 00799 00801 00802 00805 00812 00937 00938 00956 00957 01196 01197 01200 01273 01520 01564 01565 01978 01980 01981 01983 01984 02102 02103 02110)
 list(REMOVE_ITEM _restore_compiled_units "${CMAKE_CURRENT_SOURCE_DIR}/src/unit_${_irrigation_unit}.c")
endforeach()
unset(_irrigation_unit)
if(NOT RESTORE_BUILD_HYBRID)
 list(REMOVE_ITEM _restore_compiled_units "${CMAKE_CURRENT_SOURCE_DIR}/src/unit_01868.c")
endif()
add_library(restore_score_submission OBJECT runtime/high_score_submission.c runtime/campaign_score.c runtime/pyramid_summary.c
 runtime/campaign_score_numeric.c runtime/monument_progress.c runtime/monument_base_queries.c runtime/monument_complex_queries.c)
target_include_directories(restore_score_submission PRIVATE include)
target_compile_definitions(restore_score_submission PRIVATE _CRT_SECURE_NO_WARNINGS)
target_compile_options(restore_score_submission PRIVATE /W4 /WX "$<$<CONFIG:Debug>:/Od>" "$<$<CONFIG:Release>:/O2>")
add_library(restore_menu_selection OBJECT runtime/menu_group_selection.c runtime/menu_tail_ui.c
 runtime/nearest_rect.c runtime/nearest_00407870.c runtime/nearest_00407a10.c runtime/three_way_button.c runtime/list_panel_input.c
 runtime/video_open.c runtime/video_frame.c runtime/menu_prepare_integer.c runtime/menu_monument_search.c
 runtime/message_controller.c runtime/message_strings.c runtime/action_tail.c
 runtime/record_place.c runtime/record_rebuild.c runtime/record_queries.c runtime/menu_building_kind.c runtime/menu_resource_neighbors.c
 runtime/menu_building_options.c runtime/resource_availability.c
 runtime/menu_route_seed.c runtime/menu_route_queries.c runtime/menu_route_flood.c
 runtime/menu_drag_leaves.c
 runtime/menu_drag_footprint.c
 runtime/menu_drag_region_gate.c
 runtime/menu_drag_neighbors.c
 runtime/menu_drag_refresh_tile.c
 runtime/menu_drag_road_refresh.c
 runtime/menu_drag_road_variant.c
 runtime/menu_drag_building_state.c
 runtime/menu_drag_queue_publish.c
 runtime/mission_unlock.c runtime/tool_availability.c
 runtime/menu_drag_ground.c runtime/menu_drag_water_refresh.c
 runtime/menu_actor_message.c
 runtime/menu_drag_canal_refresh.c runtime/menu_drag_building_graph.c runtime/menu_drag_road_place.c
 runtime/menu_drag_terrain_graph.c runtime/menu_drag_road_pack.c runtime/menu_drag_refresh_dispatch.c runtime/menu_drag_flood_edges.c
 runtime/map_commit_fields.c runtime/map_commit_costs.c runtime/map_commit_refresh.c
 runtime/map_commit_state.c runtime/menu_drag_clear_helpers.c runtime/menu_drag_clear_footprint.c runtime/map_commit_selection.c
 runtime/menu_prepare_control.c
 runtime/menu_drag_refresh_wrappers.c runtime/menu_drag_paving_pack.c
 runtime/menu_drag_neighbor_growth.c runtime/menu_drag_growth_numeric.c runtime/menu_drag_growth_update.c
 runtime/map_commit_warnings.c
 runtime/map_commit_proximity.c runtime/map_commit_resource_warnings.c runtime/map_commit_resource_gate.c runtime/map_commit_town_warning.c
 runtime/map_commit_links.c runtime/map_commit_paths.c runtime/map_commit_connect.c
 runtime/menu_drag_paving_neighbors.c
 runtime/menu_drag_surface_passes.c runtime/menu_drag_paving_pass_model.c runtime/menu_drag_vegetation_pass_model.c runtime/menu_drag_rail_pass_model.c
 runtime/menu_drag_map_gates.c runtime/menu_drag_gate_query_model.c runtime/menu_drag_gate_publish_model.c
 runtime/menu_drag_canal_region.c runtime/menu_drag_canal_region_model.c runtime/menu_drag_redraw_dispatch.c runtime/menu_drag_redraw_dispatch_model.c
 runtime/menu_drag_preview_geometry.c runtime/menu_drag_route_query.c runtime/menu_drag_route_query_model.c
 runtime/menu_drag_actor_teardown.c runtime/menu_drag_actor_profile.c runtime/menu_drag_actor_profile_model.c
 runtime/menu_drag_actor_create.c runtime/menu_drag_actor_spawn.c runtime/menu_drag_building_cleanup.c runtime/menu_drag_group_clear.c
 runtime/menu_drag_actor_walk_link.c runtime/menu_drag_actor_walk_unlink.c runtime/actor_walking_facades.c
 runtime/map_commit_population.c runtime/city_footprint_validation.c
 runtime/menu_drag_monument_unlink.c runtime/menu_drag_monument_clear.c
 runtime/menu_drag_building_release.c runtime/menu_drag_commit.c runtime/menu_drag_commit_model.c
 runtime/menu_drag_paint_region.c runtime/menu_drag_paving_region_model.c runtime/menu_drag_vegetation_region_model.c runtime/menu_drag_route_footprint.c)
target_include_directories(restore_menu_selection PRIVATE include)
target_sources(restore_menu_selection PRIVATE
 runtime/menu_drag_land_route.c runtime/menu_drag_land_route_model.c
 runtime/menu_drag_route_walk.c runtime/menu_drag_route_walk_model.c
 runtime/menu_drag_route_paint.c runtime/menu_drag_road_paint_model.c runtime/menu_drag_canal_paint_model.c
 runtime/menu_drag_connection_plan.c runtime/menu_drag_connection_plan_model.c
 runtime/map_commit_placement.c runtime/map_commit_route_actions.c
 runtime/building_reservation.c runtime/building_neighbor_query.c runtime/building_nearest_search.c
 runtime/menu_drag_world_offset.c
 runtime/construction_tiles.c runtime/bridge_place.c runtime/building_quality_update.c runtime/displaced_population.c
 runtime/menu_drag_construction_scan.c
 runtime/menu_drag_construction_parent.c
 runtime/construction_lines.c runtime/construction_refresh.c runtime/building_allocator.c runtime/building_rect_commit.c
 runtime/menu_drag_construction_family.c
 runtime/menu_drag_preview_parent.c
 runtime/construction_controller.c runtime/construction_brush.c
 runtime/placement_support.c runtime/placement_support_model.c runtime/monument_placement_queries.c
 runtime/placement_context.c runtime/general_placement.c
 runtime/building_footprint.c runtime/building_support.c runtime/remaining_warnings.c
 runtime/storage_settings.c runtime/gatehouse_display.c runtime/formation_control.c runtime/temple_complex.c runtime/housing_cycle.c runtime/house_split.c runtime/house_layout.c runtime/house_levels.c runtime/house_evolution.c runtime/building_controllers.c runtime/building_widgets.c runtime/placement_queries.c
 runtime/building_image.c runtime/building_image_model.c
 runtime/building_create.c runtime/building_create_model.c runtime/building_methods.c runtime/monument_create.c runtime/monument_entrance.c runtime/monument_map.c runtime/monument_images.c runtime/sun_temple.c runtime/monument_complex_create.c runtime/monument_complex_positions.c runtime/monument_complex_images.c
 runtime/building_refresh_helpers.c runtime/building_refresh_model.c runtime/placement_palette.c
 runtime/map_commit_controller_model.c runtime/map_commit_complete.c
 runtime/menu_render_list_queries.c runtime/menu_building_overlay.c runtime/menu_building_overlay_model.c
 runtime/menu_resource_render.c runtime/menu_resource_render_model.c runtime/input_monument_summary.c
 runtime/validation_dialog.c runtime/menu_training_render.c runtime/menu_training_render_model.c
 runtime/mission_city_commit.c runtime/mission_load_closure.c
 runtime/widget_position.c runtime/widget_position_model.c runtime/pyramid_widget_draw.c
 runtime/widget_draw.c runtime/widget_draw_model.c runtime/world_building_draw.c
 runtime/actor_mission_init.c runtime/actor_mission_init_model.c runtime/mission_map_loader.c
 runtime/monument_rating.c runtime/building_status_render.c
 runtime/editor_map_reset.c runtime/monument_menu_render.c runtime/command_dialog_actions.c
 runtime/basic_command_actions.c runtime/monument_effects.c runtime/command_migration.c
 runtime/god_building_effects.c runtime/god_building_effects_model.c
 runtime/god_effects.c runtime/god_city.c runtime/god_helpers.c runtime/god_frog_effect.c
 runtime/god_command_actions.c runtime/god_remaining_commands.c
 runtime/actor_command.c runtime/actor_command_model.c
 runtime/invasion_commands.c runtime/invasion_model.c
 runtime/numeric_input.c runtime/numeric_input_model.c runtime/numeric_completion.c runtime/city_numeric_confirm.c
 runtime/small_button_input.c runtime/selection_button_actions.c
 runtime/detail_button_actions.c runtime/unit_button_actions.c runtime/menu_modal_buttons.c
 runtime/unit_keyboard_actions.c runtime/menu_modal_buttons2.c runtime/small_button_actions2.c
 runtime/editor_option_buttons.c runtime/editor_option_buttons2.c runtime/sidebar_command.c
 runtime/sidebar_message_actions.c runtime/sidebar_undo.c runtime/sidebar_undo_model.c
 runtime/editor_modal_actions3.c runtime/editor_short_actions.c
 runtime/editor_city_route_actions.c runtime/editor_permissions_actions.c runtime/editor_city_detail_actions.c
 runtime/editor_event_list_actions.c runtime/diplomacy_input_actions.c runtime/diplomacy_input_model.c
 runtime/editor_popup_actions.c
 runtime/actor_panel_actions.c runtime/actor_panel_model.c runtime/actor_panel_native_context.c
 runtime/editor_event_detail_actions.c runtime/editor_event_detail_model.c runtime/late_menu_actions.c
 runtime/remaining_resource_actions.c runtime/remaining_editor_list_actions.c runtime/remaining_editor_list_model.c
 runtime/remaining_selection_actions.c runtime/construction_confirm_actions.c runtime/menu_economy_actions.c
 runtime/menu_remaining_actions.c runtime/menu_message_actions.c
 runtime/menu_labor_actions.c runtime/menu_labor_model.c
 runtime/figure_menu_actions.c runtime/storage_order_actions.c
 runtime/menu_rotation_actions.c runtime/menu_rotation_model.c runtime/menu_rotation_buildings_model.c
 runtime/menu_advisor_actions.c runtime/menu_advisor_model.c
 runtime/briefing_draw.c runtime/briefing_model.c
 runtime/menu_event_actions.c runtime/menu_event_model.c
 runtime/mission_record_actions.c runtime/mission_record_model.c
 runtime/mission_setup.c
 runtime/calendar_updates.c runtime/irrigation_updates.c runtime/city_tick.c runtime/carpenter_jobs.c runtime/desirability_updates.c
 runtime/city_updates.c runtime/city_updates_model.c
 runtime/population_updates.c runtime/actor_routing_model.c
 runtime/actor_laborer.c runtime/actor_laborer_model.c
 runtime/actor_civic.c runtime/actor_civic_model.c
 runtime/actor_constable.c runtime/actor_constable_model.c runtime/actor_sound.c
 runtime/actor_transport.c runtime/actor_transport_model.c runtime/actor_transport_boundary.c
 runtime/tooltip_controller.c runtime/tooltip_controller_model.c
 runtime/actor_walking_queue_count.c runtime/actor_walking_queue_full.c runtime/actor_walking_queue_push.c
 runtime/building_world_frames.c
 runtime/world_base_model.c
 runtime/world_detail_model.c
 runtime/world_dispatch_model.c
 runtime/world_image_helpers_model.c
 runtime/world_metric_helpers_model.c
 runtime/world_support_model.c
 runtime/world_view_helpers_model.c
 runtime/building_food_render.c)
target_compile_definitions(restore_menu_selection PRIVATE _CRT_SECURE_NO_WARNINGS)
target_compile_options(restore_menu_selection PRIVATE /W4 /WX "$<$<CONFIG:Debug>:/Od>" "$<$<CONFIG:Release>:/O2>")
add_library(pharaoh_restored STATIC ${_restore_compiled_units} $<TARGET_OBJECTS:restore_score_submission> $<TARGET_OBJECTS:restore_menu_selection> runtime/decompiler_support.c runtime/city_history.c
  runtime/media_loader.c runtime/media_bindings.c runtime/legacy_member_calls.c runtime/city_migration.c runtime/compression.c runtime/chunk_migrations.c
  runtime/video_platform_win32.c runtime/display_configuration.c runtime/sound_streams.c runtime/startup_probe.c
  runtime/menu_actions.c runtime/menu_callbacks.c runtime/menu_platform_win32.c runtime/file_list_platform_win32.c runtime/family_menu.c runtime/family_delete.c runtime/family_options.c runtime/campaign_menu.c runtime/mission_menu.c runtime/load_save_menu.c runtime/town_menu.c runtime/text_templates.c runtime/menu_dialog.c
  runtime/window_messages.c runtime/window_platform_win32.c runtime/window_entry.c
  runtime/graphics_platform_win32.c runtime/text_selection.c runtime/legacy_streams.c runtime/sprite_libraries.c runtime/sprite_pixels.c runtime/sound_initialization.c runtime/sound_platform_win32.c runtime/music_manager.c runtime/flood_simulation.c runtime/actor_callbacks.c runtime/animation_state.c runtime/high_scores_menu.c)
target_sources(pharaoh_restored PRIVATE runtime/terrain_diamond.c runtime/upper_raster.c runtime/message_panel_input.c)
target_sources(pharaoh_restored PRIVATE runtime/model_loading.c)
target_sources(pharaoh_restored PRIVATE runtime/ring_queue.c)
target_sources(pharaoh_restored PRIVATE runtime/actor_ring_lifecycle.c)
target_sources(pharaoh_restored PRIVATE runtime/sprite_frame_clock.c)
target_sources(pharaoh_restored PRIVATE runtime/town_record_queries.c runtime/message_category.c runtime/actor_message.c)
target_sources(pharaoh_restored PRIVATE runtime/flood_settings.c)
target_sources(pharaoh_restored PRIVATE runtime/event_schedule.c)
target_sources(pharaoh_restored PRIVATE runtime/event_parameters.c)
target_sources(pharaoh_restored PRIVATE runtime/event_packet.c runtime/event_message_kind.c runtime/flood_event.c)
target_sources(pharaoh_restored PRIVATE runtime/flood_quality.c runtime/flood_quality_effects.c)
target_sources(pharaoh_restored PRIVATE runtime/file_list_lifecycle.c)
target_sources(pharaoh_restored PRIVATE runtime/dynamic_history.c)
target_sources(pharaoh_restored PRIVATE runtime/game_diagnostics.c)
target_sources(pharaoh_restored PRIVATE runtime/flood_cell.c)
target_sources(pharaoh_restored PRIVATE runtime/building_stock.c)
target_sources(pharaoh_restored PRIVATE runtime/world_detail.c)

target_sources(pharaoh_restored PRIVATE runtime/world_overlay_metrics.c)
set_source_files_properties(runtime/world_overlay_metrics.c PROPERTIES
 SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX")

target_sources(pharaoh_restored PRIVATE runtime/world_special_buildings.c)
target_sources(pharaoh_restored PRIVATE runtime/kingdom_advisor_render.c)
target_sources(pharaoh_restored PRIVATE runtime/labor_advisor_render.c)
target_sources(pharaoh_restored PRIVATE runtime/military_advisor_render.c)
target_sources(pharaoh_restored PRIVATE runtime/fleet_advisor_render.c)
target_sources(pharaoh_restored PRIVATE runtime/service_advisor_render.c)
target_sources(pharaoh_restored PRIVATE runtime/religion_updates.c)
target_sources(pharaoh_restored PRIVATE runtime/religion_advisor_render.c)
target_sources(pharaoh_restored PRIVATE runtime/financial_advisor_render.c)
target_sources(pharaoh_restored PRIVATE runtime/monument_advisor_render.c)
target_sources(pharaoh_restored PRIVATE runtime/chief_advisor_render.c)
target_sources(pharaoh_restored PRIVATE runtime/population_advisor_render.c)
target_sources(pharaoh_restored PRIVATE runtime/ratings_advisor_render.c)
target_sources(pharaoh_restored PRIVATE runtime/trade_advisor_render.c)
target_sources(pharaoh_restored PRIVATE runtime/trade_updates.c)
target_sources(pharaoh_restored PRIVATE runtime/financial_updates.c)
target_sources(pharaoh_restored PRIVATE runtime/tax_updates.c)
target_sources(pharaoh_restored PRIVATE runtime/tax_collector.c)
target_sources(pharaoh_restored PRIVATE runtime/actor_services.c)
target_sources(pharaoh_restored PRIVATE runtime/house_services.c)
target_sources(pharaoh_restored PRIVATE runtime/building_service_effects.c)
target_sources(pharaoh_restored PRIVATE runtime/disease_updates.c)
target_sources(pharaoh_restored PRIVATE runtime/city_health.c)
target_sources(pharaoh_restored PRIVATE runtime/city_sentiment.c)
target_sources(pharaoh_restored PRIVATE runtime/locust_updates.c)
target_sources(pharaoh_restored PRIVATE runtime/actor_movement.c runtime/actor_route_slots.c)
target_sources(pharaoh_restored PRIVATE runtime/actor_weighted_search.c runtime/actor_goal_route.c)
# Select the existing pixel-path owner once; no duplicate canonical wrapper.
target_sources(pharaoh_restored PRIVATE runtime/menu_drag_actor_move.c)
set_source_files_properties(runtime/menu_drag_actor_move.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX;/O2;/GS-;/Ob0")
# Reuse existing tile-query algorithms. Their canonical facades remain in actor_civic.c.
target_sources(pharaoh_restored PRIVATE src/unit_01460.c src/unit_01461.c)
# Reuse the pre-existing typed service-state algorithm with native phase corrections.
target_sources(pharaoh_restored PRIVATE src/unit_01320.c)
# The standalone verification targets retain their original FUN symbols.
set_property(SOURCE src/unit_01320.c APPEND PROPERTY COMPILE_DEFINITIONS "$<$<STREQUAL:$<TARGET_PROPERTY:NAME>,pharaoh_restored>:FUN_00493610=restore_actor_update_service_state>")
set_property(SOURCE src/unit_01460.c APPEND PROPERTY COMPILE_DEFINITIONS "$<$<STREQUAL:$<TARGET_PROPERTY:NAME>,pharaoh_restored>:FUN_004b56d0=restore_actor_find_combat_target_on_tile>")
set_property(SOURCE src/unit_01461.c APPEND PROPERTY COMPILE_DEFINITIONS "$<$<STREQUAL:$<TARGET_PROPERTY:NAME>,pharaoh_restored>:FUN_004b5770=restore_actor_find_projectile_victim_on_tile>")
# Reuse the unchanged resource-to-cargo-image C switch.
target_sources(pharaoh_restored PRIVATE src/unit_01310.c)
set_source_files_properties(src/unit_01310.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX;/O2;/GS-;/Ob0")
set_property(SOURCE src/unit_01310.c APPEND PROPERTY COMPILE_DEFINITIONS "$<$<STREQUAL:$<TARGET_PROPERTY:NAME>,pharaoh_restored>:FUN_0048b320=restore_actor_resource_cargo_image_group>")
# Reuse existing scans with only native access-type/prototype repairs.
target_sources(pharaoh_restored PRIVATE src/unit_01907.c src/unit_02555.c)
set_source_files_properties(src/unit_01907.c src/unit_02555.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX;/O2;/GS-;/Ob0")
set_property(SOURCE src/unit_01907.c APPEND PROPERTY COMPILE_DEFINITIONS "$<$<STREQUAL:$<TARGET_PROPERTY:NAME>,pharaoh_restored>:FUN_004e0c00=restore_actor_find_approach_kind222>")
set_property(SOURCE src/unit_01907.c APPEND PROPERTY COMPILE_DEFINITIONS "$<$<STREQUAL:$<TARGET_PROPERTY:NAME>,pharaoh_restored>:FUN_004e05f0=restore_actor_approach_kind222_accept>")
set_property(SOURCE src/unit_02555.c APPEND PROPERTY COMPILE_DEFINITIONS "$<$<STREQUAL:$<TARGET_PROPERTY:NAME>,pharaoh_restored>:FUN_00542a80=restore_actor_find_approach_kind213>")
set_property(SOURCE src/unit_02555.c APPEND PROPERTY COMPILE_DEFINITIONS "$<$<STREQUAL:$<TARGET_PROPERTY:NAME>,pharaoh_restored>:FUN_005421d0=restore_actor_approach_kind213_accept>")
# Repair existing acceptance owners in place; retain full-result RAW canonicals.
set_source_files_properties(src/unit_01906.c src/unit_02554.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX;/O2;/GS-;/Ob0")
set_property(SOURCE src/unit_01906.c src/unit_02554.c APPEND PROPERTY INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/artifacts/actor_civic_followup")
# Existing stage query keeps its canonical source and receives only the BYTE-base repair.
set_source_files_properties(src/unit_00331.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX;/O2;/GS-;/Ob0")
set_property(SOURCE artifacts/actor_civic_followup/actor_acceptance_members.c APPEND PROPERTY COMPILE_DEFINITIONS "restore_acceptance_kind230_check_raw=restore_acceptance_kind230_check_logical")
# Select repaired original route owners explicitly; their old facades are removed.
target_sources(pharaoh_restored PRIVATE src/unit_01148.c src/unit_02336.c src/unit_02337.c src/unit_02338.c src/unit_02339.c src/unit_02340.c src/unit_02304.c src/unit_02305.c src/unit_02311.c src/unit_02313.c src/unit_02319.c)
set_source_files_properties(src/unit_01148.c src/unit_02336.c src/unit_02337.c src/unit_02338.c src/unit_02339.c src/unit_02340.c src/unit_02304.c src/unit_02305.c src/unit_02311.c src/unit_02313.c src/unit_02319.c src/unit_00343.c src/unit_00357.c src/unit_00364.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX;/O2;/GS-;/Ob0")
set_property(SOURCE src/unit_01148.c src/unit_02336.c src/unit_02337.c src/unit_02338.c src/unit_02339.c src/unit_02340.c src/unit_02304.c src/unit_02305.c src/unit_02311.c src/unit_02313.c src/unit_02319.c APPEND PROPERTY INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/artifacts/actor_civic_followup")
# City follow-up: preserve active owners and reselect only excluded repairs.
target_sources(pharaoh_restored PRIVATE runtime/city_reconstruction_bridges.c src/unit_01339.c src/unit_00943.c src/unit_02309.c src/unit_00947.c)
set_source_files_properties(runtime/city_reconstruction_bridges.c src/unit_01339.c src/unit_00943.c src/unit_02309.c src/unit_00944.c src/unit_00946.c src/unit_00947.c src/unit_00953.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX;/O2;/GS-;/Ob0")
set_property(SOURCE src/unit_01339.c src/unit_00943.c src/unit_02309.c src/unit_00944.c src/unit_00946.c src/unit_00947.c src/unit_00953.c APPEND PROPERTY INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/artifacts/actor_civic_followup")
# City census, road regions, hazards and sentry refresh: original owners only.
target_sources(pharaoh_restored PRIVATE src/unit_00954.c src/unit_00933.c src/unit_00934.c src/unit_00935.c src/unit_00939.c src/unit_00942.c src/unit_01147.c)
set_source_files_properties(src/unit_01345.c src/unit_01475.c src/unit_00954.c src/unit_00933.c src/unit_00934.c src/unit_00935.c src/unit_00939.c src/unit_00942.c src/unit_01147.c src/unit_00940.c src/unit_00941.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX;/O2;/GS-;/Ob0")
# Complete the connected city-worker cohort using original source owners.
target_sources(pharaoh_restored PRIVATE src/unit_00919.c src/unit_00920.c src/unit_00921.c src/unit_00922.c src/unit_00929.c src/unit_00930.c src/unit_00931.c src/unit_00816.c src/unit_00818.c src/unit_00915.c src/unit_00925.c src/unit_01569.c)
set_source_files_properties(src/unit_00919.c src/unit_00920.c src/unit_00921.c src/unit_00922.c src/unit_00929.c src/unit_00930.c src/unit_00931.c src/unit_00816.c src/unit_00818.c src/unit_00915.c src/unit_00925.c src/unit_01569.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX;/O2;/GS-;/Ob0")
# Select the published C owners once; their bodies are not copied or counted again.
target_sources(pharaoh_restored PRIVATE
 artifacts/actor_civic_followup/actor_civic_updates.c
 artifacts/actor_civic_followup/actor_vagrant.c
 artifacts/actor_civic_followup/actor_route_pixels.c
 artifacts/actor_civic_followup/actor_route_leaves.c
 artifacts/actor_civic_followup/actor_engineer.c
 artifacts/actor_civic_followup/actor_fireman.c
 artifacts/actor_civic_followup/actor_warehouseman.c
 artifacts/actor_civic_followup/actor_walking_service.c
 artifacts/actor_civic_followup/actor_cartpusher.c
 artifacts/actor_civic_followup/actor_cart_delivery.c
 artifacts/actor_civic_followup/actor_route_approach.c
 artifacts/actor_civic_followup/actor_approach_members.c
 artifacts/actor_civic_followup/actor_approach_search.c
 artifacts/actor_civic_followup/actor_approach_building_scans.c
 artifacts/actor_civic_followup/actor_acceptance_members.c
 artifacts/actor_civic_followup/actor_worker_availability.c
 artifacts/actor_civic_followup/actor_worker_query.c
 artifacts/actor_civic_followup/actor_base_acceptance.c
 artifacts/actor_civic_followup/actor_city_migration.c)
set_source_files_properties(
 artifacts/actor_civic_followup/actor_civic_updates.c
 artifacts/actor_civic_followup/actor_vagrant.c
 artifacts/actor_civic_followup/actor_route_pixels.c
 artifacts/actor_civic_followup/actor_route_leaves.c
 artifacts/actor_civic_followup/actor_engineer.c
 artifacts/actor_civic_followup/actor_fireman.c
 artifacts/actor_civic_followup/actor_warehouseman.c
 artifacts/actor_civic_followup/actor_walking_service.c
 artifacts/actor_civic_followup/actor_cartpusher.c
 artifacts/actor_civic_followup/actor_cart_delivery.c
 artifacts/actor_civic_followup/actor_route_approach.c
 artifacts/actor_civic_followup/actor_approach_members.c
 artifacts/actor_civic_followup/actor_approach_search.c
 artifacts/actor_civic_followup/actor_approach_building_scans.c
 artifacts/actor_civic_followup/actor_acceptance_members.c
 artifacts/actor_civic_followup/actor_worker_availability.c
 artifacts/actor_civic_followup/actor_worker_query.c
 artifacts/actor_civic_followup/actor_base_acceptance.c
 artifacts/actor_civic_followup/actor_city_migration.c
 PROPERTIES SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX;/O2;/GS-;/Ob0")
target_sources(pharaoh_restored PRIVATE runtime/actor_bazaar_targets.c)
target_sources(pharaoh_restored PRIVATE runtime/buyer_transfer.c)
target_sources(pharaoh_restored PRIVATE runtime/mission_schema.c runtime/vegetation_farm_init.c)
set_source_files_properties(runtime/mission_schema.c runtime/vegetation_farm_init.c PROPERTIES
 SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX")
set_source_files_properties(runtime/buyer_transfer.c PROPERTIES
 SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX")
set_source_files_properties(src/unit_00138.c src/unit_00139.c
 src/unit_00150.c src/unit_00152.c src/unit_00153.c PROPERTIES
 SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX")
set_source_files_properties(src/unit_00160.c PROPERTIES
 SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX")
set_source_files_properties(src/unit_02059.c PROPERTIES
 SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX")
set_source_files_properties(src/unit_01072.c src/unit_01073.c PROPERTIES
 SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX")
set_source_files_properties(src/unit_00125.c src/unit_01765.c PROPERTIES
 SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX")
set_source_files_properties(src/unit_00672.c src/unit_01657.c src/unit_01658.c PROPERTIES
 SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX")
set_source_files_properties(src/unit_02357.c PROPERTIES
 SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX")
set_source_files_properties(src/unit_00587.c PROPERTIES
 SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX")
set_source_files_properties(src/unit_01772.c PROPERTIES
 SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX")
set_source_files_properties(src/unit_00473.c src/unit_00474.c src/unit_00476.c
 src/unit_00570.c src/unit_00571.c src/unit_00572.c src/unit_00600.c src/unit_01295.c PROPERTIES
 SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX")
set_source_files_properties(src/unit_00569.c src/unit_00599.c src/unit_00988.c PROPERTIES
 SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX")
set_source_files_properties(src/unit_02090.c PROPERTIES
 SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX")
set_source_files_properties(src/unit_00661.c PROPERTIES
 SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX")
set_source_files_properties(src/unit_01894.c PROPERTIES
 SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX")
set_source_files_properties(src/unit_01899.c src/unit_01901.c src/unit_01902.c src/unit_01903.c src/unit_01909.c PROPERTIES
 SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX")
set_source_files_properties(src/unit_02091.c src/unit_02092.c src/unit_02096.c src/unit_02097.c
 src/unit_02143.c src/unit_02146.c src/unit_02176.c PROPERTIES
 SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX")
set_source_files_properties(src/unit_01589.c src/unit_01606.c src/unit_02462.c PROPERTIES
 SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX")
set_source_files_properties(src/unit_00960.c src/unit_00963.c src/unit_01601.c PROPERTIES
 SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX")
set_source_files_properties(src/unit_02104.c src/unit_02111.c src/unit_02119.c src/unit_02120.c
 src/unit_02121.c src/unit_02122.c src/unit_02123.c PROPERTIES
 SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX")
set_source_files_properties(src/unit_02098.c src/unit_02101.c src/unit_02106.c src/unit_02117.c
 src/unit_02127.c src/unit_02129.c src/unit_02132.c PROPERTIES
 SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX")
set_source_files_properties(src/unit_01319.c src/unit_02581.c src/unit_02583.c PROPERTIES
 SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX")
set_source_files_properties(src/unit_00566.c src/unit_00567.c src/unit_00568.c src/unit_01162.c PROPERTIES
 SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX")
set_source_files_properties(src/unit_02112.c src/unit_02124.c src/unit_02125.c src/unit_02128.c src/unit_02131.c PROPERTIES
 SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX")
set_source_files_properties(src/unit_02352.c PROPERTIES
 SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX")
set_source_files_properties(src/unit_01587.c src/unit_01588.c PROPERTIES
 SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX")
set_source_files_properties(src/unit_00680.c src/unit_00689.c src/unit_00690.c src/unit_00706.c PROPERTIES
 SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX")
set_source_files_properties(src/unit_00715.c src/unit_01707.c src/unit_01710.c src/unit_01711.c PROPERTIES
 SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX")
set_source_files_properties(src/unit_02363.c PROPERTIES
 SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX")
set_source_files_properties(src/unit_00054.c src/unit_00055.c src/unit_00056.c src/unit_00057.c
 src/unit_00058.c src/unit_00059.c src/unit_00060.c src/unit_00061.c src/unit_01370.c src/unit_01527.c PROPERTIES
 SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX")
set_source_files_properties(runtime/actor_bazaar_targets.c PROPERTIES
 SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX")
set_source_files_properties(runtime/actor_weighted_search.c runtime/actor_goal_route.c PROPERTIES
 SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX")
target_sources(pharaoh_restored PRIVATE runtime/actor_land_search.c runtime/actor_land_route.c runtime/actor_path_occupancy.c)
set_source_files_properties(runtime/actor_land_search.c runtime/actor_land_route.c runtime/actor_path_occupancy.c PROPERTIES
 SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX")
target_sources(pharaoh_restored PRIVATE runtime/actor_route_steps.c runtime/actor_movement_validation.c)
set_source_files_properties(runtime/actor_route_steps.c runtime/actor_movement_validation.c PROPERTIES
 SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX")
set_source_files_properties(runtime/actor_movement.c runtime/actor_route_slots.c PROPERTIES
 SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX")
set_source_files_properties(runtime/locust_updates.c PROPERTIES
 SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX")
set_source_files_properties(runtime/city_sentiment.c PROPERTIES
 SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX")
set_source_files_properties(runtime/city_health.c PROPERTIES
 SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX")
set_source_files_properties(runtime/disease_updates.c PROPERTIES
  COMPILE_OPTIONS "/W4;/WX" SKIP_PRECOMPILE_HEADERS ON)
set_source_files_properties(runtime/god_helpers.c PROPERTIES
  COMPILE_OPTIONS "/W4;/WX" SKIP_PRECOMPILE_HEADERS ON)
set_source_files_properties(runtime/building_service_effects.c PROPERTIES
  COMPILE_OPTIONS "/W4;/WX" SKIP_PRECOMPILE_HEADERS ON)
set_source_files_properties(runtime/house_services.c PROPERTIES
  COMPILE_OPTIONS "/W4;/WX" SKIP_PRECOMPILE_HEADERS ON)
set_source_files_properties(runtime/actor_services.c PROPERTIES
  COMPILE_OPTIONS "/W4;/WX" SKIP_PRECOMPILE_HEADERS ON)
set_source_files_properties(runtime/tax_collector.c PROPERTIES
  COMPILE_OPTIONS "/W4;/WX" SKIP_PRECOMPILE_HEADERS ON)
set_source_files_properties(runtime/tax_updates.c PROPERTIES
  COMPILE_OPTIONS "/W4;/WX" SKIP_PRECOMPILE_HEADERS ON)
set_source_files_properties(runtime/financial_updates.c PROPERTIES
  COMPILE_OPTIONS "/W4;/WX" SKIP_PRECOMPILE_HEADERS ON)
set_source_files_properties(runtime/trade_updates.c PROPERTIES
  COMPILE_OPTIONS "/W4;/WX" SKIP_PRECOMPILE_HEADERS ON)
set_source_files_properties(src/unit_00617.c src/unit_00618.c src/unit_02632.c PROPERTIES
  COMPILE_OPTIONS "/W4;/WX" SKIP_PRECOMPILE_HEADERS ON)
set_source_files_properties(runtime/trade_advisor_render.c src/unit_02631.c PROPERTIES
  COMPILE_OPTIONS "/W4;/WX" SKIP_PRECOMPILE_HEADERS ON)
set_source_files_properties(runtime/ratings_advisor_render.c PROPERTIES
  COMPILE_OPTIONS "/W4;/WX" SKIP_PRECOMPILE_HEADERS ON)
set_source_files_properties(runtime/population_advisor_render.c src/unit_02053.c src/unit_02392.c PROPERTIES
  COMPILE_OPTIONS "/W4;/WX" SKIP_PRECOMPILE_HEADERS ON)
set_source_files_properties(runtime/chief_advisor_render.c PROPERTIES
  COMPILE_OPTIONS "/W4;/WX" SKIP_PRECOMPILE_HEADERS ON)
set_source_files_properties(runtime/monument_advisor_render.c PROPERTIES
  COMPILE_OPTIONS "/W4;/WX" SKIP_PRECOMPILE_HEADERS ON)
set_source_files_properties(runtime/financial_advisor_render.c src/unit_02448.c PROPERTIES
  COMPILE_OPTIONS "/W4;/WX" SKIP_PRECOMPILE_HEADERS ON)
set_source_files_properties(runtime/religion_advisor_render.c PROPERTIES
  COMPILE_OPTIONS "/W4;/WX" SKIP_PRECOMPILE_HEADERS ON)
set_source_files_properties(runtime/religion_updates.c PROPERTIES
 COMPILE_OPTIONS "/W4;/WX" SKIP_PRECOMPILE_HEADERS ON)
set_source_files_properties(runtime/service_advisor_render.c PROPERTIES
 COMPILE_OPTIONS "/W4;/WX" SKIP_PRECOMPILE_HEADERS ON)
set_source_files_properties(runtime/fleet_advisor_render.c PROPERTIES
 COMPILE_OPTIONS "/W4;/WX" SKIP_PRECOMPILE_HEADERS ON)
set_source_files_properties(runtime/military_advisor_render.c PROPERTIES
 COMPILE_OPTIONS "/W4;/WX" SKIP_PRECOMPILE_HEADERS ON)
set_source_files_properties(runtime/labor_advisor_render.c PROPERTIES
 SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX")
set_source_files_properties(runtime/kingdom_advisor_render.c PROPERTIES
 SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX")
set_source_files_properties(runtime/world_special_buildings.c PROPERTIES
 SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX")
target_sources(pharaoh_restored PRIVATE runtime/terrain_overlays.c)
set_source_files_properties(runtime/terrain_overlays.c PROPERTIES
 SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX")
set_source_files_properties(runtime/world_detail.c PROPERTIES
 SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX")
set_source_files_properties(runtime/building_stock.c PROPERTIES
 SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX")
set_source_files_properties(runtime/flood_cell.c PROPERTIES
 SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX")
set_source_files_properties(runtime/game_diagnostics.c PROPERTIES
 SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX")
set_source_files_properties(runtime/dynamic_history.c PROPERTIES
 SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX")
set_source_files_properties(runtime/model_loading.c PROPERTIES
 SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX")
target_sources(pharaoh_restored PRIVATE runtime/preview_footprint_seeds.c)
target_sources(pharaoh_restored PRIVATE runtime/monument_constructor.c)
set_source_files_properties(runtime/monument_constructor.c src/unit_00000.c src/unit_00097.c
 src/unit_00193.c src/unit_01991.c src/unit_02561.c src/unit_02615.c src/unit_02602.c src/unit_02589.c
 PROPERTIES SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX")
target_sources(pharaoh_restored PRIVATE runtime/placement_preview_members.c
  runtime/placement_preview_predicates.c runtime/placement_preview_platform.c)
set_source_files_properties(src/unit_00722.c src/unit_00724.c src/unit_00725.c src/unit_00726.c PROPERTIES
 SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX")
set_source_files_properties(src/unit_00180.c PROPERTIES
 SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX")
target_include_directories(pharaoh_restored PUBLIC include)
target_sources(pharaoh_restored PRIVATE runtime/world_route_geometry.c runtime/history_ring_index.c)
set_source_files_properties(runtime/world_route_geometry.c src/unit_00068.c src/unit_01822.c
 src/unit_01869.c src/unit_01870.c src/unit_01871.c src/unit_01872.c
 src/unit_01873.c src/unit_01874.c src/unit_01875.c PROPERTIES
 SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX")
target_precompile_headers(pharaoh_restored PRIVATE include/restore_types.h)
target_compile_options(pharaoh_restored PRIVATE /W0 /Od /bigobj /MP8)
set_target_properties(pharaoh_restored PROPERTIES OUTPUT_NAME pharaoh_restored)
find_package(Python3 COMPONENTS Interpreter REQUIRED)
add_custom_command(TARGET pharaoh_restored POST_BUILD
  COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/place_actor_code.py"
    "$<TARGET_FILE:pharaoh_restored>"
  VERBATIM)
set_source_files_properties(generated/pharaoh_resources.res PROPERTIES EXTERNAL_OBJECT TRUE)
add_executable(restore_link_check runtime/standalone_link_check.c generated/pharaoh_resources.res)
target_link_libraries(restore_link_check PRIVATE pharaoh_restored restore_data kernel32 user32 gdi32 shell32 ole32 oleaut32 advapi32 winmm ddraw dxguid legacy_stdio_definitions)
target_link_options(restore_link_check PRIVATE "/WHOLEARCHIVE:$<TARGET_FILE:pharaoh_restored>" /MAP /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
# Keep normalized member APIs available for offline ABI checks in the existing
# diagnostic. The game can still discard unused public ring wrappers.
foreach(_ring_entry IN ITEMS 004f64a0 004f64b0 004f64d0 004f64e0 004f6500 004f6510 004f6520 004f6560 004f6570 004f65b0 004f65f0 004f6620 004f6630 004f6650 004f6670 004f6690 004f66c0 004f66f0 004f6700 004f6760 004f6790 004f6a50 00451a70 004f68a0 004f68b0 004f68f0 004f6900 004f6950 004f6a10)
 target_link_options(restore_link_check PRIVATE "/INCLUDE:_FUN_${_ring_entry}")
endforeach()
unset(_ring_entry)
foreach(_ring_provider IN ITEMS restore_ring_004f6520 restore_ring_004f6560 restore_ring_004f6570 restore_actor_496140_queue_contains restore_actor_walking_queue_push restore_ring_004f65f0 restore_ring_004f6620 restore_ring_004f6630 restore_actor_walking_queue_count restore_actor_walking_queue_full)
 target_link_options(restore_link_check PRIVATE "/INCLUDE:_${_ring_provider}")
endforeach()
unset(_ring_provider)
foreach(_file_life IN ITEMS 00402360 00402370 00402380 00402390 0041c6d0 0041c6f0 0041c710 0041e050 0041e060 0041e070 0041e080 0041e090 0041e0a0 0041e0c0 00516570 00516580 00516590 005165a0 0056b690 0056b750)
 target_link_options(restore_link_check PRIVATE "/INCLUDE:_FUN_${_file_life}")
endforeach()
unset(_file_life)
foreach(_sprite_life IN ITEMS 0053da50 0053da60 0053da70 0053da80 0053da90 0053db00 0053db60 005404e0 00540500 0056b770 0056b7a0)
 target_link_options(restore_link_check PRIVATE "/INCLUDE:_FUN_${_sprite_life}")
endforeach()
unset(_sprite_life)
foreach(_record_query IN ITEMS 00448a60 00449640 00449660 00449690 0044bb40 0044bd00 0044d680 0044d6d0 0044d700)
 target_link_options(restore_link_check PRIVATE "/INCLUDE:_FUN_${_record_query}")
endforeach()
unset(_record_query)
foreach(_flood_setting IN ITEMS 004bd330 004bd360 004bd380 004bd550 004bd580 004bd590 004bd5d0 004bd5e0 004bed00 004bed40 00562348 004c5120)
 target_link_options(restore_link_check PRIVATE "/INCLUDE:_FUN_${_flood_setting}")
endforeach()
unset(_flood_setting)
foreach(_event_packet IN ITEMS 004449d0 004496d0 0044c0d0 0044d750 0044e6f0 0044e7b0 0044f660)
 target_link_options(restore_link_check PRIVATE "/INCLUDE:_FUN_${_event_packet}")
endforeach()
unset(_event_packet)
target_link_options(restore_link_check PRIVATE "/INCLUDE:_FUN_004487f0" "/INCLUDE:_FUN_00448a90")
target_link_options(restore_link_check PRIVATE "/INCLUDE:_FUN_0044bc20" "/INCLUDE:_FUN_0044c430" "/INCLUDE:_FUN_0044c490" "/INCLUDE:_FUN_0044c960")
foreach(_monument_base IN ITEMS 0040d940 0040ec20 00417050 00417590 004e6710 004e6b20)
 target_link_options(restore_link_check PRIVATE "/INCLUDE:_FUN_${_monument_base}")
endforeach()
unset(_monument_base)
add_executable(restore_game EXCLUDE_FROM_ALL runtime/standalone_link_check.c runtime/game_launcher.c generated/pharaoh_resources.res)
set_source_files_properties(runtime/game_launcher.c PROPERTIES COMPILE_OPTIONS "/W4;/WX")
set_source_files_properties(runtime/media_loader.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX")
set_source_files_properties(src/unit_02688.c src/unit_02693.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX")
set_source_files_properties(src/unit_02713.c src/unit_02714.c src/unit_02715.c src/unit_02716.c
 src/unit_02717.c src/unit_02718.c src/unit_02719.c src/unit_02720.c
 src/unit_02721.c src/unit_02722.c src/unit_02723.c PROPERTIES
 SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX")
set_source_files_properties(src/unit_02482.c src/unit_02704.c src/unit_00237.c src/unit_02537.c
  src/unit_02639.c src/unit_02641.c src/unit_02645.c src/unit_00134.c src/unit_00135.c PROPERTIES
  SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX")
target_compile_definitions(restore_game PRIVATE RESTORE_GAME_LAUNCHER)
target_link_libraries(restore_game PRIVATE pharaoh_restored restore_data kernel32 user32 gdi32 shell32 ole32 oleaut32 advapi32 winmm ddraw dxguid legacy_stdio_definitions)
target_link_options(restore_game PRIVATE "/WHOLEARCHIVE:$<TARGET_FILE:pharaoh_restored>" /MAP /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
set_target_properties(restore_game PROPERTIES OUTPUT_NAME PharaohRestoreC)
option(RESTORE_BUILD_HYBRID "Build the transitional game executable using the user-owned original" OFF)
if(RESTORE_BUILD_HYBRID)
  target_sources(pharaoh_restored PRIVATE runtime/hybrid_allocator.c)
else()
  target_sources(pharaoh_restored PRIVATE runtime/host_allocator.c)
endif()
set(RESTORE_GAME_EXE "" CACHE FILEPATH "Optional canonical original for the hybrid executable")
if(RESTORE_BUILD_HYBRID)
  if(NOT EXISTS "${RESTORE_GAME_EXE}")
    message(FATAL_ERROR "Set RESTORE_GAME_EXE to the canonical original, or disable RESTORE_BUILD_HYBRID for library-only work.")
  endif()
  find_package(Python3 COMPONENTS Interpreter REQUIRED)
  get_filename_component(hybrid_game_directory "${RESTORE_GAME_EXE}" DIRECTORY)
  add_library(PharaohRestoreRuntime SHARED runtime/hybrid_main.c runtime/pe_image.c runtime/runtime.def
    runtime/verify_city_history.c
    runtime/record_geometry.c runtime/verify_geometry.c runtime/memory_strings.c runtime/verify_memory_strings.c
    runtime/core_helpers.c runtime/verify_core_helpers.c runtime/ring_queue.c runtime/verify_ring_queue.c)
  target_include_directories(PharaohRestoreRuntime PRIVATE runtime)
  target_link_libraries(PharaohRestoreRuntime PRIVATE pharaoh_restored bcrypt shell32 user32 gdi32 winmm)
  target_link_options(PharaohRestoreRuntime PRIVATE /MAP /MAPINFO:EXPORTS)
  set(RESTORE_RUNTIME_OUTPUT_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/artifacts/windows" CACHE PATH "Runtime output root; use another directory while a previous game build is open")
  set_target_properties(PharaohRestoreRuntime PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${RESTORE_RUNTIME_OUTPUT_ROOT}/$<CONFIG>")
  target_compile_definitions(PharaohRestoreRuntime PRIVATE _CRT_SECURE_NO_WARNINGS)
  set(hybrid_executable "${RESTORE_RUNTIME_OUTPUT_ROOT}/$<CONFIG>/PharaohRestore.exe")
  add_custom_command(OUTPUT "${hybrid_executable}"
    COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/build_hybrid_image.py"
            "${RESTORE_GAME_EXE}" "${hybrid_executable}"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${hybrid_game_directory}/BINKW32.DLL" "$<TARGET_FILE_DIR:PharaohRestoreRuntime>"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${hybrid_game_directory}/SMACKW32.DLL" "$<TARGET_FILE_DIR:PharaohRestoreRuntime>"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${hybrid_game_directory}/mss32.dll" "$<TARGET_FILE_DIR:PharaohRestoreRuntime>"
    DEPENDS PharaohRestoreRuntime "${RESTORE_GAME_EXE}" tools/build_hybrid_image.py
    VERBATIM)
  add_custom_target(PharaohRestore ALL DEPENDS "${hybrid_executable}")
endif()
enable_testing()
add_test(NAME standalone_whole_program_link COMMAND restore_link_check)
set_tests_properties(standalone_whole_program_link PROPERTIES LABELS source-only)
list(LENGTH RESTORE_UNITS restore_procedure_count)
file(GENERATE OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/source-build.json" CONTENT
  "{\n  \"kind\": \"compiled-C-sources\",\n  \"procedure_sources\": ${restore_procedure_count},\n  \"hybrid_enabled\": \"${RESTORE_BUILD_HYBRID}\",\n  \"playable_standalone_game\": false\n}\n")
message(STATUS "C source build: ${restore_procedure_count} procedures; hybrid=${RESTORE_BUILD_HYBRID}")
add_executable(restore_city_history_verify tests/city_history.c runtime/city_history.c)
target_include_directories(restore_city_history_verify PRIVATE runtime)
add_test(NAME standalone_city_history COMMAND restore_city_history_verify)
add_executable(restore_intrinsics_verify tests/intrinsics.c)
target_include_directories(restore_intrinsics_verify PRIVATE include)
add_test(NAME intrinsic_integer_widths COMMAND restore_intrinsics_verify)
add_executable(restore_classification_verify tests/classification.c)
target_link_libraries(restore_classification_verify PRIVATE pharaoh_restored)
add_test(NAME standalone_classification COMMAND restore_classification_verify)
set_tests_properties(standalone_city_history intrinsic_integer_widths standalone_classification PROPERTIES LABELS source-only)
add_executable(restore_member_calls_verify tests/legacy_member_calls.c runtime/legacy_member_calls.c)
target_include_directories(restore_member_calls_verify PRIVATE include)
add_test(NAME standalone_member_calls COMMAND restore_member_calls_verify)
add_executable(restore_type_info_verify tests/legacy_type_info.c src/unit_02851.c src/unit_02852.c)
target_include_directories(restore_type_info_verify PRIVATE include)
add_test(NAME standalone_type_info COMMAND restore_type_info_verify)
set_tests_properties(standalone_member_calls standalone_type_info PROPERTIES LABELS source-only)
add_executable(restore_media_bindings_verify tests/media_bindings.c runtime/media_bindings.c)
target_include_directories(restore_media_bindings_verify PRIVATE include runtime)
add_test(NAME standalone_media_bindings COMMAND restore_media_bindings_verify)
set_tests_properties(standalone_media_bindings PROPERTIES LABELS source-only)
add_library(restore_data STATIC runtime/standalone_data.c generated/standalone_data.c)
target_include_directories(restore_data PUBLIC include)
add_executable(restore_data_verify tests/standalone_data.c)
target_link_libraries(restore_data_verify PRIVATE restore_data)
target_link_options(restore_data_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_data_layout COMMAND restore_data_verify)
set_tests_properties(standalone_data_layout PROPERTIES LABELS source-only)
add_executable(restore_record_selection_verify tests/record_selection.c src/unit_01600.c)
target_include_directories(restore_record_selection_verify PRIVATE include)
target_link_libraries(restore_record_selection_verify PRIVATE restore_data)
target_link_options(restore_record_selection_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_record_selection COMMAND restore_record_selection_verify)
set_tests_properties(standalone_record_selection PROPERTIES LABELS source-only)
add_executable(restore_city_migration_verify tests/city_record_migration.c src/unit_01802.c runtime/city_migration.c)
target_include_directories(restore_city_migration_verify PRIVATE include)
target_link_libraries(restore_city_migration_verify PRIVATE restore_data)
target_link_options(restore_city_migration_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_city_migration COMMAND restore_city_migration_verify)
set_tests_properties(standalone_city_migration PROPERTIES LABELS source-only)
add_executable(restore_resources_verify tests/resources.c generated/pharaoh_resources.res)
target_include_directories(restore_resources_verify PRIVATE generated)
add_test(NAME standalone_win32_resources COMMAND restore_resources_verify)
set_tests_properties(standalone_win32_resources PROPERTIES LABELS source-only)
add_executable(restore_video_verify tests/video_player.c src/unit_00154.c src/unit_00155.c
  src/unit_00156.c src/unit_00157.c src/unit_00158.c src/unit_00159.c src/unit_00178.c src/unit_00434.c src/unit_01781.c runtime/sound_streams.c)
target_include_directories(restore_video_verify PRIVATE include)
target_link_libraries(restore_video_verify PRIVATE restore_data)
target_link_options(restore_video_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_video_player COMMAND restore_video_verify)
set_tests_properties(standalone_video_player PROPERTIES LABELS source-only)
add_executable(restore_display_verify tests/display_configuration.c src/unit_01775.c src/unit_01777.c runtime/display_configuration.c)
target_include_directories(restore_display_verify PRIVATE include)
target_link_libraries(restore_display_verify PRIVATE restore_data)
target_link_options(restore_display_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_display_configuration COMMAND restore_display_verify)
set_tests_properties(standalone_display_configuration PROPERTIES LABELS source-only)
add_executable(restore_window_verify tests/window_messages.c runtime/window_messages.c runtime/window_entry.c
  runtime/display_configuration.c src/unit_01775.c src/unit_01779.c src/unit_01780.c)
target_include_directories(restore_window_verify PRIVATE include)
target_link_libraries(restore_window_verify PRIVATE restore_data)
target_link_options(restore_window_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_window_messages COMMAND restore_window_verify)
set_tests_properties(standalone_window_messages PROPERTIES LABELS source-only)
add_executable(restore_graphics_verify tests/graphics_session.c
  src/unit_00429.c src/unit_00430.c src/unit_00431.c src/unit_00432.c src/unit_00433.c src/unit_00434.c
  src/unit_01775.c src/unit_01782.c src/unit_01783.c src/unit_01784.c src/unit_01785.c src/unit_01787.c src/unit_01788.c
  runtime/display_configuration.c)
target_include_directories(restore_graphics_verify PRIVATE include)
target_link_libraries(restore_graphics_verify PRIVATE restore_data)
target_link_options(restore_graphics_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_graphics_session COMMAND restore_graphics_verify)
set_tests_properties(standalone_graphics_session PROPERTIES LABELS source-only)
add_executable(restore_effects_verify tests/surface_effects.c src/unit_01786.c src/unit_01789.c
  src/unit_01790.c src/unit_01791.c src/unit_01782.c src/unit_01787.c)
target_include_directories(restore_effects_verify PRIVATE include)
target_link_libraries(restore_effects_verify PRIVATE restore_data)
target_link_options(restore_effects_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_surface_effects COMMAND restore_effects_verify)
set_tests_properties(standalone_surface_effects PROPERTIES LABELS source-only)
add_executable(restore_allocator_verify tests/host_allocator.c runtime/host_allocator.c
  src/unit_02809.c src/unit_02810.c src/unit_02847.c src/unit_02882.c src/unit_02888.c)
target_include_directories(restore_allocator_verify PRIVATE include)
add_test(NAME standalone_host_allocator COMMAND restore_allocator_verify)
set_tests_properties(standalone_host_allocator PROPERTIES LABELS source-only)
add_executable(restore_descriptors_verify tests/file_descriptors.c)
target_include_directories(restore_descriptors_verify PRIVATE include)
target_link_libraries(restore_descriptors_verify PRIVATE pharaoh_restored restore_data)
target_link_options(restore_descriptors_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_file_descriptors COMMAND restore_descriptors_verify)
set_tests_properties(standalone_file_descriptors PROPERTIES LABELS source-only)
add_executable(restore_campaign_verify tests/campaign_index.c src/unit_00213.c src/unit_00253.c src/unit_00256.c src/unit_00258.c)
target_include_directories(restore_campaign_verify PRIVATE include)
target_link_libraries(restore_campaign_verify PRIVATE restore_data)
target_link_options(restore_campaign_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_campaign_index COMMAND restore_campaign_verify)
set_tests_properties(standalone_campaign_index PROPERTIES LABELS source-only)
add_executable(restore_format_verify tests/legacy_format.c src/unit_02805.c)
target_include_directories(restore_format_verify PRIVATE include)
add_test(NAME standalone_legacy_format COMMAND restore_format_verify)
set_tests_properties(standalone_legacy_format PROPERTIES LABELS source-only)
add_executable(restore_text_verify tests/text_selection.c runtime/text_selection.c src/unit_01672.c src/unit_01673.c src/unit_01696.c)
target_include_directories(restore_text_verify PRIVATE include)
target_link_libraries(restore_text_verify PRIVATE restore_data)
target_link_options(restore_text_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_text_selection COMMAND restore_text_verify)
set_tests_properties(standalone_text_selection PROPERTIES LABELS source-only)
add_executable(restore_grid_verify tests/startup_grid.c src/unit_00456.c src/unit_01882.c src/unit_01883.c src/unit_01884.c)
target_include_directories(restore_grid_verify PRIVATE include)
target_link_libraries(restore_grid_verify PRIVATE restore_data)
target_link_options(restore_grid_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_startup_grid COMMAND restore_grid_verify)
set_tests_properties(standalone_startup_grid PROPERTIES LABELS source-only)
add_executable(restore_frame_verify tests/frame_buffer.c runtime/host_allocator.c src/unit_00184.c src/unit_00456.c src/unit_01882.c src/unit_01883.c)
target_include_directories(restore_frame_verify PRIVATE include)
target_link_libraries(restore_frame_verify PRIVATE restore_data)
target_link_options(restore_frame_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_frame_buffer COMMAND restore_frame_verify)
set_tests_properties(standalone_frame_buffer PROPERTIES LABELS source-only)
add_executable(restore_streams_verify tests/legacy_streams.c)
target_include_directories(restore_streams_verify PRIVATE include)
target_link_libraries(restore_streams_verify PRIVATE pharaoh_restored restore_data)
target_link_options(restore_streams_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_legacy_streams COMMAND restore_streams_verify)
set_tests_properties(standalone_legacy_streams PROPERTIES LABELS source-only)
add_executable(restore_sprites_verify tests/sprite_libraries.c runtime/sprite_libraries.c runtime/host_allocator.c
  src/unit_02517.c src/unit_02518.c src/unit_02519.c src/unit_02520.c src/unit_02522.c src/unit_02528.c src/unit_02529.c
  src/unit_01733.c src/unit_01734.c src/unit_02804.c src/unit_02810.c)
target_include_directories(restore_sprites_verify PRIVATE include)
target_link_libraries(restore_sprites_verify PRIVATE restore_data)
target_link_options(restore_sprites_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_sprite_libraries COMMAND restore_sprites_verify)
set_tests_properties(standalone_sprite_libraries PROPERTIES LABELS source-only)
add_executable(restore_render_verify tests/sprite_render.c src/unit_02524.c src/unit_01725.c src/unit_01726.c)
target_include_directories(restore_render_verify PRIVATE include)
target_link_libraries(restore_render_verify PRIVATE restore_data)
target_link_options(restore_render_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_sprite_dispatch COMMAND restore_render_verify)
set_tests_properties(standalone_sprite_dispatch PROPERTIES LABELS source-only)
add_executable(restore_pixels_verify tests/sprite_pixels.c runtime/sprite_pixels.c src/unit_00128.c src/unit_00136.c src/unit_00137.c src/unit_00140.c src/unit_00146.c src/unit_00149.c src/unit_00134.c src/unit_00135.c)
target_include_directories(restore_pixels_verify PRIVATE include)
target_link_libraries(restore_pixels_verify PRIVATE restore_data)
target_link_options(restore_pixels_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_sprite_pixels COMMAND restore_pixels_verify)
set_tests_properties(standalone_sprite_pixels PROPERTIES LABELS source-only)
add_executable(restore_campaign_parser_verify tests/campaign_parser.c src/unit_00241.c src/unit_00242.c src/unit_00243.c src/unit_00245.c src/unit_00246.c src/unit_00247.c src/unit_00248.c src/unit_00249.c src/unit_00253.c src/unit_02815.c)
target_include_directories(restore_campaign_parser_verify PRIVATE include)
target_link_libraries(restore_campaign_parser_verify PRIVATE restore_data)
target_link_options(restore_campaign_parser_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_campaign_parser COMMAND restore_campaign_parser_verify)
set_tests_properties(standalone_campaign_parser PROPERTIES LABELS source-only)
add_executable(restore_sound_verify tests/sound_initialization.c src/unit_02689.c src/unit_02691.c src/unit_02692.c src/unit_02687.c src/unit_02697.c src/unit_02685.c)
target_include_directories(restore_sound_verify PRIVATE include)
target_link_libraries(restore_sound_verify PRIVATE restore_data)
target_link_options(restore_sound_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_sound_initialization COMMAND restore_sound_verify)
set_tests_properties(standalone_sound_initialization PROPERTIES LABELS source-only)
add_executable(restore_music_verify tests/music_manager.c src/unit_02473.c src/unit_02475.c src/unit_02476.c src/unit_02477.c src/unit_02479.c src/unit_02480.c src/unit_02815.c)
target_include_directories(restore_music_verify PRIVATE include)
target_link_libraries(restore_music_verify PRIVATE restore_data)
target_link_options(restore_music_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_music_manager COMMAND restore_music_verify)
set_tests_properties(standalone_music_manager PROPERTIES LABELS source-only)
add_executable(restore_sound_files_verify tests/sound_file_lookup.c src/unit_02686.c)
target_include_directories(restore_sound_files_verify PRIVATE include)
target_link_options(restore_sound_files_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_sound_file_lookup COMMAND restore_sound_files_verify)
set_tests_properties(standalone_sound_file_lookup PROPERTIES LABELS source-only)
add_executable(restore_startup_geometry_verify tests/startup_geometry.c src/unit_01965.c src/unit_01968.c src/unit_02551.c src/unit_01896.c src/unit_01895.c
 src/unit_00002.c src/unit_00099.c src/unit_00195.c src/unit_01993.c src/unit_02563.c src/unit_02617.c src/unit_02604.c src/unit_02591.c)
target_include_directories(restore_startup_geometry_verify PRIVATE include)
target_link_libraries(restore_startup_geometry_verify PRIVATE restore_data)
target_link_options(restore_startup_geometry_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_startup_geometry COMMAND restore_startup_geometry_verify)
set_tests_properties(standalone_startup_geometry PROPERTIES LABELS source-only)
add_executable(restore_directory_verify tests/directory_changes.c src/unit_03028.c)
add_test(NAME standalone_directory_changes COMMAND restore_directory_verify)
set_tests_properties(standalone_directory_changes PROPERTIES LABELS source-only)
add_executable(restore_sound_streams_verify tests/sound_streams.c src/unit_02707.c src/unit_02708.c src/unit_02709.c src/unit_02710.c src/unit_02711.c runtime/sound_streams.c)
target_include_directories(restore_sound_streams_verify PRIVATE include)
add_test(NAME standalone_sound_streams COMMAND restore_sound_streams_verify)
set_tests_properties(standalone_sound_streams PROPERTIES LABELS source-only)
add_executable(restore_tick_timer_verify tests/tick_timer.c src/unit_01876.c)
target_include_directories(restore_tick_timer_verify PRIVATE include)
target_link_libraries(restore_tick_timer_verify PRIVATE restore_data)
target_link_options(restore_tick_timer_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_tick_timer COMMAND restore_tick_timer_verify)
set_tests_properties(standalone_tick_timer PROPERTIES LABELS source-only)
add_executable(restore_sound_samples_verify tests/sound_samples.c src/unit_02691.c src/unit_02698.c src/unit_02699.c src/unit_02701.c src/unit_02702.c src/unit_02703.c)
target_include_directories(restore_sound_samples_verify PRIVATE include)
add_test(NAME standalone_sound_samples COMMAND restore_sound_samples_verify)
set_tests_properties(standalone_sound_samples PROPERTIES LABELS source-only)
add_executable(restore_menu_render_verify tests/menu_render.c src/unit_00219.c src/unit_00400.c src/unit_00401.c src/unit_02444.c src/unit_02445.c src/unit_02447.c src/unit_01667.c src/unit_01693.c)
target_include_directories(restore_menu_render_verify PRIVATE include)
target_link_libraries(restore_menu_render_verify PRIVATE restore_data)
target_link_options(restore_menu_render_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_menu_render COMMAND restore_menu_render_verify)
set_tests_properties(standalone_menu_render PROPERTIES LABELS source-only)
add_executable(restore_menu_input_verify tests/menu_input.c src/unit_00416.c src/unit_00417.c
  src/unit_01768.c src/unit_01730.c src/unit_01731.c src/unit_01836.c src/unit_01857.c src/unit_01858.c runtime/menu_actions.c)
target_include_directories(restore_menu_input_verify PRIVATE include)
target_link_libraries(restore_menu_input_verify PRIVATE pharaoh_restored restore_data kernel32 user32 gdi32 shell32 ole32 oleaut32 advapi32 winmm ddraw dxguid legacy_stdio_definitions)
target_link_options(restore_menu_input_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_menu_input COMMAND restore_menu_input_verify)
set_tests_properties(standalone_menu_input PROPERTIES LABELS source-only)
add_executable(restore_file_list_verify tests/file_list.c src/unit_00269.c src/unit_00270.c src/unit_00271.c src/unit_00287.c
  src/unit_02820.c src/unit_02821.c src/unit_02822.c)
target_include_directories(restore_file_list_verify PRIVATE include)
target_link_libraries(restore_file_list_verify PRIVATE restore_data)
target_link_options(restore_file_list_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_file_list COMMAND restore_file_list_verify)
set_tests_properties(standalone_file_list PROPERTIES LABELS source-only)
add_executable(restore_menu_layout_verify tests/menu_layout.c src/unit_01836.c src/unit_01837.c src/unit_01838.c src/unit_01839.c
 src/unit_01840.c src/unit_01841.c src/unit_01842.c src/unit_01844.c src/unit_01845.c src/unit_01846.c src/unit_01848.c
 src/unit_01849.c src/unit_01851.c src/unit_01852.c src/unit_01854.c src/unit_01855.c src/unit_01856.c src/unit_01859.c src/unit_02456.c)
target_include_directories(restore_menu_layout_verify PRIVATE include)
target_link_libraries(restore_menu_layout_verify PRIVATE restore_data)
target_link_options(restore_menu_layout_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_menu_layout COMMAND restore_menu_layout_verify)
set_tests_properties(standalone_menu_layout PROPERTIES LABELS source-only)
add_executable(restore_campaign_profile_verify tests/campaign_profile.c src/unit_00241.c src/unit_00242.c src/unit_00250.c src/unit_00251.c)
target_include_directories(restore_campaign_profile_verify PRIVATE include)
target_link_libraries(restore_campaign_profile_verify PRIVATE restore_data)
target_link_options(restore_campaign_profile_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_campaign_profile COMMAND restore_campaign_profile_verify)
set_tests_properties(standalone_campaign_profile PROPERTIES LABELS source-only)
add_executable(restore_family_menu_verify tests/family_menu.c runtime/family_menu.c src/unit_00244.c src/unit_00418.c
  src/unit_02279.c src/unit_02280.c src/unit_02281.c src/unit_02282.c src/unit_02283.c src/unit_02284.c src/unit_02286.c src/unit_02287.c
  src/unit_01672.c runtime/text_selection.c)
target_include_directories(restore_family_menu_verify PRIVATE include)
target_link_libraries(restore_family_menu_verify PRIVATE restore_data)
target_link_options(restore_family_menu_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_family_menu COMMAND restore_family_menu_verify)
set_tests_properties(standalone_family_menu PROPERTIES LABELS source-only)
add_executable(restore_text_templates_verify tests/text_templates.c src/unit_00674.c src/unit_00675.c src/unit_01656.c src/unit_01659.c src/unit_03027.c)
target_include_directories(restore_text_templates_verify PRIVATE include)
target_link_libraries(restore_text_templates_verify PRIVATE restore_data)
target_link_options(restore_text_templates_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_text_templates COMMAND restore_text_templates_verify)
set_tests_properties(standalone_text_templates PROPERTIES LABELS source-only)
add_executable(restore_family_render_verify tests/family_render.c src/unit_01652.c src/unit_01653.c src/unit_01706.c src/unit_01708.c src/unit_01712.c src/unit_02276.c src/unit_02277.c src/unit_02278.c)
target_include_directories(restore_family_render_verify PRIVATE include)
target_link_libraries(restore_family_render_verify PRIVATE restore_data)
target_link_options(restore_family_render_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_family_render COMMAND restore_family_render_verify)
set_tests_properties(standalone_family_render PROPERTIES LABELS source-only)
add_executable(restore_menu_dialog_verify tests/menu_dialog.c runtime/menu_dialog.c runtime/text_selection.c src/unit_00398.c src/unit_00399.c src/unit_00404.c src/unit_00405.c src/unit_00406.c src/unit_00418.c src/unit_00419.c src/unit_01690.c)
target_include_directories(restore_menu_dialog_verify PRIVATE include)
target_link_libraries(restore_menu_dialog_verify PRIVATE restore_data)
target_link_options(restore_menu_dialog_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_menu_dialog COMMAND restore_menu_dialog_verify)
set_tests_properties(standalone_menu_dialog PROPERTIES LABELS source-only)
add_executable(restore_family_delete_verify tests/family_delete.c runtime/family_delete.c src/unit_02282.c src/unit_02283.c)
target_include_directories(restore_family_delete_verify PRIVATE include)
target_link_libraries(restore_family_delete_verify PRIVATE restore_data)
target_link_options(restore_family_delete_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_family_delete COMMAND restore_family_delete_verify)
set_tests_properties(standalone_family_delete PROPERTIES LABELS source-only)
add_executable(restore_text_wrap_verify tests/text_wrap.c src/unit_01669.c src/unit_01703.c src/unit_01704.c src/unit_01705.c)
target_include_directories(restore_text_wrap_verify PRIVATE include)
target_link_libraries(restore_text_wrap_verify PRIVATE restore_data)
target_link_options(restore_text_wrap_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_text_wrap COMMAND restore_text_wrap_verify)
set_tests_properties(standalone_text_wrap PROPERTIES LABELS source-only)
add_executable(restore_family_options_verify tests/family_options.c runtime/family_options.c src/unit_00257.c src/unit_01655.c)
target_include_directories(restore_family_options_verify PRIVATE include)
target_link_libraries(restore_family_options_verify PRIVATE restore_data)
target_link_options(restore_family_options_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_family_options COMMAND restore_family_options_verify)
set_tests_properties(standalone_family_options PROPERTIES LABELS source-only)
add_executable(restore_campaign_menu_verify tests/campaign_menu.c runtime/campaign_menu.c src/unit_00255.c src/unit_00259.c src/unit_00261.c src/unit_00262.c src/unit_00263.c src/unit_00308.c)
target_include_directories(restore_campaign_menu_verify PRIVATE include)
target_link_libraries(restore_campaign_menu_verify PRIVATE restore_data)
target_link_options(restore_campaign_menu_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_campaign_menu COMMAND restore_campaign_menu_verify)
set_tests_properties(standalone_campaign_menu PROPERTIES LABELS source-only)
add_executable(restore_mission_menu_verify tests/mission_menu.c runtime/mission_menu.c
  src/unit_00284.c src/unit_00285.c src/unit_00286.c src/unit_00287.c
  src/unit_00293.c src/unit_00294.c src/unit_00295.c src/unit_00296.c src/unit_00297.c src/unit_00298.c src/unit_00299.c
  src/unit_00302.c src/unit_00303.c src/unit_00304.c src/unit_00305.c src/unit_00306.c src/unit_00307.c src/unit_00309.c src/unit_00310.c src/unit_00311.c)
target_include_directories(restore_mission_menu_verify PRIVATE include)
target_link_libraries(restore_mission_menu_verify PRIVATE restore_data)
target_link_options(restore_mission_menu_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_mission_menu COMMAND restore_mission_menu_verify)
set_tests_properties(standalone_mission_menu PROPERTIES LABELS source-only)
add_executable(restore_town_menu_verify tests/town_menu.c runtime/town_menu.c src/unit_00301.c src/unit_00720.c)
target_include_directories(restore_town_menu_verify PRIVATE include)
target_link_libraries(restore_town_menu_verify PRIVATE restore_data)
target_link_options(restore_town_menu_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_town_menu COMMAND restore_town_menu_verify)
set_tests_properties(standalone_town_menu PROPERTIES LABELS source-only)
add_executable(restore_compression_verify tests/compression.c runtime/compression.c src/unit_02640.c src/unit_02642.c src/unit_02644.c
  src/unit_02641.c src/unit_02645.c src/unit_02646.c src/unit_02647.c src/unit_02648.c src/unit_02649.c src/unit_02650.c src/unit_02651.c
  src/unit_02652.c src/unit_02653.c src/unit_02654.c src/unit_02655.c src/unit_02656.c src/unit_02657.c src/unit_02658.c src/unit_02659.c)
target_include_directories(restore_compression_verify PRIVATE include)
target_link_libraries(restore_compression_verify PRIVATE restore_data)
target_link_options(restore_compression_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_compression COMMAND restore_compression_verify)
set_tests_properties(standalone_compression PROPERTIES LABELS source-only)
add_executable(restore_chunk_io_verify tests/chunk_io.c runtime/chunk_migrations.c src/unit_01793.c)
target_include_directories(restore_chunk_io_verify PRIVATE include)
target_link_libraries(restore_chunk_io_verify PRIVATE restore_data)
target_link_options(restore_chunk_io_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_chunk_io COMMAND restore_chunk_io_verify)
set_tests_properties(standalone_chunk_io PROPERTIES LABELS source-only)
add_executable(restore_mission_chunks_verify tests/mission_chunks.c runtime/compression.c runtime/chunk_migrations.c src/unit_01793.c
  src/unit_02640.c src/unit_02642.c src/unit_02644.c src/unit_02652.c src/unit_02653.c src/unit_02654.c
  src/unit_02655.c src/unit_02656.c src/unit_02657.c src/unit_02658.c src/unit_02659.c)
target_include_directories(restore_mission_chunks_verify PRIVATE include)
target_link_libraries(restore_mission_chunks_verify PRIVATE restore_data advapi32)
target_link_options(restore_mission_chunks_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_mission_chunks COMMAND restore_mission_chunks_verify)
set_tests_properties(standalone_mission_chunks PROPERTIES LABELS source-only)
add_executable(restore_mission_routes_verify tests/mission_routes.c src/unit_01822.c)
target_include_directories(restore_mission_routes_verify PRIVATE include)
target_link_libraries(restore_mission_routes_verify PRIVATE restore_data)
target_link_options(restore_mission_routes_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_mission_routes COMMAND restore_mission_routes_verify)
set_tests_properties(standalone_mission_routes PROPERTIES LABELS source-only)
add_executable(restore_mission_loaders_verify tests/mission_loaders.c src/unit_01539.c src/unit_01541.c src/unit_01542.c src/unit_01543.c
  src/unit_01821.c src/unit_01823.c src/unit_01830.c src/unit_01831.c)
target_include_directories(restore_mission_loaders_verify PRIVATE include)
target_link_libraries(restore_mission_loaders_verify PRIVATE restore_data)
target_link_options(restore_mission_loaders_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_mission_loaders COMMAND restore_mission_loaders_verify)
set_tests_properties(standalone_mission_loaders PROPERTIES LABELS source-only)
add_executable(restore_mission_map_verify tests/mission_map.c runtime/compression.c runtime/chunk_migrations.c src/unit_01542.c
  src/unit_01793.c src/unit_01822.c src/unit_01823.c src/unit_02640.c src/unit_02642.c src/unit_02644.c src/unit_02652.c
  src/unit_02653.c src/unit_02654.c src/unit_02655.c src/unit_02656.c src/unit_02657.c src/unit_02658.c src/unit_02659.c)
target_include_directories(restore_mission_map_verify PRIVATE include)
target_link_libraries(restore_mission_map_verify PRIVATE restore_data advapi32)
target_link_options(restore_mission_map_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_mission_map COMMAND restore_mission_map_verify)
set_tests_properties(standalone_mission_map PROPERTIES LABELS source-only)
add_executable(restore_numeric_text_verify tests/numeric_text.c src/unit_01689.c src/unit_01697.c src/unit_01698.c
  src/unit_01700.c src/unit_01701.c src/unit_01702.c)
target_include_directories(restore_numeric_text_verify PRIVATE include)
target_link_libraries(restore_numeric_text_verify PRIVATE restore_data)
target_link_options(restore_numeric_text_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_numeric_text COMMAND restore_numeric_text_verify)
set_tests_properties(standalone_numeric_text PROPERTIES LABELS source-only)
add_executable(restore_legacy_mission_verify tests/legacy_mission.c src/unit_00695.c src/unit_01602.c src/unit_01792.c
  src/unit_01800.c src/unit_01801.c src/unit_01807.c src/unit_01809.c src/unit_01816.c src/unit_01817.c src/unit_01818.c
  src/unit_01819.c src/unit_01820.c src/unit_02199.c src/unit_02200.c)
target_include_directories(restore_legacy_mission_verify PRIVATE include)
target_link_libraries(restore_legacy_mission_verify PRIVATE restore_data advapi32)
target_link_options(restore_legacy_mission_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_legacy_mission COMMAND restore_legacy_mission_verify)
set_tests_properties(standalone_legacy_mission PROPERTIES LABELS source-only)
add_executable(restore_mission_save_verify tests/mission_save.c runtime/compression.c runtime/chunk_migrations.c
  src/unit_00316.c src/unit_00338.c src/unit_00345.c src/unit_00352.c src/unit_00359.c src/unit_00366.c
  src/unit_00695.c src/unit_01542.c src/unit_01602.c src/unit_01792.c src/unit_01793.c
  src/unit_01800.c src/unit_01801.c src/unit_01807.c src/unit_01809.c src/unit_01811.c src/unit_01815.c
  src/unit_01816.c src/unit_01817.c src/unit_01818.c src/unit_01819.c src/unit_01820.c src/unit_01821.c src/unit_01822.c
  src/unit_02199.c src/unit_02200.c src/unit_02640.c src/unit_02642.c src/unit_02644.c src/unit_02652.c
  src/unit_02653.c src/unit_02654.c src/unit_02655.c src/unit_02656.c src/unit_02657.c src/unit_02658.c src/unit_02659.c)
target_include_directories(restore_mission_save_verify PRIVATE include)
foreach(_world_route_fixture IN ITEMS restore_mission_routes_verify restore_mission_map_verify restore_mission_save_verify)
  target_sources(${_world_route_fixture} PRIVATE runtime/world_route_geometry.c src/unit_00068.c
    runtime/menu_full_input_routes.c runtime/menu_full_input_numeric.c runtime/menu_route_capacity.c)
endforeach()
foreach(_history_fixture IN ITEMS restore_mission_save_verify restore_legacy_mission_verify)
  target_sources(${_history_fixture} PRIVATE runtime/dynamic_history.c runtime/history_ring_index.c)
endforeach()
target_link_libraries(restore_mission_save_verify PRIVATE restore_data advapi32)
target_link_options(restore_mission_save_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_mission_save COMMAND restore_mission_save_verify)
set_tests_properties(standalone_mission_save PROPERTIES LABELS source-only)
add_executable(restore_load_save_menu_verify tests/load_save_menu.c runtime/load_save_menu.c src/unit_00020.c
  src/unit_00021.c src/unit_00022.c src/unit_00023.c src/unit_00287.c src/unit_01739.c src/unit_01745.c src/unit_01746.c
  src/unit_02427.c src/unit_02428.c src/unit_02429.c)
target_include_directories(restore_load_save_menu_verify PRIVATE include)
target_link_libraries(restore_load_save_menu_verify PRIVATE restore_data)
target_link_options(restore_load_save_menu_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_load_save_menu COMMAND restore_load_save_menu_verify)
set_tests_properties(standalone_load_save_menu PROPERTIES LABELS source-only)
add_executable(restore_city_prepare_verify tests/city_prepare.c runtime/city_history.c src/unit_00934.c src/unit_01009.c src/unit_01011.c src/unit_01013.c
  src/unit_01014.c src/unit_01015.c src/unit_01026.c src/unit_01036.c src/unit_01049.c src/unit_01050.c src/unit_01235.c
  src/unit_01542.c src/unit_01550.c src/unit_01552.c src/unit_01553.c src/unit_01557.c src/unit_01558.c src/unit_01826.c src/unit_01828.c
  src/unit_01582.c src/unit_01731.c src/unit_02023.c src/unit_02046.c src/unit_02047.c)
target_include_directories(restore_city_prepare_verify PRIVATE include)
target_link_libraries(restore_city_prepare_verify PRIVATE restore_data advapi32)
target_link_options(restore_city_prepare_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_city_prepare COMMAND restore_city_prepare_verify)
set_tests_properties(standalone_city_prepare PROPERTIES LABELS source-only)
add_executable(restore_terrain_update_verify tests/terrain_update.c src/unit_01009.c src/unit_01011.c
  src/unit_01037.c src/unit_01091.c src/unit_01097.c src/unit_01098.c src/unit_01893.c)
target_include_directories(restore_terrain_update_verify PRIVATE include)
target_link_libraries(restore_terrain_update_verify PRIVATE restore_data advapi32)
target_link_options(restore_terrain_update_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_terrain_update COMMAND restore_terrain_update_verify)
set_tests_properties(standalone_terrain_update PROPERTIES LABELS source-only)
add_executable(restore_terrain_redraw_verify tests/terrain_redraw.c src/unit_01009.c src/unit_01011.c src/unit_01012.c
  src/unit_01043.c src/unit_01117.c src/unit_01122.c src/unit_01137.c src/unit_01238.c)
target_include_directories(restore_terrain_redraw_verify PRIVATE include)
target_link_libraries(restore_terrain_redraw_verify PRIVATE restore_data advapi32)
target_link_options(restore_terrain_redraw_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_terrain_redraw COMMAND restore_terrain_redraw_verify)
set_tests_properties(standalone_terrain_redraw PROPERTIES LABELS source-only)
add_executable(restore_city_prices_verify tests/city_prices.c src/unit_00617.c src/unit_00618.c
  src/unit_00718.c src/unit_00741.c src/unit_02632.c)
target_include_directories(restore_city_prices_verify PRIVATE include)
target_link_libraries(restore_city_prices_verify PRIVATE restore_data advapi32)
target_link_options(restore_city_prices_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_city_prices COMMAND restore_city_prices_verify)
set_tests_properties(standalone_city_prices PROPERTIES LABELS source-only)
add_executable(restore_music_tick_verify tests/music_tick.c src/unit_02477.c src/unit_02478.c
  src/unit_02480.c tests/music_tick_streams.c src/unit_02825.c)
target_include_directories(restore_music_tick_verify PRIVATE include)
target_link_libraries(restore_music_tick_verify PRIVATE restore_data advapi32)
target_link_options(restore_music_tick_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
target_compile_options(restore_music_tick_verify PRIVATE /Gy)
target_link_options(restore_music_tick_verify PRIVATE /OPT:REF /INCREMENTAL:NO)
add_test(NAME standalone_music_tick COMMAND restore_music_tick_verify)
set_tests_properties(standalone_music_tick PROPERTIES LABELS source-only)
add_executable(restore_flood_simulation_verify tests/flood_simulation.c runtime/flood_simulation.c
  src/unit_01544.c src/unit_01545.c src/unit_01546.c src/unit_01547.c src/unit_01554.c
  src/unit_01556.c src/unit_01559.c src/unit_01561.c src/unit_01563.c)
target_include_directories(restore_flood_simulation_verify PRIVATE include)
target_link_libraries(restore_flood_simulation_verify PRIVATE restore_data advapi32)
target_link_options(restore_flood_simulation_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_flood_simulation COMMAND restore_flood_simulation_verify)
set_tests_properties(standalone_flood_simulation PROPERTIES LABELS source-only)
foreach(_flood_fields_target IN ITEMS restore_mission_loaders_verify restore_mission_map_verify restore_mission_save_verify restore_city_prepare_verify restore_flood_simulation_verify)
 target_sources(${_flood_fields_target} PRIVATE runtime/flood_settings.c)
endforeach()
unset(_flood_fields_target)
target_sources(restore_flood_simulation_verify PRIVATE runtime/flood_quality.c)
target_sources(restore_mission_loaders_verify PRIVATE runtime/game_diagnostics.c)
add_executable(restore_actor_update_verify tests/actor_update.c src/unit_01307.c)
target_include_directories(restore_actor_update_verify PRIVATE include)
target_link_libraries(restore_actor_update_verify PRIVATE restore_data advapi32)
target_link_options(restore_actor_update_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_actor_update COMMAND restore_actor_update_verify)
set_tests_properties(standalone_actor_update PROPERTIES LABELS source-only)
add_executable(restore_actor_prey_verify tests/actor_prey.c tests/actor_callback_unrelated.c runtime/actor_prey.c runtime/actor_callbacks.c)
target_include_directories(restore_actor_prey_verify PRIVATE include)
target_link_libraries(restore_actor_prey_verify PRIVATE restore_data advapi32)
target_link_options(restore_actor_prey_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_actor_prey COMMAND restore_actor_prey_verify)
set_tests_properties(standalone_actor_prey PROPERTIES LABELS source-only)
add_executable(restore_animation_update_verify tests/animation_update.c runtime/animation_state.c
  src/unit_00316.c src/unit_00318.c src/unit_00319.c src/unit_00320.c src/unit_00321.c
  src/unit_00339.c src/unit_00346.c src/unit_00353.c src/unit_00360.c src/unit_00367.c src/unit_01977.c src/unit_01979.c)
target_include_directories(restore_animation_update_verify PRIVATE include)
target_link_libraries(restore_animation_update_verify PRIVATE restore_data advapi32)
target_link_options(restore_animation_update_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_animation_update COMMAND restore_animation_update_verify)
set_tests_properties(standalone_animation_update PROPERTIES LABELS source-only)
add_executable(restore_minimap_verify tests/minimap.c src/unit_00129.c src/unit_00130.c src/unit_00563.c
  src/unit_00576.c src/unit_00577.c src/unit_00578.c src/unit_00579.c src/unit_00580.c src/unit_00581.c src/unit_02066.c src/unit_02526.c)
target_include_directories(restore_minimap_verify PRIVATE include)
target_link_libraries(restore_minimap_verify PRIVATE restore_data advapi32)
target_link_options(restore_minimap_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_minimap COMMAND restore_minimap_verify)
set_tests_properties(standalone_minimap PROPERTIES LABELS source-only)
add_library(restore_graphics_53f250_clip OBJECT src/unit_01725.c src/unit_01726.c)
target_include_directories(restore_graphics_53f250_clip PRIVATE include)
target_compile_definitions(restore_graphics_53f250_clip PRIVATE
  FUN_004cdad0=graphics_53f250_hclip_impl FUN_004cdb80=graphics_53f250_vclip_impl)
add_executable(restore_graphics_53f250_verify tests/graphics_53f250.c src/unit_02525.c $<TARGET_OBJECTS:restore_graphics_53f250_clip>)
target_include_directories(restore_graphics_53f250_verify PRIVATE include)
target_link_libraries(restore_graphics_53f250_verify PRIVATE restore_data)
target_link_options(restore_graphics_53f250_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_graphics_53f250 COMMAND restore_graphics_53f250_verify)
set_tests_properties(standalone_graphics_53f250 PROPERTIES LABELS source-only)
foreach(actor_variant IN ITEMS restore_actor_predicate_verify restore_actor_predicate_plain_verify)
  add_executable(${actor_variant} tests/actor_predicate.c tests/actor_predicate_helpers.c src/unit_00560.c)
  target_include_directories(${actor_variant} PRIVATE include)
  target_compile_definitions(${actor_variant} PRIVATE _CRT_SECURE_NO_WARNINGS)
  target_link_libraries(${actor_variant} PRIVATE restore_data advapi32)
  target_link_options(${actor_variant} PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
endforeach()

target_sources(pharaoh_restored PRIVATE runtime/dropdown_system_actions.c)
add_executable(restore_dropdown_system_actions_verify tests/dropdown_system_actions.c
  runtime/dropdown_system_actions.c src/unit_01619.c src/unit_01628.c src/unit_01631.c)
target_include_directories(restore_dropdown_system_actions_verify PRIVATE include)
target_compile_definitions(restore_dropdown_system_actions_verify PRIVATE _CRT_SECURE_NO_WARNINGS)
target_compile_options(restore_dropdown_system_actions_verify PRIVATE /W4 "$<$<CONFIG:Debug>:/Od>" "$<$<CONFIG:Release>:/O2>")
target_link_libraries(restore_dropdown_system_actions_verify PRIVATE pharaoh_restored restore_data kernel32 user32 gdi32 shell32 ole32 oleaut32 advapi32 winmm ddraw dxguid legacy_stdio_definitions)
target_link_options(restore_dropdown_system_actions_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_dropdown_system_actions COMMAND restore_dropdown_system_actions_verify)
set_tests_properties(standalone_dropdown_system_actions PROPERTIES LABELS source-only)

target_sources(pharaoh_restored PRIVATE runtime/dropdown_start_actions.c)
add_executable(restore_dropdown_start_actions_verify tests/dropdown_start_actions.c
  tests/dropdown_start_actions_real.c tests/dropdown_start_map_real.c src/unit_01809.c)
target_include_directories(restore_dropdown_start_actions_verify PRIVATE include)
target_compile_definitions(restore_dropdown_start_actions_verify PRIVATE _CRT_SECURE_NO_WARNINGS)
target_compile_options(restore_dropdown_start_actions_verify PRIVATE /W4 "$<$<CONFIG:Debug>:/Od>" "$<$<CONFIG:Release>:/O2>")
target_link_libraries(restore_dropdown_start_actions_verify PRIVATE pharaoh_restored restore_data kernel32 user32 gdi32 shell32 ole32 oleaut32 advapi32 winmm ddraw dxguid legacy_stdio_definitions)
target_link_options(restore_dropdown_start_actions_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_dropdown_start_actions COMMAND restore_dropdown_start_actions_verify)
set_tests_properties(standalone_dropdown_start_actions PROPERTIES LABELS source-only)

target_sources(pharaoh_restored PRIVATE runtime/settings_callbacks.c
  runtime/settings_callbacks_audio.c runtime/remaining_menu_actions.c)
add_executable(restore_settings_callbacks_verify
  artifacts/settings_callbacks_phase/tests/settings_callbacks.c
  artifacts/settings_callbacks_phase/tests/settings_actual_trace.c
  artifacts/settings_callbacks_phase/tests/settings_actual_plain.c)
target_include_directories(restore_settings_callbacks_verify PRIVATE include artifacts/settings_callbacks_phase/tests)
target_compile_definitions(restore_settings_callbacks_verify PRIVATE _CRT_SECURE_NO_WARNINGS)
target_compile_options(restore_settings_callbacks_verify PRIVATE /W4 "$<$<CONFIG:Debug>:/Od>" "$<$<CONFIG:Release>:/O2>")
target_link_libraries(restore_settings_callbacks_verify PRIVATE restore_data advapi32)
target_link_options(restore_settings_callbacks_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_settings_callbacks COMMAND restore_settings_callbacks_verify)
set_tests_properties(standalone_settings_callbacks PROPERTIES LABELS source-only)
include("${CMAKE_CURRENT_SOURCE_DIR}/artifacts/remaining_menu_actions_phase/shared_targets.cmake")
include("${CMAKE_CURRENT_SOURCE_DIR}/artifacts/remaining_menu_click_phase/shared_targets.cmake")
target_sources(pharaoh_restored PRIVATE runtime/selection_popup.c)
add_executable(restore_selection_popup_verify tests/selection_popup.c runtime/selection_popup.c
  src/unit_00033.c src/unit_01858.c src/unit_02421.c src/unit_00416.c src/unit_00417.c
  src/unit_01836.c src/unit_01857.c)
target_include_directories(restore_selection_popup_verify PRIVATE include)
target_compile_definitions(restore_selection_popup_verify PRIVATE _CRT_SECURE_NO_WARNINGS)
target_compile_options(restore_selection_popup_verify PRIVATE /W4 /WX "$<$<CONFIG:Debug>:/Od>" "$<$<CONFIG:Release>:/O2>")
target_link_libraries(restore_selection_popup_verify PRIVATE restore_data)
target_link_options(restore_selection_popup_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_selection_popup COMMAND restore_selection_popup_verify)
set_tests_properties(standalone_selection_popup PROPERTIES LABELS source-only)
# The frozen private recipe linked both configurations to a Debug library.
# Shared targets instead match the actual root Debug/Release CRT settings.
foreach(remaining_click_target IN ITEMS restore_remaining_menu_click_verify restore_remaining_menu_click_plain_verify)
  set_property(TARGET ${remaining_click_target} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
  set_tests_properties(${remaining_click_target} PROPERTIES LABELS source-only)
endforeach()
target_compile_definitions(restore_actor_predicate_verify PRIVATE RESTORE_ACTOR_PREDICATE_TRACE_READS)
add_test(NAME standalone_actor_predicate COMMAND restore_actor_predicate_verify)
add_test(NAME standalone_actor_predicate_plain COMMAND restore_actor_predicate_plain_verify)
set_tests_properties(standalone_actor_predicate standalone_actor_predicate_plain PROPERTIES LABELS source-only)
add_executable(restore_high_scores_menu_verify tests/high_scores_menu.c runtime/high_scores_menu.c runtime/menu_actions.c
  src/unit_01610.c src/unit_01611.c src/unit_01612.c src/unit_01613.c src/unit_01614.c src/unit_01616.c src/unit_01730.c src/unit_01731.c)
target_include_directories(restore_high_scores_menu_verify PRIVATE include)
target_link_libraries(restore_high_scores_menu_verify PRIVATE restore_data)
target_link_options(restore_high_scores_menu_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_high_scores_menu COMMAND restore_high_scores_menu_verify)
set_tests_properties(standalone_high_scores_menu PROPERTIES LABELS source-only)
foreach(viewport_variant IN ITEMS restore_viewport_controller_verify restore_viewport_controller_plain_verify)
  add_executable(${viewport_variant} tests/viewport_controller.c tests/viewport_controller_empty_callback.c src/unit_00468.c src/unit_01878.c)
  target_include_directories(${viewport_variant} PRIVATE include)
  target_compile_definitions(${viewport_variant} PRIVATE _CRT_SECURE_NO_WARNINGS)
  target_link_libraries(${viewport_variant} PRIVATE restore_data advapi32)
  target_link_options(${viewport_variant} PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
endforeach()
target_compile_definitions(restore_viewport_controller_verify PRIVATE RESTORE_VIEWPORT_CONTROLLER_TRACE)
add_test(NAME standalone_viewport_controller COMMAND restore_viewport_controller_verify)
add_test(NAME standalone_viewport_controller_plain COMMAND restore_viewport_controller_plain_verify)
set_tests_properties(standalone_viewport_controller standalone_viewport_controller_plain PROPERTIES LABELS source-only)
add_executable(restore_city_ui_verify tests/city_ui_controllers.c
  src/unit_02344.c src/unit_02345.c src/unit_02346.c src/unit_02347.c src/unit_02348.c
  src/unit_02349.c src/unit_02350.c src/unit_02433.c src/unit_02454.c src/unit_02455.c)
target_include_directories(restore_city_ui_verify PRIVATE include)
target_compile_definitions(restore_city_ui_verify PRIVATE _CRT_SECURE_NO_WARNINGS)
target_compile_options(restore_city_ui_verify PRIVATE /W0 /Od)
target_link_libraries(restore_city_ui_verify PRIVATE restore_data)
target_link_options(restore_city_ui_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_city_ui COMMAND restore_city_ui_verify)
set_tests_properties(standalone_city_ui PROPERTIES LABELS source-only)
add_executable(restore_message_panel_verify tests/message_panel.c tests/message_panel_raster.c
  src/unit_01619.c src/unit_01620.c src/unit_01621.c src/unit_01622.c src/unit_01623.c
  src/unit_01624.c src/unit_01625.c src/unit_02523.c src/unit_01730.c src/unit_01731.c)
target_include_directories(restore_message_panel_verify PRIVATE include)
target_compile_definitions(restore_message_panel_verify PRIVATE _CRT_SECURE_NO_WARNINGS)
target_compile_options(restore_message_panel_verify PRIVATE /W0 /Od)
target_link_libraries(restore_message_panel_verify PRIVATE restore_data)
target_link_options(restore_message_panel_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_message_panel COMMAND restore_message_panel_verify)
set_tests_properties(standalone_message_panel PROPERTIES LABELS source-only)
add_library(restore_terrain_diamond_clippers OBJECT src/unit_01725.c src/unit_01726.c)
target_include_directories(restore_terrain_diamond_clippers PRIVATE include)
target_compile_definitions(restore_terrain_diamond_clippers PRIVATE
  FUN_004cdad0=terrain_hclip_impl FUN_004cdb80=terrain_vclip_impl)
add_executable(restore_terrain_diamond_verify tests/terrain_diamond.c runtime/terrain_diamond.c
  src/unit_00435.c src/unit_00436.c src/unit_00437.c src/unit_00438.c src/unit_00439.c
  src/unit_00440.c src/unit_00441.c src/unit_00442.c src/unit_00443.c src/unit_00444.c src/unit_00445.c
  $<TARGET_OBJECTS:restore_terrain_diamond_clippers>)
target_include_directories(restore_terrain_diamond_verify PRIVATE include)
target_compile_definitions(restore_terrain_diamond_verify PRIVATE TERRAIN_DIAMOND_TEST_TRACE _CRT_SECURE_NO_WARNINGS)
target_compile_options(restore_terrain_diamond_verify PRIVATE /W4 /Od)
target_link_libraries(restore_terrain_diamond_verify PRIVATE restore_data)
target_link_options(restore_terrain_diamond_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_terrain_diamond COMMAND restore_terrain_diamond_verify)
set_tests_properties(standalone_terrain_diamond PROPERTIES LABELS source-only)
add_executable(restore_world_render_verify tests/world_render.c tests/world_render_real_upper.c
  tests/world_render_real_clips.c tests/world_render_real_members.c src/unit_00547.c)
target_include_directories(restore_world_render_verify PRIVATE include)
target_compile_definitions(restore_world_render_verify PRIVATE _CRT_SECURE_NO_WARNINGS)
target_link_libraries(restore_world_render_verify PRIVATE restore_data advapi32)
target_link_options(restore_world_render_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_world_render COMMAND restore_world_render_verify)
set_tests_properties(standalone_world_render PROPERTIES LABELS source-only)
add_executable(restore_event_publisher_verify tests/event_publisher.c
  src/unit_00211.c src/unit_01911.c src/unit_01914.c src/unit_01917.c src/unit_01759.c src/unit_01747.c src/unit_01619.c)
target_include_directories(restore_event_publisher_verify PRIVATE include)
target_compile_definitions(restore_event_publisher_verify PRIVATE _CRT_SECURE_NO_WARNINGS)
target_compile_options(restore_event_publisher_verify PRIVATE /W0 /Od)
target_link_libraries(restore_event_publisher_verify PRIVATE restore_data)
target_link_options(restore_event_publisher_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_event_publisher COMMAND restore_event_publisher_verify)
set_tests_properties(standalone_event_publisher PROPERTIES LABELS source-only WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
add_library(restore_upper_raster_skip OBJECT src/unit_00149.c)
target_include_directories(restore_upper_raster_skip PRIVATE include)
target_compile_definitions(restore_upper_raster_skip PRIVATE FUN_00410f20=upper_skip_impl)
add_executable(restore_upper_raster_verify tests/upper_raster.c runtime/upper_raster.c runtime/sprite_pixels.c
  src/unit_00140.c src/unit_00141.c src/unit_00142.c src/unit_00143.c src/unit_00144.c
  src/unit_00145.c src/unit_00146.c src/unit_00147.c src/unit_00148.c $<TARGET_OBJECTS:restore_upper_raster_skip>)
target_include_directories(restore_upper_raster_verify PRIVATE include)
target_compile_definitions(restore_upper_raster_verify PRIVATE _CRT_SECURE_NO_WARNINGS)
target_compile_options(restore_upper_raster_verify PRIVATE /W4 /Od)
target_link_libraries(restore_upper_raster_verify PRIVATE restore_data)
target_link_options(restore_upper_raster_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_upper_raster COMMAND restore_upper_raster_verify)
set_tests_properties(standalone_upper_raster PROPERTIES LABELS source-only)
add_executable(restore_rich_message_text_verify tests/rich_message_text.c tests/rich_message_text_raster.c
  src/unit_01630.c src/unit_01631.c src/unit_01632.c src/unit_01633.c src/unit_01634.c src/unit_01635.c
  src/unit_01669.c src/unit_01670.c)
target_include_directories(restore_rich_message_text_verify PRIVATE include)
target_compile_definitions(restore_rich_message_text_verify PRIVATE _CRT_SECURE_NO_WARNINGS)
target_compile_options(restore_rich_message_text_verify PRIVATE /W0 /Od)
target_link_libraries(restore_rich_message_text_verify PRIVATE restore_data)
target_link_options(restore_rich_message_text_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_rich_message_text COMMAND restore_rich_message_text_verify)
set_tests_properties(standalone_rich_message_text PROPERTIES LABELS source-only)
add_executable(restore_message_panel_input_verify tests/message_panel_input.c tests/message_panel_input_buttons.c
  src/unit_00407.c src/unit_01626.c src/unit_01627.c src/unit_01628.c src/unit_01629.c
  runtime/message_panel_input.c src/unit_00405.c src/unit_00406.c)
target_include_directories(restore_message_panel_input_verify PRIVATE include)
target_compile_definitions(restore_message_panel_input_verify PRIVATE _CRT_SECURE_NO_WARNINGS)
target_compile_options(restore_message_panel_input_verify PRIVATE /W0 "$<$<CONFIG:Debug>:/Od>")
target_link_libraries(restore_message_panel_input_verify PRIVATE restore_data)
target_link_options(restore_message_panel_input_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_message_panel_input COMMAND restore_message_panel_input_verify)
set_tests_properties(standalone_message_panel_input PROPERTIES LABELS source-only)
add_executable(restore_ground_render_verify tests/ground_render.c tests/ground_render_real_graphics.c
  tests/ground_render_real_upper.c tests/ground_render_real_clips.c tests/ground_render_real_helpers.c
  src/unit_00469.c src/unit_00470.c src/unit_00472.c)
target_include_directories(restore_ground_render_verify PRIVATE include)
target_compile_definitions(restore_ground_render_verify PRIVATE _CRT_SECURE_NO_WARNINGS GROUND_RENDER_VERIFY)
target_compile_options(restore_ground_render_verify PRIVATE /W4)
target_link_libraries(restore_ground_render_verify PRIVATE restore_data advapi32)
target_link_options(restore_ground_render_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_ground_render COMMAND restore_ground_render_verify)
set_tests_properties(standalone_ground_render PROPERTIES LABELS source-only)
add_library(restore_ambient_coord OBJECT src/unit_00461.c)
target_include_directories(restore_ambient_coord PRIVATE include)
target_compile_definitions(restore_ambient_coord PRIVATE FUN_00430960=ambient_coord_impl)
add_library(restore_ambient_rng OBJECT src/unit_02825.c)
target_include_directories(restore_ambient_rng PRIVATE include)
target_compile_definitions(restore_ambient_rng PRIVATE FUN_00562348=ambient_rng_impl)
add_executable(restore_ambient_sound_verify tests/ambient_sound.c src/unit_02486.c
  $<TARGET_OBJECTS:restore_ambient_coord> $<TARGET_OBJECTS:restore_ambient_rng>)
target_include_directories(restore_ambient_sound_verify PRIVATE include)
target_compile_definitions(restore_ambient_sound_verify PRIVATE _CRT_SECURE_NO_WARNINGS)
target_compile_options(restore_ambient_sound_verify PRIVATE /W4 "$<$<CONFIG:Debug>:/Od>")
target_link_libraries(restore_ambient_sound_verify PRIVATE restore_data)
target_link_options(restore_ambient_sound_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_ambient_sound COMMAND restore_ambient_sound_verify)
set_tests_properties(standalone_ambient_sound PROPERTIES LABELS source-only)
add_executable(restore_lookup_sentinel_verify tests/lookup_sentinel.c tests/lookup_sentinel_real_clear.c src/unit_00459.c)
target_include_directories(restore_lookup_sentinel_verify PRIVATE include)
target_compile_definitions(restore_lookup_sentinel_verify PRIVATE _CRT_SECURE_NO_WARNINGS)
target_compile_options(restore_lookup_sentinel_verify PRIVATE /W4)
target_link_libraries(restore_lookup_sentinel_verify PRIVATE restore_data advapi32)
target_link_options(restore_lookup_sentinel_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_lookup_sentinel COMMAND restore_lookup_sentinel_verify)
set_tests_properties(standalone_lookup_sentinel PROPERTIES LABELS source-only)
add_executable(restore_camera_bounds_verify tests/camera_bounds.c
  src/unit_00464.c src/unit_00465.c src/unit_01011.c src/unit_01151.c
  src/unit_01880.c src/unit_01881.c src/unit_01885.c src/unit_01886.c)
target_include_directories(restore_camera_bounds_verify PRIVATE include)
target_compile_definitions(restore_camera_bounds_verify PRIVATE _CRT_SECURE_NO_WARNINGS)
target_compile_options(restore_camera_bounds_verify PRIVATE /W4)
target_link_libraries(restore_camera_bounds_verify PRIVATE restore_data advapi32)
target_link_options(restore_camera_bounds_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_camera_bounds COMMAND restore_camera_bounds_verify)
set_tests_properties(standalone_camera_bounds PROPERTIES LABELS source-only)
add_executable(restore_city_ui_layout_verify tests/city_ui_layout.c src/unit_02343.c src/unit_02353.c
  src/unit_02344.c src/unit_02345.c src/unit_02346.c src/unit_02347.c src/unit_02348.c
  src/unit_02349.c src/unit_02350.c src/unit_02433.c src/unit_02454.c src/unit_02455.c)
target_include_directories(restore_city_ui_layout_verify PRIVATE include)
target_compile_definitions(restore_city_ui_layout_verify PRIVATE _CRT_SECURE_NO_WARNINGS)
target_compile_options(restore_city_ui_layout_verify PRIVATE /W4 "$<$<CONFIG:Debug>:/Od>")
target_link_libraries(restore_city_ui_layout_verify PRIVATE restore_data)
target_link_options(restore_city_ui_layout_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_city_ui_layout COMMAND restore_city_ui_layout_verify)
set_tests_properties(standalone_city_ui_layout PROPERTIES LABELS source-only)
add_executable(restore_actor_render_verify tests/actor_render.c tests/actor_render_real_renderer.c
  tests/actor_render_real_graphics.c tests/actor_render_real_clips.c src/unit_00562.c)
target_include_directories(restore_actor_render_verify PRIVATE include)
target_compile_definitions(restore_actor_render_verify PRIVATE _CRT_SECURE_NO_WARNINGS)
target_compile_options(restore_actor_render_verify PRIVATE /W4)
target_link_libraries(restore_actor_render_verify PRIVATE restore_data advapi32)
target_link_options(restore_actor_render_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_actor_render COMMAND restore_actor_render_verify)
set_tests_properties(standalone_actor_render PROPERTIES LABELS source-only)
add_executable(restore_city_top_chrome_verify tests/city_top_chrome.c src/unit_00396.c src/unit_02443.c
  src/unit_02343.c src/unit_02353.c src/unit_02344.c src/unit_02345.c src/unit_02346.c
  src/unit_02347.c src/unit_02348.c src/unit_02349.c src/unit_02350.c
  src/unit_02433.c src/unit_02454.c src/unit_02455.c)
target_include_directories(restore_city_top_chrome_verify PRIVATE include)
target_compile_definitions(restore_city_top_chrome_verify PRIVATE _CRT_SECURE_NO_WARNINGS)
target_compile_options(restore_city_top_chrome_verify PRIVATE /W4 "$<$<CONFIG:Debug>:/Od>")
target_link_libraries(restore_city_top_chrome_verify PRIVATE restore_data)
target_link_options(restore_city_top_chrome_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_city_top_chrome COMMAND restore_city_top_chrome_verify)
set_tests_properties(standalone_city_top_chrome PROPERTIES LABELS source-only)
add_executable(restore_mirrored_raster_verify tests/mirrored_raster.c tests/mirrored_raster_real_skip.c
  tests/mirrored_raster_plain.c src/unit_00131.c)
target_include_directories(restore_mirrored_raster_verify PRIVATE include)
target_compile_definitions(restore_mirrored_raster_verify PRIVATE _CRT_SECURE_NO_WARNINGS RESTORE_MIRRORED_RASTER_TRACE)
target_compile_options(restore_mirrored_raster_verify PRIVATE /W4)
target_link_libraries(restore_mirrored_raster_verify PRIVATE restore_data advapi32)
target_link_options(restore_mirrored_raster_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_mirrored_raster COMMAND restore_mirrored_raster_verify)
set_tests_properties(standalone_mirrored_raster PROPERTIES LABELS source-only)
add_executable(restore_preview_footprints_verify tests/preview_footprint_seeds.c runtime/preview_footprint_seeds.c
  src/unit_00002.c src/unit_00099.c src/unit_00195.c src/unit_01993.c
  src/unit_02563.c src/unit_02617.c src/unit_02604.c src/unit_02591.c src/unit_01965.c src/unit_01968.c)
target_include_directories(restore_preview_footprints_verify PRIVATE include)
target_compile_definitions(restore_preview_footprints_verify PRIVATE _CRT_SECURE_NO_WARNINGS)
target_compile_options(restore_preview_footprints_verify PRIVATE /W4 "$<$<CONFIG:Debug>:/Od>")
target_link_libraries(restore_preview_footprints_verify PRIVATE restore_data)
target_link_options(restore_preview_footprints_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_preview_footprints COMMAND restore_preview_footprints_verify)
set_tests_properties(standalone_preview_footprints PROPERTIES LABELS source-only)
add_executable(restore_mirrored_clip_verify tests/mirrored_clip.c tests/mirrored_clip_real_skip.c
  tests/mirrored_clip_plain132.c tests/mirrored_clip_plain133.c src/unit_00132.c src/unit_00133.c)
target_include_directories(restore_mirrored_clip_verify PRIVATE include)
target_compile_definitions(restore_mirrored_clip_verify PRIVATE _CRT_SECURE_NO_WARNINGS RESTORE_MIRRORED_CLIP_TRACE)
target_compile_options(restore_mirrored_clip_verify PRIVATE /W4)
target_link_libraries(restore_mirrored_clip_verify PRIVATE restore_data advapi32)
target_link_options(restore_mirrored_clip_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_mirrored_clip COMMAND restore_mirrored_clip_verify)
set_tests_properties(standalone_mirrored_clip PROPERTIES LABELS source-only)
add_executable(restore_placement_preview_verify tests/placement_preview.c tests/placement_preview_helpers.c src/unit_00565.c
  runtime/preview_footprint_seeds.c src/unit_00002.c src/unit_00099.c src/unit_00195.c src/unit_01993.c
  src/unit_02563.c src/unit_02617.c src/unit_02604.c src/unit_02591.c src/unit_01965.c src/unit_01968.c)
target_include_directories(restore_placement_preview_verify PRIVATE include)
target_compile_definitions(restore_placement_preview_verify PRIVATE _CRT_SECURE_NO_WARNINGS RESTORE_PLACEMENT_PREVIEW_PRODUCTION)
target_compile_options(restore_placement_preview_verify PRIVATE /W0 "$<$<CONFIG:Debug>:/Od>")
target_link_libraries(restore_placement_preview_verify PRIVATE restore_data)
target_link_options(restore_placement_preview_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_placement_preview COMMAND restore_placement_preview_verify)
set_tests_properties(standalone_placement_preview PROPERTIES LABELS source-only)
target_sources(pharaoh_restored PRIVATE runtime/actor_callbacks_human.c runtime/actor_callbacks_human_boundary.c)
add_executable(restore_actor_human_verify tests/actor_human.c tests/actor_human_helpers.c runtime/actor_callbacks_human.c)
target_include_directories(restore_actor_human_verify PRIVATE include)
target_compile_definitions(restore_actor_human_verify PRIVATE _CRT_SECURE_NO_WARNINGS RESTORE_ACTOR_HUMAN_PRODUCTION)
target_compile_options(restore_actor_human_verify PRIVATE /W0 "$<$<CONFIG:Debug>:/Od>")
target_link_libraries(restore_actor_human_verify PRIVATE restore_data)
target_link_options(restore_actor_human_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_actor_human COMMAND restore_actor_human_verify)
set_tests_properties(standalone_actor_human PROPERTIES LABELS source-only)
add_executable(restore_region_validation_verify tests/region_validation.c
  src/unit_01202.c src/unit_01203.c src/unit_01205.c src/unit_01206.c runtime/placement_preview_predicates.c)
target_include_directories(restore_region_validation_verify PRIVATE include)
target_compile_definitions(restore_region_validation_verify PRIVATE _CRT_SECURE_NO_WARNINGS)
target_compile_options(restore_region_validation_verify PRIVATE /W4 "$<$<CONFIG:Debug>:/Od>")
target_link_libraries(restore_region_validation_verify PRIVATE restore_data advapi32)
target_link_options(restore_region_validation_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_region_validation COMMAND restore_region_validation_verify)
set_tests_properties(standalone_region_validation PROPERTIES LABELS source-only)
add_executable(restore_tool_selector_verify tests/tool_selector.c tests/tool_selector_helpers.c src/unit_00042.c)
target_include_directories(restore_tool_selector_verify PRIVATE include)
target_compile_definitions(restore_tool_selector_verify PRIVATE _CRT_SECURE_NO_WARNINGS RESTORE_TOOL_SELECTOR_PRODUCTION)
target_compile_options(restore_tool_selector_verify PRIVATE /W0 "$<$<CONFIG:Debug>:/Od>")
target_link_libraries(restore_tool_selector_verify PRIVATE restore_data)
target_link_options(restore_tool_selector_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_tool_selector COMMAND restore_tool_selector_verify)
set_tests_properties(standalone_tool_selector PROPERTIES LABELS source-only)
add_executable(restore_actor_coordinates_verify tests/actor_coordinates.c tests/actor_coordinates_real_offset.c
  tests/actor_coordinates_real_inverse.c tests/actor_coordinates_real_world.c tests/actor_coordinates_real_actor.c)
target_include_directories(restore_actor_coordinates_verify PRIVATE include)
target_compile_definitions(restore_actor_coordinates_verify PRIVATE _CRT_SECURE_NO_WARNINGS)
target_compile_options(restore_actor_coordinates_verify PRIVATE /W4)
target_link_libraries(restore_actor_coordinates_verify PRIVATE restore_data advapi32)
target_link_options(restore_actor_coordinates_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_actor_coordinates COMMAND restore_actor_coordinates_verify)
set_tests_properties(standalone_actor_coordinates PROPERTIES LABELS source-only)
add_executable(restore_camera_navigation_verify tests/camera_navigation.c tests/camera_navigation_real.c)
target_include_directories(restore_camera_navigation_verify PRIVATE include)
target_compile_definitions(restore_camera_navigation_verify PRIVATE _CRT_SECURE_NO_WARNINGS)
target_compile_options(restore_camera_navigation_verify PRIVATE /W4)
target_link_libraries(restore_camera_navigation_verify PRIVATE restore_data advapi32)
target_link_options(restore_camera_navigation_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_camera_navigation COMMAND restore_camera_navigation_verify)
set_tests_properties(standalone_camera_navigation PROPERTIES LABELS source-only)
target_sources(pharaoh_restored PRIVATE runtime/actor_callbacks_small.c runtime/actor_callbacks_small_state_adapter.c)
add_library(actor_small_state_adapter OBJECT runtime/actor_callbacks_small_state_adapter.c)
target_compile_definitions(actor_small_state_adapter PRIVATE restore_actor_small_state=actor_small_state_adapter_real)
add_library(actor_small_query825 OBJECT src/unit_00825.c)
target_compile_definitions(actor_small_query825 PRIVATE FUN_004578b0=actor_small_query_825_real)
add_library(actor_small_query826 OBJECT src/unit_00826.c)
target_compile_definitions(actor_small_query826 PRIVATE FUN_004579d0=actor_small_query_826_real)
add_library(actor_small_classifier OBJECT src/unit_01602.c)
target_compile_definitions(actor_small_classifier PRIVATE FUN_004c5120=actor_small_classify_real)
foreach(t actor_small_state_adapter actor_small_query825 actor_small_query826 actor_small_classifier)
 target_include_directories(${t} PRIVATE include)
 target_compile_options(${t} PRIVATE /W4)
endforeach()
add_executable(restore_actor_callbacks_small_verify tests/actor_callbacks_small.c runtime/actor_callbacks_small.c src/unit_01320.c
 $<TARGET_OBJECTS:actor_small_state_adapter> $<TARGET_OBJECTS:actor_small_query825>
 $<TARGET_OBJECTS:actor_small_query826> $<TARGET_OBJECTS:actor_small_classifier>)
target_include_directories(restore_actor_callbacks_small_verify PRIVATE include)
target_compile_definitions(restore_actor_callbacks_small_verify PRIVATE _CRT_SECURE_NO_WARNINGS)
target_compile_options(restore_actor_callbacks_small_verify PRIVATE /W4)
target_link_libraries(restore_actor_callbacks_small_verify PRIVATE restore_data advapi32)
target_link_options(restore_actor_callbacks_small_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_actor_callbacks_small COMMAND restore_actor_callbacks_small_verify)
set_tests_properties(standalone_actor_callbacks_small PROPERTIES LABELS source-only)
add_executable(restore_raw_tile_raster_verify tests/raw_tile_raster.c src/unit_00151.c)
target_include_directories(restore_raw_tile_raster_verify PRIVATE include)
target_compile_definitions(restore_raw_tile_raster_verify PRIVATE _CRT_SECURE_NO_WARNINGS)
target_compile_options(restore_raw_tile_raster_verify PRIVATE /W4)
target_link_libraries(restore_raw_tile_raster_verify PRIVATE restore_data advapi32)
target_link_options(restore_raw_tile_raster_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_raw_tile_raster COMMAND restore_raw_tile_raster_verify)
set_tests_properties(standalone_raw_tile_raster PROPERTIES LABELS source-only)
target_sources(pharaoh_restored PRIVATE runtime/actor_callbacks_chain.c runtime/actor_callbacks_chain_sound.c)
add_library(actor_chain_root OBJECT src/unit_01352.c)
target_compile_definitions(actor_chain_root PRIVATE FUN_0049b3c0=actor_chain_root_real)
add_library(actor_chain_tile OBJECT "src/unit_01462.c")
target_compile_definitions(actor_chain_tile PRIVATE FUN_004b57e0=actor_chain_tile_real)
add_library(actor_chain_sound OBJECT runtime/actor_callbacks_chain_sound.c)
target_compile_definitions(actor_chain_sound PRIVATE restore_actor_chain_sound=actor_chain_sound_real)
foreach(t actor_chain_root actor_chain_tile actor_chain_sound)
 target_include_directories(${t} PRIVATE include)
 target_compile_options(${t} PRIVATE /W4 $<$<CONFIG:Debug>:/Od>)
endforeach()
add_executable(restore_actor_callbacks_chain_verify tests/actor_callbacks_chain.c runtime/actor_callbacks_chain.c "src/unit_01130.c" $<TARGET_OBJECTS:actor_chain_root> $<TARGET_OBJECTS:actor_chain_tile> $<TARGET_OBJECTS:actor_chain_sound>)
target_include_directories(restore_actor_callbacks_chain_verify PRIVATE include)
target_compile_definitions(restore_actor_callbacks_chain_verify PRIVATE _CRT_SECURE_NO_WARNINGS)
target_compile_options(restore_actor_callbacks_chain_verify PRIVATE /W4 $<$<CONFIG:Debug>:/Od>)
target_link_libraries(restore_actor_callbacks_chain_verify PRIVATE restore_data advapi32)
target_link_options(restore_actor_callbacks_chain_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED /MAP)

add_test(NAME standalone_actor_callbacks_chain COMMAND restore_actor_callbacks_chain_verify)
set_tests_properties(standalone_actor_callbacks_chain PROPERTIES LABELS source-only)
add_executable(restore_tool_selector_input_verify tests/tool_selector_input.c tests/tool_selector_input_real.c)
target_include_directories(restore_tool_selector_input_verify PRIVATE include)
target_compile_definitions(restore_tool_selector_input_verify PRIVATE _CRT_SECURE_NO_WARNINGS)
target_compile_options(restore_tool_selector_input_verify PRIVATE /W4)
target_link_libraries(restore_tool_selector_input_verify PRIVATE restore_data advapi32)
target_link_options(restore_tool_selector_input_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_tool_selector_input COMMAND restore_tool_selector_input_verify)
set_tests_properties(standalone_tool_selector_input PROPERTIES LABELS source-only)
# Apply the five frozen files.1335/1358 already appear in RESTORE_UNITS.
# Frozen human1416/1427/header are unchanged prerequisites.
target_sources(pharaoh_restored PRIVATE
  runtime/actor_callback_496140.c runtime/actor_callback_496140_queue.c)
# Coordinator-owned actor registry:
#   #include "actor_callback_496140.h"
#   case 0x00496140u:return restore_actor_00496140;
# Entry is void(void), reading current actor DWORD D056F8.
# Existing verifiers compiling the registry directly need coordinator-owned
# link/test support for this additional callback reference.

add_executable(restore_actor_496140_verify
  tests/actor_496140.c tests/actor_496140_helpers.c runtime/actor_callback_496140.c)
target_include_directories(restore_actor_496140_verify PRIVATE include)
target_compile_definitions(restore_actor_496140_verify PRIVATE
  _CRT_SECURE_NO_WARNINGS RESTORE_ACTOR_496140_PRODUCTION)
target_compile_options(restore_actor_496140_verify PRIVATE /W0 "$<$<CONFIG:Debug>:/Od>")
target_link_libraries(restore_actor_496140_verify PRIVATE restore_data)
target_link_options(restore_actor_496140_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_actor_496140 COMMAND restore_actor_496140_verify)
set_tests_properties(standalone_actor_496140 PROPERTIES LABELS source-only)
# Helpers include1335/1358/queue/accepted1416+1427 with boundary renames;
# do not compile those five separately in this verifier.
# tools/verify_actor_496140_emulation.py --production --exe <shared target exe>
# writes metadata/actor-496140-emulation.json. Retain Debug/O2 snapshots.
# No startup initialization, menu routing, fixtures or original code calls added.

add_executable(restore_diagnostic_input_verify tests/diagnostic_input_isolation.c runtime/window_entry.c)
target_include_directories(restore_diagnostic_input_verify PRIVATE include)
target_compile_options(restore_diagnostic_input_verify PRIVATE /W4)
add_test(NAME standalone_diagnostic_input COMMAND restore_diagnostic_input_verify)
set_tests_properties(standalone_diagnostic_input PROPERTIES LABELS source-only)
target_sources(pharaoh_restored PRIVATE runtime/actor_combat_boundaries.c)
add_library(actor_combat_direction OBJECT "src/unit_01415.c")
target_compile_definitions(actor_combat_direction PRIVATE FUN_004b1d10=actor_combat_direction_real)
add_library(actor_combat_exit OBJECT "src/unit_01417.c")
target_compile_definitions(actor_combat_exit PRIVATE FUN_004b1da0=actor_combat_exit_real)
add_library(actor_combat_step OBJECT "src/unit_01435.c")
target_compile_definitions(actor_combat_step PRIVATE FUN_004b3f00=actor_combat_step_real)
add_library(actor_combat_route OBJECT "src/unit_01427.c")
target_compile_definitions(actor_combat_route PRIVATE FUN_004b3250=actor_combat_route_real)
add_library(actor_combat_classA OBJECT "src/unit_02065.c")
target_compile_definitions(actor_combat_classA PRIVATE FUN_004ea9a0=actor_combat_classA_real)
add_library(actor_combat_classC OBJECT "src/unit_02066.c")
target_compile_definitions(actor_combat_classC PRIVATE FUN_004ea9c0=actor_combat_classC_real)
add_library(actor_combat_sound OBJECT "runtime/actor_combat_boundaries.c")
target_compile_definitions(actor_combat_sound PRIVATE restore_actor_combat_sound=actor_combat_sound_real)
foreach(t actor_combat_direction actor_combat_exit actor_combat_step actor_combat_route actor_combat_classA actor_combat_classC actor_combat_sound)
 target_include_directories(${t} PRIVATE include)
 target_compile_options(${t} PRIVATE /W4 $<$<CONFIG:Debug>:/Od>)
endforeach()
add_executable(restore_actor_combat_verify tests/actor_combat.c src/unit_01414.c "src/unit_01130.c"
 $<TARGET_OBJECTS:actor_combat_direction> $<TARGET_OBJECTS:actor_combat_exit> $<TARGET_OBJECTS:actor_combat_step> $<TARGET_OBJECTS:actor_combat_route> $<TARGET_OBJECTS:actor_combat_classA> $<TARGET_OBJECTS:actor_combat_classC> $<TARGET_OBJECTS:actor_combat_sound>)
target_include_directories(restore_actor_combat_verify PRIVATE include)
target_compile_definitions(restore_actor_combat_verify PRIVATE _CRT_SECURE_NO_WARNINGS)
target_compile_options(restore_actor_combat_verify PRIVATE /W4 $<$<CONFIG:Debug>:/Od>)
target_link_libraries(restore_actor_combat_verify PRIVATE restore_data advapi32)
target_link_options(restore_actor_combat_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED /MAP)

add_test(NAME standalone_actor_combat COMMAND restore_actor_combat_verify)
set_tests_properties(standalone_actor_combat PROPERTIES LABELS source-only)
add_executable(restore_camera_pan_verify tests/camera_pan.c tests/camera_pan_real.c)
target_include_directories(restore_camera_pan_verify PRIVATE include)
target_compile_definitions(restore_camera_pan_verify PRIVATE _CRT_SECURE_NO_WARNINGS)
target_compile_options(restore_camera_pan_verify PRIVATE /W4)
target_link_libraries(restore_camera_pan_verify PRIVATE restore_data advapi32)
target_link_options(restore_camera_pan_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_camera_pan COMMAND restore_camera_pan_verify)
set_tests_properties(standalone_camera_pan PROPERTIES LABELS source-only)
# Main library additions (the ten numbered replacements are already in RESTORE_UNITS):
target_sources(pharaoh_restored PRIVATE runtime/dropdown_menu_options.c
  runtime/dropdown_menu_boundary.c runtime/dropdown_menu_clock.c)

# Deterministic controller proof; platform clock and deep raster/action boundaries are in fixture.
add_executable(restore_dropdown_menu_verify tests/dropdown_menu.c tests/dropdown_menu_helpers.c
  src/unit_00397.c src/unit_00408.c src/unit_00409.c src/unit_00410.c src/unit_00411.c
  src/unit_00412.c src/unit_00413.c src/unit_00414.c src/unit_00415.c src/unit_01843.c
  src/unit_01842.c runtime/dropdown_menu_options.c)
target_include_directories(restore_dropdown_menu_verify PRIVATE include)
target_link_libraries(restore_dropdown_menu_verify PRIVATE restore_data)
target_compile_definitions(restore_dropdown_menu_verify PRIVATE _CRT_SECURE_NO_WARNINGS)
target_compile_options(restore_dropdown_menu_verify PRIVATE /W0 "$<$<CONFIG:Debug>:/Od>")
target_link_options(restore_dropdown_menu_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_dropdown_menu COMMAND restore_dropdown_menu_verify)
set_tests_properties(standalone_dropdown_menu PROPERTIES LABELS source-only)

# Native replay after copied files/shared build:
# python tools/verify_dropdown_menu_emulation.py --production --exe build/c-only-debug/Debug/restore_dropdown_menu_verify.exe
# => metadata/dropdown-menu-emulation.json
# Repeat with optimized shared target; preserve per-config reports before overwrite.

# No new startup call or global callback mapping is required for the five owned option callbacks:
# restore_dropdown_callback resolves those five explicitly then delegates other tokens to the existing
# central restore_menu_callback. Further action tokens need their own native review;
# the earlier count of nineteen unmapped callbacks is historical, not a current inventory.
# Reviewed406090 and40A0E0 now route through restore_remaining_menu_action.
# Do not include dropdown_menu.h into legacy1868 without reconciling its old-style declarations:
# native426030 reads onlybars while its two original callers push an unused count4 too.
# The existing caller bytes/stack shape are preserved; this stage does not mutate unit1868.

# unit02446 is already part of RESTORE_UNITS; no new production runtime/startup/callback source.
add_executable(restore_dropdown_tiler_verify tests/dropdown_tiler.c src/unit_02446.c)
target_include_directories(restore_dropdown_tiler_verify PRIVATE include)
target_link_libraries(restore_dropdown_tiler_verify PRIVATE restore_data)
target_compile_definitions(restore_dropdown_tiler_verify PRIVATE _CRT_SECURE_NO_WARNINGS)
target_compile_options(restore_dropdown_tiler_verify PRIVATE /W0 "$<$<CONFIG:Debug>:/Od>")
target_link_options(restore_dropdown_tiler_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_dropdown_tiler COMMAND restore_dropdown_tiler_verify)
# python tools/verify_dropdown_tiler_emulation.py --production --exe build/c-only-debug/Debug/restore_dropdown_tiler_verify.exe
# => metadata/dropdown-tiler-emulation.json
# Repeat against the optimized target and retain both exact report/binary hashes.

set_tests_properties(standalone_dropdown_tiler PROPERTIES LABELS source-only)
include("${CMAKE_CURRENT_SOURCE_DIR}/tools/map_cells_shared_targets.cmake")
target_sources(pharaoh_restored PRIVATE runtime/sprite_shutdown_lifecycle.c runtime/sprite_shutdown_boundaries.c)
set_source_files_properties(src/unit_02677.c src/unit_02678.c src/unit_02679.c src/unit_02681.c
 PROPERTIES SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX")
add_library(sprite_shutdown_release OBJECT "src/unit_02529.c")
target_compile_definitions(sprite_shutdown_release PRIVATE FUN_0053f790=sprite_shutdown_release_real)
add_library(sprite_shutdown_library_destroy OBJECT src/unit_02533.c)
target_compile_definitions(sprite_shutdown_library_destroy PRIVATE FUN_00540500=sprite_shutdown_library_destroy_real)
add_library(sprite_shutdown_lifecycle OBJECT runtime/sprite_shutdown_lifecycle.c)
target_compile_definitions(sprite_shutdown_lifecycle PRIVATE FUN_005404e0=sprite_shutdown_library_construct_real)
add_library(sprite_shutdown_boundaries OBJECT runtime/sprite_shutdown_boundaries.c)
target_compile_definitions(sprite_shutdown_boundaries PRIVATE restore_sprite_shutdown_free=sprite_shutdown_free_real restore_sprite_shutdown_delete=sprite_shutdown_delete_real restore_sprite_shutdown_register=sprite_shutdown_register_real restore_sprite_shutdown_slot=sprite_shutdown_slot_real)
foreach(t sprite_shutdown_release sprite_shutdown_library_destroy sprite_shutdown_lifecycle sprite_shutdown_boundaries)
 target_include_directories(${t} PRIVATE include)
 target_compile_options(${t} PRIVATE /W4 $<$<CONFIG:Debug>:/Od>)
endforeach()
add_executable(restore_sprite_shutdown_verify tests/sprite_shutdown.c src/unit_01990.c src/unit_02512.c src/unit_02513.c src/unit_02514.c src/unit_02515.c src/unit_02516.c "src/unit_02678.c" "runtime/host_allocator.c"
 $<TARGET_OBJECTS:sprite_shutdown_release> $<TARGET_OBJECTS:sprite_shutdown_library_destroy> $<TARGET_OBJECTS:sprite_shutdown_lifecycle> $<TARGET_OBJECTS:sprite_shutdown_boundaries>)
target_include_directories(restore_sprite_shutdown_verify PRIVATE include)
target_compile_definitions(restore_sprite_shutdown_verify PRIVATE _CRT_SECURE_NO_WARNINGS)
target_compile_options(restore_sprite_shutdown_verify PRIVATE /W4 $<$<CONFIG:Debug>:/Od>)
target_link_libraries(restore_sprite_shutdown_verify PRIVATE restore_data advapi32)
target_link_options(restore_sprite_shutdown_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED /MAP)

add_test(NAME standalone_sprite_shutdown COMMAND restore_sprite_shutdown_verify)
set_tests_properties(standalone_sprite_shutdown PROPERTIES LABELS source-only)
include("${CMAKE_CURRENT_SOURCE_DIR}/artifacts/scalar_binding_batch_001/shared_targets.cmake")

# Ten complete native dropdown callbacks, including their real central routing.
target_sources(pharaoh_restored PRIVATE runtime/dropdown_actions.c)
foreach(dropdown_action_variant IN ITEMS trace plain)
  if(dropdown_action_variant STREQUAL "trace")
    set(dropdown_action_target restore_dropdown_actions_verify)
  else()
    set(dropdown_action_target restore_dropdown_actions_plain_verify)
  endif()
  add_executable(${dropdown_action_target} tests/dropdown_actions.c runtime/dropdown_actions.c)
  target_include_directories(${dropdown_action_target} PRIVATE include)
  target_compile_definitions(${dropdown_action_target} PRIVATE _CRT_SECURE_NO_WARNINGS)
  if(dropdown_action_variant STREQUAL "trace")
    target_compile_definitions(${dropdown_action_target} PRIVATE RESTORE_DROPDOWN_ACTION_TRACE)
  endif()
  target_compile_options(${dropdown_action_target} PRIVATE /W4 "$<$<CONFIG:Debug>:/Od>" "$<$<CONFIG:Release>:/O2>")
  target_link_libraries(${dropdown_action_target} PRIVATE pharaoh_restored restore_data kernel32 user32 gdi32 shell32 ole32 oleaut32 advapi32 winmm ddraw dxguid legacy_stdio_definitions)
  target_link_options(${dropdown_action_target} PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
  add_test(NAME standalone_dropdown_actions_${dropdown_action_variant} COMMAND ${dropdown_action_target})
  set_tests_properties(standalone_dropdown_actions_${dropdown_action_variant} PROPERTIES LABELS source-only)
endforeach()
set(RESTORE_ORIGINAL_EXE "" CACHE FILEPATH "Optional original executable for differential verification")
if(RESTORE_ORIGINAL_EXE)
  find_package(Python3 COMPONENTS Interpreter REQUIRED)
  set(fixture_header "${CMAKE_CURRENT_BINARY_DIR}/generated/original_fixtures.h")
  add_custom_command(OUTPUT "${fixture_header}"
    COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/generate_fixtures.py"
            "${RESTORE_ORIGINAL_EXE}" "${fixture_header}"
    DEPENDS tools/generate_fixtures.py "${RESTORE_ORIGINAL_EXE}"
    VERBATIM)
  add_executable(restore_verify tests/differential.c "${fixture_header}")
  target_include_directories(restore_verify PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/generated")
  target_link_libraries(restore_verify PRIVATE pharaoh_restored)
  add_test(NAME differential_original_x86 COMMAND restore_verify)
  get_filename_component(game_directory "${RESTORE_ORIGINAL_EXE}" DIRECTORY)
  if(RESTORE_BUILD_HYBRID)
    add_test(NAME hybrid_image_inspect COMMAND "${hybrid_executable}" --game-dir "${game_directory}" --inspect)
    add_test(NAME hybrid_abi_self_test COMMAND "${hybrid_executable}" --game-dir "${game_directory}" --self-test)
    add_test(NAME hybrid_compatibility_startup COMMAND "${hybrid_executable}" --game-dir "${game_directory}" --compat-self-test)
    add_test(NAME hybrid_hook_identity_guard COMMAND "${hybrid_executable}" --game-dir "${game_directory}" --hook-guard-test)
    add_test(NAME hybrid_experimental_hook_install COMMAND "${hybrid_executable}" --game-dir "${game_directory}" --experimental-hook-test)
    set_tests_properties(hybrid_image_inspect hybrid_abi_self_test hybrid_compatibility_startup hybrid_hook_identity_guard hybrid_experimental_hook_install PROPERTIES RUN_SERIAL TRUE)
  endif()
endif()

# Frozen complete editor validation and display confirmation algorithms.
target_sources(pharaoh_restored PRIVATE runtime/editor_validation.c
  runtime/display_confirmation.c runtime/display_confirmation_clock.c)
add_executable(restore_editor_validation_verify
  artifacts/editor_validation_phase/tests/editor_validation.c
  artifacts/editor_validation_phase/tests/editor_actual_trace.c
  artifacts/editor_validation_phase/tests/editor_actual_plain.c)
target_include_directories(restore_editor_validation_verify PRIVATE include artifacts/editor_validation_phase/tests)
target_compile_definitions(restore_editor_validation_verify PRIVATE _CRT_SECURE_NO_WARNINGS)
target_compile_options(restore_editor_validation_verify PRIVATE /W4 "$<$<CONFIG:Debug>:/Od>" "$<$<CONFIG:Release>:/O2>")
target_link_libraries(restore_editor_validation_verify PRIVATE restore_data advapi32)
target_link_options(restore_editor_validation_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_editor_validation COMMAND restore_editor_validation_verify)
set_tests_properties(standalone_editor_validation PROPERTIES LABELS source-only)
add_executable(restore_display_confirmation_verify tests/display_confirmation.c runtime/display_confirmation.c
  src/unit_00025.c src/unit_00026.c src/unit_02436.c src/unit_01766.c src/unit_00119.c
  src/unit_00416.c src/unit_00417.c src/unit_01836.c src/unit_01857.c src/unit_01858.c src/unit_01775.c)
target_include_directories(restore_display_confirmation_verify PRIVATE include)
target_compile_definitions(restore_display_confirmation_verify PRIVATE _CRT_SECURE_NO_WARNINGS)
target_compile_options(restore_display_confirmation_verify PRIVATE /W4 /WX "$<$<CONFIG:Debug>:/Od>" "$<$<CONFIG:Release>:/O2>")
target_link_libraries(restore_display_confirmation_verify PRIVATE restore_data)
target_link_options(restore_display_confirmation_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_display_confirmation COMMAND restore_display_confirmation_verify)
set_tests_properties(standalone_display_confirmation PROPERTIES LABELS source-only)

# Complete native replay callback, fallback and real profile getter composition.
target_sources(pharaoh_restored PRIVATE runtime/replay_mission.c)
add_executable(restore_replay_mission_verify
  artifacts/replay_mission_phase/tests/replay_mission.c
  artifacts/replay_mission_phase/tests/replay_actual_trace.c
  artifacts/replay_mission_phase/tests/replay_actual_plain.c)
target_include_directories(restore_replay_mission_verify PRIVATE include artifacts/replay_mission_phase/tests)
target_compile_definitions(restore_replay_mission_verify PRIVATE _CRT_SECURE_NO_WARNINGS)
target_compile_options(restore_replay_mission_verify PRIVATE /W4 "$<$<CONFIG:Debug>:/Od>" "$<$<CONFIG:Release>:/O2>")
target_link_libraries(restore_replay_mission_verify PRIVATE restore_data advapi32)
target_link_options(restore_replay_mission_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_replay_mission COMMAND restore_replay_mission_verify)
set_tests_properties(standalone_replay_mission PROPERTIES LABELS source-only)

# Whole initial menu dispatcher; renderer algorithms remain qualified boundaries.
target_sources(pharaoh_restored PRIVATE runtime/initial_menu_dispatch_boundary.c)
add_executable(restore_initial_menu_dispatch_verify
  artifacts/initial_menu_dispatch_phase/tests/initial_dispatch.c
  artifacts/initial_menu_dispatch_phase/tests/actual_trace.c
  artifacts/initial_menu_dispatch_phase/tests/actual_plain.c)
target_include_directories(restore_initial_menu_dispatch_verify PRIVATE include artifacts/initial_menu_dispatch_phase/tests)
target_compile_definitions(restore_initial_menu_dispatch_verify PRIVATE _CRT_SECURE_NO_WARNINGS)
target_compile_options(restore_initial_menu_dispatch_verify PRIVATE /W4 /WX "$<$<CONFIG:Debug>:/Od>" "$<$<CONFIG:Release>:/O2>")
target_link_libraries(restore_initial_menu_dispatch_verify PRIVATE restore_data)
target_link_options(restore_initial_menu_dispatch_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_initial_menu_dispatch COMMAND restore_initial_menu_dispatch_verify)
set_tests_properties(standalone_initial_menu_dispatch PROPERTIES LABELS source-only)
add_executable(restore_menu_existing_actions_verify
  artifacts/menu_existing_actions_phase/tests/menu_existing_actions.c
  artifacts/menu_existing_actions_phase/tests/existing_actual_entries.c
  artifacts/menu_existing_actions_phase/tests/existing_actual_loadsave.c
  artifacts/menu_existing_actions_phase/tests/existing_actual_helpers.c
  artifacts/menu_existing_actions_phase/tests/unreached_link_sentinels.c)
target_include_directories(restore_menu_existing_actions_verify PRIVATE include artifacts/menu_existing_actions_phase/tests)
target_compile_definitions(restore_menu_existing_actions_verify PRIVATE _CRT_SECURE_NO_WARNINGS)
target_compile_options(restore_menu_existing_actions_verify PRIVATE /W4 /Gy "$<$<CONFIG:Debug>:/Od>" "$<$<CONFIG:Release>:/O2>")
target_link_libraries(restore_menu_existing_actions_verify PRIVATE restore_data advapi32)
target_link_options(restore_menu_existing_actions_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED /OPT:REF /INCREMENTAL:NO)
add_test(NAME standalone_menu_existing_actions COMMAND restore_menu_existing_actions_verify)
set_tests_properties(standalone_menu_existing_actions PROPERTIES LABELS source-only)

# Complete native1001-instruction training panel; explicit query/render boundaries.
add_executable(restore_training_panel_verify
  artifacts/training_panel_phase/tests/training_panel.c
  artifacts/training_panel_phase/tests/training_actual_trace.c
  artifacts/training_panel_phase/tests/training_actual_plain.c)
target_include_directories(restore_training_panel_verify PRIVATE include artifacts/training_panel_phase/tests)
target_compile_definitions(restore_training_panel_verify PRIVATE _CRT_SECURE_NO_WARNINGS)
target_compile_options(restore_training_panel_verify PRIVATE /W4 "$<$<CONFIG:Debug>:/Od>" "$<$<CONFIG:Release>:/O2>")
target_link_libraries(restore_training_panel_verify PRIVATE restore_data advapi32)
target_link_options(restore_training_panel_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_training_panel COMMAND restore_training_panel_verify)
set_tests_properties(standalone_training_panel PROPERTIES LABELS source-only)

# Complete modal/window-Escape, cursor selector and real resource-query closure.
target_sources(pharaoh_restored PRIVATE runtime/menu_keyboard_modal.c runtime/menu_keyboard_modal_platform.c runtime/menu_cursor_platform.c)
add_executable(restore_menu_keyboard_modal_verify
  tests/menu_keyboard_modal.c
  tests/menu_keyboard_modal_keyboard_context.c
  runtime/menu_keyboard_modal.c
  src/unit_02464.c
  src/unit_02465.c
  src/unit_02466.c
  src/unit_02470.c
  src/unit_02472.c
  runtime/window_messages.c
  runtime/text_selection.c
  src/unit_01672.c
  src/unit_01673.c
  src/unit_00416.c
  src/unit_00417.c
  src/unit_01836.c
  src/unit_01845.c
  src/unit_01846.c
  src/unit_01856.c
  src/unit_01857.c
  src/unit_01858.c
  src/unit_01730.c
  src/unit_01731.c
  src/unit_01767.c
  src/unit_01768.c
  runtime/display_confirmation.c
  src/unit_00025.c
  src/unit_00026.c)
target_include_directories(restore_menu_keyboard_modal_verify PRIVATE include tests)
target_compile_definitions(restore_menu_keyboard_modal_verify PRIVATE _CRT_SECURE_NO_WARNINGS)
target_compile_definitions(restore_menu_keyboard_modal_verify PRIVATE MODAL_KEYBOARD_CONTEXT_SOURCE="${CMAKE_CURRENT_SOURCE_DIR}/src/unit_00274.c")
target_compile_options(restore_menu_keyboard_modal_verify PRIVATE /W4 /WX "$<$<CONFIG:Debug>:/Od>" "$<$<CONFIG:Release>:/O2>")
target_link_libraries(restore_menu_keyboard_modal_verify PRIVATE restore_data)
target_link_options(restore_menu_keyboard_modal_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_menu_keyboard_modal COMMAND restore_menu_keyboard_modal_verify)
set_tests_properties(standalone_menu_keyboard_modal PROPERTIES LABELS source-only)
add_executable(restore_menu_cursor_verify
  tests/menu_cursor.c
  src/unit_01769.c)
target_include_directories(restore_menu_cursor_verify PRIVATE include tests)
target_compile_definitions(restore_menu_cursor_verify PRIVATE _CRT_SECURE_NO_WARNINGS)
target_compile_options(restore_menu_cursor_verify PRIVATE /W4 /WX "$<$<CONFIG:Debug>:/Od>" "$<$<CONFIG:Release>:/O2>")
target_link_libraries(restore_menu_cursor_verify PRIVATE restore_data)
target_link_options(restore_menu_cursor_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_menu_cursor COMMAND restore_menu_cursor_verify)
set_tests_properties(standalone_menu_cursor PROPERTIES LABELS source-only)
add_executable(restore_training_queries_verify
  artifacts/training_queries_phase/tests/training_queries.c
  artifacts/training_queries_phase/tests/queries_actual.c)
target_include_directories(restore_training_queries_verify PRIVATE include artifacts/training_queries_phase/tests)
target_compile_definitions(restore_training_queries_verify PRIVATE _CRT_SECURE_NO_WARNINGS)
target_compile_options(restore_training_queries_verify PRIVATE /W4 "$<$<CONFIG:Debug>:/Od>" "$<$<CONFIG:Release>:/O2>")
target_link_libraries(restore_training_queries_verify PRIVATE restore_data advapi32)
target_link_options(restore_training_queries_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_training_queries COMMAND restore_training_queries_verify)
set_tests_properties(standalone_training_queries PROPERTIES LABELS source-only)

# Whole4D75D0 native-verified refresh controller. Deep render/media endpoints
# are controlled only in the oracle; the product links its actual C functions.
target_sources(pharaoh_restored PRIVATE runtime/refresh_menu_dispatch_boundary.c)
add_executable(restore_refresh_menu_dispatch_verify
  artifacts/refresh_menu_dispatch_phase/tests/refresh_dispatch.c
  artifacts/refresh_menu_dispatch_phase/tests/actual_mission_buttons.c
  src/unit_01867.c
  src/unit_00286.c src/unit_00293.c src/unit_00295.c
  src/unit_00296.c src/unit_00297.c src/unit_02276.c)
target_include_directories(restore_refresh_menu_dispatch_verify PRIVATE include artifacts/refresh_menu_dispatch_phase/tests)
target_compile_definitions(restore_refresh_menu_dispatch_verify PRIVATE _CRT_SECURE_NO_WARNINGS)
target_compile_options(restore_refresh_menu_dispatch_verify PRIVATE /W4 /WX "$<$<CONFIG:Debug>:/Od>" "$<$<CONFIG:Release>:/O2>")
# Actual295 is MOV EAX,57CD70;RET. Its unchanged DWORD* to byte* return has
# known C4133; this file alone retains the same suppression as private proof.
set_source_files_properties(src/unit_00295.c PROPERTIES COMPILE_OPTIONS /wd4133)
target_link_libraries(restore_refresh_menu_dispatch_verify PRIVATE restore_data advapi32)
target_link_options(restore_refresh_menu_dispatch_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_refresh_menu_dispatch COMMAND restore_refresh_menu_dispatch_verify)
set_tests_properties(standalone_refresh_menu_dispatch PROPERTIES LABELS source-only)

# Whole actor-order controller and real linked actor lookup; no dispatcher edits.
add_executable(restore_menu_window_cursor_consumer_verify
  tests/menu_window_cursor_consumer.c
  tests/menu_window_cursor_consumer_actual.c
  src/unit_00085.c
  src/unit_01769.c)
target_include_directories(restore_menu_window_cursor_consumer_verify PRIVATE include tests)
target_compile_definitions(restore_menu_window_cursor_consumer_verify PRIVATE
  _CRT_SECURE_NO_WARNINGS
  CONSUMER_ROUTE_SOURCE="${CMAKE_CURRENT_SOURCE_DIR}/src/unit_01427.c"
  CONSUMER_DIRECTION_SOURCE="${CMAKE_CURRENT_SOURCE_DIR}/src/unit_02334.c"
  CONSUMER_SCALE_SOURCE="${CMAKE_CURRENT_SOURCE_DIR}/src/unit_01731.c"
  CONSUMER_LOOKUP_SOURCE="${CMAKE_CURRENT_SOURCE_DIR}/src/unit_01455.c")
target_compile_options(restore_menu_window_cursor_consumer_verify PRIVATE /W4 /WX "$<$<CONFIG:Debug>:/Od>" "$<$<CONFIG:Release>:/O2>")
target_link_libraries(restore_menu_window_cursor_consumer_verify PRIVATE restore_data)
target_link_options(restore_menu_window_cursor_consumer_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_menu_window_cursor_consumer COMMAND restore_menu_window_cursor_consumer_verify)
set_tests_properties(standalone_menu_window_cursor_consumer PROPERTIES LABELS source-only)

# Complete five-function text algorithms and actual actor-order flood producer.
add_executable(restore_menu_text_glyph_verify
  artifacts/menu_text_glyph_phase/tests/text_glyph.c
  src/unit_01667.c src/unit_01691.c src/unit_01692.c src/unit_01693.c src/unit_01694.c)
target_include_directories(restore_menu_text_glyph_verify PRIVATE include artifacts/menu_text_glyph_phase/tests)
target_compile_definitions(restore_menu_text_glyph_verify PRIVATE _CRT_SECURE_NO_WARNINGS)
target_compile_options(restore_menu_text_glyph_verify PRIVATE /W4 /WX "$<$<CONFIG:Debug>:/Od>" "$<$<CONFIG:Release>:/O2>")
target_link_libraries(restore_menu_text_glyph_verify PRIVATE restore_data advapi32)
target_link_options(restore_menu_text_glyph_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_menu_text_glyph COMMAND restore_menu_text_glyph_verify)
set_tests_properties(standalone_menu_text_glyph PROPERTIES LABELS source-only)
add_executable(restore_actor_order_flood_verify
  tests/actor_order_flood.c
  tests/actor_order_flood_actual.c
  src/unit_02335.c)
target_include_directories(restore_actor_order_flood_verify PRIVATE include tests)
target_compile_definitions(restore_actor_order_flood_verify PRIVATE
  _CRT_SECURE_NO_WARNINGS
  FLOOD_FILL_SOURCE="${CMAKE_CURRENT_SOURCE_DIR}/src/unit_00119.c")
target_compile_options(restore_actor_order_flood_verify PRIVATE /W4 /WX "$<$<CONFIG:Debug>:/Od>" "$<$<CONFIG:Release>:/O2>")
target_link_libraries(restore_actor_order_flood_verify PRIVATE restore_data)
target_link_options(restore_actor_order_flood_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_actor_order_flood COMMAND restore_actor_order_flood_verify)
set_tests_properties(standalone_actor_order_flood PROPERTIES LABELS source-only)

# Actual LOS algorithms retain x87 extended slopes; no implicit-ST0/global2775 replacement.
target_sources(pharaoh_restored PRIVATE runtime/actor_order_los_numeric.c runtime/menu_full_input_numeric.c)
add_executable(restore_actor_order_los_verify
  tests/actor_order_los.c
  tests/actor_order_los_actual.c
  runtime/actor_order_los_numeric.c
  runtime/menu_full_input_numeric.c
  src/unit_00086.c
  src/unit_00087.c
  src/unit_02335.c)
target_include_directories(restore_actor_order_los_verify PRIVATE include tests)
target_compile_definitions(restore_actor_order_los_verify PRIVATE
  _CRT_SECURE_NO_WARNINGS
  LOS_FILL_SOURCE="${CMAKE_CURRENT_SOURCE_DIR}/src/unit_00119.c")
target_compile_options(restore_actor_order_los_verify PRIVATE /W4 /WX "$<$<CONFIG:Debug>:/Od>" "$<$<CONFIG:Release>:/O2>")
target_link_libraries(restore_actor_order_los_verify PRIVATE restore_data)
target_link_options(restore_actor_order_los_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_actor_order_los COMMAND restore_actor_order_los_verify)
set_tests_properties(standalone_actor_order_los PROPERTIES LABELS source-only)

# Checkpoint135: exact frozen map backlog; typed numeric runtimes and verification targets.
include("${CMAKE_CURRENT_SOURCE_DIR}/artifacts/map_refresh_integration_phase/shared_targets.cmake")

# Checkpoint135 build correction: P3 fixtures share P2's authenticated record data.
foreach(_map135_target IN ITEMS restore_map_refresh_postpass3_verify restore_map_refresh_postpass3_plain_verify)
 target_include_directories(${_map135_target} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/artifacts/map_refresh_postpass2_phase")
endforeach()
unset(_map135_target)

# Actual actor-order call bridge; the numbered C units already belong to the library.
target_sources(pharaoh_restored PRIVATE runtime/actor_order_action_calls.c runtime/menu_list_getter.c)
include("${CMAKE_CURRENT_SOURCE_DIR}/artifacts/map_record_refresh_phase/CMakeLists.txt")
include("${CMAKE_CURRENT_SOURCE_DIR}/artifacts/map_record_decay_phase/CMakeLists.txt")
include("${CMAKE_CURRENT_SOURCE_DIR}/artifacts/map_menu_scroll_phase/CMakeLists.txt")
include("${CMAKE_CURRENT_SOURCE_DIR}/artifacts/map_validation_phase/CMakeLists.txt")
include("${CMAKE_CURRENT_SOURCE_DIR}/artifacts/map_view_commit_phase/CMakeLists.txt")
include("${CMAKE_CURRENT_SOURCE_DIR}/artifacts/map_file_load_phase/CMakeLists.txt")
include("${CMAKE_CURRENT_SOURCE_DIR}/artifacts/tooltip_timing_phase/CMakeLists.txt")
include("${CMAKE_CURRENT_SOURCE_DIR}/artifacts/map_hover_geometry_phase/CMakeLists.txt")
target_sources(pharaoh_restored PRIVATE runtime/menu_table_actions.c)

# Build the instruction-translated menu in the main source-only library.
# Its entry remains explicit until every boundary is reconstructed.
if(NOT RESTORE_BUILD_HYBRID)
 set(_direct_menu_sources direct_c/menu_input_controller.c direct_c/menu_input_boundary.c
  direct_c/menu_inline_capacity.c direct_c/menu_004f96e0.c direct_c/menu_004f7e50.c runtime/menu_route_capacity.c
  runtime/menu_input_entry.c)
 target_sources(pharaoh_restored PRIVATE ${_direct_menu_sources})
 target_include_directories(pharaoh_restored PRIVATE direct_c)
 set_source_files_properties(${_direct_menu_sources} PROPERTIES SKIP_PRECOMPILE_HEADERS ON)
 set_source_files_properties(runtime/menu_input_entry.c PROPERTIES COMPILE_OPTIONS "/W4;/WX;/O2;/GS-;/Ob0")
 unset(_direct_menu_sources)
endif()

foreach(_monument_create IN ITEMS 004015a0 0040da90 00417180 004e6840)
 target_link_options(restore_link_check PRIVATE "/INCLUDE:_FUN_${_monument_create}")
endforeach()
unset(_monument_create)

foreach(_monument_entrance IN ITEMS 00401af0 0040eb90 00417490 004e6ae0)
 target_link_options(restore_link_check PRIVATE "/INCLUDE:_FUN_${_monument_entrance}")
endforeach()
unset(_monument_entrance)

foreach(_monument_map IN ITEMS 004017a0 0040dca0 004e69c0)
 target_link_options(restore_link_check PRIVATE "/INCLUDE:_FUN_${_monument_map}")
endforeach()
unset(_monument_map)

foreach(_monument_image IN ITEMS 00401800 0040de80 00417310 004e6a70)
 target_link_options(restore_link_check PRIVATE "/INCLUDE:_FUN_${_monument_image}")
endforeach()
unset(_monument_image)

target_link_options(restore_link_check PRIVATE "/INCLUDE:_FUN_005417b0" "/INCLUDE:_FUN_00541920")

foreach(_complex_create IN ITEMS 00545c30 00559130 00556480 0054fba0)
 target_link_options(restore_link_check PRIVATE "/INCLUDE:_FUN_${_complex_create}")
endforeach()
unset(_complex_create)

foreach(_complex_position IN ITEMS 00545a30 00558f00 00556230 0054f8a0)
 target_link_options(restore_link_check PRIVATE "/INCLUDE:_FUN_${_complex_position}")
endforeach()
unset(_complex_position)

target_link_options(restore_link_check PRIVATE "/INCLUDE:_restore_placement_position_query")

foreach(_complex_image IN ITEMS 00545760 00558c00 00555ea0 0054f490)
 target_link_options(restore_link_check PRIVATE "/INCLUDE:_FUN_${_complex_image}")
endforeach()
unset(_complex_image)

target_link_options(restore_link_check PRIVATE "/INCLUDE:_restore_house_check_global_mode" "/INCLUDE:_restore_house_compare_desirability" "/INCLUDE:_restore_house_check_requirements")

target_link_options(restore_link_check PRIVATE "/INCLUDE:_restore_house_update_level")

target_link_options(restore_link_check PRIVATE "/INCLUDE:_restore_house_try_merge" "/INCLUDE:_restore_house_apply_layout")

foreach(_house_layout IN ITEMS 0046b480 0046b840 0046b940 0046b9e0 0046bab0 0046bbc0 0046bcb0 0046bda0)
 target_link_options(restore_link_check PRIVATE "/INCLUDE:_FUN_${_house_layout}")
endforeach()
unset(_house_layout)

target_link_options(restore_link_check PRIVATE "/INCLUDE:_restore_house_subdivide")
foreach(_house_split IN ITEMS 0046be90 0046c1c0 0046c500 0046cb30 0046ccf0)
 target_link_options(restore_link_check PRIVATE "/INCLUDE:_FUN_${_house_split}")
endforeach()
unset(_house_split)

# Retain canonical housing-cycle ABI entries in the offline diagnostic target.
foreach(_housing_cycle IN ITEMS 004528b0 00454b40 0046b740)
 target_link_options(restore_link_check PRIVATE "/INCLUDE:_FUN_${_housing_cycle}")
endforeach()
unset(_housing_cycle)

# Keep all temple-complex canonical entries available to offline verification.
foreach(_temple_complex IN ITEMS 004c0860 004c0be0 004c0d50 004c3420 004c52d0)
 target_link_options(restore_link_check PRIVATE "/INCLUDE:_FUN_${_temple_complex}")
endforeach()
unset(_temple_complex)

# Retain canonical formation providers for offline native/C comparison.
foreach(_formation_control IN ITEMS 00472790 004b6e10 004b7c80)
 target_link_options(restore_link_check PRIVATE "/INCLUDE:_FUN_${_formation_control}")
endforeach()
unset(_formation_control)

# Keep the final three reviewed building-policy canonical entries available.
foreach(_building_policy IN ITEMS 0046b100 004bfb30 004c53d0)
 target_link_options(restore_link_check PRIVATE "/INCLUDE:_FUN_${_building_policy}")
endforeach()
unset(_building_policy)

# Keep public availability ABI entries for whole-PE offline verification.
target_link_options(restore_link_check PRIVATE "/INCLUDE:_FUN_00559480" "/INCLUDE:_FUN_005596d0" "/INCLUDE:_restore_resource_availability_dispatch")
target_link_options(restore_link_check PRIVATE "/INCLUDE:_FUN_0043f250" "/INCLUDE:_FUN_0043f3a0" "/INCLUDE:_FUN_00441040" "/INCLUDE:_FUN_004413b0")
target_link_options(restore_link_check PRIVATE "/INCLUDE:_FUN_0043f410" "/INCLUDE:_FUN_00441130")
target_link_options(restore_link_check PRIVATE "/INCLUDE:_FUN_004405c0" "/INCLUDE:_FUN_00440700" "/INCLUDE:_FUN_00440850" "/INCLUDE:_FUN_00440b10" "/INCLUDE:_FUN_00440ed0")
target_link_options(restore_link_check PRIVATE "/INCLUDE:_FUN_0046fef0" "/INCLUDE:_FUN_00405d90")
target_link_options(restore_link_check PRIVATE "/INCLUDE:_FUN_004743f0" "/INCLUDE:_FUN_00474700")

target_link_options(restore_link_check PRIVATE "/INCLUDE:_FUN_0046ac10" "/INCLUDE:_FUN_0046ac50" "/INCLUDE:_FUN_004c0920")
target_link_options(restore_link_check PRIVATE "/INCLUDE:_FUN_004b4980")
target_link_options(restore_link_check PRIVATE "/INCLUDE:_FUN_00454190" "/INCLUDE:_FUN_0045d570" "/INCLUDE:_FUN_00475b50" "/INCLUDE:_FUN_004b7590" "/INCLUDE:_FUN_0050f0f0")
target_link_options(restore_link_check PRIVATE "/INCLUDE:_restore_god_building_0045b2a0" "/INCLUDE:_restore_god_building_0046d0f0")
target_link_options(restore_game PRIVATE "/INCLUDE:_restore_god_building_0045b2a0" "/INCLUDE:_restore_god_building_0046d0f0")
target_sources(pharaoh_restored PRIVATE runtime/storage_delivery.c)
target_link_options(restore_link_check PRIVATE "/INCLUDE:_FUN_0045b230" "/INCLUDE:_FUN_0045d510" "/INCLUDE:_FUN_0045b490" "/INCLUDE:_FUN_0045b2a0")
target_link_options(restore_link_check PRIVATE "/INCLUDE:_restore_actor_message_C_0045b230" "/INCLUDE:_restore_actor_message_C_0045d510" "/INCLUDE:_restore_transport_C_0045d510" "/INCLUDE:_restore_placement_support_C_0045b490" "/INCLUDE:_restore_god_C_0045b2a0")
target_link_options(restore_game PRIVATE "/INCLUDE:_restore_actor_message_C_0045b230" "/INCLUDE:_restore_actor_message_C_0045d510" "/INCLUDE:_restore_transport_C_0045d510" "/INCLUDE:_restore_placement_support_C_0045b490" "/INCLUDE:_restore_god_C_0045b2a0")
target_link_options(restore_link_check PRIVATE "/INCLUDE:_FUN_0045b570" "/INCLUDE:_FUN_0045b700" "/INCLUDE:_FUN_0045b4d0" "/INCLUDE:_restore_placement_support_C_0045b570" "/INCLUDE:_restore_placement_support_C_0045b4d0" "/INCLUDE:_restore_god_C_0045b700")
target_link_options(restore_game PRIVATE "/INCLUDE:_FUN_0045b570" "/INCLUDE:_FUN_0045b700" "/INCLUDE:_FUN_0045b4d0" "/INCLUDE:_restore_placement_support_C_0045b570" "/INCLUDE:_restore_placement_support_C_0045b4d0" "/INCLUDE:_restore_god_C_0045b700")
target_link_options(restore_link_check PRIVATE "/INCLUDE:_restore_god_building_0045b700")
target_link_options(restore_game PRIVATE "/INCLUDE:_restore_god_building_0045b700")

target_sources(pharaoh_restored PRIVATE runtime/storage_trade.c)
foreach(_storage_trade_target IN ITEMS restore_link_check restore_game)
 foreach(_storage_trade_entry IN ITEMS FUN_004434e0 FUN_004435f0 FUN_0045cf50 FUN_0045d180 FUN_0045d9e0
   restore_actor_message_C_004434e0 restore_actor_message_C_004435f0 restore_actor_message_C_0045cf50 restore_actor_message_C_0045d180 restore_transport_C_0045d9e0)
  target_link_options(${_storage_trade_target} PRIVATE "/INCLUDE:_${_storage_trade_entry}")
 endforeach()
endforeach()
unset(_storage_trade_entry)
unset(_storage_trade_target)

target_sources(pharaoh_restored PRIVATE runtime/storage_transactions.c)
foreach(_storage_transaction_target IN ITEMS restore_link_check restore_game)
 target_link_options(${_storage_transaction_target} PRIVATE "/INCLUDE:_FUN_0045d010" "/INCLUDE:_restore_civic_C_0045d010")
 target_link_options(${_storage_transaction_target} PRIVATE "/INCLUDE:_FUN_0045d370" "/INCLUDE:_restore_civic_C_0045d370")
 target_link_options(${_storage_transaction_target} PRIVATE "/INCLUDE:_FUN_0045d4d0" "/INCLUDE:_restore_civic_C_0045d4d0")
 target_link_options(${_storage_transaction_target} PRIVATE "/INCLUDE:_FUN_0045b880" "/INCLUDE:_restore_transport_C_0045b880")
 target_link_options(${_storage_transaction_target} PRIVATE "/INCLUDE:_FUN_0045bb50" "/INCLUDE:_restore_routing_C_0045bb50")
 target_link_options(${_storage_transaction_target} PRIVATE "/INCLUDE:_FUN_0045dc50" "/INCLUDE:_restore_routing_C_0045dc50")
endforeach()
unset(_storage_transaction_target)

target_sources(pharaoh_restored PRIVATE runtime/granary.c)
foreach(_granary_target IN ITEMS restore_link_check restore_game)
 target_link_options(${_granary_target} PRIVATE "/INCLUDE:_FUN_0045dcc0" "/INCLUDE:_restore_transport_C_0045dcc0")
 target_link_options(${_granary_target} PRIVATE "/INCLUDE:_FUN_0045de80" "/INCLUDE:_restore_transport_C_0045de80")
 target_link_options(${_granary_target} PRIVATE "/INCLUDE:_FUN_0045e040" "/INCLUDE:_restore_city_C_0045e040")
 target_link_options(${_granary_target} PRIVATE "/INCLUDE:_FUN_0045e150" "/INCLUDE:_restore_transport_C_0045e150")
 target_link_options(${_granary_target} PRIVATE "/INCLUDE:_FUN_0045e220" "/INCLUDE:_restore_sidebar_C_0045e220")
 target_link_options(${_granary_target} PRIVATE "/INCLUDE:_FUN_0045e360" "/INCLUDE:_restore_transport_C_0045e360")
endforeach()
unset(_granary_target)

target_sources(pharaoh_restored PRIVATE runtime/industry_logistics.c)
foreach(_industry_target IN ITEMS restore_link_check restore_game)
 target_link_options(${_industry_target} PRIVATE "/INCLUDE:_FUN_0045eab0" "/INCLUDE:_restore_transport_C_0045eab0")
 target_link_options(${_industry_target} PRIVATE "/INCLUDE:_FUN_0045f450" "/INCLUDE:_restore_transport_C_0045f450")
 target_link_options(${_industry_target} PRIVATE "/INCLUDE:_FUN_0045f7b0" "/INCLUDE:_restore_transport_C_0045f7b0")
 target_link_options(${_industry_target} PRIVATE "/INCLUDE:_FUN_00460890" "/INCLUDE:_restore_transport_C_00460890")
endforeach()
unset(_industry_target)

target_sources(pharaoh_restored PRIVATE runtime/city_supply.c)
foreach(_city_supply_target IN ITEMS restore_link_check restore_game)
 target_link_options(${_city_supply_target} PRIVATE "/INCLUDE:_FUN_0045acf0")
 target_link_options(${_city_supply_target} PRIVATE "/INCLUDE:_FUN_0045d5d0")
 target_link_options(${_city_supply_target} PRIVATE "/INCLUDE:_FUN_0045d630")
 target_link_options(${_city_supply_target} PRIVATE "/INCLUDE:_FUN_0045e6c0")
 target_link_options(${_city_supply_target} PRIVATE "/INCLUDE:_FUN_0045e6e0")
 target_link_options(${_city_supply_target} PRIVATE "/INCLUDE:_restore_transport_C_0045acf0")
 target_link_options(${_city_supply_target} PRIVATE "/INCLUDE:_restore_city_C_0045d5d0")
 target_link_options(${_city_supply_target} PRIVATE "/INCLUDE:_restore_city_C_0045d630")
 target_link_options(${_city_supply_target} PRIVATE "/INCLUDE:_restore_city_C_0045e6c0")
 target_link_options(${_city_supply_target} PRIVATE "/INCLUDE:_restore_city_C_0045e6e0")
 target_link_options(${_city_supply_target} PRIVATE "/INCLUDE:_restore_advisor_C_0045d5d0")
 target_link_options(${_city_supply_target} PRIVATE "/INCLUDE:_restore_advisor_C_0045d630")
endforeach()
unset(_city_supply_target)

target_sources(pharaoh_restored PRIVATE runtime/warehouse_stock.c)
foreach(_warehouse_target IN ITEMS restore_link_check restore_game)
 target_link_options(${_warehouse_target} PRIVATE "/INCLUDE:_FUN_0045aa30" "/INCLUDE:_restore_city_C_0045aa30")
 target_link_options(${_warehouse_target} PRIVATE "/INCLUDE:_FUN_0045aaa0" "/INCLUDE:_restore_city_C_0045aaa0")
 target_link_options(${_warehouse_target} PRIVATE "/INCLUDE:_FUN_0045aae0" "/INCLUDE:_restore_city_C_0045aae0")
endforeach()
unset(_warehouse_target)

target_sources(pharaoh_restored PRIVATE runtime/housing_capacity.c)
foreach(_housing_target IN ITEMS restore_link_check restore_game)
 target_link_options(${_housing_target} PRIVATE "/INCLUDE:_FUN_004558b0")
 target_link_options(${_housing_target} PRIVATE "/INCLUDE:_FUN_004583f0")
 target_link_options(${_housing_target} PRIVATE "/INCLUDE:_FUN_004e8610")
 target_link_options(${_housing_target} PRIVATE "/INCLUDE:_FUN_00458440")
 target_link_options(${_housing_target} PRIVATE "/INCLUDE:_FUN_004e8f80")
 target_link_options(${_housing_target} PRIVATE "/INCLUDE:_FUN_004e2fb0")
 target_link_options(${_housing_target} PRIVATE "/INCLUDE:_restore_city_C_004558b0")
 target_link_options(${_housing_target} PRIVATE "/INCLUDE:_restore_city_C_004583f0")
 target_link_options(${_housing_target} PRIVATE "/INCLUDE:_restore_event_C_004583f0")
 target_link_options(${_housing_target} PRIVATE "/INCLUDE:_restore_city_C_004e8610")
 target_link_options(${_housing_target} PRIVATE "/INCLUDE:_restore_city_C_00458440")
 target_link_options(${_housing_target} PRIVATE "/INCLUDE:_restore_city_C_004e8f80")
endforeach()
unset(_housing_target)

target_sources(pharaoh_restored PRIVATE runtime/population_demography.c)
foreach(_demography_target IN ITEMS restore_link_check restore_game)
 target_link_options(${_demography_target} PRIVATE "/INCLUDE:_FUN_00458e20" "/INCLUDE:_restore_city_C_00458e20")
 target_link_options(${_demography_target} PRIVATE "/INCLUDE:_FUN_00458f00" "/INCLUDE:_restore_city_C_00458f00")
 target_link_options(${_demography_target} PRIVATE "/INCLUDE:_FUN_004e92c0" "/INCLUDE:_restore_city_C_004e92c0")
 target_link_options(${_demography_target} PRIVATE "/INCLUDE:_FUN_004e92f0" "/INCLUDE:_restore_city_C_004e92f0")
 target_link_options(${_demography_target} PRIVATE "/INCLUDE:_FUN_004e8650" "/INCLUDE:_restore_city_C_004e8650")
 target_link_options(${_demography_target} PRIVATE "/INCLUDE:_FUN_004e8720" "/INCLUDE:_restore_city_C_004e8720")
 target_link_options(${_demography_target} PRIVATE "/INCLUDE:_FUN_004e87a0" "/INCLUDE:_restore_city_C_004e87a0")
 target_link_options(${_demography_target} PRIVATE "/INCLUDE:_FUN_004e9460" "/INCLUDE:_restore_city_C_004e9460")
 target_link_options(${_demography_target} PRIVATE "/INCLUDE:_FUN_00458880" "/INCLUDE:_restore_city_C_00458880")
endforeach()
unset(_demography_target)

target_sources(pharaoh_restored PRIVATE runtime/migration_flow.c)
foreach(_migration_target IN ITEMS restore_link_check restore_game)
 target_link_options(${_migration_target} PRIVATE "/INCLUDE:_FUN_00458540" "/INCLUDE:_restore_city_C_00458540")
 target_link_options(${_migration_target} PRIVATE "/INCLUDE:_FUN_00458950" "/INCLUDE:_restore_city_C_00458950")
 target_link_options(${_migration_target} PRIVATE "/INCLUDE:_FUN_00458b60" "/INCLUDE:_restore_city_C_00458b60")
 target_link_options(${_migration_target} PRIVATE "/INCLUDE:_FUN_00458d00" "/INCLUDE:_restore_city_C_00458d00")
 target_link_options(${_migration_target} PRIVATE "/INCLUDE:_FUN_00458d80" "/INCLUDE:_restore_city_C_00458d80")
endforeach()
unset(_migration_target)

target_sources(pharaoh_restored PRIVATE runtime/labor_allocation.c)
foreach(_labor_target IN ITEMS restore_link_check restore_game)
 target_link_options(${_labor_target} PRIVATE "/INCLUDE:_FUN_00459070")
 target_link_options(${_labor_target} PRIVATE "/INCLUDE:_FUN_00459100")
 target_link_options(${_labor_target} PRIVATE "/INCLUDE:_FUN_00459400")
 target_link_options(${_labor_target} PRIVATE "/INCLUDE:_FUN_00459350")
 target_link_options(${_labor_target} PRIVATE "/INCLUDE:_FUN_004595a0")
 target_link_options(${_labor_target} PRIVATE "/INCLUDE:_FUN_00459670")
 target_link_options(${_labor_target} PRIVATE "/INCLUDE:_restore_city_C_00459070")
 target_link_options(${_labor_target} PRIVATE "/INCLUDE:_restore_city_C_00459100")
 target_link_options(${_labor_target} PRIVATE "/INCLUDE:_restore_city_C_00459400")
 target_link_options(${_labor_target} PRIVATE "/INCLUDE:_restore_city_C_00459350")
 target_link_options(${_labor_target} PRIVATE "/INCLUDE:_restore_city_C_004595a0")
 target_link_options(${_labor_target} PRIVATE "/INCLUDE:_restore_city_C_00459670")
 target_link_options(${_labor_target} PRIVATE "/INCLUDE:_restore_labor_C_00459400")
 target_link_options(${_labor_target} PRIVATE "/INCLUDE:_restore_labor_C_004595a0")
 target_link_options(${_labor_target} PRIVATE "/INCLUDE:_restore_labor_C_00459670")
endforeach()
unset(_labor_target)

set_source_files_properties(runtime/building_access.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX;/O2;/GS-;/Ob0")
target_sources(pharaoh_restored PRIVATE runtime/building_access.c)
foreach(_access_target IN ITEMS restore_link_check restore_game)
 target_link_options(${_access_target} PRIVATE "/INCLUDE:_FUN_00457a40" "/INCLUDE:_FUN_00457af0" "/INCLUDE:_restore_city_C_00457a40" "/INCLUDE:_restore_city_C_00457af0")
endforeach()
unset(_access_target)

set_source_files_properties(runtime/building_access_search.c src/unit_00830.c src/unit_01188.c src/unit_01189.c src/unit_01269.c src/unit_02135.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX;/O2;/GS-;/Ob0")
target_sources(pharaoh_restored PRIVATE runtime/building_access_search.c)
foreach(_search_target IN ITEMS restore_link_check restore_game)
 target_link_options(${_search_target} PRIVATE "/INCLUDE:_FUN_00481060" "/INCLUDE:_restore_city_C_00481060")
 target_link_options(${_search_target} PRIVATE "/INCLUDE:_FUN_00481210" "/INCLUDE:_restore_city_C_00481210")
 target_link_options(${_search_target} PRIVATE "/INCLUDE:_FUN_00487d00" "/INCLUDE:_restore_city_C_00487d00")
 target_link_options(${_search_target} PRIVATE "/INCLUDE:_FUN_004f30b0" "/INCLUDE:_restore_city_C_004f30b0")
 target_link_options(${_search_target} PRIVATE "/INCLUDE:_FUN_00458270" "/INCLUDE:_restore_city_C_00458270")
endforeach()
unset(_search_target)

set_source_files_properties(runtime/building_removal.c src/unit_01580.c src/unit_00588.c src/unit_00945.c src/unit_01312.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX;/O2;/GS-;/Ob0")
target_sources(pharaoh_restored PRIVATE runtime/building_removal.c)
foreach(_removal_target IN ITEMS restore_link_check restore_game)
 target_link_options(${_removal_target} PRIVATE "/INCLUDE:_FUN_004c03d0" "/INCLUDE:_restore_invasion_C_004c03d0")
 target_link_options(${_removal_target} PRIVATE "/INCLUDE:_FUN_0043fd00" "/INCLUDE:_restore_god_C_0043fd00")
 target_link_options(${_removal_target} PRIVATE "/INCLUDE:_FUN_004695d0" "/INCLUDE:_restore_god_C_004695d0")
 target_link_options(${_removal_target} PRIVATE "/INCLUDE:_FUN_0048c0e0" "/INCLUDE:_restore_invasion_C_0048c0e0")
endforeach()
unset(_removal_target)

set_source_files_properties(runtime/building_destruction.c src/unit_00986.c src/unit_01581.c src/unit_00985.c src/unit_01576.c src/unit_02038.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX;/O2;/GS-;/Ob0")
target_sources(pharaoh_restored PRIVATE runtime/building_destruction.c)
foreach(_destruction_target IN ITEMS restore_link_check restore_game)
 target_link_options(${_destruction_target} PRIVATE "/INCLUDE:_FUN_0046d100" "/INCLUDE:_restore_god_C_0046d100")
 target_link_options(${_destruction_target} PRIVATE "/INCLUDE:_FUN_004c04a0" "/INCLUDE:_restore_god_C_004c04a0")
 target_link_options(${_destruction_target} PRIVATE "/INCLUDE:_FUN_0046d0f0" "/INCLUDE:_restore_god_C_0046d0f0")
 target_link_options(${_destruction_target} PRIVATE "/INCLUDE:_FUN_004bfee0" "/INCLUDE:_restore_god_C_004bfee0")
 target_link_options(${_destruction_target} PRIVATE "/INCLUDE:_FUN_004e9170" "/INCLUDE:_restore_god_C_004e9170")
 target_link_options(${_destruction_target} PRIVATE "/INCLUDE:_restore_god_building_0046d100")
 target_link_options(${_destruction_target} PRIVATE "/INCLUDE:_restore_god_building_004c04a0")
 target_link_options(${_destruction_target} PRIVATE "/INCLUDE:_restore_god_building_0046d0f0")
 target_link_options(${_destruction_target} PRIVATE "/INCLUDE:_restore_god_building_004bfee0")
 target_link_options(${_destruction_target} PRIVATE "/INCLUDE:_restore_god_building_004e9170")
endforeach()
unset(_destruction_target)

set_source_files_properties(runtime/map_surface_repair.c runtime/menu_drag_vegetation_pass_model.c runtime/menu_drag_rail_pass_model.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX;/O2;/GS-;/Ob0")
target_sources(pharaoh_restored PRIVATE runtime/map_surface_repair.c)
foreach(_surface_target IN ITEMS restore_link_check restore_game)
 target_link_options(${_surface_target} PRIVATE "/INCLUDE:_FUN_00476530" "/INCLUDE:_restore_C_00476530" "/INCLUDE:_FUN_0047b1a0" "/INCLUDE:_restore_C_0047b1a0")
endforeach()
unset(_surface_target)

set_source_files_properties(runtime/access_repair_flood.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX;/O2;/GS-;/Ob0")
target_sources(pharaoh_restored PRIVATE runtime/access_repair_flood.c)
foreach(_repair_target IN ITEMS restore_link_check restore_game)
 target_link_options(${_repair_target} PRIVATE "/INCLUDE:_FUN_00517c20" "/INCLUDE:_restore_city_C_00517c20")
endforeach()
unset(_repair_target)

set_source_files_properties(runtime/map_region_repair.c runtime/menu_drag_paving_pass_model.c runtime/menu_drag_canal_region_model.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX;/O2;/GS-;/Ob0")
target_sources(pharaoh_restored PRIVATE runtime/map_region_repair.c)
foreach(_region_target IN ITEMS restore_link_check restore_game)
 target_link_options(${_region_target} PRIVATE "/INCLUDE:_FUN_00475720" "/INCLUDE:_restore_C_00475720" "/INCLUDE:_FUN_0047bae0" "/INCLUDE:_restore_C_0047bae0")
endforeach()
unset(_region_target)

set_source_files_properties(runtime/building_occupancy_query.c src/unit_01295.c src/unit_01296.c src/unit_01297.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX;/O2;/GS-;/Ob0")
target_sources(pharaoh_restored PRIVATE runtime/building_occupancy_query.c)
foreach(_occupancy_target IN ITEMS restore_link_check restore_game)
 target_link_options(${_occupancy_target} PRIVATE "/INCLUDE:_FUN_004893c0" "/INCLUDE:_restore_actor_message_C_004893c0")
 target_link_options(${_occupancy_target} PRIVATE "/INCLUDE:_FUN_00489470" "/INCLUDE:_restore_actor_message_C_00489470")
 target_link_options(${_occupancy_target} PRIVATE "/INCLUDE:_FUN_004894a0" "/INCLUDE:_restore_actor_message_C_004894a0")
 target_link_options(${_occupancy_target} PRIVATE "/INCLUDE:_restore_actor_building_query" "/INCLUDE:_restore_ring_004f6a50" "/INCLUDE:_FUN_004f68f0" "/INCLUDE:_FUN_004a4340")
endforeach()
unset(_occupancy_target)

set_source_files_properties(runtime/building_processing_query.c src/unit_00846.c src/unit_01275.c src/unit_01572.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX;/O2;/GS-;/Ob0")
target_sources(pharaoh_restored PRIVATE runtime/building_processing_query.c)
foreach(_processing_target IN ITEMS restore_link_check restore_game)
 target_link_options(${_processing_target} PRIVATE "/INCLUDE:_FUN_00459240")
 target_link_options(${_processing_target} PRIVATE "/INCLUDE:_FUN_00487f80")
 target_link_options(${_processing_target} PRIVATE "/INCLUDE:_FUN_004bf9d0")
 target_link_options(${_processing_target} PRIVATE "/INCLUDE:_restore_actor_message_C_00459240" "/INCLUDE:_restore_actor_message_C_004bf9d0")
endforeach()
unset(_processing_target)

foreach(_message_target IN ITEMS restore_link_check restore_game)
 foreach(_message_entry IN ITEMS 0044d6d0 0044d700 00495b60 0053c060 0053c100 0053d3a0)
  target_link_options(${_message_target} PRIVATE "/INCLUDE:_FUN_${_message_entry}" "/INCLUDE:_restore_actor_message_C_${_message_entry}")
 endforeach()
endforeach()
unset(_message_entry)
unset(_message_target)

set_source_files_properties(runtime/figure_menu_actions.c src/unit_02501.c src/unit_02504.c src/unit_02505.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX;/O2;/GS-;/Ob0")
foreach(_voice_target IN ITEMS restore_link_check restore_game)
 foreach(_voice_entry IN ITEMS 0053d280 0053d0b0 0053bfe0 00539a70)
  target_link_options(${_voice_target} PRIVATE "/INCLUDE:_FUN_${_voice_entry}")
 endforeach()
 target_link_options(${_voice_target} PRIVATE "/INCLUDE:_restore_menu_overlay_C_0053d280")
endforeach()
unset(_voice_entry)
unset(_voice_target)

set_source_files_properties(runtime/sound_streams.c src/unit_02704.c src/unit_02705.c src/unit_02707.c src/unit_02708.c src/unit_02709.c src/unit_02710.c src/unit_02711.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX;/O2;/GS-;/Ob0")
foreach(_stream_target IN ITEMS restore_link_check restore_game)
 foreach(_stream_entry IN ITEMS 0055f32f 0055f38d 0055f449 0055f561 0055f5a1 0055f5f4 0055f639)
  target_link_options(${_stream_target} PRIVATE "/INCLUDE:_FUN_${_stream_entry}")
 endforeach()
endforeach()
unset(_stream_entry)
unset(_stream_target)

target_sources(pharaoh_restored PRIVATE runtime/sound_fades.c)
set_source_files_properties(runtime/sound_fades.c runtime/settings_callbacks_audio.c src/unit_02706.c src/unit_02712.c src/unit_02494.c src/unit_02495.c src/unit_02496.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX;/O2;/GS-;/Ob0")
foreach(_fade_target IN ITEMS restore_link_check restore_game)
 foreach(_fade_entry IN ITEMS 0055f3eb 0055f67e 0053b690 0053b6f0 0053b790)
  target_link_options(${_fade_target} PRIVATE "/INCLUDE:_FUN_${_fade_entry}")
 endforeach()
endforeach()
unset(_fade_entry)
unset(_fade_target)

target_sources(pharaoh_restored PRIVATE runtime/sound_ambient.c runtime/sound_ambient_profile.c)
set_source_files_properties(runtime/sound_ambient.c runtime/sound_ambient_profile.c src/unit_02493.c src/unit_02497.c src/unit_02498.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX;/O2;/GS-;/Ob0")
foreach(_ambient_target IN ITEMS restore_link_check restore_game)
 foreach(_ambient_entry IN ITEMS 0053b670 0053b830 0053b960)
  target_link_options(${_ambient_target} PRIVATE "/INCLUDE:_FUN_${_ambient_entry}")
 endforeach()
endforeach()
unset(_ambient_entry)
unset(_ambient_target)

target_sources(pharaoh_restored PRIVATE runtime/sound_samples.c runtime/sound_file_lookup.c runtime/owned_buffer.c)
set_source_files_properties(runtime/sound_samples.c runtime/sound_file_lookup.c runtime/owned_buffer.c src/unit_02483.c src/unit_02499.c src/unit_02500.c src/unit_02686.c src/unit_02725.c src/unit_02726.c src/unit_02848.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX;$<$<CONFIG:Release>:/O2>;/GS-;/Ob0")
foreach(_sample_target IN ITEMS restore_link_check restore_game)
 foreach(_sample_entry IN ITEMS 00539c60 0053bc80 0053bcf0 0055dd3f 00560280 005602b0)
  target_link_options(${_sample_target} PRIVATE "/INCLUDE:_FUN_${_sample_entry}")
 endforeach()
endforeach()
unset(_sample_entry)
unset(_sample_target)
target_sources(restore_sound_files_verify PRIVATE runtime/sound_file_lookup.c runtime/owned_buffer.c src/unit_02725.c src/unit_02726.c src/unit_02848.c)

target_sources(pharaoh_restored PRIVATE runtime/array_lifecycle.c runtime/owned_buffer_lifecycle.c runtime/sound_file_lifecycle.c)
set_source_files_properties(runtime/array_lifecycle.c runtime/owned_buffer_lifecycle.c runtime/sound_file_lifecycle.c src/unit_02682.c src/unit_02683.c src/unit_02684.c src/unit_02685.c src/unit_02690.c src/unit_02724.c src/unit_02776.c src/unit_02777.c src/unit_02778.c src/unit_02779.c src/unit_02780.c src/unit_04391.c src/unit_04392.c src/unit_04393.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX;$<$<CONFIG:Release>:/O2>;/GS-;/Ob0")
foreach(_lifecycle_target IN ITEMS restore_link_check restore_game)
 foreach(_lifecycle_entry IN ITEMS 0055dbc0 0055dbf8 0055dc18 0055dc63 0055e58a 00560210 00561363 005613cb 005613e3 00561441 005614ab 0056e140 0056e155 0056e168)
  target_link_options(${_lifecycle_target} PRIVATE "/INCLUDE:_FUN_${_lifecycle_entry}")
 endforeach()
endforeach()
unset(_lifecycle_target)
unset(_lifecycle_entry)
target_sources(restore_sound_verify PRIVATE runtime/array_lifecycle.c runtime/owned_buffer_lifecycle.c runtime/owned_buffer.c runtime/sound_file_lifecycle.c src/unit_02682.c src/unit_02683.c src/unit_02684.c src/unit_02724.c src/unit_02725.c src/unit_02726.c src/unit_02776.c src/unit_02778.c src/unit_02779.c)

add_executable(restore_array_lifecycle_verify tests/array_lifecycle_exceptions.c runtime/array_lifecycle.c runtime/owned_buffer_lifecycle.c runtime/owned_buffer.c runtime/sound_file_lifecycle.c src/unit_02682.c src/unit_02683.c src/unit_02684.c src/unit_02685.c src/unit_02724.c src/unit_02725.c src/unit_02726.c src/unit_02776.c src/unit_02778.c src/unit_02779.c)
target_include_directories(restore_array_lifecycle_verify PRIVATE include)
target_compile_options(restore_array_lifecycle_verify PRIVATE /W4 /WX)
target_link_options(restore_array_lifecycle_verify PRIVATE /BASE:0x10000 /DYNAMICBASE:NO /FIXED)
add_test(NAME standalone_array_lifecycle COMMAND restore_array_lifecycle_verify)
set_tests_properties(standalone_array_lifecycle PROPERTIES LABELS source-only)

# Named building/UI/actor bank reconstruction; canonical entry points retained.
target_sources(pharaoh_restored PRIVATE runtime/sound_banks.c)
set_source_files_properties(runtime/sound_banks.c src/unit_02484.c src/unit_02485.c src/unit_02487.c src/unit_02488.c src/unit_02489.c src/unit_02490.c src/unit_02491.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX;$<$<CONFIG:Release>:/O2>;/GS-;/Ob0")
foreach(_bank_entry IN ITEMS 00539cf0 00539d60 0053af70 0053af90 0053b090 0053b100 0053b130)
 target_link_options(restore_link_check PRIVATE /INCLUDE:_FUN_${_bank_entry})
 target_link_options(restore_game PRIVATE /INCLUDE:_FUN_${_bank_entry})
endforeach()

# Complete 2D sample playback, cache references, loading and ordered channel reset.
target_sources(pharaoh_restored PRIVATE runtime/sound_playback.c runtime/sound_sample_reset.c)
set_source_files_properties(runtime/sound_playback.c runtime/sound_sample_reset.c src/unit_02691.c src/unit_02698.c src/unit_02699.c src/unit_02701.c src/unit_02702.c src/unit_02703.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX;$<$<CONFIG:Release>:/O2>;/GS-;/Ob0")
target_sources(restore_sound_verify PRIVATE runtime/sound_sample_reset.c)
target_sources(restore_sound_samples_verify PRIVATE runtime/sound_playback.c runtime/sound_sample_reset.c)
foreach(_playback_entry IN ITEMS 0055e5d2 0055ecf3 0055edd0 0055f02d 0055f07f 0055f0e8)
 target_link_options(restore_link_check PRIVATE /INCLUDE:_FUN_${_playback_entry})
 target_link_options(restore_game PRIVATE /INCLUDE:_FUN_${_playback_entry})
endforeach()
unset(_playback_entry)

# Preserve nested disabled gates and explicit receivers for sample stopping.
target_sources(pharaoh_restored PRIVATE runtime/sound_stop_channels.c runtime/sound_stop_loops.c)
set_source_files_properties(runtime/sound_stop_channels.c runtime/sound_stop_loops.c src/unit_02694.c src/unit_02695.c src/unit_02696.c src/unit_02697.c src/unit_02700.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX;$<$<CONFIG:Release>:/O2>;/GS-;/Ob0")
target_sources(restore_sound_verify PRIVATE runtime/sound_stop_channels.c src/unit_02694.c src/unit_02695.c)
foreach(_stop_entry IN ITEMS 0055eb99 0055ebd4 0055ec0f 0055ec7c 0055efdc)
 target_link_options(restore_link_check PRIVATE /INCLUDE:_FUN_${_stop_entry})
 target_link_options(restore_game PRIVATE /INCLUDE:_FUN_${_stop_entry})
endforeach()
unset(_stop_entry)

# Native 2D sample-pool resize: wrapped sizes and retained partial state.
target_sources(pharaoh_restored PRIVATE runtime/sound_allocation.c)
target_sources(restore_sound_verify PRIVATE runtime/sound_allocation.c)
set_source_files_properties(runtime/sound_allocation.c src/unit_02687.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX;$<$<CONFIG:Release>:/O2>;/GS-;/Ob0")
target_link_options(restore_link_check PRIVATE /INCLUDE:_FUN_0055dea2)
target_link_options(restore_game PRIVATE /INCLUDE:_FUN_0055dea2)

# Device lifecycle, whole boot and original CRT construction/exit phases.
target_sources(pharaoh_restored PRIVATE runtime/sound_device.c runtime/sound_shutdown.c runtime/sound_boot.c runtime/sound_constructor.c runtime/music_constructor.c runtime/audio_global_lifecycle.c)
set_source_files_properties(runtime/sound_device.c runtime/sound_shutdown.c runtime/sound_boot.c runtime/sound_constructor.c runtime/music_constructor.c runtime/audio_global_lifecycle.c src/unit_02692.c src/unit_02693.c src/unit_02474.c src/unit_02689.c src/unit_02475.c src/unit_01735.c src/unit_01736.c src/unit_01737.c src/unit_01738.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX;$<$<CONFIG:Release>:/O2>;/GS-;/Ob0")
target_sources(restore_sound_verify PRIVATE runtime/sound_device.c runtime/sound_constructor.c src/unit_02849.c)
target_sources(restore_music_verify PRIVATE runtime/music_constructor.c)
foreach(_device_entry IN ITEMS 0055e6c4 0055e92b 005394f0 0055e38b 00539550 004cdf70 004cdf80 004cdf90 004cdfa0 004cdfb0 004cdfc0 004cdfd0 004cdfe0)
 target_link_options(restore_link_check PRIVATE /INCLUDE:_FUN_${_device_entry})
 target_link_options(restore_game PRIVATE /INCLUDE:_FUN_${_device_entry})
endforeach()
unset(_device_entry)

# Literal music configuration/insertion/population range and byte-set tokenizer.
target_sources(pharaoh_restored PRIVATE runtime/music_configuration.c runtime/music_tracks.c runtime/music_population_range.c runtime/byte_tokenizer.c)
target_sources(restore_music_verify PRIVATE runtime/music_configuration.c runtime/music_tracks.c runtime/music_population_range.c runtime/byte_tokenizer.c src/unit_02805.c)
target_sources(restore_campaign_parser_verify PRIVATE runtime/byte_tokenizer.c)
target_sources(restore_music_tick_verify PRIVATE runtime/music_population_range.c)
set_source_files_properties(runtime/music_configuration.c runtime/music_tracks.c runtime/music_population_range.c runtime/byte_tokenizer.c src/unit_02473.c src/unit_02476.c src/unit_02477.c src/unit_02480.c src/unit_02815.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON COMPILE_OPTIONS "/W4;/WX;$<$<CONFIG:Release>:/O2>;/GS-;/Ob0")
foreach(_music_config_entry IN ITEMS 00539250 005395e0 00539730 00539a50 00561db1)
 target_link_options(restore_link_check PRIVATE /INCLUDE:_FUN_${_music_config_entry})
 target_link_options(restore_game PRIVATE /INCLUDE:_FUN_${_music_config_entry})
endforeach()
unset(_music_config_entry)
