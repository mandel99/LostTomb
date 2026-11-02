import {readFile,writeFile,access} from 'node:fs/promises';
import {createHash} from 'node:crypto';
import path from 'node:path';
import {fileURLToPath} from 'node:url';
const root=path.resolve(path.dirname(fileURLToPath(import.meta.url)),'..');
const read=f=>readFile(path.join(root,f),'utf8'),parse=async f=>JSON.parse(await read(f));
const hash=async f=>createHash('sha256').update(await readFile(path.join(root,f))).digest('hex');
const save=(f,data)=>writeFile(path.join(root,f),JSON.stringify(data,null,2)+'\n');
const debug=await parse('metadata/debug-session.json'),render=await parse('metadata/sprite-render-emulation.json'),pixels=await parse('metadata/sprite-pixels-emulation.json');
const receivers=await parse('metadata/sprite-render-receivers.json');
if(!render.passed||render.checked!==1171||!pixels.passed||pixels.checked!==129||receivers.recognized_global_receivers!==646||receivers.unresolved_receivers.length)throw new Error('Missing native/C verification');
for(const report of [render,pixels])for(const [file,digest] of Object.entries(report.source_hashes))
 if(await hash(file)!==digest)throw new Error('Stale native comparison: '+file);
for(const [file,report] of [['build/c-only-debug/Debug/restore_render_verify.exe',render],['build/c-only-debug/Debug/restore_pixels_verify.exe',pixels]])
 if(await hash(file)!==report.compiled_C_sha256)throw new Error('C oracle binary changed: '+file);
for(const file of ['artifacts/render-debug-final-build.log','artifacts/render-release-final-build.log'])
 if(!(await read(file)).includes('100% tests passed, 0 tests failed out of 27'))throw new Error('Missing final tests: '+file);
const archive='metadata/debug-session-sprites.json';
try {await access(path.join(root,archive));}catch(error){if(error.code!=='ENOENT')throw error;await save(archive,debug);}
const log='artifacts/startup-render-final.log',probe=await read(log);
if(!probe.includes('FUN_0041ae40')||!probe.includes('_RTC_UninitUse')||!probe.includes('EXIT 80000003'))throw new Error('Unexpected final startup failure');
const watched=Object.fromEntries([...probe.matchAll(/WATCH ([0-9a-f]{8})=([0-9a-f]{8})/gi)].map(m=>[m[1].toLowerCase(),parseInt(m[2],16)]));
for(const [a,v] of Object.entries({'01265184':30,'0126517c':25140,'01265178':915,'00f5732c':23935,'00d3b580':23935,'00d3b5b8':0}))
 if(watched[a]!==v)throw new Error('Unexpected live state at '+a);
const files=new Set([...Object.keys(debug.source_hashes),...Object.keys(render.source_hashes),...Object.keys(pixels.source_hashes),
 'src/unit_02359.c','src/unit_02360.c','CMakeLists.txt','README.md','tools/record_sprite_render_repair.mjs']);
const source_hashes={};for(const file of files)source_hashes[file]=await hash(file);
const evidence_hashes={...debug.evidence_hashes};
for(const file of [log,'artifacts/startup-render-first.log','artifacts/startup-render-progress.log','artifacts/render-debug-final-build.log','artifacts/render-release-final-build.log',
 'metadata/sprite-render-emulation.json','metadata/sprite-pixels-emulation.json','metadata/sprite-render-receivers.json','metadata/sprite-render-xrefs.json',
 'metadata/sprite-render-dispatch-native.asm','metadata/sprite-clipping-native.asm','metadata/sprite-plain-copy-native.asm','metadata/sprite-render-before.json'])evidence_hashes[file]=await hash(file);
const failure={stage:'Campaign parser, after all 30 graphics library loads and the final loading-image call',function:'FUN_0041ae40',address:'0x0041ae40',
 source:'src/unit_00249.c:117',caller:'FUN_00413ed0',caller_source:'src/unit_00177.c:188',
 reason:'Generated parser uses an uninitialized in_ECX object receiver; local frame layout and helper receiver/return contracts also need reconstruction',
 kind:'MSVC uninitialized-variable runtime check',debugger_exit:'0x80000003',fixed:false,
 next_work:'Reconstruct the campaign object, parser frame and helper ABI from native evidence, then verify real and synthetic campaign files offline'};
const report={format:1,generated_at:new Date().toISOString(),scope:'Standalone C sprite dispatch, clipping, copy and startup-progress repair',
 original_game_executed:false,hybrid_executed:false,ollama_used:false,external_zip_bodies_used:false,UI_automation_used:false,
 source_only_tests:{Debug:{passed:27,total:27},Release:{passed:27,total:27}},
 component_checks:{dispatcher_checks:22,dispatcher_scenarios:7,pixel_checks:258,pixel_scenarios:85},
 native_comparison:{dispatcher:{report:'metadata/sprite-render-emulation.json',cases:1171,entries:['0x0053e590','0x004cdad0','0x004cdb80'],
  compared:['signed return','180 bytes of clipping/draw state','filename storage','scratch pointer','ordered endpoint arguments','stack cleanup and saved registers'],
  controlled_endpoints:['low-level raster routines','file IO','conversion/free'],modifier_arguments_normalized_to_low_16_bits:true},
  pixel_copy:{report:'metadata/sprite-pixels-emulation.json',cases:129,entries:['0x0040fb60','0x0040ffd0','0x00410090'],
   exact_framebuffer_bytes:true,source_preservation:true,stack_and_saved_registers:true,controlled_drawing_endpoints:false}},
 reconstruction:{renderer_receiver:'Explicit manager internally; public three-argument global facade matches the statically identified native call sites',
  receiver_audit:'metadata/sprite-render-receivers.json',native_global_receiver:'0x00f55bb0',recognized_call_sites:646,
  progress_callers:['src/unit_02359.c','src/unit_02360.c'],pixel_header:'include/sprite_render.h',copy_implementation:'runtime/sprite_pixels.c',
  fixes:['Restore 32-bit image IDs, pixel pointers, clip state and coordinates; dimensions and modifier color use native word widths',
   'Restore the filename table index, original-versus-alias descriptor selection and actual raster arguments',
   'Remove a false unresolved-return diagnostic after a void copy',
   'Fix signed clipping intermediates; native comparison caught a fully-above-screen rejection regression',
   'Copy 16-bit pixels with row strides and transparent 0xf81f preservation']},
 live_startup:{loaded_libraries:watched['01265184'],next_image:watched['0126517c'],next_tag:watched['01265178'],
  pixel_file_bytes:watched['01265180'],final_image_id:watched['00d3b580'],final_image_pixel_pointer:watched['00d3b5b8'],log},
 user_preferences_preserved:{CD_check_bypassed:true,child_only_16bit_compatibility:true,original_exe_required_for_build:false},
 last_startup_failure:failure,full_game_loop_reached:false,gameplay_tested:false,
 limitations:['All 30 library loads returned, but final loading-image pixel pointer is zero; correct final-screen content and all loaded image descriptors remain unverified.',
  'Dispatcher equivalence uses controlled raster/file/conversion endpoints; only the three separately compared copy routines claim exact native pixel equivalence on the documented valid domain.',
  'Special compressed/tinted/isometric raster bodies and the cropped external-image reader still contain separate unresolved contracts.',
  'C rejects oversized filenames/invalid filename indices and ignores nonpositive pixel-copy dimensions; these guards intentionally differ from unsafe native behavior.',
  'The campaign parser is the next observed runtime failure; other startup paths, the game loop and missions remain unfinished.'],
 source_hashes,evidence_hashes};
await save('metadata/sprite-render-repair.json',report);
debug.observations=debug.observations.filter(o=>o.report!=='metadata/sprite-render-repair.json');
debug.observations.push({log,site:'FUN_0041ae40',status:'Passed all 30 library loads; campaign parser stops on missing receiver',report:'metadata/sprite-render-repair.json'});
debug.generated_at=report.generated_at;debug.last_startup_failure=failure;debug.current_repair_report='metadata/sprite-render-repair.json';debug.previous_snapshot=archive;
debug.source_hashes=source_hashes;debug.evidence_hashes=evidence_hashes;debug.source_only_tests=report.source_only_tests;debug.limitations=report.limitations;
debug.reproduce.watch_addresses='1265184,126517c,1265178,1265180,f5732c,d3b580,d3b5b8,e92e18';
for(const [name,file] of Object.entries({debug:'build/c-only-debug/Debug/restore_link_check.exe',release:'build/c-only/Release/restore_link_check.exe',debugger:'build/c-only-debug/Debug/restore_debug.exe'}))
 debug.build.images[name]={file,sha256:await hash(file),bytes:(await readFile(path.join(root,file))).length};
await save('metadata/debug-session.json',debug);
const manual=await parse('metadata/standalone-manual-repairs.json');
manual.repairs=[{addresses:['0x0053e590','0x004cdad0','0x004cdb80','0x0040fb60','0x0040ffd0','0x00410090','0x00520af0','0x00520b20'],
 change:'Explicit sprite-render manager, full-width progress IDs/coordinates, signed clipping, correct dispatch arguments and independent 16-bit copies',
 verification:'27 source-only tests in both configurations, 1171 native/C dispatcher cases, 129 native/C pixel cases, bounded startup through 30 library loads',
 report:'metadata/sprite-render-repair.json',limitation:'Remaining special raster, cropped-reader and campaign contracts are unreviewed; final loading-image descriptor has a zero pixel pointer'},
 ...manual.repairs.filter(r=>r.report!=='metadata/sprite-render-repair.json')];
await save('metadata/standalone-manual-repairs.json',manual);
console.log(JSON.stringify({tests:report.source_only_tests,native_dispatch_cases:render.checked,native_pixel_cases:pixels.checked,live:report.live_startup,next_failure:failure.function,playable:false}));
